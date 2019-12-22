// -*- mode: C;-*-
#pragma once
#include "defines.h"

typedef struct {
    uint8_t ksl;
    uint8_t multiple;
    uint8_t feedback;
    uint8_t attack;
    uint8_t sustain;
    uint8_t eg;
    uint8_t decay;
    uint8_t releaseRate;
    uint8_t totalLevel;
    uint8_t am;
    uint8_t vib;
    uint8_t ksr;
    uint8_t con;
} OplRegs;

typedef struct {
    uint8_t iPercussive;
    uint8_t iVoiceNum;
    OplRegs oplModulator;
    OplRegs oplCarrier;
    uint8_t iModWaveSel;
    uint8_t iCarWaveSel;
} Instrument;

typedef enum {
    REST = 0,
    C  = 0x2ae,
    CS = 0x16b,
    D  = 0x181,
    DS = 0x198,
    E  = 0x1b0,
    F  = 0x1ca,
    FS = 0x1e5,
    G  = 0x202,
    GS = 0x220,
    A  = 0x241,
    AS = 0x263,
    B  = 0x287
} Note;

typedef enum {
    O0 = 0,
    O1 = 1,
    O2 = 2,
    O3 = 3,
    O4 = 4,
    O5 = 5,
    O6 = 6,
    O7 = 7
} Octave;

typedef enum {
    C1 = 0,
    C2 = 1,
    C3 = 2,
    C4 = 3,
    C5 = 4,
    C6 = 5,
    C7 = 6,
    C8 = 7,
    C9 = 8,
    CH_LEN
} Channel;

typedef enum {
    D1 = 16,//full
    D2 = 8, //half
    D4 = 4,//quarter
    D8 = 2,//eighth
    D16 = 1,//sixteenth
} Duration;

typedef struct {
    Octave octave;
    Note note;
    Duration duration;
} Sound;
