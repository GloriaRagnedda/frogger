#include "include.h"
#include "variabili_esterne.h"

void generaTane()
{
    // Funzione per generare le tane sullo schermo
    for (size_t i = 0; i < NUMTANE; i++)
    {
        tana[i].y = ALTEZZA_TANE - ALTEZZA_TANA; // Imposta la coordinata y della tana
        tana[i].x = (i + 1) * (COLS / (NUMTANE + 1)) - LARG_TANA / 2; // Calcola la coordinata x della tana
        
        // Itera attraverso l'altezza di ogni tana
        for (size_t j = 0; j < ALTEZZA_TANA; j++)
        {
            // Itera attraverso la larghezza di ogni tana
            for (size_t k = 0; k < LARG_TANA; k++)
            {
                attron(COLOR_PAIR(tana[i].colore)); // Imposta il colore della tana
                mvaddch(tana[i].y + j, tana[i].x + k, ' '); // Disegna la tana sulla finestra
                attroff(COLOR_PAIR(tana[i].colore)); // Disattiva il colore della tana
            }
        }
    }
}

void updateTanaWindow()
{
    // Aggiorna la finestra delle tane
    if (tanaWin != NULL) {
        delwin(tanaWin); // Cancella la finestra delle tane se esiste già
    }
    
    tanaWin = subwin(stdscr, ALTEZZA_TANE, COLS, TANA_Y, 0); // Crea una nuova finestra per le tane
    wbkgd(tanaWin, COLOR_PAIR(7)); // Imposta lo sfondo della finestra delle tane con il colore rosso

    generaTane(); // Chiama la funzione per disegnare le tane sulla finestra
}

void updateAcquaWindow()
{
    // Aggiorna la finestra dell'acqua
    if (acqua != NULL) {
        delwin(acqua); // Cancella la finestra dell'acqua se esiste già
    }

    acqua = subwin(stdscr, ALTEZZA_ACQUA, COLS, ACQ_Y, 0); // Crea una nuova finestra per l'acqua
    wbkgd(acqua, COLOR_PAIR(2)); // Imposta lo sfondo della finestra dell'acqua con il colore blu
}

void color(void)
{
    // Imposta lo sfondo per l'intera finestra
    wbkgd(stdscr, COLOR_PAIR(1));

    // Aggiorna le finestre con i rispettivi colori
    updateTanaWindow();
    updateAcquaWindow();

    wrefresh(stdscr); // Aggiorna lo schermo 
}

