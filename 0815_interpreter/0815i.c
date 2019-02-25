#include <stdio.h>
#include <stdlib.h>
#include "0815_lib.h"
#ifdef __DEBUG__
#define DEBUG(m...) fprintf(stderr, m)
#else
#define DEBUG(m...) asm("nop")
#endif

long long regX = 0, regY = 0, regZ = 0;
_labelList labels;

int main(int argc, char** argv) {


    labels.length = 0;

    char bufc;

    if(argc != 2) {
        ERROR("USAGE: 0815i <0815 SCRIPT>\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");

    if(!fp) {
        ERROR("File '%s' could not be found or opened for reading\n", argv[1]);
        return 1;
    }

    while(!feof(fp)) {
        bufc = fgetc(fp);
        DEBUG("Got '%c', ", bufc);
        switch(bufc) {
            case '<': DEBUG("calling push()"); push(fp); break;
            case 'x': DEBUG("calling swap()"); swap(fp); break;
            case 'X': DEBUG("calling swap()"); swap(fp); break;
            case '}': DEBUG("calling label()"); label(fp); break;
            case '\n':
            default: DEBUG("nothing happening");
        }
        DEBUG("\n");
    }

    fclose(fp);
    return 0;
}