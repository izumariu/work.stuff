#include "0815_lib.h"

/* PRIVATE */ uint8_t hexCharToDec(char c) {
    if(c >= 48 && c <= 57)
        return (c - 48);
    else if(c >= 65 && c <= 70)
        return (c - 55);
    else if(c >= 97 && c <= 102)
        return (c - 87);
    ERROR("FATAL: '%c' is not a valid hexadecimal character", c);
    exit(1);
}

/* PRIVATE */ void pushLabel(char* label, uint16_t offset) {

}

void push(FILE* fp) {
    int64_t buf = 0;
    uint8_t length;
    char charbuf = fgetc(fp);

    if(charbuf != ':') return;

    for(length = 0; length <= 64; length++) {
        charbuf = fgetc(fp);
        if(charbuf == ':') break;
        DEBUG("\npush(): got char '%c'", charbuf);
        buf = (buf * 16) + hexCharToDec(charbuf);
    }

    if(length == 0) {
        ERROR_POS("\nFATAL: Push was called, but no value was provided");
        exit(1);
    } else if(length == 64) {
        if(fgetc(fp) != ':' && charbuf != ':') {
            ERROR_POS("\nFATAL: Overflow of value found");
            exit(1);
        }
    }

    DEBUG("\npush(): final value is 0x%lx(%lu)", buf, buf);
    regX = buf;
    DEBUG("\npush(): X => 0x%064lx", regX);

}

void swap(FILE* fp) {
    int64_t temp = regX;
    regX = regY;
    regY = temp;
    DEBUG("\nswap(): X <=> Y");
}

void label(FILE* fp) {
    char buf = fgetc(fp);
    if(buf != ':') return;
}