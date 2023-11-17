#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "function.h"
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_ENTRY_SIZE 50
#define MAX_ENTRY_PARAM 50

int main(int argc, char **argv) {
    char *argv_execv[MAX_ENTRY_PARAM];
    argv_execv[0] = "";

    char entry[MAX_ENTRY_SIZE];

    char *strToken;
    int i;
    int test;
    int back_flag = -1, chevron_flag = -1 , pipe_flag = -1, sc_flag = -1;   // sc = semicolon ";"

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

        // Test chaine vide
        test = test_entry_void(entry);

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

            // Test présence "&", ">", "|", ";"
            back_flag = deleteTarget(argv_execv, i-1, "&");
            chevron_flag = test_caractere(argv_execv, i-1, ">");
            pipe_flag = test_caractere(argv_execv, i-1, "|");
            sc_flag = test_caractere(argv_execv, i-1, ";");

            if (strcmp(argv_execv[0],"exit\0") != 0 && len > 1) {

                // Pas commande "cd", "pwd" (built-in), et pas "|", ">"
                if (strcmp(argv_execv[0],"cd") != 0 && strcmp(argv_execv[0],"pwd") != 0 && chevron_flag == -1 && pipe_flag == -1 && sc_flag == -1) {
                    execvp_fct(argv_execv, back_flag);
                }
                // Commande cd
                else if (strcmp(argv_execv[0],"cd") == 0) {
                    chdir(argv_execv[1]);
                }
                // Commande pwd
                else if (strcmp(argv_execv[0],"pwd") == 0) {
                    char buff[200];
                    getcwd(buff, sizeof buff);
                    printf("%s\n", buff);
                }
                // Commande avec redirection ">"
                else if (chevron_flag != -1) {
                    redirection(argv_execv, chevron_flag);                               
                }
                // Commande avec pipe "|"
                else if (pipe_flag != -1) {
                    pipe_fct(argv_execv, pipe_flag); 
                }
                else if (sc_flag != -1) {
                    deleteTarget(argv_execv, sc_flag, ";");
                    argv_execv[sc_flag] = NULL;
                    execvp_fct(argv_execv, 0);
                    char **argv_execv_tempo = argv_execv + sc_flag + 1;
                    i -= sc_flag +1;
                    do {
                        sc_flag = test_caractere(argv_execv_tempo, i-1, ";");
                        if (sc_flag != -1) {
                            deleteTarget(argv_execv_tempo, sc_flag, ";");
                            argv_execv_tempo[sc_flag] = NULL;
                            execvp_fct(argv_execv_tempo, 0);
                            argv_execv_tempo = argv_execv_tempo + sc_flag + 1;
                            i -= 1 + sc_flag;
                        }
                    }while (sc_flag != -1);
                    argv_execv_tempo[sc_flag] = NULL;
                    execvp_fct(argv_execv_tempo, 0);
                }
            }
        }
    }

    return 0;
}