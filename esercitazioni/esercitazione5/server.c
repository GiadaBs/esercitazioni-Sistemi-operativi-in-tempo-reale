#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>


#define BUF_SIZE 1000

#define MAX_GUIDE 10

int port = 8000;


//memorizza le guide
typedef struct{
	char nome[30];
	int min_visitatori;
	int max_visitatori;
	int attuali_visitatori;
	int socket;
	int attivo
} Guida;

Guida guide[MAX_GUIDE];
int num_guide = 0;

void aggiungi_guida(char* nome, int min, int max, int socket){
	if(num_guide < MAX_GUIDE){
		strcpy(guide[num_guide].nome, nome);
		guide[num_guide].min_visitatori = min;
		guide[num_guide].max_visitatori = max;
		guide[num_guide].attuali_visitatori = 0;
		guide[num_guide].socket = socket;
		guide[num_guide].attivo = 1;
		printf("Guida registrata: nome=%s, min=%d, max=%d\n", nome, min, max);
		num_guide++;
	}
}

void gestisci_client(int client_socket){
	char buffer[BUF_SIZE];
	recv(client_socket, buffer, BUF_SIZE, 0);

	if(strncmp(buffer, "GUIDA", 5) == 0){
		char nome[30];
		int min, max;
		sscanf(buffer, "GUIDA %s %d %d", nome, &min, &max);
		aggiungi_guida(nome, min, max, client_socket);
		
	}
	else if(strncmp(buffer, "VISITATORE", 10) == 0) {
		int num_visitatori;
		sscanf(buffer, "VISITATORE %d", &num_visitatori );
		printf("Gruppo di %d visitatori in attesa \n", num_visitatori);

	for(int i = 0; i<num_guide; i++){
		printf("Controllo guida: nome=%s, min=%d, max=%d, attuali=%d, visitatori=%d\n", guide[i].nome, guide[i].min_visitatori, guide[i].max_visitatori, guide[i].attuali_visitatori, num_visitatori);

		if(num_visitatori >= guide[i].min_visitatori && num_visitatori<= guide[i].max_visitatori){
		send(client_socket, guide[i].nome, strlen(guide[i].nome) +1, 0);
		close(client_socket);
		return;
		}
	}
	send(client_socket, "NON DISPONIBILE", 15, 0);
	}
}
int main() 
{
	int server_socket, client_socket;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	socklen_t addr_size;

	// Socket opening
	server_socket = socket( AF_INET, SOCK_STREAM, 0 );  
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// Address bindind to socket
	if ( bind( server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}	
	
	printf("\nWaiting for a new connection...\n");

	if(listen(server_socket, 5) == -1){
		perror("errore listen");
		exit(1);
	}

	while(1) 
	{
		
		
		// New connection acceptance		
		addr_size = sizeof(cli_addr);
		client_socket = accept(server_socket, (struct sockaddr *)&cli_addr, &addr_size );    
		if (client_socket >= 0) 
		{
			gestisci_client(client_socket);
		}
	}

	close(server_socket);
	return 0;
}



