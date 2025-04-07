#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>


#define BUF_SIZE 1000


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{
	int sock;
	struct sockaddr_in serv_addr;
 	struct hostent* server;	


	char buffer[1024];
	char nome[30];
	int min_visitatori, max_visitatori;

	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	serv_addr.sin_port = htons(port);
	
	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sock == -1 ) 
	{
		perror("Error opening socket\n");
		exit(1);
	}    

	if ( connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error connecting to socket\n");
		exit(1);
	}

	printf("Inserisci il nome della guida: ");
	scanf("%s", nome);
	printf("Numero minimo visitatori: ");
	scanf("%d", &min_visitatori);
	printf("numero massimo visitatori: ");
	scanf("%d", &max_visitatori);

	

	sprintf(buffer, "GUIDA %s &d &d", nome, min_visitatori, max_visitatori);
	send(sock,buffer, strlen(buffer) + 1, 0);
	printf("Guida registrata, in attesa di visitatori...\n");

	recv(sock, buffer, sizeof(buffer), 0);
	printf("Visita iniziata con %s\n", buffer);


	close(sock);

	return 0;
}



