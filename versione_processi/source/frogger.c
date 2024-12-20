#include "include.h"
#include "variabili_esterne.h"

void menu_finale(); // Dichiarazione della funzione per il menu finale
void color();
void ranaLogic();
void piantaLogic();
// Funzione per cancellare un oggetto dall'interfaccia grafica
void clearObj(Obj object)
{
    for (int i = 0; i < object.altezza; ++i)
    {
        for (int j = 0; j < object.larghezza; ++j)
        {
            mvprintw(object.y + i, object.x + j, " "); // Cancella il carattere nella posizione specificata
        }
    }
}

// Funzione per gestire il tempo di gioco
bool gestioneTempo(time_t *lastUpdateTime, int *updateInterval, int *count) {
    time_t currentTime = time(NULL);
    if ((currentTime - *lastUpdateTime) >= *updateInterval) { // Controlla se è il momento di aggiornare il tempo
        *lastUpdateTime = currentTime; // Aggiorna il tempo dell'ultimo aggiornamento
        
            (*count)--; // Decrementa il conteggio del tempo
            if (*count == 0) { // Se il tempo è scaduto
                return true;
            }
        
        mvprintw(1, 20, "        ");
        attron(COLOR_PAIR(4)); // Attiva il colore specificato
        mvwprintw(stdscr, 1, 20, "Tempo:%d", *count); // Stampa il tempo rimanente al centro della schermata
        attroff(COLOR_PAIR(4)); // Disattiva il colore specificato
        refresh(); // Aggiorna lo schermo
    }
    return false;
}

// Funzione per stampare un oggetto sull'interfaccia grafica
void printObj(Obj object) {
    color(); // Funzione per gestire i colori

    for (int i = 0; i < object.altezza; ++i) {
        for (int j = 0; j < object.larghezza; ++j) {
            attron(COLOR_PAIR(object.colore)); // Attiva il colore specificato per l'oggetto
            mvprintw(object.y + i, object.x + j, "%c", object.oggetto[i][j]); // Stampa il carattere dell'oggetto
            attroff(COLOR_PAIR(object.colore)); // Disattiva il colore specificato
        }
    }
    refresh(); // Aggiorna lo schermo
}

// Funzione per uccidere un oggetto
void killObj(Obj obj, int pipeSignal[2]) {
    Signal killer;
    killer.flag = true; // Imposta il flag a true per segnalare la distruzione dell'oggetto
    killer.id = obj.id; // Imposta l'ID dell'oggetto da uccidere
    write(pipeSignal[1], &killer, sizeof(Signal)); // Scrive il segnale per uccidere l'oggetto sul pipe
    killer.flag = false; // Ripristina il flag
}

