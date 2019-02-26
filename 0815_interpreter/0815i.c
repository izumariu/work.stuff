#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "0815_lib.h"

int64_t regX = 0, regY = 0, regZ = 0;
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

        if(bufc == -1) break;

        #ifdef __DEBUG__
        if(bufc < 32)
            DEBUG("Got %3d, ", bufc);
        else
            DEBUG("Got '%c', ", bufc);
        #endif

        switch(bufc) {
            case '<': DEBUG("calling push()"); push(fp); break;
            case 'x': DEBUG("calling swap()"); swap(fp); break;
            case 'X': DEBUG("calling swap()"); swap(fp); break;
            case '}': DEBUG("calling label()"); label(fp); break;
            case '|': DEBUG("calling _geti()"); _geti(); break;
            case '!': DEBUG("calling _getc()"); _getc(); break;
            case '%': DEBUG("calling _puti()"); _puti(); break;
            case '$': DEBUG("calling _putc()"); _putc(); break;
            case '~': DEBUG("calling rollRL()"); rollRL(); break;
            case '=': DEBUG("calling rollRR()"); rollRR(); break;
            case '^': DEBUG("calling jmpne()"); jmpne(fp); break;
            case '+': DEBUG("calling _add()"); _add(); break;
            case '-': DEBUG("calling _sub()"); _sub(); break;
            case '*': DEBUG("calling _mul()"); _mul(); break;
            case '/': DEBUG("calling _div()"); _div(); break;
            default: DEBUG("nothing happening"); break;
        }
        DEBUG("\n");
    }

    fclose(fp);

    DEBUG("X = 0x%lX ; Y = 0x%lX ; Z = 0x%lX\n", regX, regY, regZ);

    return 0;
}