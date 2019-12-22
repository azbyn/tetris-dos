// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "print.h"

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320
#define TOP_VISIBLE_PX 2

typedef uint8_t Color;

uint8_t* const ScreenData = ((uint8_t*) 0xA0000) - 20*SCREEN_WIDTH - 32;

void pokeScreen(uint16_t index, uint8_t val) {
    
    /*asm volatile ("mov $0x0A0000, %%edi\n"
                  "mov (%%edi, %%ebx), %%al"
                  :
                  :"al"(val), "bx"(val)
                  :"edi");*/
    //*((uint8_t*)0xA0000+index) = val;
    /*
    asm volatile("mov %%al, %%es:(%%bx)\n"
                 :
                 : "al"(val),"bx"(index)
                 :);*/
    ScreenData[index] = val;
}

//uint8_t PrevVgaMode;
void setVgaMode(uint8_t mode) {
    asm volatile("xor %%ah, %%ah\n"
                 "int $0x10\n"
                 :
                 : "al"(mode)
                 : "cc");
}
void initVga() {
    /* asm volatile ( */
    /*     "mov $15, %%ah\n" */
    /*     "int $0x10\n" */
    /*     //"mov $0xA000, %%bx\n" */
    /*     //"mov %%bx, %%es\n" */
    /*     : "=al"(PrevVgaMode) */
    /*     : */
    /*     : /\*"bx"*\/); */
    setVgaMode(0x13);
}
void delVga() {
    setVgaMode(0x3/* PrevVgaMode*/);
}
void fill(Color col) {
    for (uint16_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i)
        pokeScreen(i, col);
}
void fillTile(uint16_t tileX, uint16_t tileY, Color col) {
    uint16_t it = (tileY * 8+TOP_VISIBLE_PX) * SCREEN_WIDTH;
    it += tileX*8;
    uint16_t endL = it +8;
    uint16_t end = it + SCREEN_WIDTH *8;
    //do while generates less code
    do {
        do {
            pokeScreen(it, col);
            ++it;
        } while(it < endL);
        endL += SCREEN_WIDTH;
        it += SCREEN_WIDTH - 8;
    } while (it < end);
    /*
    for (; it < end; endL += ScreenWidth, it += ScreenWidth-8)
        for (; it < endL; ++it)
        pokeScreen(it, col);*/
}
void drawTile(uint16_t tileX, uint16_t tileY, const uint8_t* tileData, const Color* palette) {
    uint16_t it = (tileY * 8+TOP_VISIBLE_PX) * SCREEN_WIDTH;
    it += tileX*8;
    uint16_t endL = it +8;
    uint16_t end = it + SCREEN_WIDTH *8;

    //do while generates less code
    do {
        do {
            Color col = palette[*tileData++];
            pokeScreen(it, col);
            ++it;
        } while(it < endL);
        endL += SCREEN_WIDTH;
        it += SCREEN_WIDTH - 8;
    } while (it < end);
}
// move the cursor
void moveCursor(uint8_t x, uint8_t y) {
    //uint16_t dx = y << 8 | x;
    asm volatile("mov %0, %%dh\n"
                 "mov %1, %%dl\n"
                 "mov $0, %%bh\n"
                 "mov $2, %%ah\n"
                 "int $0x10\n"
                 :
                 : "r"(y), "r"(x) //"dl"(x), "r"(y)
                 // : "dl"(x), "dh"(y)
                 :"ah", "bh");
}
/*
  %macro moveCursor 2 ;(y, x)
  mov ah, 2
  mov bh, 0         ;page
  mov dl, %2        ;col
  mov dh, %1        ;row
  int 10h
  %endmacro

  %macro movePrint 3 ;(y, x, msg)
  moveCursor %1, %2
  printStr %3
  %endmacro

 */

void fillRect(uint16_t startX, uint16_t startY,
              uint16_t sizeX, uint16_t sizeY, Color col) {
    uint16_t it = startY * SCREEN_WIDTH;
    it += startX;
    uint16_t endL = it + sizeX;
    uint16_t end = it + SCREEN_WIDTH *sizeY;
    uint16_t inc = SCREEN_WIDTH - sizeX;

    //do while generates less code
    do {
        do {
            pokeScreen(it, col);
            ++it;
        } while(it < endL);
        endL += SCREEN_WIDTH;
        it += inc;
    } while (it < end);
}