void gameLogic(int pipeFrogger[2], int pipeRana[2], int pipeSignal[2])
{
    // Chiude il lato di scrittura del pipeFrogger e il lato di lettura del pipeRana
    close(pipeFrogger[1]);
    close(pipeRana[0]);
    
    // Imposta il pipeRana in modalità non bloccante per la lettura
    fcntl(pipeRana[1], F_SETFL, O_NONBLOCK);

    // Variabili per gestire il tempo
    int timeLimit = 20; // Limite di tempo per ogni manche
    time_t lastUpdateTime = time(NULL); // ultimo periodo di tempo aggiornato
    int updateInterval = 1; // Frequenza di aggiornamento in secondi
    bool tempoScaduto = false;

    // Struttura temporanea per salvare la lettura dal pipe
    Obj myObject = { -1, -1, ' ', 0};
    
    // Inizializzazione delle strutture oggetto
    Obj pianta[NUMPIANTE] = {0, 0, PIANTA}, 
        pPianta[NUMPIANTE] = {0, 0, PROIETTILIPIANTA},
        pRana = {COLS+2, 0, PROIETTILIRANA, 1}, 
        rana = {0, 0, RANA, 0},
        coccodrilli[NUM_COCCODRILLI] = {-14, -2, COCCODRILLO},
        coccodrilliPrec[NUM_COCCODRILLI];

    // Struttura per comunicare con il gestore
    Signal killer = {false, 0};

    // Flag per la rana sul coccodrillo
    bool cocco = false;

    // Variabili per le tane
    bool ranaInTana = false;
    int tanaOccupata=0;

    // Ciclo principale del gioco
    while (1)
    {
        //Chiamata alla funzione timer
        tempoScaduto = gestioneTempo(&lastUpdateTime, &updateInterval, &timeLimit);
        if (tempoScaduto)
        {
            timeLimit = 20;
            viteRana--;
            clearObj(rana);
            rana.x = RANA_INIZIALE_X;
            killObj(rana, pipeSignal);
        }
        
        // Legge l'oggetto dal pipeFrogger
        read(pipeFrogger[0], &myObject, sizeof(Obj));
        
        // Stampa degli oggetti e aggiornamento delle loro copie
        if (myObject.id == 0)
        {
            // Se l'oggetto è la rana, cancella l'oggetto precedente, aggiorna la rana e stampa la nuova rana
            clearObj(rana);
            rana = myObject;
            printObj(rana);
        }
        else if (myObject.id == 1)
        {
            // Se l'oggetto è un proiettile rana, cancella l'oggetto precedente, aggiorna il proiettile e stampa il nuova proiettile
            clearObj(pRana);
            pRana = myObject;
            printObj(pRana);
        }
        else if (myObject.id < NUMPIANTE + 2)
        {
            // Se l'oggetto è una pianta, cancella l'oggetto precedente, aggiorna la pianta e stampa la nuova pianta
            clearObj(pianta[myObject.id - 2]);
            pianta[myObject.id - 2] = myObject;
            printObj(pianta[myObject.id - 2]);
        }
        else if (myObject.id < 2 + 2 * NUMPIANTE)
        {
            // Se l'oggetto è un proiettile pianta, cancella l'oggetto precedente, aggiorna il proiettile e stampa il nuova proiettile
            clearObj(pPianta[myObject.id - (NUMPIANTE + 2)]);
            pPianta[myObject.id - (NUMPIANTE + 2)] = myObject;
            printObj(pPianta[myObject.id - (NUMPIANTE + 2)]);
        }
        else if (myObject.id < 2 + 2 * NUMPIANTE + NUM_COCCODRILLI)
        {
            // Se l'oggetto è un coccodrillo, cancella l'oggetto precedente, aggiorna il coccodrillo e stampa il nuovo coccodrillo
            clearObj(coccodrilli[myObject.id - (2 + 2 * NUMPIANTE)]);
            coccodrilli[myObject.id - (2 + 2 * NUMPIANTE)] = myObject;
            printObj(coccodrilli[myObject.id - (2 + 2 * NUMPIANTE)]);
        }
    


/*************
* Collisioni**
**************/

        //In questa collisione si controlla se il proiettile della rana ha colpito una pianta
        for (size_t i = 0; i < NUMPIANTE; i++) {
            // Itera attraverso tutte le piante nel gioco
            // Controlla se il proiettile della rana ha colpito la pianta
            if ((pRana.x >= pianta[i].x && pRana.x < pianta[i].x + LARG_PIANTA) && (pRana.y == pianta[i].y)) {
                // Se il proiettile della rana colpisce una pianta
                pRana.y = -1; // La posizione del proiettile viene ripristinata 
                killObj(pRana, pipeSignal); // Invia un segnale per uccidere il proiettile della rana attraverso la pipe
                clearObj(pianta[i]); // Cancella l'oggetto pianta dalla visualizzazione
                killObj(pianta[i], pipeSignal); // Invia un segnale per uccidere la pianta attraverso la pipe
            }
        }


        //coccodrilli

        rana.flag = true; // Imposta il flag della rana a true. Questo indica che la rana deve cambiare posizione ad ogni iterazione del ciclo.

        cocco = false; // Inizializza la variabile cocco a false. Questa sarà utilizzata per controllare se la rana si trova sopra un coccodrillo 

        for(int i = 0; i < NUM_COCCODRILLI; i++) { // Ciclo che scorre tutti i coccodrilli 
            // Controllo se il proiettile della rana colpisce un coccodrillo
            if ((pRana.x >= coccodrilli[i].x && pRana.x < coccodrilli[i].x + LARG_COCCODRILLI) && (pRana.y == coccodrilli[i].y)) {
                // Se il proiettile della rana colpisce un coccodrillo, il coccodrillo e il proiettile viene distrutto
                killObj(coccodrilli[i], pipeSignal); // Invia un segnale per cambiare il colore del coccodrillo buono in cattivo
                clearObj(pRana); // Rimuove il proiettile della rana dalla visualizzazione
                killObj(pRana, pipeSignal); // Invia un segnale per indicare che il proiettile della rana è stato eliminato
                pRana.y = -1; // La posizione del proiettile viene ripristinata
            }

            // Controllo se la rana è su un coccodrillo
            if(rana.y == coccodrilli[i].y) { 
                // Verifica se la coordinata y della rana corrisponde a quella del coccodrillo
                if (coccodrilli[i].colore != 2 && rana.x + 1 >= coccodrilli[i].x && rana.x + 2 <= coccodrilli[i].x + LARG_COCCODRILLI && rana.x >= 0 && rana.x < COLS && cocco == false) {
                    // Se la rana si trova su un coccodrillo, e non si trova già su un altro
                    cocco = true; // Imposta cocco a true per indicare che la rana si trova su un coccodrillo
                    if (coccodrilli[i].x != coccodrilliPrec[i].x) {
                        // Verifica se la posizione del coccodrillo è cambiata rispetto alla posizione precedente
                        rana.x = rana.x + coccodrilli[i].x - coccodrilliPrec[i].x; // Aggiorna la posizione della rana in base al movimento del coccodrillo
                        write(pipeRana[1], &rana, sizeof(Obj)); // Comunica la nuova posizione della rana attraverso il pipe
                    }
                }
            }
            coccodrilliPrec[i] = coccodrilli[i]; // Memorizza la posizione corrente del coccodrillo per la prossima iterazione
        }

        if (rana.y >= coccodrilli[0].y && rana.y <= coccodrilli[35].y && !cocco) {
            // Se la rana non si trova su un coccodrillo e cade nell'acqua
            viteRana--; // Decrementa di uno il numero di vite della rana
            clearObj(rana); // Rimuove la rana dalla visualizzazione
            rana.y = RANA_INIZIALE_Y; // Ripristina la posizione della rana
            killObj(rana, pipeSignal); // Invia un segnale per indicare che la rana è stata eliminata
        }

        //in questa collisione si controlla se la rana è stata colpita dal proiettile pianta o ha toccato una pianta
        for (size_t i = 0; i < NUMPIANTE; i++) {
            // Controllo se la rana è colpita dal proiettile della pianta
            if (pPianta[i].y == rana.y && pPianta[i].x >= rana.x && pPianta[i].x <= rana.x + LARG_RANA) {
                viteRana--; // Decrementa di uno il numero di vite della rana
                clearObj(rana); // Rimuove la rana dalla visualizzazione
                rana.x = RANA_INIZIALE_X; // Ripristina la posizione della rana
                killObj(rana, pipeSignal); // Invia un segnale per uccidere la rana attraverso il pipe
            }

            // Controllo se la rana tocca una pianta
            if (rana.y >= pianta[i].y && rana.y <= pianta[i].y + ALTEZZA_PIANTA && rana.x + LARG_RANA >= pianta[i].x && rana.x <= pianta[i].x + LARG_PIANTA) {
                viteRana--; // Decrementa il numero di vite della rana
                clearObj(rana); // Rimuove la rana dalla visualizzazione
                rana.x = RANA_INIZIALE_X; // Sposta la rana al di fuori dello schermo
                killObj(rana, pipeSignal); // Invia un segnale per uccidere la rana attraverso il pipe
            }
        }


        // Controlla se la rana raggiunge le tane
        if (rana.y < tana[0].y + ALTEZZA_TANA) {
            ranaInTana = false; // Imposta il flag ranaInTana a false. 

            // Itera tutte le tane presenti nel gioco
            for (size_t i = 0; i < NUMTANE; i++) {
                if (rana.x >= tana[i].x && rana.x < tana[i].x + LARG_TANA) { // Controlla se la rana è dentro la tana
                    ranaInTana = true; // Imposta ranaInTana a true. Ciò significa che la tana non è stata occupata 
                    if (tana[i].flag == true) { // Se la tana è vuota
                        tana[i].flag = false; // Imposta il flag della tana a false, indicando che è stata occupata 
                        tana[i].colore = 2; // Cambia il colore della tana 
                        tanaOccupata++; // Incrementa il numero di tane occupate

                        // Riporta la rana in posizione di partenza dopo aver occupato la tana
                        rana.x = RANA_INIZIALE_X;
                        rana.y = RANA_INIZIALE_Y;
                        killObj(rana, pipeSignal); // Uccide la rana
                    } else {
                        // Se la tana è già occupata, la rana perde una vita e viene chiamata la funzione killObj per uccidere la rana
                        viteRana--;
                        rana.y = LINES;
                        killObj(rana, pipeSignal);
                    }
                }
            }

            // Se ranaInTana è false,la rana perde una vita e viene chiamata la funzione killObj per uccidere la rana
            if (!ranaInTana) {
                viteRana--;
                clearObj(rana);
                rana.y = LINES;
                killObj(rana, pipeSignal);
                ranaInTana = false;
            }
        }

        // Controlla se le vite della rana sono esaurite
        if (viteRana <= 0) {
            // Ripristina il colore delle tane e mostra il menu finale della sconfitta
            tana[5].colore = 1;
            killer.flag=true;
            killer.id=-1;
            write(pipeSignal[1], &killer, sizeof(Signal));
            menu_finale(PERSO);
            break;
        }

        // Controlla se tutte le tane sono occupate
        if (tanaOccupata == 5) {
            // Mostra il menu finale della vittoria
            menu_finale(VINTO);
        }

/********************
* Stampa vite rana*
********************/
    attron(COLOR_PAIR(4));
    mvprintw(1, 1, "Vite Rana: %d", viteRana);
    attroff(COLOR_PAIR(4));
    refresh();
    }
} 

