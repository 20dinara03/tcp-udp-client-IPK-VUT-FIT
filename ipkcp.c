#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <stdbool.h>

#define BUFSIZE 1024
bool is_upd = false;

void tcp(char* buf, int client_socket,int bytestx, int bytesrx)
{
  bzero(buf, BUFSIZE);
  while (strcmp(buf, "BYE\n") != 0)
  {
    /* nacteni zpravy od uzivatele */
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin);
    /* odeslani zpravy na server */
    bytestx = send(client_socket, buf, strlen(buf), 0);
    if (bytestx < 0) 
      perror("ERROR in sendto");
    /* prijeti odpovedi a jeji vypsani */
    bytesrx = recv(client_socket, buf, BUFSIZE, 0);
    if (bytesrx < 0) 
      perror("ERROR in recvfrom");
      
    printf("Echo from server: %s", buf);
  }

}

void upd(char* buf, int client_socket,int bytestx, int bytesrx, socklen_t serverlen, struct sockaddr_in server_address)
{
  while(1)
  {
    char buf[BUFSIZE] = { '\0' };
    char buf_exp[BUFSIZE];

    /* nacteni zpravy od uzivatele */
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf_exp, BUFSIZE, stdin);

    buf[0]=0x00;
    memcpy(&buf[1], buf_exp, sizeof(strlen(buf_exp))); // второй агрумент был strlen(buf_exp)
    memcpy(buf + 2, buf_exp, strlen(buf_exp) + 1);


    /* odeslani zpravy na server */
    serverlen = sizeof(server_address);
    bytestx = sendto(client_socket, buf, strlen(buf_exp), 0, (struct sockaddr *) &server_address, serverlen);
    if (bytestx < 0) 
      perror("ERROR: sendto");

    /* prijeti odpovedi a jeji vypsani */
    bytesrx = recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);
    
    memcpy(buf, &buf[3], buf[2]);
    buf[buf[2]] = '\0';

    if (bytesrx < 0){perror("ERROR: recvfrom");}
    if(buf[0] == 1 && buf[1] == 0)
    {
      printf("Echo from server: %s\n", buf);
    }
    else if (buf[0] == 1 && buf[1] == 1)
    {
      printf("Error echo from server: %s\n", buf);
    }
    else
    {
      perror("Unexpected response message\n");
    }
  }
}

void handle_sigint() {
  //handle ctrl+c
}

int main (int argc, const char * argv[]) {
	int client_socket, port_number, bytestx, bytesrx;
  socklen_t serverlen;
  const char *server_hostname;
  struct hostent *server;
  struct sockaddr_in server_address;
  char buf[BUFSIZE];
  bzero(buf, BUFSIZE); // clear buffer

  signal(SIGINT, handle_sigint); // handle SIGINT Ctrl+C 
     
  /* 1. test vstupnich parametru: */
  if (argc != 7) 
  {
    fprintf(stderr,"usage: %s -h <hostname> -p <port> -m <mode>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if(strcmp(argv[1],"-h")==0){server_hostname = argv[2];}
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
    if(strcmp(argv[6],"udp") == 0){is_upd = true;}
    else if (strcmp(argv[6],"tcp") == 0){is_upd = false;}
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
    tcp(buf,client_socket,bytestx,bytesrx);
   }
   else
   {
    upd(buf,client_socket,bytestx,bytesrx,serverlen,server_address);
   }

    close(client_socket);
    return 0;
}