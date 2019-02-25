#include "sm64rng.h"

unsigned short rng64(unsigned short input){
    unsigned short S0 = (rngseed & 0xFF) << 8;
    S0 ^= rngseed;

    rngseed = (S0 << 8) + (S0 >> 8);
    S0 = rngseed ^ ((S0 & 0xFF) << 1);

    unsigned short S1 = (0xFF80 ^ (S0 >> 1));

    rngseed = S0 & 1 ? S1 ^ 0x8180 : S1 ^ 0x1FF4;

    return rngseed % input;
}