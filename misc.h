// -*- mode: C;-*-
#pragma once

#include "defines.h"
#include "print.h"

#define assert(b, msg)  \
    do {                \
        if (!(b)) {     \
            onExit();   \
            print(msg); \
            exitImpl(-1);   \
        }               \
    } while(0);
#define LEN(x) (sizeof(x) / sizeof(x[0]))

void onExit();
void exitImpl(uint8_t r) {
    asm volatile("mov $0x4C, %%ah\n"
                 "int $0x21\n"
                 :
                 : "al"(r)
                 : /*"ah"*/);
}

void exit(uint8_t r) {
    onExit();
    exitImpl(r);
}
void memcpy(uint8_t* dst, const uint8_t* src, size_t n) {
    for (int i = 0; i < n; ++i)
        dst[i] = src[i];
}
void memset(uint8_t* dst, uint8_t val, size_t n) {
    for (int i = 0; i < n; ++i)
        dst[i] = val;
}
char tolower(char c) {
    if (c < 'A' || c > 'Z') return c;
    return c | 0x20;
}

typedef uint16_t time_t;
uint16_t time() {
    //returns in cx and dx time (dx is the least significant, and we return that)
    uint16_t res;
    asm volatile("mov $0, %%ah\n"
                 "int $0x1A\n"
                 : "=dx"(res)
                 :
                 :"ah", "cx");
    return res;
}

uint8_t time8() { return time(); }

//returns t1 - t0
uint16_t timeDiff(uint8_t t1, uint8_t t0) {
    if (t1 >= t0) return t1 - t0;
    return t1 - (int8_t)(t0);
    /*
      t1, t0 -> t1 - t0
      5, 0   -> 5
      1, 255 -> 2
      255, 254 -> 1
    */
}
/*void testTimeDiff() {
  assert(timeDiff(0, 0) == 0, "1");
  assert(timeDiff(5, 0) == 5, "2");
  assert(timeDiff(1, 255) == 2, "3");
  assert(timeDiff(255, 254) == 1, "4");
  }*/

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %b0,%1": :"a" (val), "Nd" (port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t res;
    asm volatile("inb %1,%0": "=a" (res):"Nd" (port));
    return res;
}
static inline void cli() { asm volatile("cli":::"cc"); }
static inline void sti() { asm volatile("sti":::"cc"); }
static inline void hlt() { asm volatile("hlt":); }
static inline void nop() { asm volatile("nop":); }

void tick_wait(uint8_t t) {//tick_wait
    uint8_t t0 = time8();
    while (timeDiff(time8(), t0) < t);
}
