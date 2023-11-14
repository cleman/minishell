#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char *argv_execv[2];
    argv_execv[1] = NULL;

    char entry[50];

    while (strcmp(entry, "exit\0") != 0) {
        // Affichage de l'invite de command
        printf("$ ");
        fgets(entry, 50, stdin);

        // Supprimer le saut de ligne à la fin de l'entrée
        size_t len = strlen(entry);

        if (len > 0 && entry[len - 1] == '\n') {
            entry[len - 1] = '\0';
        }

        if (strcmp(entry,"exit\0") != 0) {
            argv_execv[0] = entry;

            if (fork() == 0) {
                if (execvp(entry, argv_execv) == -1) perror("Erreur lors du execv");
                exit(0);
            }
        }
        wait(NULL);
    }

    return 0;
}