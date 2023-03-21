#include <stdio.h>

#ifdef _WIN32
int main(int argc, char*  argv[]) {
    fprintf(stderr, "Windows is not supported platform");
    exit(0);
}
#else
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#define BUFSIZE 1024

bool is_upd = false;
int client_socket;

void tcp(char* buf ,int bytestx, int bytesrx)
{
    bzero(buf, BUFSIZE);
    while (strcmp(buf, "BYE\n") != 0)
    {
        /* nacteni zpravy od uzivatele */
        bzero(buf, BUFSIZE);
        fgets(buf, BUFSIZE, stdin);
        /* odeslani zpravy na server */
        bytestx = (int) send(client_socket, buf, strlen(buf), 0);
        if (bytestx < 0)
            perror("ERROR in sendto");
        /* prijeti odpovedi a jeji vypsani */
        bzero(buf, BUFSIZE);
        bytesrx = (int) recv(client_socket, buf, BUFSIZE, 0);
        if (bytesrx < 0)
            perror("ERROR in recvfrom");

        printf("%s", buf);
    }
}

void upd(char* buf, int bytestx, int bytesrx, socklen_t serverlen, struct sockaddr_in server_address)
{
    while(1)
    {
        char buf_exp[BUFSIZE];

        /* nacteni zpravy od uzivatele */
        bzero(buf, BUFSIZE);
        fgets(buf_exp, BUFSIZE, stdin);

        buf[0] = 0x0;
        buf[1] = strlen(buf_exp);
        memcpy(buf + 2, buf_exp, strlen(buf_exp) + 1);

        /* odeslani zpravy na server */
        serverlen = sizeof(server_address);
        bytestx = (int) sendto(client_socket, buf, strlen(buf_exp) + 2, 0, (struct sockaddr *) &server_address, serverlen);
        if (bytestx < 0)
            perror("ERROR: sendto");

        bzero(buf, BUFSIZE);
        /* prijeti odpovedi a jeji vypsani */
        bytesrx = (int) recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);

        if (bytesrx < 0) {perror("ERROR: recvfrom");}

        if(buf[1] == 0x0)
        {
            buf = buf + 3;
            printf("OK: %s\n", buf);
        }
        else if (buf[1] == 0x1)
        {
            fprintf(stderr, "ERR: %s", buf);
        }
        else
        {
            perror("Unexpected response message\n");
        }
    }
}

void ctrl_c() {
    char buffer[BUFSIZE];
    bzero(buffer, BUFSIZE);

    if (client_socket > 0) {
        if (!is_upd) {
            strcpy(buffer, "BYE\n");
            send(client_socket, buffer, 4, 0);
            printf("\n");
            printf("%s", buffer);
            recv(client_socket, buffer, BUFSIZE, 0); 
            printf("%s", buffer);
            
        }
        close(client_socket); // close socket
    }
    exit(EXIT_SUCCESS); // exit
}

int main (int argc, const char * argv[]) {
    int port_number, bytestx = -1, bytesrx = -1;
    socklen_t serverlen = -1;
    const char *server_hostname;
    struct hostent *server;
    struct sockaddr_in server_address;
    char buf[BUFSIZE];
    bzero(buf, BUFSIZE); // clear buffer

    signal(SIGINT, &ctrl_c); // handle SIGINT Ctrl+C

    /* 1. test vstupnich parametru: */
    if (argc != 7)
    {
        fprintf(stderr,"usage: %s -h <hostname> -p <port> -m <mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1],"-h")==0) {server_hostname = argv[2];}
    else
    {
        fprintf(stderr,"usage: %s -h <hostname> -p <port> -m <mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[3],"-p")==0){port_number = atoi(argv[4]);}
    else
    {
        fprintf(stderr,"usage: %s -h <hostname> -p <port> -m <mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[5],"-m")==0)
    {
        if(strcmp(argv[6],"udp") == 0) {is_upd = true;}
        else if (strcmp(argv[6],"tcp") == 0) {is_upd = false;}
        else
        {
          fprintf(stderr,"Unknown mode: %s tcp or udp\n", argv[0]);
          exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr,"usage: %s -h <hostname> -p <port> -m <mode>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(port_number < 0 || port_number > 65536)
    {
        fprintf(stderr,"ERROR: Port must be in interval 0-65536,not %d\n", port_number); //тут поменял на %d
        exit(EXIT_FAILURE);
    }

    /* 2. ziskani adresy serveru pomoci DNS */

    if ((server = gethostbyname(server_hostname)) == NULL)
    {
        fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
        exit(EXIT_FAILURE);
    }

    /* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);

    /* tiskne informace o vzdalenem soketu */
    printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    /* Vytvoreni soketu */
    if (!is_upd)
    {
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
    }
    else
    {
        client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    }

    if (client_socket <= 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
    {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }

    if (!is_upd)
    {
        tcp(buf,bytestx,bytesrx);
    }
    else
    {
        upd(buf,bytestx,bytesrx,serverlen,server_address);
    }

    close(client_socket);
    return 0;
}
#endif