#include "include.h"
#include "variabili_esterne.h"

void clearObj(Obj object);

// Funzione per la logica dei proiettili sparati dalle piante
void pPiantaLogic(int pipeFrogger[2], Obj pianta, int pipeProiettile[2])
{
    // Imposta la pipe dei proiettili come non bloccante
    fcntl(pipeProiettile[1], F_SETFL, O_NONBLOCK);

    close(pipeFrogger[0]);
    
    // Inizializzazione di un oggetto proiettile
    Obj proiettile = {pianta.x,pianta.y, PROIETTILIPIANTA,pianta.id+NUMPIANTE, 4, false, DIM_PROIETTILI, DIM_PROIETTILI};
    
    // Flag per l'uscita dal ciclo
    bool flag = false;
    
    // Ciclo principale del proesso proiettile
    while (proiettile.y<LINES)
    {
        // Lettura del flag di interruzione dalla pipe dei proiettili
        read(pipeProiettile[1], &flag, sizeof(bool));
        if (flag)
        {
            break; // Esce dal ciclo
        }
        
        // Incrementa la coordinata y del proiettile per farlo muovere verso il basso
        proiettile.y++;
        
        // Scrive il proiettile nella pipe di Frogger per renderlo visibile sullo schermo
        write(pipeFrogger[1], &proiettile, sizeof(Obj));

        // Attesa in microsecondi prima di aggiornare la posizione del proiettile
        usleep(50000);        
    }
    // Termina il processo con successo
    exit(EXIT_SUCCESS);
}

// Funzione per la logica della pianta
void piantaLogic(int pipeFrogger[2], int pipeProiettile[2], int x, int id, int nascosta)
{
    close(pipeFrogger[0]);

    // Crea un oggetto pianta
    Obj myObject = {x, LINES/5, PIANTA,id, 4, false, ALTEZZA_PIANTA, LARG_PIANTA};

    // Variabili per il ritardo della pianta e per nasconderla per un periodo di tempo casuale
    int delay = rand() % 100;
    
    // Quando la pianta viene richiamata dopo essere stata colpita, viene cancellata e attende un tempo casuale prima di riapparire
    if (nascosta!=0)
    {
        clearObj(myObject);
        sleep(nascosta);
        nascosta=0;
    }
    
    // Ciclo principale del processo pianta
    while (1)
    {
        // Il contatore delay viene decrementato. Questo controlla il tempo prima che la pianta possa sparare un nuovo proiettile
        delay--;
        if (delay <= 0)
        {
            // Fork per il processo del proiettile
            pid_t pidProiettile1 = fork();
            if (pidProiettile1 < 0) // Controllo che il processo sia stato creato correttamente. Così non fosse si stampa un errore e si termina il processo con errore
            {
                perror("fork call");
                exit(1);
            }
            else if (pidProiettile1 == 0) // Se il processo figlio è stato creato correttamente
            {
                // Esegue la funzione per la logica dei proiettili
                pPiantaLogic(pipeFrogger, myObject, pipeProiettile);
            }
            // Si imposta un nuovo valore casuale per delay, in modo che la pianta possa sparare nuovi proiettili dopo un certo periodo di tempo casuale
            delay = rand() % 500;
        }
        // Scrive la pianta nella pipe di Frogger
        write(pipeFrogger[1], &myObject, sizeof(Obj));
        // Attesa in microsecondi prima di iniziare la successiva iterazione
        usleep(10000);
    }
}
