#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "0815_lib.h"

int64_t regX = 0, regY = 0, regZ = 0;
_labelList labels;
_queue queue;
FILE* fp;

int main(int argc, char** argv) {

    labels.length = 0;
    queue.length = 0;

    char bufc;

    if(argc != 2) {
        ERROR("USAGE: 0815i <0815 SCRIPT>\n");
        return 1;
    }

    fp = fopen(argv[1], "r");

    if(!fp) {
        ERROR("File '%s' could not be found or opened for reading\n", argv[1]);
        return 1;
    }

    while(!feof(fp)) {
        bufc = fgetc(fp);

        if(bufc == -1) break;

        #ifdef __DEBUG__
        if(bufc < 32)
            DEBUG("Got ASCII %3d, ", bufc);
        else
            DEBUG("Got '%c', ", bufc);
        #endif

        switch(bufc) {
            case '<': DEBUG("calling push()\n");   push();    break;
            case 'x': DEBUG("calling swap()\n");   swap();    break;
            case 'X': DEBUG("calling swap()\n");   swap();    break;
            case '}': DEBUG("calling label()\n");  label();   break;
            case '|': DEBUG("calling _geti()\n");  _geti();   break;
            case '!': DEBUG("calling _getc()\n");  _getc();   break;
            case '%': DEBUG("calling _puti()\n");  _puti();   break;
            case '$': DEBUG("calling _putc()\n");  _putc();   break;
            case '~': DEBUG("calling rollRL()\n"); rollRL();  break;
            case '=': DEBUG("calling rollRR()\n"); rollRR();  break;
            case '#': DEBUG("calling jmpeq()\n");  jmpeq();   break;
            case '^': DEBUG("calling jmpne()\n");  jmpne();   break;
            case '?': DEBUG("calling clearQ()\n"); clearQ();  break;
            case '>': DEBUG("calling enQ()\n");    enQ();     break;
            case '{': DEBUG("calling deQ()\n");    deQ();     break;
            case '@': DEBUG("calling rollQL()\n"); rollQL();  break;
            case '&': DEBUG("calling rollQR()\n"); rollQR();  break;
            case '+': DEBUG("calling _add()\n");   _add();    break;
            case '-': DEBUG("calling _sub()\n");   _sub();    break;
            case '*': DEBUG("calling _mul()\n");   _mul();    break;
            case '/': DEBUG("calling _div()\n");   _div();    break;
            default: DEBUG("nothing happening\n");            break;
        }
    }

    fclose(fp);

    DUMP_REGS();

    return 0;
}