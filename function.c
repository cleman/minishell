#include "function.h"
#include <string.h>
#include <stdio.h>

int back(char **argv_execv, int i) {
    size_t len_tab = strlen(argv_execv[i-1]);

    if (len_tab == 1 && strcmp(argv_execv[i-1],"&") == 0) {
        argv_execv[i-1] = NULL;
        return 1;
    }
    if (argv_execv[i-1][len_tab-1] == '&') {
        argv_execv[i-1][len_tab-1] = '\0';
        return 1;
    }
    return 0;
}