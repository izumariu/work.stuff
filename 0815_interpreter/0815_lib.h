#ifndef __0815_LIB_H__
#define __0815_LIB_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __DEBUG__
#define DEBUG(m...) fprintf(stderr, m)
#else
#define DEBUG(m...) asm("nop")
#endif

#define ERROR(args...) fprintf(stderr, args)
#define ERROR_POS(args...) fprintf(stderr, "%s at character %lu\n", args, ftell(fp))

typedef struct {
    char* name;
    uint64_t offset;
} _label;

typedef struct {
    _label* list;
    uint16_t length;
} _labelList;

extern int64_t regX, regY, regZ;
extern _labelList labels;

extern void push(FILE*);
extern void swap(FILE*);
extern void label(FILE*);
extern void _geti();
extern void _getc();
extern void _puti();
extern void _putc();
extern void rollRL();
extern void rollRR();
extern void jmpeq(FILE*);
extern void jmpne(FILE*);
extern void clearq();
extern void enQ();
extern void deQ();
extern void rollQL(FILE*);
extern void rollQR(FILE*);
extern void _add();
extern void _sub();
extern void _mul();
extern void _div();

#endif