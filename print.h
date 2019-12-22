// -*- mode: C;-*-
#pragma once

#include "defines.h"

#define putc printChar
/* #define print1s(s) printString(s "$") */
/* #define print1(s) _Generic(                             \ */
/*         (s),                                            \ */
/*         const char*: print1s,                           \ */
/*         uint16_t, uint8_t:printInt                      \ */
/*         char: printChar                                 \ */
/*         )(s) */
/* #define print(s, ...) ({ print1(s); print(__VA_ARGS__); }) */
#define print(s) printString(s "$")

void printString(const char* string) {
    asm volatile("mov $0x09, %%ah\n"
                 "int $0x21\n"
                 : /* no output */
                 : "d"(string)
                 : "ah");
}
void printChar(char c) {
    asm volatile("mov $2, %%ah\n"
                 "int $0x21\n"
                 : /* no output */
                 : "d"(c)
                 : );
}

void puts(const char* s) {
    while (*s) printChar(*s++);
}
void printIntb(uint16_t n, uint16_t base) {
    uint16_t count = 0;
    for (;;) {
        asm("mov $0, %%dx\n"
            "div %%bx\n"
            "push %%dx\n"
            : "=ax"(n)
            : "ax"(n), "bx"(base)
            : "dx","memory", "cc");
        ++count;
        if (n == 0) break;
    }
    if (base <= 10) {
        for (;;) {
            asm volatile("pop %%dx\n"
                         "add $'0', %%dx\n"
                         "mov $2, %%ah\n"
                         "int $0x21\n"
                         :
                         :
                         : "ah", "dx","cc");
            if (--count == 0) break;
        }
    } else {
        for (;;) {
            char d;
            asm volatile("pop %%dx\n" : "=dx"(d) : : );
            d += d < 10 ? '0' : ('A'-10);

            asm volatile("mov $2, %%ah\n"
                         "int $0x21\n"
                         :
                         : "dx"(d)
                         : "ah");
            if (--count == 0) break;
        }
    }
}

void printInt(uint16_t n) { printIntb(n, 10); }

void printDWord(dword n) {
    union {
        uint32_t d;
        uint16_t w[2];
    } t = {.d = n};
    //if (t.w[1])
    printIntb(t.w[1], 16);
    print("-");
    printIntb(t.w[0], 16);
}
void printInt16(uint16_t n) {
    printIntb(n, 16);
    /*
    uint16_t count = 0;
    for (;;) {
        asm("mov %%dx, %%ax\n"
            "and $16, %%dx\n"
            "shr $4, %%ax\n"
            "push %%dx\n"
            : "=ax"(n)
            : "ax"(n)
            : "dx", "memory");
        ++count;
        if (n == 0) break;
    }
    print("Count:");
    printInt(count, 10);
    if (count == 0) print("count is zero\n$");
    for (;;) {
        uint16_t d;
        asm volatile("pop %%dx\n" : "=dx"(d));
        print("J");
        printInt(d, 16);
        print("\n");
        / *
        d += d < 10 ? '0' : ('A'-10);

        asm volatile("mov $2, %%ah\n"
                     "int $0x21\n"
                     :
                     : "dx"(d)
                     : "ah","memory");* /
        if (--count == 0) break;
    }*/
}
