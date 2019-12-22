#include "print.h"
#include "misc.h"
#include "graphics.h"
#include "gameGraphics.h"
#include "input.h"
#include "sound.h"

asm("call  dosmain\n"
    "mov   $0x4C,%ah\n"
    "int   $0x21\n");

void onExit() {
    stopSound();
    delVga();
    print("s:");
    printInt(sizeof(bassNotes2));
}


int dosmain() {
    initVga();
    initSound();
    initVga();

    initRand();
    resetGame();
    restartPlayer();
    drawInfo();

    drawInit();
    // Debug only |
    //            v
    //drawBoard();
    for (;Running;) {
        gravity();
        input();
        soundUpdate();
    }
    drawEndScreen();

    while (tolower(readKey()) != 'q');
    onExit();

    return 0;
}
