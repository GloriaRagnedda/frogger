#include "include.h"
#include "variabili_esterne.h"

int main(void){
	initscr(); // Inizializza l'ambiente ncurses
	cbreak(); // Abilita la modalità di lettura non bufferizzata
    noecho(); // Disabilita l'eco dei caratteri digitati
    nodelay(stdscr, TRUE); // Imposta la modalità di attesa non bloccante
    keypad(stdscr, TRUE); // Abilita l'input da tastiera
	srand(time(NULL)); // Inizializza il generatore di numeri casuali con il tempo attuale
	
	// Ottiene le dimensioni dello schermo
	int max_y, max_x, var;
	getmaxyx(stdscr, max_y, max_x); 

	// Inizializzazione dei colori del gioco
   	start_color(); 
   	init_pair(1, COLOR_GREEN, COLOR_GREEN); // Imposta il colore del prato
    init_pair(2, COLOR_BLUE, COLOR_BLUE); // Imposta il colore del fiume
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW); // Imposta il colore del testo
	init_pair(4, COLOR_BLACK, COLOR_WHITE); // Imposta il colore degli oggetti
	init_pair(5, COLOR_BLACK, COLOR_RED); // Imposta il colore dei coccodrilli cattivi
	init_pair(7, COLOR_RED, COLOR_RED); // Imposta il colore delle tane


	init_color(COLOR_WHITE, 1000, 1000, 1000); 
	assume_default_colors(COLOR_WHITE, COLOR_BLACK); // Imposta i colori predefiniti

	// Inizializzazione del colore delle tane e del flag
	for (size_t i = 0; i < NUMTANE; i++)
	{
		tana[i].colore = 1;
		tana[i].flag = true;
	}
	int opzioneSelezionata = menu_main(); // Visualizza il menu principale e ottiene la scelta dell'utente
    curs_set(0); // Cursore nascosto

	switch(opzioneSelezionata){
		case 15: // Prima opzione
			inizio_gioco(max_y,max_x); // Avvia il gioco
			break;
		case 16: //Seconda opzione
			curs_set(1); // Mostra il cursore
			menu_uscita(); // Visualizza il messaggio di uscita
			break;
	}
	
	// Uscita di gioco
	endwin(); 
	return 0; 
}
