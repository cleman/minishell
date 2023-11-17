#ifndef FUNCTION
#define FUNCTION

void execvp_fct(char **argv_execv, int back_flag);
int deleteTarget(char **argv_execv, int i, char *target);
int test_caractere(char **argv_execv, int i, char *caractere);
int test_entry_void(char entry[50]);
void redirection(char **argv_execv, int chevron_flag);
void pipe_fct(char **argv_execv, int pipe_flag);

#endif