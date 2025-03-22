#ifndef _LIST_H
#define _LIST_H

#define BOOL int
#define FALSE 0
#define TRUE (!FALSE)

/*** struttura giocatore ***/
typedef struct {
    char name[50];/*nome del giocatore*/
	int games_played; /*numero di partite giocate*/
	int score; /*punteggio accumulato*/
} Player;

/* Nodo della lista (per uso interno) */
struct LINKED_LIST_NODE {
    Player player;
	int sockfd; 
    struct LINKED_LIST_NODE* next;
};

/* Per semplicità... */
typedef struct LINKED_LIST_NODE NODE;

/* La lista non è altro che il puntatore associato al primo elemento */
typedef NODE* LIST;

/*** Costruttore/Distruttore ***/

/* Inizializza una nuova lista */
LIST NewList();

/* Azzera la lista */
LIST DeleteList(LIST l);

/*** Predicati ***/

/* ritorna TRUE se la lista e' vuota */
BOOL isEmpty(LIST l);

/* ritorna il numero di elementi presenti nella lista */
int getLength(LIST l);

/*** Selettori ***/
Player* FindPlayer(LIST l, char* name);

LIST AddPlayer(LIST l, char* name, int sockfd);

LIST UpdateScore(LIST l, char* name, int score);

/* Stampa a video la lista (esegue PrintItem() su tutti gli elementi) */
void PrintList(LIST l);

#endif
