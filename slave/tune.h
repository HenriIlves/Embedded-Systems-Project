#ifndef TUNE_H
#define TUNE_H

#define BPM (90)
#define FULL (60000/BPM)
#define HALF (FULL/2)
#define QUARTER (FULL/4)

// --- Octave 2 ---
#define C2  131
#define CS2 139    // C# / Db
#define D2  147
#define DS2 156    // D# / Eb
#define E2  165
#define F2  175
#define FS2 185    // F# / Gb
#define G2  196
#define GS2 208    // G# / Ab
#define A2  220
#define AS2 233    // A# / Bb
#define B2  247

// --- Octave 3 ---
#define C3  262
#define CS3 277    // C# / Db
#define D3  293
#define DS3 311    // D# / Eb
#define E3  330
#define F3  349
#define FS3 369    // F# / Gb
#define G3  392
#define GS3 415    // G# / Ab
#define A3  440
#define AS3 466    // A# / Bb
#define B3  494

// --- Octave 4 ---
#define C4  523
#define CS4 554    // C# / Db
#define D4  587
#define DS4 622    // D# / Eb
#define E4  659
#define F4  698
#define FS4 740    // F# / Gb
#define G4  784
#define GS4 831    // G# / Ab
#define A4  880
#define AS4 932    // A# / Bb
#define B4  988

// --- Octave 5 ---
#define C5  1047
#define CS5 1109
#define D5  1175
#define DS5 1245
#define E5  1319
#define F5  1397
#define FS5 1480
#define G5  1568
#define GS5 1661
#define A5  1760
#define AS5 1865
#define B5  1976

typedef struct note {
    uint16_t frequency_hz;
    uint16_t duration_ms;
} note_t;

#endif
