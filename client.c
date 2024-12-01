#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


//constants for client setup 
#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int clientSocket;
    struct sockaddr_in serverAddr; //server address structure
    char buffer[BUFFER_SIZE]; //for receiving commands
    char command[BUFFER_SIZE]; //for sending data

    // here we are checking if the IP address is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server IP Address>\n", argv[0]); //this is the usage message which indicates if there is an IP or not, if there's not one it will just exit
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
    serverAddr.sin_port = htons(PORT); // here we are setting the port number already mentioned in the constants
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
    fgets(command, BUFFER_SIZE, stdin); // reads user input
    command[strcspn(command, "\n")] = 0; 

    // Send command to the server
    send(clientSocket, command, strlen(command), 0);

    // Receive response from server
    printf("Server response:\n");
    while (recv(clientSocket, buffer, BUFFER_SIZE, 0) > 0) {
        printf("%s", buffer); // print server output
        memset(buffer, 0, BUFFER_SIZE); // Clear buffer for next message
    }

    close(clientSocket);
    return 0;
}