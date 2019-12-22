// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "misc.h"
#include "print.h"
#include "tetrisPiece.h"

uint16_t RngState;
uint16_t BagIndex;
uint8_t RandBags[TP_LEN*2];
uint8_t * const RandBag1 = RandBags;
uint8_t * const RandBag2 = RandBags+ TP_LEN;

/*
  A simple 16 bit random number generator
  https://lemire.me/blog/2019/07/03/a-fast-16-bit-random-number-generator/
  returns a number between 0 and 0xFFFF
 */
uint16_t randWord() {
    uint16_t res;
    RngState += 0xFC15;
    asm volatile("mov $0x2AB, %%dx\n"
                 "mul %%dx\n"
                 "xor %%dx, %%ax\n"
                 :"=ax"(res)
                 :"ax"(RngState)
                 :"dx");
    return res;
}
uint16_t getRandomTill(uint16_t s) {
    // Idk how it works, but it does
    uint16_t x = randWord();
    uint32_t m = (uint32_t)x * (uint32_t)s;
    uint16_t l = (uint16_t)m;
    if (l < s) {
        uint16_t t = -s % s;
        while (l < t) {
            x = randWord();
            m = (uint32_t)x * (uint32_t)s;
            l = (uint16_t)m;
        }
    }
    return m >> 16;
}
void randShuffle(uint8_t* ptr, uint16_t size) {
    for (int16_t i = size - 1; i >= 0; --i) {
        uint8_t t = ptr[i];
        uint16_t rand = getRandomTill(i);
        ptr[i] = ptr[rand];
        ptr[rand] = t;
    }
}
/*
#if DEBUG
void drawPiecePoint(uint16_t tileX, uint16_t tileY, PieceType type);
void printRandBags() {
    //moveCursor(0,0);
    for (uint16_t i = 0; i < TP_LEN; ++i) {
        //printInt(RandBag1[i]);
        drawPiecePoint(i, 0, RandBag1[i]);
    }
    //putc('\n');
    for (uint16_t i = 0; i < TP_LEN; ++i) {
        //printInt(RandBag2[i]);
        drawPiecePoint(i, 1, RandBag2[i]);
    }
}
#endif*/
void randRestart() {
    BagIndex = 0;
    
    randShuffle(RandBag1, TP_LEN);
    randShuffle(RandBag2, TP_LEN);
    //printRandBags();
}
uint16_t randSeed() { return time(); }
void initRand() {
    RngState = randSeed();
    for (uint16_t i = 0; i < TP_LEN; ++i) {
        RandBag1[i] = i;
        RandBag2[i] = i;
    }
    randRestart();
}
uint8_t incrementRand() {
    if (BagIndex >= TP_LEN) {
        BagIndex = 0;// -= TP_LEN;
        memcpy(RandBag1, RandBag2, TP_LEN);
        randShuffle(RandBag2, TP_LEN);
        //printRandBags();
    }
    return RandBags[BagIndex++];
}
uint8_t nextPiece(uint16_t i) {
    return RandBags[BagIndex + i];
}
