#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
    //if (argc != 2) return 0;

    char path[50];
    char *argv_execv[2];
    argv_execv[1] = NULL;

    printf("On est là\n");
    char entry[50];

    while (strcmp(entry, "exit\n") != 1) {
        printf("$ ");
        fgets(entry, 50, stdin);

        // Supprimer le saut de ligne à la fin de l'entrée
        size_t len = strlen(entry);

        if (len > 0 && entry[len - 1] == '\n') {
            entry[len - 1] = '\0';
        }

        if (strcmp(entry,"ls") || strcmp(entry,"who")) {
            argv_execv[0] = entry;
            strcpy(path,"/bin/");
            strcat(path, entry);

            if (execv(path, argv_execv) == -1) perror("Erreur lors du execv");
        }
    }

    return 0;
}