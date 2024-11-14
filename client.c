#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];

    // Check if IP address is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server IP Address>\n", argv[0]);
        exit(1);
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(clientSocket);
        exit(1);
    }

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        exit(1);
    }

    // Display available commands
    printf("Available commands:\n - CPU\n - MEMORY\n - DISK\n - UPTIME\n");
    printf("Enter a command: ");
    fgets(command, BUFFER_SIZE, stdin);
    command[strcspn(command, "\n")] = 0; // Remove newline

    // Send command to server
    send(clientSocket, command, strlen(command), 0);

    // Receive response from server
    printf("Server response:\n");
    while (recv(clientSocket, buffer, BUFFER_SIZE, 0) > 0) {
        printf("%s", buffer); // Display server output
        memset(buffer, 0, BUFFER_SIZE); // Clear buffer for next message
    }

    close(clientSocket);
    return 0;
}