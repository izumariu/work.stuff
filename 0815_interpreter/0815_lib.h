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

#define DUMP_REGS() DEBUG("X = 0x%lX ; Y = 0x%lX ; Z = 0x%lX\n", regX, regY, regZ)
#define DUMP_QUEUE() for(int __queue__dump__index__=0;__queue__dump__index__<queue.length;__queue__dump__index__++){DEBUG("%lX ", queue.list[__queue__dump__index__]);}; DEBUG("\n");

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

typedef struct {
    int64_t* list;
    uint16_t length;
} _queue;

extern int64_t regX, regY, regZ;
extern _labelList labels;
extern _queue queue;
extern FILE* fp;

extern void push();
extern void swap();
extern void label();
extern void _geti();
extern void _getc();
extern void _puti();
extern void _putc();
extern void rollRL();
extern void rollRR();
extern void jmpeq();
extern void jmpne();
extern void clearQ();
extern void enQ();
extern void deQ();
extern void rollQL();
extern void rollQR();
extern void _add();
extern void _sub();
extern void _mul();
extern void _div();

#endif