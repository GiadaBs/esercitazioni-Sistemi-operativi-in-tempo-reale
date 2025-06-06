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
	int num_visitatori;
	
	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

 	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sock == -1 ) 
	{
		perror("Error creation socket\n");
		exit(1);
	}    


	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	serv_addr.sin_port = htons(port);
	
	
	if ( connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error connecting to socket\n");
		exit(1);
	}


	printf("numero di visitatori nel gruppo: ");
	scanf("%d", &num_visitatori);

	sprintf(buffer, "VISITATORE %d", num_visitatori);
	/* This sends the string plus the string terminator '\0' */
	if ( send(sock, buffer, strlen(buffer)+1, 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	if ( recv(sock, buffer, sizeof(buffer), 0) == -1 ) 
	{
		perror("Error in receiving response from server\n");
		exit(1);
	}

	printf("\nResponse from server: \"%s\"\n", buffer);

	close(sock);

	return 0;
}



