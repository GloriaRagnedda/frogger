#include <curses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// Definizione delle costanti per le dimensioni del prato e dell'acqua di gioco
#define ALTEZZA_ACQUA 24
#define ACQ_Y LINES/3
#define PRATO_H 32
#define PRATO_Y LINES/3

// Definizione dei valori per lo stato di gioco
#define PERSO 0
#define VINTO 1

// Definizione delle costanti per le tane 
#define ALTEZZA_TANE 9
#define TANA_Y 0

// Posizione iniziale della rana
#define RANA_INIZIALE_X COLS / 2 - 1
#define RANA_INIZIALE_Y LINES / 3 + ALTEZZA_ACQUA

// Definizione delle costanti per i flussi e i coccodrilli per flusso
#define FLUSSI 12
#define COCCODRILLIFLUSSO 3

// Definizione delle costanti per le lunghezze/larghezze massime degli oggetti 
#define MAX_CHAR 20

#define ALTEZZA_RANA 2
#define LARG_RANA 5
#define ALTEZZA_PIANTA 3
#define LARG_PIANTA 6
#define ALTEZZA_COCCODRILLI 2
#define LARG_COCCODRILLI 14
#define ALTEZZA_TANA 3
#define LARG_TANA 7
#define DIM_PROIETTILI 1
#define MAX_CHAR 20
#define NUM_COCCODRILLI 36
#define NUMPIANTE 4
#define NUMTANE 5
#define TANA {"       ", "       ", "       "}

// Definizione delle costanti per le rappresentazioni grafiche degli oggetti 
#define RANA {"@ - @", "(___)"}
#define PIANTA {"  _  ", " ( ) ", "(_X_)","  j"}
#define COCCODRILLO {"__mMmMm0_0____", "--/\\-/\\---^^^^"}
#define PROIETTILIPIANTA {"o"}
#define PROIETTILIRANA {"|"}
#define TANA {"       ", "       ", "       "}

// Definizione dei valori booleani
#define TRUE 1
#define FALSE 0

// Definizione dei valori del menu iniziale e finale
#define MENU_INIZIALE 15
#define MENU_FINALE 16

#define BLOCCO_CHAR '#' // Definisce il carattere per il blocco
#define BLOCCO_STELLA '*' // Definisce il carattere per il blocco a forma di stella

// Definizione dei colori di gioco
#define C_ERBA 0
#define C_ACQUA 1
#define C_SFONDO_TANE 2
#define C_RANA 3
#define C_PIANTA 4
#define C_TANA 5
#define C_BUONO 6
#define C_CATTIVO 7
#define C_ALERT 8
#define C_PROIETTILE 9



// Definizione della struttura per gli oggetti del gioco
typedef struct Obj
{
     int x;
    int y;
    char oggetto[MAX_CHAR][MAX_CHAR];
    int id;
	int colore;
	bool flag;
	int altezza;
	int larghezza;
} Obj;

// Definizione della struttura Segnali
typedef struct Signal
{
	bool flag;
	int id;
}Signal;

int main(void);
void coccodrillo(int pipeFrogger[2], int pipeCoccodrillo[2], Obj coccodrillo, int speed, int verso);
void clearObj(Obj object);
bool gestioneTempo(time_t *lastUpdateTime, int *updateInterval, int *count);
void printObj(Obj object);
void killObj(Obj obj, int pipeSignal[2]);
void gameLogic(int pipeFrogger[2], int pipeRana[2], int pipeSignal[2]);
void gestore(int pipeFrogger[2], int pipeSignal[2], int pipeRana[2]);
void stampa_finale(int x_coordinate, int y_coordinate, int valore);
void stampa_rana(int x_coordinate, int y_coordinate);
void blocco_interno(int SIZE_H, int SIZE_V,int y_coord, int x_coord);
void costruzione_blocco(int SIZE_H, int SIZE_V);
void blocco_schermo() ;
int opzioni() ;
int menu_uscita();
void menu_finale(int valore);
int inizio_gioco(int max_y, int max_x);
int menu_main();