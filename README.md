# README
# IPK project 1
author: **Dinara Garipova** (xgarip00)  
date: **2023-03-21**
Gitea profile: [xgarip00](https://git.fit.vutbr.cz/xgarip00)

# Task:
## Client-aplication
This code implements a client application that sends messages to a server using either TCP or UDP protocols. The client accepts the following command-line arguments:

-h: the hostname of the server to connect to.
-p: the port number on which the server is listening.
-m: the mode of the communication, either tcp or udp.
The client sends messages entered by the user from the console to the server and prints the server's response. The communication can be interrupted by sending the command BYE. The client also handles the SIGINT signal, which is triggered when the user presses Ctrl+C.

#### Usage
```bash
Usage: ./ipkcpc -h hostname -p port -m mode (tcp/udp)
```
* <hostname> - the hostname or IP address of the server
* <port> - the port number to use for communication with the server
* <mode> - either "tcp" or "udp" for specifying the protocol to use for communication

# Requirements
To compile and run this code, you need:

* C compiler (GCC or Clang)
* Linux/Unix system (Ubuntu, Debian, Fedora, macOS)
* Basic knowledge of C programming

## Compilation
This project can be compiled using Makefile:
```bash
make
```
### Compatibility
File can be compiled at 3 different platforms:
* Linux
* Windows **_(not supported)_**
* MacOS


# Implementation
The code is written in C and consists of a single file client.c. The code uses the sys/socket.h, netdb.h, arpa/inet.h, netinet/in.h, unistd.h, stdlib.h, string.h, and signal.h libraries.

>The main function parses the command-line arguments and establishes a connection to the server using either the TCP or UDP protocol, depending on the value of the -m argument. The tcp function implements the communication using the TCP protocol, while the upd function implements the communication using the UDP protocol. The ctrl_c function is called when the SIGINT signal is triggered and closes the client's socket.

## Required libraries
* stdio.h
* sys/socket.h
* netdb.h
* arpa/inet.h
* netinet/in.h
* unistd.h
* stdlib.h
* string.h
* signal.h
* stdbool.h

## Functions

### tcp(char* buf ,int bytestx, int bytesrx)
Description: This function handles communication using TCP protocol.
 Parameters:
* buf - a character array to store the message
* bytestx - number of bytes sent
* bytesrx - number of bytes received
* Returns: void

### upd(char* buf, int bytestx, int bytesrx, socklen_t serverlen, struct sockaddr_in server_address)

Description: This function handles communication using UDP protocol.
Parameters:
* buf - a character array to store the message
* bytestx - number of bytes sent
* bytesrx - number of bytes received
* serverlen - the size of the server address structure
* server_address - a sockaddr_in struct containing the server address and port number
Returns: void

### ctrl_c(int sig)
Description: This function handles SIGINT signal for terminating the program.
Parameters:
sig - the signal number
Returns: void

### main(int argc, const char * argv[])
Description: The main function for the client program
Parameters:
* argc - the number of arguments passed to the program
* argv - an array of strings containing the arguments passed to the program
Returns: int (0 if successful)

## Global Variables
### is_udp
Description: A boolean variable indicating whether the UDP protocol is being used
### client_socket
Description: A socket file descriptor for the client
### BUFSIZE
Description: A constant integer representing the maximum size of the buffer.

# Sequence Diagram
   participant User
   participant Program
   participant Server
   
   ```mermaid
   sequenceDiagram
     User->>Main: Starts the program with input arguments
     activate Main
     Main->>CTR-C Sig: Registers the SIGINT signal handler
     Main->>client_socket: Creates a socket and sets up the server address
     activate client_socket
     client_socket->>Main: activated socket
     deactivate client_socket

     loop send/recv
         User->>Main: Enters a message to be sent to the server
         Main->>Server: Sends the message
         Server->>Main: Receives a response
         Main->>Stdout: Prints the received response
         User->>Main: Enters another message to be sent to the server
         Main->>Server: Sends the message
         Server->>Main: Receives a response
         Main->>Stdout: Prints the received response
     end

     User->>Main: Presses Ctrl+C
     Main->>Server: Sends "BYE" message (if using TCP)
     Main->>Stdout: Closes the socket
     Main->>Stdout: Exits
     deactivate Main
   ```


# Bibliography
* [C standard](https://en.cppreference.com/w/c/language)
* [Simple socket's tutor](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html)
* Demo Stubs