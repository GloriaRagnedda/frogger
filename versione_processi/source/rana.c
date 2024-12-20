#include "include.h"
#include "variabili_esterne.h"

// Funzione per la logica dei proiettili sparati dalla rana
void pRanaLogic(int pipeFrogger[2], int pipePRana[2], Obj rana)
{
    // Chiude il file descriptor di lettura per pipeFrogger
    close(pipeFrogger[0]);
    // Chiude il file descriptor di scrittura per pipePRana
    close(pipePRana[1]);
    // Imposta la lettura da pipePRana come non bloccante
    fcntl(pipePRana[0], F_SETFL, O_NONBLOCK);
    
    // Crea un oggetto proiettile con le coordinate della rana
    Obj proiettile = {rana.x+2,rana.y,{"|"},1, 4, false, DIM_PROIETTILI, DIM_PROIETTILI};

    // Variabile flag usata per controllare se è stato ricevuto un segnale di interruzione
    bool flag = false;

    // Ciclo principale del processo proiettile
    while (proiettile.y > -1)
    {
        // Lettura del flag dalla pipe
        read(pipePRana[0], &flag, sizeof(bool));
        // Se il flag è true, significa che c'è stata un'interruzione (il proiettile ha raggiunto la fine dello schermo o c'è stata unua collisione con altri oggetti) e quindi si esce dal ciclo
        if (flag)
        {
            flag=false;// Aggiorna il flag a false e si esce dal ciclo
            break;
        }

        // Se il flag non è true e il proiettile non è stato interrotto, si sposta il proiettile verso l'alto
        proiettile.y--;
        // Scrive il proiettile nella pipe di Frogger
        write(pipeFrogger[1], &proiettile, sizeof(Obj));
        // Attesa in microsecondi prima della prossima iterazione
        usleep(10000);
    }
    // Terminazione del processo
    exit(EXIT_SUCCESS);
}

// Logica della rana
void ranaLogic(int pipeFrogger[2], int pipeRana[2], int pipePRana[2])
{
    // L'input viene impostato come non bloccante
    timeout(0);
    // Chiude il file descriptor di lettura della pipe principale
    close(pipeFrogger[0]);
    // Imposta la lettura da pipeRana come non bloccante
    fcntl(pipeRana[0], F_SETFL, O_NONBLOCK);

    // Inizializzazione degli oggetti
    Obj rana = {COLS/2-1, LINES/3+ALTEZZA_ACQUA, {"@ - @", "(___)"}, 0, 4, false, ALTEZZA_RANA, LARG_RANA}, // Rana
        prec = rana,  // Posizione della rana al ciclo precedente
        data; // Oggetto per lo spostamento della rana sul coccodrillo
    data.flag = false;

    // Variabile per salvare il valore dell'input
    int input;

    // Scrive la posizione iniziale della rana nella pipe di Frogger per renderla visibile sullo schermo
    write(pipeFrogger[1], &rana, sizeof(Obj));

    // Ciclo while per gestire il movimento della rana e l'input dell'utente
    while (1)
    {
        // Input
        input = getch();

        // Lettura da pipeRana, usata quando la rana si trova su un coccodrillo
        read(pipeRana[0], &data, sizeof(Obj));
        // Quando avviene la lettura, la rana si sposta nelle coordinate appena lette
        if (data.flag==true && data.x>0 && data.x<COLS-LARG_RANA)
        {
            rana.x = data.x;
            rana.y = data.y;
            data.flag = false;
        }

        // Gestione dell'input
        switch (input)
        {
        //Sinistra
        case KEY_LEFT:
            if (rana.x > 1)
                rana.x -= 1;
            break;
        //Destra
        case KEY_RIGHT:
            if (rana.x < COLS - 2)
                rana.x += 1;
            break;
        //Giù
        case KEY_DOWN:
            if(rana.y < LINES-1)
                rana.y=rana.y+ALTEZZA_RANA;
                break;
        //Sù
        case KEY_UP:
            if(rana.y>0)
                rana.y = rana.y - ALTEZZA_RANA;
            break;
        //Spazio, viene sparato il proiettile
        case ' ':
            // Creazione del processo proiettile
            pid_t pidProiettiliRana = fork();
            if (pidProiettiliRana<0)
            {
                perror("fork call");
                exit(1);
            }
            else if (pidProiettiliRana==0)
            {
                pRanaLogic(pipeFrogger, pipePRana, rana);
            }
            break;
        //Uscita dal gioco
        case 'q':
            close(pipeFrogger[1]);
            endwin();
            exit(1);
            break;
        }
        
        // La scrittura sulla pipe principale avviene solo se la rana si è spostata
        if (rana.x != prec.x || rana.y != prec.y)
        {
            write(pipeFrogger[1], &rana, sizeof(Obj));
        }
        prec = rana;
    }
}

