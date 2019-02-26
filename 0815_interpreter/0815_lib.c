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

    uint16_t length = 0;
    char buf = fgetc(fp);
    if(buf != ':') return;

    char *label = calloc(1, 1); // VALUE MUST NOT BE FREE()D!!

    if(fgetc(fp) == ':') {
        ERROR_POS("\nFATAL: Label was called, but no label was provided");
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
    DEBUG("\n_geti(): calling scanf(\"%%lx\", &regX)");
    scanf("%lx", &regX);
}

void _getc() {
    DEBUG("\n_getc(): calling scanf(\"%%c\", &buf)");
    char buf;
    scanf("%c", &buf);
    DEBUG("\n_getc(): X = (int16_t)buf");
    regX = (int16_t)buf;
}

void _puti() {
    DEBUG("\n_puti(): printf(\"%%lx\", regZ)");
    printf("%lx", regZ);
}

void _putc() {
    DEBUG("\n_putc(): putchar(regZ %% 256)");
    putchar(regZ % 256);
}

void rollRL() {
    DEBUG("\nrollRL(): tmp = X");
    int64_t tmp = regX;
    DEBUG("\nrollRL(): X = Y");
    regX = regY;
    DEBUG("\nrollRL(): Y = Z");
    regY = regZ;
    DEBUG("\nrollRL(): Z = tmp");
    regZ = tmp;
}

void rollRR() {
    DEBUG("\nrollRR(): tmp = Y");
    int64_t tmp = regY;
    DEBUG("\nrollRR(): X = Z");
    regX = regZ;
    DEBUG("\nrollRR(): Y = X");
    regY = regX;
    DEBUG("\nrollRR(): Z = tmp");
    regZ = tmp;
}

void jmpeq(FILE* fp) {

    uint16_t length = 0;
    char buf = fgetc(fp);
    if(buf != ':') return;

    char *label = calloc(1, 1); // VALUE MUST NOT BE FREE()D!!

    if(fgetc(fp) == ':') {
        ERROR_POS("\nFATAL: JumpIfNotZero was called, but no label was provided");
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

    DEBUG("\njmpeq(): found label offset at %lu", off);

    if(regZ == 0) {
        DEBUG("\njmpeq(): Z is 0, calling fseek(fp, %ld, SEEK_SET)", off);
        fseek(fp, off, SEEK_SET);
    }

}

void jmpne(FILE* fp) {

    uint16_t length = 0;
    char buf = fgetc(fp);
    if(buf != ':') return;

    char *label = calloc(1, 1); // VALUE MUST NOT BE FREE()D!!

    if(fgetc(fp) == ':') {
        ERROR_POS("\nFATAL: JumpIfNotZero was called, but no label was provided");
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

    DEBUG("\njmpne(): found label offset at %lu", off);

    if(regZ != 0) {
        DEBUG("\njmpne(): Z is not 0, calling fseek(fp, %ld, SEEK_SET)", off);
        fseek(fp, off, SEEK_SET);
    }

}

void _add() {
    DEBUG("\n_add(): Z = X + Y");
    regZ = regX + regY;
}

void _sub() {
    DEBUG("\n_sub(): Z = X - Y");
    regZ = regX - regY;
}

void _mul() {
    DEBUG("\n_mul(): Z = X * Y");
    regZ = regX * regY;
}

void _div() {
    DEBUG("\n_div(): Z = X / Y");
    regZ = regX / regY;
    DEBUG("\n_div(): Y = X %% Y");
    regY = regX % regY;
}