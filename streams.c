#include <stdio.h>

int main () {
    FILE* file = fopen("file", "r+");
    char* str;
    fgets(file, &str, 'r+');
    printf("%s \n", str);
}