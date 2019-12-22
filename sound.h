// -*- mode: C;-*-
#pragma once
#include "defines.h"
#include "misc.h"
#include "sound_defs.h"
#include "notes.h"
#include "instruments.h"

/* adlib default ports */
const word addr_port = 0x0388;
const word data_port = 0x0389;

void soundport(uint8_t index, uint8_t value) {
    int i;

    outb(addr_port, index);
    for (i = 0; i < 6; i++)
        inb(addr_port);

    outb(data_port, value);
    for (i = 0; i < 36; i++)
        inb(addr_port);
}

void testSound() {
    unsigned char i, r1, r2;
    /* 1. Reset T1 and T2: write 60h to register 4. */
    soundport(0x04, 0x60);
    /* 2. Reset the IRQ: write 80h to register 4. */
    soundport(0x04, 0x80);
    /* 3. Read status register: read at 388h. Save the result. */
    r1 = inb(addr_port);
    /* 4. Set timer 1 to FFh: write FFh to register 2. */
    soundport(0x02, 0xff);
    /* 5. Unmask and start timer 1: write 21h to register 4. */
    soundport(0x04, 0x21);
    /* 6. Wait in a delay loop for at least 80 µsec. */
    for (i = 0; i < 80; i++)
        inb(addr_port);
    /* 7. Read status register: read at 388h. Save the result. */
    r2 = inb(addr_port);
    /* 8. Reset T1,T2 and IRQ as in steps 1 and 2. */
    soundport(0x04, 0x60);
    soundport(0x04, 0x80);
    /* 9. Test the results of the two reads: the first should
       be 0, the second should be C0h. If either is incorrect,
       then the ALMSC is not presents.
       NOTE1: You should AND the result bytes with E0h because
       the unused bits are undefined.
       NOTE2: This testing method doesn't work in SoundBlaster.
    */
    if ((r1 & 0xe0) != 0x00 || (r2 & 0xe0) != 0xc0) {
        assert(0, "No Adlib sound card found\n");
    }
}
void playNoteImpl(unsigned channel, unsigned octave, unsigned freq) {
    soundport(0xB0 + channel, 0);
    soundport(0xA0 + channel, freq & 255);
    soundport(0xB0 + channel, 0x20 + (octave << 2) + (freq >> 8));
}
void playNote(Note n, Octave o, Channel c) {
    if (n == REST) {
        playNoteImpl(c, 0, 0);
    } else {
        if (n == C) {
            if (o == 0) {
                n = 0;
            } else {
                o--;
            }
        }
        playNoteImpl(c, o, n);
    }
}
void nosound() {
    for (int c = 0; c < CH_LEN; ++c)
        playNoteImpl(c, 0, 0);
}
uint8_t adlibjam(Channel c) {
    if (c < 3) return c;
    if(c < 6) return 5 + c;
    return 10 + c;
}

void setInstrImpl(uint8_t ch, const OplRegs *r, uint8_t waveSel) {
    soundport(0x60 + ch, (r->attack << 4) + r->decay);
    soundport(0x80 + ch, (r->sustain << 4) + r->releaseRate);
    uint8_t flags = (r->ksr<< 4) | (r->eg << 5) | (r->vib << 6) | (r->am << 7);
    soundport(0x20 + ch, flags | r->multiple);
    soundport(0x40 + ch, (r->ksl << 6) | r->totalLevel);
    soundport(0xE0 + ch, waveSel);
}
void setInstr(Channel ch, const Instrument *i) {
    //soundport(0xc0 + ch, (s->fb << 1) + s->c);
    ch = adlibjam(ch);
    setInstrImpl(ch, &i->oplModulator, i->iModWaveSel);
    ch += 3;
    setInstrImpl(ch, &i->oplCarrier, i->iCarWaveSel);
}
typedef struct {
    int ticksRemaining;
    const Sound* start;
    size_t i;
    size_t len;
    size_t ticksPassed;
} ChannelData;
#define NEW_CD(notes) ((ChannelData) {\
                .ticksRemaining = 0,  \
                .start = notes,       \
                .i = 0,               \
                .len = LEN(notes) })

bool soundUpdateImpl(ChannelData* cd, Channel ch,
                     const char* msg,
                     int mod, bool v, uint8_t dt) {
    if (cd->ticksRemaining <= 0) {
        if (cd->i >= cd->len) {
            cd->ticksPassed = 0;
            cd->i = 0;
        }
        const Sound* s = cd->start + cd->i;
        size_t i = cd->i;
        if (s->note != REST)
            playNote(s->note, s->octave, ch+(i%mod));
        cd->ticksRemaining += (2 * s->duration);
        if (v) {
            puts(msg);
            print("i:");
            printInt(i);
            print(" TICKS remaining = ");
            printInt(cd->ticksRemaining);
            print("\n");
        }
        ++(cd->i);
    } //else {
    cd->ticksRemaining -= dt;
    cd->ticksPassed += dt;
    //}
    return false;
}
ChannelData currSong[4];
uint8_t soundPrevTime = 0;
void soundUpdate() {
    uint8_t now = time8();
    uint8_t dt = timeDiff(now, soundPrevTime);
    if (dt >= 1) {
        soundPrevTime = now;
        soundUpdateImpl(&currSong[0], C1, "T1", 2, false, dt);
        soundUpdateImpl(&currSong[1], C3, "T2", 2, false, dt);
        soundUpdateImpl(&currSong[2], C5, "B1", 4, false, dt);
        soundUpdateImpl(&currSong[3], C9, "B2", 1, false, dt);
    }
}
void initSound() {
    testSound();
    soundPrevTime = time8();
    currSong[0] = NEW_CD(notes);
    currSong[1] = NEW_CD(notes2);
    currSong[2] = NEW_CD(bassNotes);//Ends on 374
    //bass channel2 only fires once so we can use an existing channel
    currSong[3] = NEW_CD(bassNotes2);
    for (size_t i = 0; i < 4; ++i)
        setInstr(i, &piano1);
    for (size_t i = 4; i < CH_LEN; ++i)
        setInstr(i, &piano1bass);//&piano1bass);
    soundPrevTime = time8();
}
void stopSound() {
    nosound();
}
