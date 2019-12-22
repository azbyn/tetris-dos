// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "tetrisPiece.h"
#include "random.h"
#include "game.h"

PiecePoints PlayerPiecePoints;
PiecePoints PlayerGhostPoints;
uint16_t PlayerRot;
Point8 PlayerPos;
PieceType PlayerPieceType;
PieceType HoldPiece;
bool LastWasHold;
uint8_t LastDropTime;

void drawHold();
void drawPlayer();
void drawPlayfield();
void incrementPlayerPiece();
void drawNext();
void fillPfPoints(const PiecePoints pp, uint8_t col);
void drawGhostPoints();


void getGhostPointsBackup(PiecePoints bak) {
    for (size_t i = 0; i < 4; ++i)
        bak[i] = PlayerGhostPoints[i];
}
void restoreGhostPointsBackup(const PiecePoints bak) {
    for (size_t i = 0; i < 4; ++i)
        PlayerGhostPoints[i] = bak[i];
}

void resetDrop() {
    LastDropTime = time8();
}
void placeOnPf(const PiecePoints pp) {
    for (size_t i = 0; i < 4; ++i) {
        Point8 pt = pp[i];
        setPlayfieldAt(pt.x, pt.y, PlayerPieceType + 1);
    }
    checkClearedLines();

    incrementPlayerPiece();
    //drawPlayfield();
    drawPlayer();
    LastWasHold = false;

}
void updatePtsBase(PiecePoints pts, Point8 p) {
    for (size_t i = 0; i < 4; ++i)
        pts[i] = addPoints(PieceRotations[PlayerPieceType][PlayerRot][i], p);
}
void updatePlayerPoints() {
    updatePtsBase(PlayerPiecePoints, PlayerPos);
}

void updateGhostPoints() {
    Point8 p = PlayerPos;
    for (;;) {
        for (int i = 0; i < 4; ++i) {
            PlayerGhostPoints[i] = addPoints(PieceRotations[PlayerPieceType][PlayerRot][i], p);
            Point8 gp = PlayerGhostPoints[i];
            //printInt(gp.y);
            //print("j");
            if (playfieldAt(gp.x, gp.y) || (int8_t)(gp.y) < 0/* VISIBLE_PF_HEIGHT+2*/) {
                ++p.y;
                updatePtsBase(PlayerGhostPoints, p);
                /* print("ugp"); */
                /* printInt(PlayerGhostPoints[0].x); */
                /* print(","); */
                /* printInt(PlayerGhostPoints[0].y); */
                //drawGhostPoints(PlayerGhostPoints);

                return;
            }
        }
        --p.y;
    }
}
void updateGhostPointsAndDrawAll(const PiecePoints bak) {
    PiecePoints gbak;
    getGhostPointsBackup(gbak);
    updateGhostPoints();
    fillPfPoints(gbak, 0);
    fillPfPoints(bak, 0);
    drawGhostPoints();

    drawPlayer();
}

void fall();
void playerSetPiece(PieceType pt) {
    //if (p == TP_I && game.Matrix(4, Height - 1)) game.End();
    PlayerPos.x = 3;
    PlayerPos.y = VISIBLE_PF_HEIGHT - 2;
    PlayerRot = 0;
    PlayerPieceType = pt;
    updatePlayerPoints();
    for (size_t i = 0; i < 4; ++i) {
        Point8 p = PlayerPiecePoints[i];
        if (playfieldAt(p.x, p.y)) {
            for (size_t j = 0; j < 4; ++j) {
                ++PlayerPos.y; //just visual
                updatePlayerPoints();
                endGame();
                return;
            }
        }
    }
    fall();
    updateGhostPoints();
    drawGhostPoints();
    resetDrop();
}
void incrementPlayerPiece() {
    playerSetPiece(incrementRand());
    drawNext();
}
void restartPlayer() {//restart when game restarts
    LastWasHold = false;
    HoldPiece = TP_EMPTY;

    incrementPlayerPiece();
}
void getPointsBackup(PiecePoints bak) {
    for (size_t i = 0; i < 4; ++i)
        bak[i] = PlayerPiecePoints[i];
}
void restorePointsBackup(const PiecePoints bak) {
    for (size_t i = 0; i < 4; ++i)
        PlayerPiecePoints[i] = bak[i];
}

void fall() {
    --PlayerPos.y;
    PiecePoints bak;
    getPointsBackup(bak);
    updatePlayerPoints();
    for (size_t i = 0; i < 4; ++i){
        Point8 pt = PlayerPiecePoints[i];
        if (playfieldAt(pt.x, pt.y) || pt.y >= PF_HEIGHT) {
            ++PlayerPos.y;
            placeOnPf(bak);
            return;
        }
    }
    fillPfPoints(bak, 0);
    drawPlayer();
}


