// -*- mode: C;-*-
#pragma once
#include "sound_defs.h"

Instrument piano = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {1, 3, 3, 15, 5, 0, 1, 3, 15, 0, 0, 0, 1},
    .oplCarrier = {0, 7, 0, 15, 7, 0, 2, 4, 0, 0, 0, 1, 1},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};
Instrument guitar1 = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {0, 1, 5, 15, 1, 0, 2, 15, 17, 0, 0, 0, 1},
    .oplCarrier = {0, 1, 1, 15, 8, 0, 5, 8, 0, 0, 0, 0, 0},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};

Instrument piano2 = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {2, 1, 3, 15, 5, 0, 2, 1, 29, 0, 1, 0, 1},
    .oplCarrier = {0, 3, 246, 15, 15, 0, 2, 1, 0, 0, 0, 1, 1},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};
Instrument piano3 = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {1, 1, 3, 15, 5, 0, 1, 0, 15, 0, 0, 0, 1},
    .oplCarrier = {0, 1, 4, 13, 7, 0, 2, 12, 4, 0, 0, 0, 1},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};
Instrument piano4 = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {1, 1, 4, 15, 6, 0, 1, 0, 13, 0, 0, 0, 1},
    .oplCarrier = {0, 1, 4, 13, 7, 0, 2, 11, 0, 0, 0, 1, 1},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};
Instrument bass1 = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {0, 1, 5, 2, 2, 0, 5, 15, 21, 0, 0, 0, 1},
    .oplCarrier = {2, 1, 21, 6, 6, 1, 5, 12, 0, 0, 0, 0, 1},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};
Instrument bass2 = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {0, 1, 5, 15, 14, 0, 2, 15, 29, 0, 0, 0, 1},
    .oplCarrier = {0, 1, 4, 15, 7, 0, 5, 8, 0, 0, 0, 0, 1},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};

const char bassV = 24;// 32; //127;
Instrument guitar1bass = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {0, 1, 5, bassV/*15*/, 1, 0, 2, 15, bassV/*17*/, 0, 0, 0, 1},
    .oplCarrier = {0, 1, 1, 15/* bassV*//*15*/, 8, 0, 5, 8, 0, 0, 0, 0, 0},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};

const char trebbleV = 15; //127;
Instrument piano1 = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {1, 1, 3, trebbleV/*15*/, 5, 0, 1, 3, trebbleV /*15*/, 0, 0, 0, 1},
    .oplCarrier = {0, 1, 246, 13, 7, 0, 2, 4, 0, 0, 0, 1, 1},
    .iModWaveSel = 0,
    .iCarWaveSel = 0
};


Instrument piano1bass = {
    .iPercussive = 0,
    .iVoiceNum = 0,
    .oplModulator = {
        .ksl = 1,
        .multiple = 1,
        .feedback = 3,
        .attack = bassV,
        .sustain = 5,
        .eg = 0,
        .decay = 1,
        .releaseRate = 3,
        .totalLevel = bassV,
        .am = 0,
        .vib = 0,
        .ksr = 0,
        .con = 1,
    },
    .oplCarrier = {
        .ksl = 0,
        .multiple = 1,
        .feedback = 246,
        .attack = 13,
        .sustain = 7,
        .eg = 0,
        .decay = 2,
        .releaseRate = 4,
        .totalLevel = 0,
        .am = 0,
        .vib = 0,
        .ksr = 1,
        .con = 1,
    },
    .iModWaveSel = 1,
    .iCarWaveSel = 1
};
