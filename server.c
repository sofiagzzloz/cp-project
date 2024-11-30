#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

//Constants for server setup
#define PORT 8080
#define BUFFER_SIZE 1024

//Function to convert a string to uppercase
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]); //Each character turns into uppercase
    }
}

//Function to handle a single client connection
void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];  //Buffer to store the client's request
    char response[BUFFER_SIZE]; //Buffer to store the server's response
    char *command;             //Pointer to store the system command

    //Receives a request from the client
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
        perror("Receive failed"); //If errors occur during receiving
        close(clientSocket);      //Closes the socket in case of failure
        return;
    }

    buffer[bytesReceived] = '\0';
    to_uppercase(buffer);         //Converts the request to uppercase
    printf("Received request: %s\n", buffer);

    //Map high-level commands to specific system commands
    if (strcmp(buffer, "CPU") == 0) {
        command = "top -l 1 | grep 'CPU usage'"; //To get CPU usage
    } else if (strcmp(buffer, "MEMORY") == 0) {
        command = "vm_stat"; //To get memory status
    } else if (strcmp(buffer, "DISK") == 0) {
        command = "df -h /"; //To get disk space info
    } else if (strcmp(buffer, "UPTIME") == 0) {
        command = "uptime"; //To get system uptime
    } else {
        //For unknown commands
        snprintf(response, sizeof(response), "Unknown command: %s\n", buffer);
        send(clientSocket, response, strlen(response), 0); // Sends error response to client
        close(clientSocket); // Closes client socket
        return;
    }

    //Executes the mapped system command and sends the output to the client
    FILE *fp = popen(command, "r"); //Opens a pipe to run the command
    if (fp == NULL) {
        perror("Failed to run command");
        close(clientSocket);
        return;
    }

    //Reads the output of the command and sends it back to the client
    while (fgets(response, sizeof(response), fp) != NULL) {
        send(clientSocket, response, strlen(response), 0);
    }

    pclose(fp);          //Close pipe
    close(clientSocket); //Close socket
}

int main() {
    int serverSocket, clientSocket;            //Server and client socket file descriptors
    struct sockaddr_in serverAddr, clientAddr; //Structs to hold server and client addresses
    socklen_t addrLen = sizeof(clientAddr);    //Length of the client address struct

    //Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    //Configure the server address struct
    serverAddr.sin_family = AF_INET;         //Use IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; //Accept connections on any network interface
    serverAddr.sin_port = htons(PORT);       //Set the port number

    //Bind the socket to the port and address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        exit(1);
    }

    //For incoming connections
    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(1);
    }
    printf("Server listening on port %d\n", PORT);

    //Server loop to accept and handle client connections
    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen); //Accept a new connection
        if (clientSocket < 0) {
            perror("Client accept failed");
            continue; //keep the server running
        }
        printf("Client connected\n");

        handleClient(clientSocket);
    }

    //Close server socket
    close(serverSocket);
    return 0;
}
