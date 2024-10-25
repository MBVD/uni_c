#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LEN 256


int main (int argc, char* argv[]) {
    struct stat buf;
    printf("%d", argc);
    for (int i = 0; i<argc; i++){
        printf(argv[i]);
        if (lstat(argv[i], &buf) < 0){
            perror("error with lstat");
            exit(1);
        }
        if (S_ISREG(buf.st_mode)){
            printf("default file");
        }
        if (S_ISDIR(buf.st_mode)){
            printf("catalog");
        }
    }
    exit(0);
}