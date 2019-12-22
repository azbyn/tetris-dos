// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "tetrisPiece.h"
#include "graphics.h"
#include "game.h"
#include "random.h"
#include "player.h"

#define START_X (((SCREEN_WIDTH / 8) - PF_WIDTH) /2)
#define VISIBLE_NEXT_PIECES 3

#define GHOST_COLOR 0x12

const uint8_t TetrominoTile[8*8] = {
    1,1,1,1, 1,1,1,1,
    2,1,1,1, 1,1,1,2,
    2,2,0,0, 0,0,2,2,
    2,2,0,0, 0,0,2,2,

    2,2,0,0, 0,0,2,2,
    2,2,0,0, 0,0,2,2,
    2,3,3,3, 3,3,3,2,
    3,3,3,3, 3,3,3,3,
};

/*
// pure, lighter, close_but_muted, darker
Color WallPalette[4] = {
    8, 24, 21, 19//; gray
};
*/
const Color PiecePalettes[TP_LEN+1][4] = {
    [TP_I] = {52, 11, 53, 03}, // cyan
    [TP_L] = {32, 9, 55, 1},   // blue
    [TP_J] = {41, 6, 42, 114}, // orange
    [TP_O] = {44, 45, 43, 14}, // yellow
    [TP_S] = {48, 10, 47, 2},  // green
    [TP_T] = {36, 13, 34, 5},  // magenta
    [TP_Z] = {40, 12, 39, 4},  // red
    [TP_WALL] = {8, 24, 21, 19}//; gray
};
Color const *WallPalette = PiecePalettes[TP_WALL];

void drawPiecePoint(uint16_t tileX, uint16_t tileY, PieceType type) {
    drawTile(tileX, tileY, TetrominoTile, PiecePalettes[type]);
}


void hTileLine(uint16_t x, uint16_t y, uint16_t size, PieceType type) {
    for (int i = x, end = x + size; i < end; ++i)
        drawPiecePoint(i, y, type);
}

void vTileLine(uint16_t x, uint16_t y, uint16_t size, PieceType type) {
    for (int j = y, end = y + size; j < end; ++j)
        drawPiecePoint(x, j, type);
}
void drawBg() {
    // playfield border:
    hTileLine(START_X, VISIBLE_PF_HEIGHT, PF_WIDTH, TP_WALL);
    vTileLine(START_X-1, 0,VISIBLE_PF_HEIGHT+1, TP_WALL);
    vTileLine(START_X+PF_WIDTH, 0,VISIBLE_PF_HEIGHT+1, TP_WALL);
    // next pieces border:
    vTileLine(START_X + PF_WIDTH+1+4, 2, 3*VISIBLE_NEXT_PIECES, TP_WALL);
    hTileLine(START_X+PF_WIDTH+1, 1, 5, TP_WALL);
    hTileLine(START_X+PF_WIDTH+1, 2+3*VISIBLE_NEXT_PIECES, 5, TP_WALL);
    moveCursor(START_X+PF_WIDTH+1, 0);
    print("Next: ");
    //hold border:

    hTileLine(START_X-6, 1, 5, TP_WALL);
    vTileLine(START_X-6, 2, 3, TP_WALL);
    hTileLine(START_X-6, 5, 5, TP_WALL);
    //printInt(BagIndex);
}
void fillPlayfield() {
    fillRect(START_X*8, TOP_VISIBLE_PX, PF_WIDTH*8, VISIBLE_PF_HEIGHT*8, 0);
}
void drawPiece(uint16_t tileX, uint16_t tileY, PieceType p) {
    fillRect(tileX * 8, tileY*8+TOP_VISIBLE_PX, 4*8, 3*8, 0);
    for (size_t i = 0; i < 4; ++i) {
        Point8 pt = PieceRotations[p][0][i];
        drawPiecePoint(pt.x+ tileX, (3- pt.y) + tileY, p);
    }
}
void drawNext() {
    for (size_t i = 0; i < VISIBLE_NEXT_PIECES; ++i)
        drawPiece(START_X+PF_WIDTH+1, 2+3*i, nextPiece(i));
}
void fillPfPoints(const PiecePoints pp, Color col) {
    for (size_t i = 0; i < 4; ++i) {
        Point8 pt = pp[i];
        if (pt.y >= VISIBLE_PF_HEIGHT) continue;
        fillTile(START_X+pt.x, PF_HEIGHT - pt.y-6, col);
    }
}
void drawPfPoint(uint16_t x, uint16_t y, PieceType type) {
    if (y >= VISIBLE_PF_HEIGHT) return;
    drawTile(START_X+ x, PF_HEIGHT - y - 6/* + 1*/,
             TetrominoTile, PiecePalettes[type]);
}

void drawGhostPoints() {
    //print("DGP");
    fillPfPoints(PlayerGhostPoints, GHOST_COLOR);
}


void drawPlayer() {
    for (size_t i = 0; i < 4; ++i) {
        Point8 pt = PlayerPiecePoints[i];
        drawPfPoint(pt.x, pt.y, PlayerPieceType);
        //printChar(' ');
        //printInt(pt.y);
    }
    /*
      setcol(player.GetPieceType() + 1);
      for (auto pt : player.GetPoints()) {
      DrawBlock(pt);
      }*/
}
void drawInit() {
    drawBg();
    fillPlayfield();
    drawNext();
    drawGhostPoints();
    drawPlayer();
}
void drawPlayfield() {
    fillPlayfield();
    for (size_t y = 0; y < VISIBLE_PF_HEIGHT; ++y)
        for (size_t x = 0; x < PF_WIDTH; ++x) {
            uint8_t p = playfieldAt(x, y);
            if (p > 0)
                drawPfPoint(x, y, p-1);
        }
}
void drawVal(uint8_t x, uint8_t y, const char* msg, uint16_t num) {
    moveCursor(x, y);
    puts(msg);
    moveCursor(x+2, y+1);
    printInt(num);
}
void drawInfo() {
    const int x = START_X + PF_WIDTH + 1;
    drawVal(1,VISIBLE_PF_HEIGHT - 4, "Level:", Level);
    drawVal(1,VISIBLE_PF_HEIGHT - 2, "Goal:",  goalLines());
    drawVal(x,VISIBLE_PF_HEIGHT - 7, "Highscore:", HiScore);
    drawVal(x,VISIBLE_PF_HEIGHT - 4, "Score:", Score);
    drawVal(x,VISIBLE_PF_HEIGHT - 2, "Cleared Lines:", TotalClearedLines);
}
void drawHold() {
    if (HoldPiece != TP_EMPTY)
        drawPiece(START_X - 5, 2, HoldPiece);
}
void drawEndScreen() {
    fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    uint16_t x = SCREEN_WIDTH/16;
    uint16_t y = SCREEN_HEIGHT/16;
    moveCursor(x-5, y-2);
    print("Game Over");
    moveCursor(x- 3, y);
    print("Score:");
    moveCursor(x- 3, y+1);
    printInt(Score);
    moveCursor(x- 7, y+3);
    print("Press q to quit");
}
