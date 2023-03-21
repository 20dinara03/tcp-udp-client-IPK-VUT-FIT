README
This code implements a client application that sends messages to a server using either TCP or UDP protocols. The client accepts the following command-line arguments:

-h: the hostname of the server to connect to.
-p: the port number on which the server is listening.
-m: the mode of the communication, either tcp or udp.
The client sends messages entered by the user from the console to the server and prints the server's response. The communication can be interrupted by sending the command BYE. The client also handles the SIGINT signal, which is triggered when the user presses Ctrl+C.

Compilation
To compile the code, use a C compiler with the following command:

Copy code
gcc -o client client.c
Usage
The client is executed by running the compiled client executable followed by the command-line arguments described above. For example, to connect to a server with hostname localhost on port 8080 using the TCP protocol, run:

bash
Copy code
./client -h localhost -p 8080 -m tcp
To connect to the same server using the UDP protocol, run:

bash
Copy code
./client -h localhost -p 8080 -m udp
Implementation
The code is written in C and consists of a single file client.c. The code uses the sys/socket.h, netdb.h, arpa/inet.h, netinet/in.h, unistd.h, stdlib.h, string.h, and signal.h libraries.

The main function parses the command-line arguments and establishes a connection to the server using either the TCP or UDP protocol, depending on the value of the -m argument. The tcp function implements the communication using the TCP protocol, while the upd function implements the communication using the UDP protocol. The ctrl_c function is called when the SIGINT signal is triggered and closes the client's socket.

Compatibility
The code has been tested on Linux and macOS platforms using the GCC compiler. The code uses some features that may not be available on Windows platforms.