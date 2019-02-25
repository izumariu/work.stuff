#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t* parseBounds(char*);

int main(int argc, char** argv) {

    if(argc != 3) {
        fprintf(stderr, "USAGE: range [from]-[to] <file>\n");
        return 1;
    }

    FILE* fp = fopen(argv[2], "r");

    int line = 1;
    char bufc;

    uint64_t* bounds = parseBounds(argv[1]);

    while(!feof(fp)) {
        bufc = fgetc(fp);
        if(bufc == '\n') line++;
        if( (bounds[0] > 0 ? (line >= bounds[0]) : 1) && (bounds[1] > 0 ? (line <= bounds[1]) : 1 ) ) {
            putchar(bufc);
        }
    }

    fclose(fp);
    free(bounds);

    return 0;

}

uint64_t* parseBounds(char* boundArgument) {

    uint64_t* ret = calloc(2, sizeof(uint64_t));
    uint8_t ptr = 0;

    for(int i = 0; boundArgument[i] != '\0'; i++) {

        if( (ptr >= 2 || boundArgument[i] < '0' || boundArgument[i] > '9') && boundArgument[i] != '-' ) {
            fprintf(stderr, "Specify the range using either '<number>-', '-<number>' or '<number>-<number>'\n");
            exit(1);
        }

        if( boundArgument[i] == '-' ) ptr++;
        else ret[ptr] = (ret[ptr] * 10) + (boundArgument[i] - 48);

    }

    if(ret[0] == 0 && ret[1] == 0) {
        fprintf(stderr, "Specify the range using either '<number>-', '-<number>' or '<number>-<number>'\n");
        exit(1);
    }

    return ret;

}
