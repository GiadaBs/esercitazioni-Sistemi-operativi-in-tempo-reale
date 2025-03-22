#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/* Inizializza una nuova lista */
LIST NewList() {
    return NULL;
}

/* Azzera la lista */
LIST DeleteList(LIST l) {
    /* dealloca tutti i nodi della lista */
    while (!isEmpty(l)) {
        NODE* todel = l;
        l = l->next;
        /*deleteNode(todel);*/
		free(todel);
    }

    /* la lista e' ora vuota, e puo' essere utilizzata di nuovo */
    return NULL; /*NewList();*/
}

/* ritorna TRUE se la lista e' vuota */
BOOL isEmpty(LIST l) {
    return l == NULL;
}

/* ritorna il numero di elementi presenti nella lista */
int getLength(LIST l) {
    int size = 0;

    while (!isEmpty(l)) {
        ++size;
        l = l->next;
    }

    return size;
}

/*cerca un giocatore nela lista*/
Player* FindPlayer(LIST l, char* name){
	while(l){
		if(strcmp(l->player.name, name) == 0){
			return &l->player;
		}
		l = l->next;
	}
	return NULL;
}

/*aggiungo un nuovo giocatore se non esiste già*/
LIST AddPlayer(LIST l, char* name, int sockfd){
	if(FindPlayer(l, name) != NULL) {
		return l; /*cioe che il giocatore esiste */
	}

	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	assert(new_node != NULL);

	strcpy(new_node-> player.name, name);
	new_node->player.games_played =0;
	new_node->player.score = 0;
	new_node->sockfd = sockfd;
	new_node->next = l;

	return new_node;

}

LIST UpdateScore(LIST l, char* name, int score) {
	Player* p = FindPlayer(l, name);
	if(p){
		p->games_played++;
		p->score +=score;
	}
	return l;
}


/* Stampa a video la lista (esegue PrintItem() su tutti gli elementi) */
void PrintList(LIST l) {
    while (l) {
		printf("Giocatore: %s, Partite: %d, Punti: %d\n ", l->player.name, l->player.games_played, l->player.score);
		l= l->next;
    }
}
