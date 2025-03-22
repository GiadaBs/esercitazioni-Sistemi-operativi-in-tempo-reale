#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>
#include <time.h>

#include "list.h"

#define BUF_SIZE 1000


int port = 8000;


#define N 3 //num min di giocatori

LIST players= NULL; //lista players iscritti
LIST connectedPlayers = NULL; //giocatori connessi

void handle_client(int sockfd);
void start_game();
void send_results(int sockfd, char *player_name, int score);

int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	// Socket opening
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );  
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket");
		exit(1);
	}
	
	int options = 1;
	if(setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof (options)) < 0) {
		perror("Error on setsockopt");
		exit(1);
	}

	bzero( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) 
	{
		perror("Error on listen");
		exit(1);
	}

	printf("\nWaiting for a new connection...\n");
   //char buf[BUF_SIZE];	

	while(1) 
	{
		socklen_t address_size = sizeof( cli_addr );	
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			continue;
		}
		handle_client(newsockfd);
	}
	close(sockfd);
	return 0;
		
}

void handle_client(int sockfd){
	char buf[BUF_SIZE];
	bzero(buf, BUF_SIZE);

	if(recv(sockfd, buf, BUF_SIZE, 0) <=0){
		perror("Errore");
		close(sockfd);
		return;
	}
	printf("Socket associato al giocatore %s: %d\n", buf, sockfd);
	printf("Giocatore connesso: %s\n", buf);
	players = AddPlayer(players, buf, sockfd);
	connectedPlayers = AddPlayer(connectedPlayers, buf, sockfd);
	PrintList(players);
	PrintList(connectedPlayers);
	
	if(getLength(connectedPlayers)>= N){
		start_game();
	}
	
}

void start_game(){
	printf("Inizio Partita\n");
	LIST temp = connectedPlayers;
	srand(time(NULL));
	int scores[] = {3, 2, 1};
	int i = 0;

	while(temp && i<3){
		players = UpdateScore(players, temp->player.name, scores[i]);
		send_results(temp-> sockfd, temp->player.name, scores[i]);		
		temp = temp -> next;
		i++;
	}

	connectedPlayers = DeleteList(connectedPlayers);
	printf("Partita conclusa. Punteggi Aggiornati:\n");
	PrintList(players);
}

void send_results(int sockfd, char *player_name, int score) {
	printf("invio risultato a %s sul socket %d...\n", player_name, sockfd);	
	char result_msg[BUF_SIZE];
	snprintf(result_msg, BUF_SIZE, "Ciao %s, hai ricevuto %d punti", player_name, score);
	
	if(send(sockfd, result_msg, strlen(result_msg) + 1, 0) == -1){
	perror("Errore invio risultati");
	}
	close(sockfd);
}










