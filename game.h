// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "misc.h"

#define PF_WIDTH 10
#define VISIBLE_PF_HEIGHT 20
#define HIDDEN_PF_HEIGHT 5
#define PF_STRIDE 16
#define PF_HEIGHT (VISIBLE_PF_HEIGHT+HIDDEN_PF_HEIGHT)

uint8_t Playfield[PF_HEIGHT*PF_STRIDE];

bool Running;
uint16_t Level;
uint16_t Score;
uint16_t HiScore;
uint16_t ClearedLinesThisLevel;
uint16_t TotalClearedLines;

const uint16_t speeds[] = {
    // in ticks (1/18.2 seconds)
    //18*3,
    18,
    14,
    11,
    9,
    6,
    4,
    3,
    2,
    1,
    1,
    /*
    0.043,
    0.028,
    0.018,
    0.011,
    0.007,*/

};
uint16_t getSpeed() {
    uint16_t l = Level -1;
    if (l >= sizeof(speeds)/sizeof(*speeds))
        return 1;
    return speeds[l];
}
uint16_t requiredLines() {
    return Level * 5;
}
uint16_t goalLines() {
    return requiredLines() - ClearedLinesThisLevel;
}

/*
  Playfield resb (PlayfieldSizeY*16)
  PlayfieldSize equ $-Playfield

  VisiblePlayfield equ (Playfield+ (HiddenPlayfieldSizeY*16))
  VisiblePlayfieldSize equ (Height *16)
  -*/
uint8_t playfieldAt(uint16_t x, uint16_t y) {
    return Playfield[y * PF_STRIDE + x];
}

void setPlayfieldAt(uint16_t x, uint16_t y, uint8_t val) {
    Playfield[y * PF_STRIDE + x] = val;
}
/*
uint8_t visiblePlayfieldAt(uint16_t x, uint16_t y) {
    return VisiblePlayfield[y * PF_STRIDE + x];
    }*/
void resetGame() {
    Level = 1;
    Score = 0;
    HiScore = 0;
    ClearedLinesThisLevel = 0;
    TotalClearedLines = 0;
    Running = true;
    for (size_t i = 0; i < sizeof(Playfield) / sizeof(Playfield[0]); ++i)
        Playfield[i] = 0;
}
void pause() {
    exit(0);
}
void endGame() {
    Running = false;
}

void drawInfo();
void increaseScore(uint16_t s) {
    Score += s;
    if (Score > HiScore)
        HiScore = Score;
    drawInfo();
}
void clearLine(int y) {
    //print("c:");
    //printInt(y);
    ++TotalClearedLines;
    if (++ClearedLinesThisLevel >= requiredLines()) {
        ++Level;
        ClearedLinesThisLevel = 0;
    }

    memcpy(Playfield + y * PF_STRIDE,
           Playfield + (y + 1) * PF_STRIDE,
           sizeof(*Playfield) * PF_STRIDE * (PF_HEIGHT - y - 1));
    memset(Playfield + (PF_HEIGHT - 1) * PF_STRIDE, 0, PF_WIDTH);
}
void drawPlayfield();
void checkClearedLines() {
    unsigned y = 0;
    int clearedLines = 0;
    while (y < PF_HEIGHT) {
        for (unsigned x = 0; x < PF_WIDTH; ++x) {
            if (!playfieldAt(x, y))
                goto nextLine;
        }
        clearLine(y);
        ++clearedLines;
        continue; //redo line
    nextLine:
        ++y;
    }
    switch (clearedLines) {
    case 0: return;
    case 1: increaseScore(Level * 40); break;
    case 2: increaseScore(Level * 100); break;
    case 3: increaseScore(Level * 300); break;
    case 4: increaseScore(Level * 1200); break;
    }
    drawPlayfield();
}
