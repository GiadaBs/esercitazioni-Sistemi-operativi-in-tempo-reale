#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>
#include"list.h"

char *host_name = "127.0.0.1"; /* local host */
int port = 8000;

int main(int argc, char *argv[]) 
{

	ItemType msg;
	int answer;

	if (argc < 3){
		printf("Usage: %s titolo num_copie.\n", argv[0]);
		return -1;
		}
	strncpy(msg.titolo, argv[1], TITOLO_LENGTH);	
	msg.titolo[TITOLO_LENGTH] = '\0';	
	msg.num_copie =atoi(argv[2]);
	msg.tipo = TIPO_CASA_ED;
		
	struct sockaddr_in serv_addr;
 	struct hostent* server;	
	
	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	serv_addr.sin_port = htons(port);
	
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket\n");
		exit(1);
	}    


	if ( connect(sockfd, (void*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error connecting to socket\n");
		exit(1);
	}

	//invio messaggio al server
	printf("Sending book \"%s\" with %d copies to server...\n", msg.titolo, msg.num_copie);

	/* This sends the string plus the string terminator '\0' */
	if ( send(sockfd, &msg, sizeof(msg), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("Message sent. Waiting for response...\n");
	
	
//mi metto in attesa del libro
	if ( recv(sockfd, &answer, sizeof(answer), 0) == -1 ) 
	{
		perror("Error in receiving response from server\n");
		exit(1);
	}

	printf("\nResponse from server OK\n");

	close(sockfd);

	return 0;
}



