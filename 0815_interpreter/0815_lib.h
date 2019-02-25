#ifndef __0815_LIB_H__
#define __0815_LIB_H__
#include <stdio.h>
#include <stdlib.h>

#define ERROR(args...) fprintf(stderr, args)

typedef struct {
    char* name;
    unsigned long offset;
} _label;

typedef struct {
    _label* list;
    unsigned int length;
} _labelList;

extern long long regX, regY, regZ;

extern void push(FILE*);
extern void swap(FILE*);
extern void label(FILE*);

#endif