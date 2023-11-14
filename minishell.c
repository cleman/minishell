#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) return 0;

    char path[50];
    char *argv_execv[2];
    argv_execv[1] = NULL;

    if (strcmp(argv[1],"ls") || strcmp(argv[1],"who")) {
        argv_execv[0] = argv[1];
        strcpy(path,"/bin/");
        strcat(path, argv[1]);
        execv(path, argv_execv);
    }
}