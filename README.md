REMOTE SYSTEM INFORMATION RETRIEVAL TOOL

ABSTRACT
The object of this project is to create a client-server monitoring tool that gathers whatever system metrics the user asks for (CPU usage, memory space and disk space). 
It essentially allows for real time monitoring of the machine's resources from a local network.
With the use of socket programming in C and the TCP protocol, our system establishes a client-server connection that facilitates secure and efficient data exchange.

INTRODUCTION
TCP (Transmission Control Protocol) is a fundamental communication standard for connection-oriented data transfer across networks. 
It ensures that data packets are delivered correctly by coordinating with the Internet Protocol (IP). 
Through the use of flow control, packet acknowledgment, and error connection it guarantees reliable data delivery. 
In order to create this tool that allows a client to submit predetermined commands to a remote server that retrieves and returns the needed system information, this project makes use of TCP.


KEY TERMINOLOGY
Socket:  It is used to transmit commands from the client to the server and receive the related system information in return.

IP Address: It is he unique identification of the server system in the LAN, allowing the client to establish a connection to request system information.

Port Number: it is the server's logical access point that watches for incoming requests.

Bind: It allocates the server's socket to a particular IP address and port number.

Listen: It accepts incoming connections from the client by entering a listening state thanks to this system call.

Accept: It accepts a client connection and starts a session in which instructions and answers can be shared.

Send: It enables the server to deliver the system data (such as CPU utilization and memory statistics) back to the client.

Receive: It obtains client-sent commands (such as "CPU" and "UPTIME") for processing on the server side. It retrieves the server's response on the client side.

Close:  It ends the client server connection.

System Command Execution: The server associates the client's request with a system-level command that has been predefined and runs the command to obtain the pertinent data.

CPU Usage: Says the percentage of CPU resources in use.

Memory Statistics: Gives the current memory usage and availability.

Disk Usage: Shows the storage use of the system's disk.

System Uptime: Reports how long the system has been running.

HOW TO RUN It:
-Run the server on your first computer (turn it into executables)

-Use the ifconfig/ipconfig command to retrieve the IP addresses of the systems

-Replace the IP address in the server code with the actual IP address of the server computer

-Run the server on the other computer 

-Compile the server: gcc server.c -o server -> run it wit ./server

-Compile the client: gcc client.c -o client

-Run the client on your first computer like this: ./client <IPAddress>


REQUIRMENTS: 
-Must be connected to the same wifi network

-Make sure to not be using a VPN

-Must have 2 computers
