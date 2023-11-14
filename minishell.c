#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "function.h"

int main(int argc, char **argv) {
    char *argv_execv[10];
    argv_execv[0] = "";

    char entry[50];

    char *strToken;
    int i;
    int test;
    int back_flag;

    while (strcmp(argv_execv[0], "exit\0") != 0) {
        // Affichage de l'invite de commande
        printf("$ ");
        fgets(entry, 50, stdin);
        if (back_flag == 1) wait(NULL);

        // Modification caractère de fin
        size_t len = strlen(entry);
        if (len > 1) {
            if (len > 0 && entry[len - 1] == '\n') {
                entry[len - 1] = '\0';
            }
        }

        test = 0;
        for (int n = 0; n < len-1; n++) {
            if (entry[n] != ' ') {
                test = 1;
                break;
            }
        }

        if (test == 1) {
            // Découpage avec " "
            strToken = strtok(entry, " ");
            argv_execv[0] = strToken;
            i = 1;
            while (strToken != NULL) {
                strToken = strtok(NULL, " ");
                argv_execv[i] = strToken;
                i++;
            }

            argv_execv[i] = NULL;

            back_flag = back(argv_execv, i-1);
            printf("flag : %d\n", back_flag);

            if (strcmp(argv_execv[0],"exit\0") != 0 && len > 1) {

                if (strcmp(argv_execv[0],"cd") != 0 && strcmp(argv_execv[0],"pwd") != 0) {
                    if (fork() == 0) {
                        if (execvp(argv_execv[0], argv_execv) == -1) perror("Erreur lors du execv");
                        exit(0);
                    }
                    if (back_flag == 0) wait(NULL);
                }
                else if (strcmp(argv_execv[0],"cd") == 0) {
                    chdir(argv_execv[1]);
                }
                else if (strcmp(argv_execv[0],"pwd") == 0) {
                    char buff[200];
                    getcwd(buff, sizeof buff);
                    printf("%s\n", buff);
                }
            }
        }
    }

    return 0;
}