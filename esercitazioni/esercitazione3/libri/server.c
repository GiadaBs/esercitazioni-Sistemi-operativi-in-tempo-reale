#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>
#include"list.h"

#define BUF_SIZE 1000


int port = 8000;


int main() 
{
	LIST libri;
	LIST lettori;
	
	ItemType msg;
	int answer;

	ItemType* libro;
	ItemType* lettore;

	libri= NewList();
	lettori = NewList();

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


	socklen_t address_size = sizeof( cli_addr );	
	

	while(1) 
	{
		printf("\nWaiting for a new connection...\n");
		
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		
		
		// Message reception
		if ( recv( newsockfd, &msg, sizeof(msg), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}
		//qui ricevo il messaggio
		printf("Received from client: \"%s\"\n", msg.titolo);
		PrintItem(msg);
		
		if(msg.tipo == TIPO_LETTORE){
			printf("Gestione lettore\n");
			libro= Find(libri, msg);
			if(libro != NULL){
				printf("Libro Trovato\n");
				answer=1;
				if(send(newsockfd, &answer, sizeof(answer), 0) == -1){
					perror("Error on send");
					exit(1);
				}
				close(newsockfd);

				//aggiorno la disponibilità di libri
				libro->num_copie--;
			
				if(libro->num_copie == 0){
					printf("Terminato Libri %s - lo elimino.\n", libro->titolo);
					libri = Dequeue(libri, *libro);
				}


			} else{
				//NON HO IL LIBRO
				printf("Libro non disponibile \n");
				msg.sockfd = newsockfd;
				lettori = EnqueueLast(lettori, msg);
			}
			

		}else if (msg.tipo == TIPO_CASA_ED){
				printf("Gestione Casa Editrice\n");
				answer = 1;
				if(send(newsockfd, &answer, sizeof(answer), 0) == -1){
					perror("Error on send");
					exit(1);
				}
				close(newsockfd);
					
			lettore = Find(lettori,msg);

			while(msg.num_copie >0 && lettore != NULL){
			
			printf("Ho trovato il lettore");
			PrintItem(*lettore);

			if(send(lettore->sockfd, &answer, sizeof(answer), 0) == -1){
				perror("Error on send");
				exit(1);		
			}
			close(lettore->sockfd);
			
			lettori= Dequeue(lettori, *lettore);
	
			msg.num_copie --;

			if(msg.num_copie>0){
				lettore = Find(lettori, msg);
			}
		}
		
		if(msg.num_copie>0){
			libro = Find(libri, msg);
			if(libro == NULL){
				libri = EnqueueLast(libri, msg);
			}
			else{
				libro->num_copie+= msg.num_copie;
			}
		  }
		printf("lista libri\n");
		PrintList(libri);
		printf("Lista lettori\n");
		PrintList(lettori);
	    }
		else{
			printf("Tipo %d non riconosciuto.\n", msg.tipo);
			close(newsockfd);
		}

		close(newsockfd);
	}

	close(sockfd);
	return 0;
}



