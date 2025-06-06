#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>
#include"list.h"

#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{
	
	if (argc < 3) {
        printf("Usage: %s <quantita_vaccini> <minimo_richiesto>\n", argv[0]);
        exit(1);
    }

    double quantita_vaccini = atof(argv[1]);
    double minimo_richiesto = atof(argv[2]);
    char tipo = 'F';
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if ((server = gethostbyname(host_name)) == 0) {
        perror("Error resolving local host\n");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
    serv_addr.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error opening socket\n");
        exit(1);
    }

    if (connect(sockfd, (void *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Error connecting to socket\n");
        exit(1);
    }

    send(sockfd, &tipo, sizeof(char), 0);
    send(sockfd, &quantita_vaccini, sizeof(double), 0);
    send(sockfd, &minimo_richiesto, sizeof(double), 0);
    
    char buf[BUF_SIZE];
    bzero(buf, BUF_SIZE);
    if (recv(sockfd, buf, BUF_SIZE, 0) == -1) {
        perror("Error receiving response from server\n");
        exit(1);
    }

    printf("Response from server: \"%s\"\n", buf);

    close(sockfd);
    return 0;
}


