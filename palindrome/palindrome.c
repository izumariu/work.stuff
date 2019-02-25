#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "palindrome_macros.h"
#include "sm64rng.h"

BOOL looksLikeInteger(char*);
char getRandChar();
unsigned short rngseed;

int main(int argc, char** argv) {

    rngseed = (unsigned short)time(NULL);

    if(argc != 2) {
        PANIC("USAGE: %s <NUMBER OF CHARACTERS>\n", __FILE__);
    }

    if(!looksLikeInteger(argv[1])) {
        PANIC("Argument 1 must be an unsigned integer!\n");
    }

    int palin_length = atoi(argv[1]);
    int half_length = ((palin_length - (palin_length % 2)) / 2);
    char* palin = malloc(half_length);
    int palin_ptr = 0;

    for( int t = 0 ; t < half_length ; t++ ) {
        palin[palin_ptr] = getRandChar();
        putchar(palin[palin_ptr++]);
    }

    if(palin_length % 2 == 1)
        putchar(getRandChar());

    while(palin_ptr >= 0)
        putchar(palin[--palin_ptr]);

    free(palin);

    putchar('\n');

    return 0;

}

BOOL looksLikeInteger(char* subject) {

    for( int ptr = 0 ; subject[ptr] != '\0' ; ptr++ )
        if(subject[ptr] < '0' || subject[ptr] > '9')
            return FALSE;

    return TRUE;

}

char getRandChar() {

    int cset = rng64(3);
    int rint;
    char rchr;

    switch(cset) {
        case 0:
            rint = rng64(26);
            rchr = rint + 65;
            break;
        case 1:
            rint = rng64(26);
            rchr = rint + 97;
            break;
        case 2:
            rint = rng64(26);
            rchr = rint + 48;
            break;
        default:
            printf("Oops. %d\n", cset);
            break;
    }

    return rchr;

}