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

/* PRIVATE */ _label createLabel(char* name, uint64_t offset) {
    _label r;
    r.name = name;
    r.offset = offset;
    return r;
}

/* PRIVATE */ void pushLabel(char* label, uint16_t offset) {
    if(!labels.list)
        labels.list = malloc(sizeof(_label) * ++labels.length);
    else
        labels.list = realloc(labels.list, sizeof(_label) * ++labels.length);
    labels.list[labels.length - 1] = createLabel(label, offset);
}

/* PRIVATE */ uint64_t findLabelOffset(FILE* fp, char* label) {

    for(int i = 0; i < labels.length; i++)
        if(strcmp(labels.list[i].name, label) == 0)
            return labels.list[i].offset;

    ERROR_POS("FATAL: Undefined label");
    exit(1);

}

void push() {
    int64_t buf = 0;
    uint8_t length;
    char charbuf = fgetc(fp);

    if(charbuf != ':') return;

    for(length = 0; length <= 64; length++) {
        charbuf = fgetc(fp);
        if(charbuf == ':') break;
        DEBUG("push(): got char '%c'\n", charbuf);
        buf = (buf * 16) + hexCharToDec(charbuf);
    }

    if(length == 0) {
        ERROR_POS("ERROR: Push was called, but no value was provided");
        exit(1);
    } else if(length == 64) {
        if(fgetc(fp) != ':' && charbuf != ':') {
            ERROR_POS("FATAL: Overflow of value found");
            exit(1);
        }
    }

    DEBUG("push(): final value is 0x%lx(%lu)\n", buf, buf);
    regX = buf;
    DEBUG("push(): X => 0x%lx\n", regX);

}

void swap() {
    int64_t temp = regX;
    regX = regY;
    regY = temp;
    DEBUG("swap(): X <=> Y\n");
}

void label() {

    uint16_t length = 0;
    char buf = fgetc(fp);
    if(buf != ':') return;

    char *label = calloc(1, 1); // VALUE MUST NOT BE FREE()D!!

    if(fgetc(fp) == ':') {
        ERROR_POS("FATAL: Label was called, but no label was provided");
        exit(1);
    }

    fseek(fp, -1, SEEK_CUR);

    for(;;) {
        buf = fgetc(fp);
        if(buf == ':') break;
        label = realloc(label, ++length);
    }

    label = realloc(label, ++length);
    label[length-1] = 0;

    pushLabel(label, ftell(fp));

}

void _geti() {
    DEBUG("_geti(): calling scanf(\"%%lx\", &regX)\n");
    scanf("%lx", &regX);
}

void _getc() {
    DEBUG("_getc(): calling scanf(\"%%c\", &buf)\n");
    char buf;
    scanf("%c", &buf);
    DEBUG("_getc(): X = (int16_t)buf\n");
    regX = (int16_t)buf;
}

void _puti() {
    DEBUG("_puti(): printf(\"%%lx\", regZ)\n");
    printf("%lx", regZ);
}

void _putc() {
    DEBUG("_putc(): putchar(regZ %% 256)\n");
    putchar(regZ % 256);
}

void rollRL() {
    DUMP_REGS();
    DEBUG("rollRL(): tmp = X\n");
    int64_t tmp = regX;
    DEBUG("rollRL(): X = Y\n");
    regX = regY;
    DEBUG("rollRL(): Y = Z\n");
    regY = regZ;
    DEBUG("rollRL(): Z = tmp\n");
    regZ = tmp;
    DUMP_REGS();
}

void rollRR() {
    DUMP_REGS();
    DEBUG("rollRR(): tmp = Z\n");
    int64_t tmp = regZ;
    DEBUG("rollRR(): Z = Y\n");
    regZ = regY;
    DEBUG("rollRR(): Y = X\n");
    regY = regX;
    DEBUG("rollRR(): X = tmp\n");
    regX = tmp;
    DUMP_REGS();
}

void jmpeq() {

    uint16_t length = 0;
    char buf = fgetc(fp);
    if(buf != ':') return;

    char *label = calloc(1, 1); // VALUE MUST NOT BE FREE()D!!

    if(fgetc(fp) == ':') {
        ERROR_POS("FATAL: JumpIfNotZero was called, but no label was provided");
        exit(1);
    }

    fseek(fp, -1, SEEK_CUR);

    for(;;) {
        buf = fgetc(fp);
        if(buf == ':') break;
        label = realloc(label, ++length);
    }

    label = realloc(label, ++length);
    label[length-1] = '\0';

    uint64_t off = findLabelOffset(fp, label);

    DEBUG("jmpeq(): found label offset at %lu\n", off);

    if(regZ == 0) {
        DEBUG("jmpeq(): Z is 0, calling fseek(fp, %ld, SEEK_SET)\n", off);
        fseek(fp, off, SEEK_SET);
    }

}

