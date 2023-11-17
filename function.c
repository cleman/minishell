#include "function.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>

// Execution de la commande dans le tableau avec execvp
void execvp_fct(char **argv_execv, int back_flag) {
    if (fork() == 0) {
        if (execvp(argv_execv[0], argv_execv) == -1) perror("Erreur lors du execv");
        exit(0);
    }
    if (back_flag == 0) wait(NULL);
}

// Test présence "&"
int deleteTarget(char **argv_execv, int i, char *target) {
    size_t len_tab = strlen(argv_execv[i-1]);

    if (len_tab == 1 && strcmp(argv_execv[i-1],target) == 0) {
        argv_execv[i-1] = NULL;
        return 1;
    }
    if (strcmp(&(argv_execv[i-1][len_tab-1]), target) == 0) {
        argv_execv[i-1][len_tab-1] = '\0';
        return 1;
    }
    return 0;
}

// Test présence/emplacement caractère
int test_caractere(char **argv_execv, int i, char *caractere) {
    for (int n = 0; n < i-1; n++) {
        if (strcmp(caractere, argv_execv[n]) == 0) return n;
    }
    return -1;
}

// Test commande vide
int test_entry_void(char entry[50]) {
    for (int n = 0; n < 50-1; n++) {
        if (entry[n] != ' ') {
            return 1;
        }
    }
    return 0;
}

// Commande de redirection ">"
void redirection(char **argv_execv, int chevron_flag) {
    int oldstdout = dup(STDOUT_FILENO);
    int fd = open(argv_execv[chevron_flag+1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    argv_execv[chevron_flag] = NULL;
    if (fork() == 0) {
        if (execvp(argv_execv[0], argv_execv) == -1) perror("Erreur lors du execv");
        exit(0);
    }
    wait(NULL);
    dup2(oldstdout, STDOUT_FILENO);
}

// Commande avec pipe "|"
void pipe_fct(char **argv_execv, int pipe_flag) {
    int pipefd[2];
    int oldstdout = dup(STDOUT_FILENO);
    int oldstdin = dup(STDIN_FILENO);

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork le premier processus
    if (fork() == 0) {
        close(pipefd[0]);   // Ferme lecture

        //STDOUT -> pipefd[1]
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("Erreur durant dup2");
            exit(EXIT_FAILURE);
        }

        argv_execv[pipe_flag] = NULL;
        if (execvp(argv_execv[0], argv_execv) == 1) {  // Premiere fonction
            perror("Erreur durant execvp");
            exit(EXIT_FAILURE);
        }

        perror("Erreur durant execvp");
        exit(EXIT_FAILURE);
    }
    dup2(oldstdout, STDOUT_FILENO);     // redirection de la sortie vers stdout
    wait(NULL);     // Wait premier enfant

    // Fork le second processus
    if (fork() == 0) {
        
        close(pipefd[1]);   // Ferme ecriture

        //STDIN -> pipefd[0]
            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("Erreur durant dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
        
        char **argv_after_pipe = argv_execv + pipe_flag + 1;    // Nouveau tableau qui commence après le pipe

        if (execvp(argv_after_pipe[0], argv_after_pipe) == -1) {    // Deuxième fonction
            perror("Erreur durant execvp");
            exit(EXIT_FAILURE);
        }
        
        exit(0);
    }
    close(pipefd[1]);
    dup2(oldstdin, STDIN_FILENO);   // redirection de l'entrée vers stdin
    wait(NULL);     // Wait second enfant
}