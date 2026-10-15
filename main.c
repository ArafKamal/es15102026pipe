#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

/*
    * Esercizio 1: Scambio dati semplice tramite Pipe (Padre e Figlio)
    Scrivere un programma C in cui il processo padre crea una pipe anonima tramite pipe(fd)
    e genera un processo figlio tramite fork(). Il figlio calcola la somma di tutti i numeri pari
    contenuti in un array di 10 interi e invia il risultato al padre scrivendolo nella pipe.
    Il padre, nel frattempo, calcola la somma dei numeri dispari dello stesso array,
    legge il risultato inviato dal figlio tramite read(), attende la terminazione del
    figlio con wait(NULL) e stampa a schermo le due somme parziali e la somma totale complessiva.

 */
    int numeri[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Errore pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Errore fork");
        exit(1);
    }

    if (pid == 0) {
        // Processo figlio
        close(fd[0]);

        int sommaPari = 0;

        for (int i = 0; i < 10; i++) {
            if (numeri[i] % 2 == 0) {
                sommaPari += numeri[i];
            }
        }

        if (write(fd[1], &sommaPari, sizeof(sommaPari))!= sizeof(sommaPari)) {
            perror("Errore write");
            close(fd[1]);
            exit(1);
        }

        close(fd[1]);
        exit(0);
    }

    // Processo padre
    close(fd[1]);

    int sommaDispari = 0;
    int sommaPari;

    for (int i = 0; i < 10; i++) {
        if (numeri[i] % 2 != 0) {
            sommaDispari += numeri[i];
        }
    }

    if (read(fd[0], &sommaPari, sizeof(sommaPari)) != sizeof(sommaPari)) {
        perror("Errore read");
        close(fd[0]);
        wait(NULL);
        exit(1);
    }

    close(fd[0]);
    wait(NULL);

    printf("Somma numeri pari: %d\n", sommaPari);
    printf("Somma numeri dispari: %d\n", sommaDispari);
    printf("Somma totale: %d\n", sommaPari + sommaDispari);














    return 0;
}