void jmpne() {

    uint16_t length = 0;
    char buf = fgetc(fp);
    if(buf != ':') return;

    char *label = calloc(1, 1); // VALUE MUST NOT BE FREE()D!!

    if(fgetc(fp) == ':') {
        ERROR_POS("FATAL: JumpIfNotZero was called, but no label was provided");
        exit(1);
    }

    fseek(fp, -1, SEEK_CUR);

    for(;;) {
        buf = fgetc(fp);
        if(buf == ':') break;
        label = realloc(label, ++length);
    }

    label = realloc(label, ++length);
    label[length-1] = '\0';

    uint64_t off = findLabelOffset(fp, label);

    DEBUG("jmpne(): found label offset at %lu\n", off);

    if(regZ != 0) {
        DEBUG("jmpne(): Z is not 0, calling fseek(fp, %ld, SEEK_SET)\n", off);
        fseek(fp, off, SEEK_SET);
    }

}

void clearQ() {

    if(queue.list) {
        DEBUG("clearQ(): calling free(queue.list)\n");
        free(queue.list);
    }

    queue.length = 0;

}

void enQ() {

    if(!queue.list) {
        queue.list = malloc(++queue.length * sizeof(_queue));
    } else {
        queue.list = realloc(queue.list, sizeof(_queue) * ++queue.length);
    }

    queue.list[queue.length - 1] = regZ;

}

void deQ() {

    if(!queue.list) {
        ERROR_POS("ERROR: Requested dequeue, but queue length was 0");
    }

    regX = *(queue.list);

    for(int i = 0; i <= queue.length - 2; i++) {
        queue.list[i] = queue.list[i+1];
    }

    if(--queue.length > 0)
        queue.list = realloc(queue.list, queue.length);
    else
        clearQ();

}

void rollQL() {

    DUMP_QUEUE();

    int64_t buf = 0;
    uint8_t length;
    char charbuf = fgetc(fp);

    if(charbuf == ':') {
        for(length = 0; length <= 64; length++) {
            charbuf = fgetc(fp);
            if(charbuf == ':') break;
            DEBUG("push(): got char '%c'\n", charbuf);
            buf = (buf * 16) + hexCharToDec(charbuf);
        }

        if(length == 0) {
            ERROR_POS("ERROR: RollQueueLeft was called using argument syntax, but no argument was provided");
        } else if(length == 64) {
            if(fgetc(fp) != ':' && charbuf != ':') {
                ERROR_POS("FATAL: Overflow of value found");
                exit(1);
            }
        }
    } else {
        fseek(fp, -1, SEEK_CUR);
        buf = 1;
    }

    int64_t tmp;

    for(int64_t times = 0; times < buf; times++) {

        tmp = *(queue.list);

        for(int i = 0; i <= queue.length - 2; i++)
            queue.list[i] = queue.list[i+1];

        queue.list[queue.length - 1] = tmp;
        DUMP_QUEUE();

    }

}

void rollQR() {

    DUMP_QUEUE();

    int64_t buf = 0;
    uint8_t length;
    char charbuf = fgetc(fp);

    if(charbuf == ':') {
        for(length = 0; length <= 64; length++) {
            charbuf = fgetc(fp);
            if(charbuf == ':') break;
            DEBUG("push(): got char '%c'\n", charbuf);
            buf = (buf * 16) + hexCharToDec(charbuf);
        }

        if(length == 0) {
            ERROR_POS("ERROR: RollQueueLeft was called using argument syntax, but no argument was provided");
        } else if(length == 64) {
            if(fgetc(fp) != ':' && charbuf != ':') {
                ERROR_POS("FATAL: Overflow of value found");
                exit(1);
            }
        }
    } else {
        fseek(fp, -1, SEEK_CUR);
        buf = 1;
    }

    int64_t tmp;

    for(int64_t times = 0; times < buf; times++) {

        tmp = queue.list[queue.length-1];

        for(int i = queue.length - 2; i >= 0; i--)
            queue.list[i+1] = queue.list[i];

        *(queue.list) = tmp;

        DUMP_QUEUE();

    }

}

void _add() {
    DEBUG("_add(): Z = X + Y\n");
    regZ = regX + regY;
}

void _sub() {
    DEBUG("_sub(): Z = X - Y\n");
    regZ = regX - regY;
}

void _mul() {
    DEBUG("_mul(): Z = X * Y\n");
    regZ = regX * regY;
}

void _div() {
    DEBUG("_div(): Z = X / Y\n");
    regZ = regX / regY;
    DEBUG("_div(): Y = X %% Y\n");
    regY = regX % regY;
}