void moveHorBase(int8_t increment) {
    PlayerPos.x += increment;
    PiecePoints bak;
    getPointsBackup(bak);
    updatePlayerPoints();
    for (size_t i = 0; i < 4; ++i) {
        Point8 pt = PlayerPiecePoints[i];
        if (playfieldAt(pt.x, pt.y) || pt.x >= PF_WIDTH) {
            PlayerPos.x -= increment;
            restorePointsBackup(bak);
            return;
        }
        /*
        if (game.Matrix(pt) != 0 || pt.x < 0 || pt.x >= Width) {
            pos.x -= x;
            points = bk;
            return;
            }*/
    }
    updateGhostPointsAndDrawAll(bak);
}
void moveLeft() { moveHorBase(-1); }
void moveRight() { moveHorBase(1); }
void softDrop() {
    increaseScore(1);
    resetDrop();
    fall();
}
bool checkRotation(const PiecePoints bak, const PiecePoints gbak) {
    updatePlayerPoints();
    for (size_t i = 0; i < 4; ++i) {
        Point8 pt = PlayerPiecePoints[i];
        if (playfieldAt(pt.x, pt.y) || /*pt.x < 0 || */pt.x >= PF_WIDTH) {
            return false;
        }
    }
    updateGhostPoints();
    fillPfPoints(gbak, 0);
    fillPfPoints(bak, 0);
    drawGhostPoints();

    drawPlayer();
    return true;
}
void rotateBase(int8_t i) {
    PiecePoints bak;
    PiecePoints gbak;
    getPointsBackup(bak);
    getGhostPointsBackup(gbak);

    uint16_t tmpRot = PlayerRot;
    PlayerRot += i;
    PlayerRot &= 3;
    //if (rotation < 0) rotation = 3;
    //else if (rotation > 3) rotation = 0;
    if (checkRotation(bak, gbak)) return;
    // wall kick right
    uint8_t tmpX = PlayerPos.x;
    ++PlayerPos.x;
    if (checkRotation(bak, gbak)) return;
    // wall kick left
    PlayerPos.x = tmpX - 1;
    if (checkRotation(bak, gbak)) return;
    // special cases for I
    if (PlayerPieceType == TP_I) {
        PlayerPos.x = tmpX + 2;
        if (checkRotation(bak, gbak)) return;
        PlayerPos.x = tmpX - 2;
        if (checkRotation(bak, gbak))
            return;
    }
    // can't wall kick
    PlayerPos.x = tmpX;
    PlayerRot = tmpRot;
    restoreGhostPointsBackup(gbak);
    restorePointsBackup(bak);
}

void rotateClockwise() { rotateBase(1); }
void rotateCCW() { rotateBase(-1); }
void holdPiece() {
    PiecePoints bak;
    PiecePoints gbak;
    if (HoldPiece == TP_EMPTY) {
        HoldPiece = PlayerPieceType;
        getPointsBackup(bak);
        getGhostPointsBackup(gbak);
        incrementPlayerPiece();
    } else if (!LastWasHold) {
        PieceType tmp = PlayerPieceType;
        getPointsBackup(bak);
        getGhostPointsBackup(gbak);
        playerSetPiece(HoldPiece);
        HoldPiece = tmp;
    } else return;
    
    LastWasHold = true;
    drawHold();
    fillPfPoints(bak, 0);
    fillPfPoints(gbak, 0);
    //updatePlayerPoints();
    updateGhostPoints();
    drawGhostPoints();
    drawPlayer();
}
void hardDrop() {
    PiecePoints bak;
    getPointsBackup(bak);
    /*

      --PlayerPos.y;
      updatePlayerPoints();
      for (size_t i = 0; i < 4; ++i){
      Point8 pt = PlayerPiecePoints[i];
      if (playfieldAt(pt.x, pt.y) || pt.y >= PF_HEIGHT) {
      ++PlayerPos.y;
      placeOnPf(bak);
      return;
      }
      }
      fillPfPoints(bak, 0);
      drawPlayer();

     */
    for (;;) {
        --PlayerPos.y;
        updatePlayerPoints();

        for (size_t i = 0; i < 4; ++i){
            Point8 pt = PlayerPiecePoints[i];
            if (playfieldAt(pt.x, pt.y) || pt.y >= PF_HEIGHT) {
                ++PlayerPos.y;
                updatePlayerPoints();
                fillPfPoints(bak, 0);
                drawPlayer();
                placeOnPf(PlayerPiecePoints);
                return;
            }
        }
        increaseScore(2);
    }
}
void gravity() {
    uint8_t now = time8();
    int16_t diff = timeDiff(now, LastDropTime);
    //HiScore = diff;
    //drawInfo();
    if (diff >= getSpeed()) {
        LastDropTime = now;
        fall();
    }
}
