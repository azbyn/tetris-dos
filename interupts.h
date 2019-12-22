// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "print.h"
#include "misc.h"

typedef struct FarPtr {
    uint16_t offset;
    uint16_t segment;
} FarPtr;
void printFarPtr(FarPtr ptr) {
    printInt16(ptr.segment);
    print(":");
    printInt16(ptr.offset);
}
void* farPtrToAbsolute(FarPtr ptr) {
    return (void*) ((uintptr_t)ptr.segment * 16 + ptr.offset);
}

FarPtr absoluteToFarPtr(void* ptr) {
    return (FarPtr) {.segment = (uintptr_t)ptr/16,
            .offset = (uintptr_t)ptr | 0xF };
}

void* getInteruptVector(uint8_t intNumber) {
    FarPtr res;
    asm volatile("int $0x21\n"
                 "mov %%es, %%ax\n"
                 :"=ax"(res.segment), "=bx"(res.offset)
                 :"ax"(0x3500|intNumber));
    return farPtrToAbsolute(res);
}
inline uint16_t getDs() {
    uint16_t ds;
    asm ("mov %%ds, %%ax": "=ax"(ds));
    return ds;
}
inline void setDs(uint16_t ds) {
    asm ("mov %%ax, %%ds":: "ax"(ds));
}

void setInteruptVector(uint8_t intNumber, void* val) {
    uint16_t ds = getDs();
    print("ds:");
    printInt16(ds);
    print("\n");
    FarPtr p = absoluteToFarPtr(val);
    asm volatile("mov %%bx, %%ds\n"
                 "int $0x21\n" :
                 : "bx"(p.segment), "dx"(p.offset), "ax"(0x2500|intNumber));
    setDs(ds);
}

void* PrevTimerInterupt;
const uint8_t Interupt = 0x8;
void __attribute__((naked))  _quack() {
    // save all, and establish DS, assuming our CS == DS
    asm volatile ("int3\n"
                  "pusha\n"
                  //"cli\n"
                  "mov %%cs, %%ax\n" // not needed in dosbox?
                  "mov %%ax, %%ds\n"
                  : /* no output */
                  : /* no input */
                  : /* no clobber*/);
    print("QUACK!\n");
    // call original
    if (PrevTimerInterupt) {
        asm volatile (//"mov (%0), %%eax\n"
            ".code16\n"
            "pushf\n" // for iret return to us!		
            "lcall *(%0)\n" // * absolute jump
            ".code16gcc\n"
            : /* no output */
            : "m"(PrevTimerInterupt)
            : /* no clobber*/);
    } else {
        outb(0x20, 0x20); // Ack PIC
        //outp(0xa0, 0x20); // Ack 2nd, cascaded PIC
    }
  
    // return from interrupt
    asm volatile ("popa\n"
                  ".code16\n"
                  "iret\n"
                  ".code16gcc\n"
                  : /* no output */
                  : /* no input */
                  : /* no clobber*/);

}
void tick_wait(uint8_t t);
void initInterupts() {
    print("initInterupts\n");
    //print("Quack @");
    //PtrUnion f = { .ptr = &fun};
    //printQuad(f.qword);
    //print("\n");

    PrevTimerInterupt = getInteruptVector(/*0x08 + Blaster.irq*/Interupt);
    print("pti: ");
    printDWord(PrevTimerInterupt);
    //printFarPtr(PrevTimerInterupt);
    print("\n");
    //assert(f.words[1]==0, "Small memory model expected");
    // Replace IRQ0 with quack

    asm volatile("  mov _quack, %%dx\n"
                 "  push %%ds\n"
                 "  mov %%cs, %%bx\n"
                 "  mov %%bx, %%ds\n"
                 "  int $0x21\n"
                 "  pop %%ds\n"
                 /* ".j:\n" */
                 /* "  call quack\n" */
                 :
                 : /*"dx"(quack), */"ax"(0x2500|Interupt)
                 :"dx", "bx");
    print("END\n");
}
void restoreTimerInterupt() {
    print("RESTORE\n");
    setInteruptVector(Interupt, PrevTimerInterupt);
}
