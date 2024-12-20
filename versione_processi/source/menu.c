#include "include.h"
#include "variabili_esterne.h"


int main();
/***************************************************************
* Funzioni per la stampa della schermata di inizio e fine gioco*
****************************************************************/

// Funzione per stampare la schermata finale in base alla vittoria o sconfitta
void stampa_finale(int x_coordinate, int y_coordinate, int valore){
	if (!valore)
	{
		move(x_coordinate, y_coordinate);
		addstr(" __     __           _");
		move(++x_coordinate, y_coordinate);
		addstr(" \\ \\   / /          | |");
		move(++x_coordinate, y_coordinate);
		addstr("  \\ \\_/ /__  _   _  | | ___  ___  ___");
		move(++x_coordinate, y_coordinate);
		addstr("   \\   / _ \\| | | | | |/ _ \\/ __|/ _ \\");
		move(++x_coordinate, y_coordinate);
		addstr("    | | (_) | |_| | | | (_) \\__ \\  __/");
		move(++x_coordinate, y_coordinate);
		addstr("    |_|\\___/ \\__,_| |_|\\___/|___/\\___|");
	}else{
		move(x_coordinate, y_coordinate);
		addstr(" __   __            _ ");
		move(++x_coordinate, y_coordinate);
		addstr(" \\ \\ / /           | |");
		move(++x_coordinate, y_coordinate);
		addstr("  \\ V /___  _   _  | | ___  ___  ___ ");
		move(++x_coordinate, y_coordinate);
		addstr("   \\ // _ \\| | | | | |/ _ \\/ __|/ _ \\");
		move(++x_coordinate, y_coordinate);
		addstr("   | | (_) | |_| | | | (_) \\__ \\  __/");
		move(++x_coordinate, y_coordinate);
		addstr("   |_|\\___/ \\__,_| |_|\\___/|___/\\___|");
	}
	
	
	return;
}

// Funzione per stampare la rana nella schermata iniziale
void stampa_rana(int x_coordinate, int y_coordinate) {
    mvaddstr(x_coordinate, y_coordinate, "            _   _");
    mvaddstr(++x_coordinate, y_coordinate, "           (.)_(.)");
    mvaddstr(++x_coordinate, y_coordinate, "        _ (   _   ) _");
    mvaddstr(++x_coordinate, y_coordinate, "       / \\/`-----'\\/");
    mvaddstr(++x_coordinate, y_coordinate, "     __\\ ( (     ) ) /__");
    mvaddstr(++x_coordinate, y_coordinate, "     )   /\\ \\._./ /\\   (");
    mvaddstr(++x_coordinate, y_coordinate, "      )_/ /|\\   /|\\ \\_(");

    return;
}

// Funzione per la creazione di un blocco interno 
void blocco_interno(int SIZE_H, int SIZE_V,
		 int y_coord, int x_coord){

	int x_end = SIZE_V + x_coord;
	int y_end = SIZE_H + y_coord;

	for (int x = x_coord; x <= x_end; ++x)
		for (int y = y_coord; y <= y_end; ++y)
			if (x == x_coord || x == x_end || y == y_coord || y == y_end){
				move(x, y);
				printw("%c", BLOCCO_CHAR);
			}

	return;
}

// Funzione per la costruzione di un blocco
void costruzione_blocco(int SIZE_H, int SIZE_V){
	blocco_interno(SIZE_H, SIZE_V, 0, 0);
	return;
}

// Funzione per la creazione di un blocco di schermo
void blocco_schermo() {
		int max_y, max_x;
		getmaxyx(stdscr, max_y, max_x); 

		for (int y = 0; y < max_y; ++y) {
			for (int x = 0; x < max_x; ++x) {
				if (y == 0 || y == max_y - 1 || x == 0 || x == max_x - 1) {
					move(y, x);
					printw("%c", BLOCCO_CHAR);
				}
			}
		}

		refresh();
		return;
}

/********************************************
* Funzioni per la gestione del menù di gioco*
*********************************************/

// Navigazione nel menu 
int opzioni() {
	int x_coordinate = 15, // Coordinata x dello schermo in cui si trova la prima opzione
	    y_coordinate = 10; // Coordinata y dello schermo in cui si trova la prima opzione

	move(x_coordinate, y_coordinate); // Muovi il cursore sulla prima opzione
	noecho(); // Evita sovrascritture sullo schermo

	while(true) {
		int key = getch(); // Ottieni l'input dall'utente

		switch(key) {
			case KEY_UP: // Se viene premuta la freccia in sù
				move(--x_coordinate, y_coordinate); // Il cursore si sposta verso l'alto
				if (x_coordinate < MENU_INIZIALE) // Se il cursore è al di sopra del limite del menu
					x_coordinate = MENU_FINALE; // Riporta il cursore all'ultima opzione
				break;
			case KEY_DOWN: // Se viene premuta la freccia giù
				move(++x_coordinate, y_coordinate); // Il cursore si sposta verso il basso
				if (x_coordinate > MENU_FINALE) // Se il cursore è al di sotto del limite del menu
					x_coordinate = MENU_INIZIALE; // Riporta il cursore alla prima opzione
				break;
			case '\n': // Se viene premuto il tasto 'Invio'
				return x_coordinate; // Restituisci la posizione del cursore, che rappresenta l'opzione selezionata
				break;
		}
		move(x_coordinate, y_coordinate); // Muovi il cursore alla posizione corrente
	}
}

