#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <netdb.h>
#include"list.h"

#define BUF_SIZE 1000

int port = 8000;


LIST fornitori = NULL;
LIST centri = NULL;


void gestisci_fornitore(int newsockfd) {
    ItemType fornitore;
    recv(newsockfd, &fornitore.value, sizeof(double), 0);
    double minimo;
    recv(newsockfd, &minimo, sizeof(double), 0);
    
    fornitori = EnqueueOrdered(fornitori, fornitore);
    printf("Fornitore registrato con %f vaccini, minimo richiesto: %f\n", fornitore.value, minimo);

}

void gestisci_centro(int newsockfd) {
    ItemType centro;
    recv(newsockfd, &centro.value, sizeof(double), 0);
    
    centri = EnqueueOrdered(centri, centro);
    printf("Centro registrato con richiesta di %f vaccini\n", centro.value);

}

void distribuisci_vaccini() {
    LIST tmp_forn = fornitori;
    while (!isEmpty(tmp_forn)) {
        LIST tmp_cent = centri;
        double vaccini_disponibili = tmp_forn->item.value;
        LIST selezionati = NewList();
        double totale_assegnato = 0;
        
        while (!isEmpty(tmp_cent) && vaccini_disponibili > 0) {
            if (tmp_cent->item.value <= vaccini_disponibili) {
                selezionati = EnqueueLast(selezionati, tmp_cent->item);
                vaccini_disponibili -= tmp_cent->item.value;
                totale_assegnato += tmp_cent->item.value;
            }
            tmp_cent = tmp_cent->next;
        }
        
        if (totale_assegnato >= tmp_forn->item.value / 2) {
            fornitori = Dequeue(fornitori, tmp_forn->item);
            while (!isEmpty(selezionati)) {
                centri = Dequeue(centri, selezionati->item);
                selezionati = DequeueFirst(selezionati);
            }
            printf("Vaccini distribuiti con successo!\n");

			
        }
        tmp_forn = tmp_forn->next;
    }
}


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


	socklen_t address_size = sizeof( cli_addr );	
	
   char buf[BUF_SIZE];	

	while(1) 
	{
		printf("\nWaiting for a new connection...\n");
		
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		
		char tipo;
		recv(newsockfd, &tipo, sizeof(char), 0);
		if(tipo == 'F'){
			gestisci_fornitore(newsockfd);
		}else if(tipo == 'C'){
			gestisci_centro(newsockfd);
		}

		distribuisci_vaccini(newsockfd);
		close(newsockfd);
	}

	close(sockfd);
	return 0;
}