void gestore(int pipeFrogger[2], int pipeSignal[2], int pipeRana[2]) 
{
    // Dichiarazione delle variabili per memorizzare i PID dei processi figli
    pid_t pidRana, pidCoccodrillo[NUM_COCCODRILLI], pidPianta[NUMPIANTE];

    // Dichiarazione delle pipe per la comunicazione tra il gestore e i figli
    int pipeCoccodrillo[NUM_COCCODRILLI][2], pipeProiettile[NUMPIANTE][2], pipePRana[2];

    // Inizializzazione di un oggetto per configurare i coccodrilli
    Obj object = {-13, ACQ_Y, COCCODRILLO, 0, 4, false, ALTEZZA_COCCODRILLI, LARG_COCCODRILLI};
    
    // Inizializzazione delle variabili: speed, per controllare la velocità di ogni flusso;
    // count, per il tempo di gioco; piantaX, per la posizione x delle piante;
    // piantaNascosta, per nascondere la pianta quando colpita dal proiettile rana;
    // verso, per impostare il verso dei flussi
    int speed, count = 0, piantaX[NUMPIANTE], piantaNascosta = 0, verso = 0;

    // Creazione delle pipe 
    if (pipe(pipePRana) == -1) {
        perror("creazione pipe proiettile rana");
    }
    
    for (size_t i = 0; i < NUM_COCCODRILLI; i++) {
        if (pipe(pipeCoccodrillo[i]) == -1) {
            perror("creazione pipe coccodrilli");
        }
    }
    for (size_t i = 0; i < NUMPIANTE; i++) {
        if (pipe(pipeProiettile[i]) == -1) {
            perror("creazione pipe proiettili delle piante");
        }
    }
    
    close(pipeSignal[1]); // Chiusura del lato di scrittura della pipe di segnalazione

    Signal sig = {false, 0}; // Inizializzazione di una struttura di segnalazione

    // Creazione di un processo figlio per gestire la rana
    pidRana = fork();
    if(pidRana < 0) {
        perror("fork call");
        exit(EXIT_FAILURE);
    } else if(pidRana == 0) {
        // Il processo figlio gestisce la logica della rana
        ranaLogic(pipeFrogger, pipeRana, pipePRana);
    }

    // Creazione di processi figli per gestire le piante
    for (size_t i = 0; i < NUMPIANTE; i++) {
        // Calcolo della posizione delle piante
        piantaX[i] = COLS / (NUMPIANTE + 2) * (i + 1)+ 10 ;
        pidPianta[i] = fork();
        if (pidPianta[i] < 0) {
            perror("fork call");
        } else if (pidPianta[i] == 0) {
            // Il processo figlio gestisce la logica della pianta
            piantaLogic(pipeFrogger, pipeProiettile[i], piantaX[i], i + 2, piantaNascosta);
        }
    }
    
    // Creazione di processi figli per gestire i coccodrilli
    for (size_t i = 0; i < FLUSSI; i++) {
        // Generazione di velocità e direzione casuali per i coccodrilli
        speed = rand() % 40000 + 30000;
        verso = rand() % 2;
        object.y = ACQ_Y + (2 * i); // Calcolo della posizione verticale dei coccodrilli
        for (size_t j = 0; j < COCCODRILLIFLUSSO; j++) {
            object.id = count + 2 * NUMPIANTE + 2; // Id univoco per ogni coccodrillo
            object.colore = rand() % 2 + 4; // Colore casuale per i coccodrilli, o rosso per i cattivi o bianco per i buoni
            pidCoccodrillo[count] = fork();
            switch (count%3) // verifico la posizione del coccodrillo che sto generando all'interno del flusso
            {
            case 0: // il primo coccodrillo di ogni fluso parte dalla posizione x=-13
                object.x = -13;
            case 1:
                object.x = object.x+(rand()%20+14); // il secondo cocodrillo del flusso parte più a destra del primo con distanza casuale
                break;
            case 2:
                object.x -= object.x+(rand()%20+14);//il terzo cocodrillo del flusso parte più a destra del secondo con distanza casuale
                break;
            default:
                break;
            }
            if (pidCoccodrillo[count] < 0) {
                perror("fork call");
            } else if (pidCoccodrillo[count] == 0) {
                // Il processo figlio gestisce la logica dei coccodrilli
                coccodrillo(pipeFrogger, pipeCoccodrillo[count], object, speed, verso);
            }
            count++;
        }
    }
    

    while (1)
    {
        srand(time(NULL)); // Inizializza il generatore di numeri casuali

        // Lettura dei segnali dalla pipe Signal
        read(pipeSignal[0], &sig, sizeof(Signal));
        if (sig.flag)
        {
            // Gestione dei diversi tipi di segnali
            if (sig.id ==-1)
            {
                kill(-1*getpid(), 9);
            }
            else if (sig.id == 0)
            {
                // Se il segnale è relativo alla rana
                // Termina il processo figlio della rana corrente
                // E crea un nuovo processo figlio per gestire la rana
                kill(pidRana, 9);
                pidRana = fork();
                if (pidRana < 0)
                {
                    perror("fork call");
                }
                else if (pidRana == 0)
                {
                    // Il nuovo processo figlio gestisce la logica della rana
                    ranaLogic(pipeFrogger, pipeRana, pipePRana);
                }
            }
            else if (sig.id == 1)
            {
                // Se il segnale è relativo al proiettile della rana
                // Scrive il segnale nella pipe per la rana
                write(pipePRana[1], &sig.flag, sizeof(bool));
            }
            else if (sig.id < 2 + NUMPIANTE)
            {
                // Se il segnale è relativo al proiettile di una pianta
                // Scrive il segnale nella pipe per il proiettile corrispondente alla pianta
                // Termina il processo figlio della pianta corrente
                // E crea un nuovo processo figlio per gestire la pianta
                write(pipeProiettile[sig.id - 2][1], &sig.flag, sizeof(bool));
                kill(pidPianta[sig.id - 2], 9);
                piantaNascosta = rand() % 4 + 1; // Genera un valore casuale per la pianta nascosta
                pidPianta[sig.id - 2] = fork();
                if (pidPianta[sig.id - 2] < 0)
                {
                    perror("fork call");
                }
                else if (pidPianta[sig.id - 2] == 0)
                {
                    // Il nuovo processo figlio gestisce la logica della pianta
                    piantaLogic(pipeFrogger, pipeProiettile[sig.id - 2], piantaX[sig.id - 2], sig.id, piantaNascosta);
                }
            }
            else if (sig.id < 2 + (NUMPIANTE * 2))
            {
                // Se il segnale è relativo al secondo proiettile di una pianta
                // Scrive il segnale nella pipe per il proiettile corrispondente alla pianta
                write(pipeProiettile[sig.id - (2 + NUMPIANTE)][1], &sig.flag, sizeof(bool));
            }
            else if (sig.id < 2 + (NUMPIANTE * 2) + NUM_COCCODRILLI)
            {
                // Se il segnale è relativo ai coccodrilli
                // Scrive il segnale nella pipe per i coccodrilli corrispondenti
                write(pipeCoccodrillo[sig.id - (2 + (NUMPIANTE * 2))][1], &sig.flag, sizeof(bool));
            }
            else
            {
                // Se il segnale è relativo a un altro tipo di proiettile
                // Scrive il segnale nella pipe per il proiettile corrispondente
                write(pipeProiettile[sig.id - (2 + (2 * NUMPIANTE))][1], &sig.flag, sizeof(bool));
            }
        }
    }

}