// Funzione del menu di uscita dal gioco
int menu_uscita() {
	int x_coordinate = 5, // Coordinata x del menu uscita
	    y_coordinate = 10; // Coordinata y del menu uscita

	
	clear(); // Pulizia dello schermo
	costruzione_blocco(52, 10); // Costruzione di un blocco di dimensioni specifiche
	move(x_coordinate, y_coordinate); // Spostamento del blocco nelle coordinate dichiarate

	// Inizializzazione del colore del testo
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	// Domanda all'utente
	addstr("Sicuro di voler abbandonare? (S/N)");
	move(++x_coordinate, y_coordinate);

	// Disattivazione del colore del testo
	attroff(COLOR_PAIR(3));

	while(1) {
		// Si ottiene la risposta dell'utente, se sì viene chiuso il gioco; altrimenti si riparte dal menù di gioco 
		int valore = getch(); // Ottieni l'input dall'utente
		if (valore == 'S' || valore == 's') { // Se viene premuto 'S' o 's'
			endwin(); // Chiudi la finestra curses
			clear(); // Pulisci lo schermo
			exit(0); // Termina il programma
		} else if (valore == 'N' || valore == 'n') // Se viene premuto 'N' o 'n'
			main(); // Torna al menu principale
	}
}

// Funzione del menu finale del gioco
void menu_finale(int valore) {
	// Si ottengono le dimensioni della finestra
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x); 

	// Pulizia dello schermo di gioco 
	clear();
	stampa_finale(8, max_x/3, valore); // Stampa la schermata finale in base al valore passato

	// Inizializzazione del colore del testo
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));
	
	// Messaggio di uscita
	move(16, max_x/3);		
	addstr("Exiting the game..."); 

	

	// Disattivazione del colore del testo
	attroff(COLOR_PAIR(3));	
	refresh();
	sleep(2); // pausa di due secondi prima di uscire dal gioco
	
	//main();

	return;
}

// Funzione che gestisce l'avvio del gioco 
int inizio_gioco(int max_y, int max_x) {
	int var;

	// Effettua la pulizia dello schermo
	clear();

	// Costruisce il blocco iniziale dello schermo
	blocco_schermo();

	// Inizializza il colore del testo per i messaggi
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	// Imposta un timeout per ricevere l'input dall'utente (100000 millisecondi)
	timeout(100000);

	// Posiziona il cursore per il messaggio di inizio gioco
	move(8, max_x/3);
	addstr("Premi qualcosa per iniziare");
	attroff(COLOR_PAIR(3));

	// Disattiva il timeout per l'input
	timeout(-1);

	// Avvia il gioco Frogger se l'utente preme un tasto
	if(var = getch()) {
		
		// Cancella il messaggio di inizio gioco
		attron(COLOR_PAIR(1));
		move(8, max_x/3);
		addstr("                              ");
		attroff(COLOR_PAIR(1));
		
		// Inizializza il generatore di numeri casuali
		srand(time(NULL));

		// Crea le pipe necessarie per la comunicazione tra processi
		int pipeFrogger[2], pipeRana[2], pipeSegnali[2];
		if(pipe(pipeFrogger) == -1 || pipe(pipeRana) == -1 || pipe(pipeSegnali) == -1) {
			perror("creazione pipe");
			exit(EXIT_FAILURE);
		}

		// Imposta le pipe della rana come non bloccanti
		if (fcntl(pipeRana[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(pipeRana[1], F_SETFL, O_NONBLOCK) == -1) {
			perror("fcntl");
			exit(EXIT_FAILURE);
		}

		// Crea un processo figlio per gestire il gestore
		pid_t pidGestore = fork();
		if (pidGestore < 0) { // Restituisce errore se la fork non è andata a buon fine
			perror("fork call");
		} else if (pidGestore == 0) {
			// Processo figlio: gestore del gioco Frogger
			gestore(pipeFrogger, pipeSegnali, pipeRana);
		} else {
			// Processo padre: logica di gioco Frogger
			gameLogic(pipeFrogger, pipeRana, pipeSegnali);

			// Se la rana ha vite rimanenti, ricomincia il gioco
			if (viteRana > 0) {
				//inizio_gioco(max_y, max_x);
			} else {
				// Altrimenti, reimposta le vite della rana e mostra il menu di uscita
				viteRana = 3;
				curs_set(1); // Abilita la visualizzazione del cursore
				//menu_uscita(); // Visualizza il menu di uscita
			}
		}
		return 0;
	}
}

// Funzione che gestisce il menu principale 
int menu_main() {
	int x_coordinate = 11, y_coordinate = 10;

	// Pulisce lo schermo e costruisce il blocco iniziale
	clear();
	blocco_schermo();

	// Visualizza l'immagine della rana
	stampa_rana(5, 10);

	// Visualizza il messaggio di benvenuto e le opzioni del menu
	move(++x_coordinate, y_coordinate);
	addstr("Benvenuto su Frogger!!");
	move(++x_coordinate, y_coordinate);
	addstr("-----------------------");
	move(++x_coordinate, y_coordinate);
	addstr("Scegli un opzione:");
	move(++x_coordinate, y_coordinate);

	// Inizializza il colore per il testo
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	addstr("1) Inizia il gioco");
	move(++x_coordinate, y_coordinate);
	addstr("2) Esci");

	// Disattiva il colore per il testo
	attroff(COLOR_PAIR(3));

	move(++x_coordinate, y_coordinate);
	move(++x_coordinate, y_coordinate);
	addstr("Consiglio: usa le frecce direzionali su e giu per selezionare e premi ENTER per inviare!");

	// Restituisce l'opzione selezionata dall'utente
	return opzioni();
}
