#include "include.h"
#include "variabili_esterne.h"

// Definizione della funzione per il movimento dei coccodrilli
void coccodrillo(int pipeFrogger[2], int pipeCoccodrillo[2], Obj coccodrillo, int speed, int verso) 
{
    fcntl(pipeCoccodrillo[0], F_SETFL, O_NONBLOCK); // Si imposta la pipe del coccodrillo come non bloccante
    bool flag = false, tempo = false; // Dichiarazione delle variabili flag e tempo come false
    time_t lastUpdateTime = time(NULL); // Inizializzazione del tempo dell'ultimo aggiornamento
    int intervallo = rand()%4+1; // Generazione di un intervallo casuale tra 1 e 4
    srand(time(NULL)); // Inizializzazione del generatore di numeri casuali
    
    // Ciclo infinito per la gestione del cambio di colore del coccodrillo in base al tempo
    while (1) {

        read(pipeCoccodrillo[0], &flag, sizeof(bool)); // Legge dalla pipe del coccodrillo il valore del flag
        
        if (flag) // Se il flag è true
        {
            coccodrillo.colore = 4; // Imposta il colore del coccodrillo a 4
            flag = false; // Reimposta flag a false
        }
        
        time_t current = time(NULL); // Ottiene il tempo corrente
        
        // Controlla se è passato un intervallo di tempo
        if (current - lastUpdateTime >= intervallo)
        {
            lastUpdateTime = time(NULL); // Aggiorna il tempo dell'ultimo aggiornamento
            tempo = true; // Imposta tempo a true
        }

        // Gestione del cambio di colore del coccodrillo in base al tempo
        if (tempo && coccodrillo.colore == 5) 
        {
            // Verifica se tempo è true e se il colore corrente del coccodrillo è 5
            coccodrillo.colore = 3; // Imposta il colore del coccodrillo a 3
            intervallo = 1; // Imposta l'intervallo a 1
            tempo = false; // Resetta il flag di tempo
        }
        else if (tempo && coccodrillo.colore == 3)
        {
            // Verifica se tempo è true e se il colore corrente del coccodrillo è 3
            coccodrillo.colore = 2; // Imposta il colore del coccodrillo a 2
            intervallo = rand() % 3 + 1; // Imposta un intervallo casuale compreso tra 1 e 3
            tempo = false; // Resetta il flag di tempo
        }
        else if (tempo && coccodrillo.colore == 2)
        {
            // Verifica se tempo è true e se il colore corrente del coccodrillo è 2
            coccodrillo.colore = 5; // Imposta il colore del coccodrillo a 5
            intervallo = rand() % 4 + 1; // Imposta un intervallo casuale compreso tra 1 e 4
            tempo = false; // Resetta il flag di tempo
        }

        // Movimento del coccodrillo
        if (verso == 0) {
            // Se il coccodrillo sta procedendo verso destra
            coccodrillo.x++; // Incrementa la coordinata x del coccodrillo

            // Controlla se il coccodrillo ha superato il margine destro della finestra
            if (coccodrillo.x >= COLS) {
                coccodrillo.x = 0; // Riporta il coccodrillo alla posizione iniziale a sinistra della finestra
            }
        } else if (verso == 1) {
            // Se il coccodrillo sta procedendo verso sinistra
            coccodrillo.x--; // Decrementa la coordinata x del coccodrillo

            // Controlla se il coccodrillo ha superato il margine sinistro della finestra
            if (coccodrillo.x <= -LARG_COCCODRILLI) {
                coccodrillo.x = COLS - 1; // Riporta il coccodrillo alla posizione iniziale a destra della finestra
            }
        }

        write(pipeFrogger[1], &coccodrillo, sizeof(Obj)); // Scrive il coccodrillo nella pipe
        usleep(speed); // Attendere in base alla velocità calcolata in precedenza
    }
}

