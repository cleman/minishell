#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "function.h"
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    char *argv_execv[100];
    argv_execv[0] = "";

    char entry[50];

    char *strToken;
    int i;
    int test;
    int back_flag, chevron_flag;

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
            // Découpage de l'entrée autour des espaces (" ")
            strToken = strtok(entry, " ");
            argv_execv[0] = strToken;
            i = 1;
            while (strToken != NULL) {
                strToken = strtok(NULL, " ");
                argv_execv[i] = strToken;
                i++;
            }
            // Caractère de fin
            argv_execv[i] = NULL;

            // Test présence "&""
            back_flag = back(argv_execv, i-1);
            // Test présence ">"
            chevron_flag = chevron(argv_execv, i-1);

            if (strcmp(argv_execv[0],"exit\0") != 0 && len > 1) {

                if (strcmp(argv_execv[0],"cd") != 0 && strcmp(argv_execv[0],"pwd") != 0 && chevron_flag == -1) {
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
                else if (chevron_flag != -1) {
                    int oldstdout = dup(STDOUT_FILENO);
                    int fd = open(argv_execv[chevron_flag+1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
                    dup2(fd, STDOUT_FILENO);
                    close (fd);
                    argv_execv[chevron_flag] = NULL;
                    if (fork() == 0) {
                        if (execvp(argv_execv[0], argv_execv) == -1) perror("Erreur lors du execv");
                        exit(0);
                    }
                    wait(NULL);
                    dup2(oldstdout, STDOUT_FILENO);                                        
                }
            }
        }
    }

    return 0;
}