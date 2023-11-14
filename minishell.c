#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv) {
    char *argv_execv[10];

    char entry[50];

    char *strToken;
    int i;

    while (strcmp(entry, "exit\0") != 0) {
        // Affichage de l'invite de command
        printf("$ ");
        fgets(entry, 50, stdin);

        // Supprimer le saut de ligne à la fin de l'entrée
        size_t len = strlen(entry);

        if (len > 0 && entry[len - 1] == '\n') {
            entry[len - 1] = '\0';
        }

        strToken = strtok(entry, " ");
        argv_execv[0] = strToken;
        i = 1;
        while (strToken != NULL) {
            strToken = strtok(NULL, " ");
            argv_execv[i] = strToken;
            i++;
        }
        argv_execv[i] = NULL;

        if (strcmp(entry,"exit\0") != 0) {

            if (fork() == 0) {
                if (execvp(argv_execv[0], argv_execv) == -1) perror("Erreur lors du execv");
                exit(0);
            }
        }
        wait(NULL);
    }

    return 0;
}