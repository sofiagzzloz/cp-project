#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

#include <ctype.h>

// Convert command to uppercase
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char *command;

    // Receive a request from the client
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
        perror("Receive failed");
        close(clientSocket);
        return;
    }
    
    buffer[bytesReceived] = '\0';
    to_uppercase(buffer); // Convert command to uppercase
    printf("Received request: %s\n", buffer);

    // Map high-level commands to specific system commands
    if (strcmp(buffer, "CPU") == 0) {
        command = "top -l 1 | grep 'CPU usage'";
    } else if (strcmp(buffer, "MEMORY") == 0) {
        command = "vm_stat";
    } else if (strcmp(buffer, "DISK") == 0) {
        command = "df -h /";
    } else if (strcmp(buffer, "UPTIME") == 0) {
        command = "uptime";
    } else {
        snprintf(response, sizeof(response), "Unknown command: %s\n", buffer);
        send(clientSocket, response, strlen(response), 0);
        close(clientSocket);
        return;
    }

    // Execute the mapped command and send response back to client
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        close(clientSocket);
        return;
    }

    // Collect command output
    while (fgets(response, sizeof(response), fp) != NULL) {
        send(clientSocket, response, strlen(response), 0);
    }

    pclose(fp);
    close(clientSocket); // Close client socket after response
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        exit(1);
    }

    // Listen for connections
    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(1);
    }
    printf("Server listening on port %d\n", PORT);

    // Accept and handle client connections
    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket < 0) {
            perror("Client accept failed");
            continue;
        }
        printf("Client connected\n");
        handleClient(clientSocket);
    }

    close(serverSocket);
    return 0;
}