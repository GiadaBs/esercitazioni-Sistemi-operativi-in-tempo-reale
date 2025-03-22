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
	
	if (argc < 2){
		printf("Uso: %s <nome_giocatore>\n", argv[0]);
		exit(1);
	}
	
	
	char *player_name= argv[1];
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


	printf("invio nome giocatore:%s\n", player_name);

	/* This sends the string plus the string terminator '\0' */
	if ( send(sockfd, player_name, strlen(player_name)+1, 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("Message sent. Waiting for response...\n");
	
	char buf[BUF_SIZE];	
	bzero(buf, BUF_SIZE);

	if ( recv(sockfd, buf, BUF_SIZE, 0) <= 0 ) 
	{
		perror("Error ricezione risultato\n");
		close(sockfd);
		exit(1);
	}

	printf("risultato partita: %s\n", buf);

	close(sockfd);

	return 0;
}



