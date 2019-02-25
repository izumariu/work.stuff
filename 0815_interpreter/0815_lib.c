#include "0815_lib.h"

void push(FILE* fp) {

}

void swap(FILE* fp) {
    long long temp = regX;
    regX = regY;
    regY = temp;
    fgetc(fp);
}

void label(FILE* fp) {
    char buf = fgetc(fp);
    if(buf != ':') return;

}