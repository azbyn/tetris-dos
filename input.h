// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "misc.h"
#include "print.h"
#include "player.h"
#include "game.h"
#include "random.h"

#define KEY_ESC 0x1B
#define KEY_F1 0x3B

#define KEY_LEFT (0x4B|0x20)
#define KEY_RIGHT (0x4D|0x20)
#define KEY_UP (0x48|0x20)
#define KEY_DOWN (0x50|0x20)

#define MOD_SHIFT 3 // either shift
#define MOD_CTRL 4  // either ctrl
#define MOD_ALT 8   // either alt

/*
struct Key {
    uint8_t ascii;
    char code;
};
typedef struct Key Key;
static_assert(sizeof(Key) == 2, "Invalid size");

void printKey(Key k) {
    print("{ key: ");
    printInt(k.ascii);
    print(", code: ");
    printInt(k.code);
    print("}");
}
*/

uint8_t readKeyModifiers() {
    uint8_t res;
    asm volatile("mov $2, %%ah\n"
                 "int $0x16\n"
                 : "=al"(res)
                 :
                 :);
    return res;
}
char readKey() {
    char res;

    asm volatile ("  mov $6, %%ah\n"
                  "  mov $0xFF, %%dl\n"
                  "  int $0x21\n"
                  "  jnz .end\n"
                  "  xor %%ax, %%ax\n"
                  ".end:\n"
                  : "=al"(res)
                  :
                  :"cc");
    /* asm volatile ("  mov $1, %%ah\n" */
    /*               "  int $0x16\n" */
    /*               "  jnz .end\n" */
    /*               "  xor %%ax, %%ax\n" */
    /*               ".end:\n" */
    /*               //:"=ax"( * ((uint16_t*) &res)) */
    /*               :"=al"(res) */
    /*               : */
    /*               :"ax", "cc"); */
    return res;
}
void input() {
    char c = tolower(readKey());
    switch (c) {
    case 'q':
    case KEY_ESC:
    case KEY_F1:
        pause();
        break;
    case KEY_LEFT: moveLeft(); break;
    case KEY_RIGHT: moveRight(); break;
    case KEY_DOWN: softDrop(); break;
    case KEY_UP: rotateClockwise(); break;
    //ctrl-c
    case 3: exit(0); break;
    case 'x':
    case 's':
        rotateClockwise();
        break;
    case 'a':
    case 'z':
        rotateCCW();
        break;
    case 'c':
    case 'd':
        holdPiece();
        break;
    case ' ':
        hardDrop();
        break;
/*
#if DEBUG
    case 0: break;
    default:
        putc(' ');
        printInt16(c);
        break;
#endif
*/
    }
    /*
      ; clobbers ax
      input:
      ;check modifier keys:
      ; TODO do this nicer (with a timer or something)
      mov ah, 2
      int 16h
      test al, MOD_SHIFT
      jnz holdPiece

      test al, MOD_CTRL
      jnz rotateCcw

      ;check other keys

      ;mov ah, 1    ; direct console input
      ;int 16h      ; al 'char' = getch(), zero flag = char available
      mov ah, 0x06
      mov dl, 0xff  ; direct console input
      int 21h       ; al 'char' = getch(), zero flag = char available
      jz .end
     */
}
