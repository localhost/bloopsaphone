//
// bloopsaphone.h
// the chiptune maker for portaudio
//
#ifndef BLOOPSAPHONE_H
#define BLOOPSAPHONE_H

#define BLOOPSAPHONE_VERSION "1.0"

#define BLOOPS_STOP 0
#define BLOOPS_PLAY 1
#define BLOOPS_MUTE 2

#define BLOOPS_SQUARE   0
#define BLOOPS_SAWTOOTH 1
#define BLOOPS_SINE     2
#define BLOOPS_NOISE    3

typedef struct {
  unsigned char type, pan;
  float volume;
  float punch;
  float attack;
  float sustain;
  float decay;
  float freq, limit, slide, dslide; // pitch
  float square, sweep;              // square wave
  float vibe, vspeed, vdelay;       // vibrato
  float lpf, lsweep, resonance, hpf, hsweep;
                                    // hi-pass, lo-pass
  float arp, aspeed;                // arpeggiator
  float phase, psweep;              // phaser
  float repeat;                     // repeats?
} bloopsaphone;

#define BLOOPS_HI_OCTAVE 8

typedef struct {
  char tone, octave, duration;
} bloopsanote;

typedef struct {
  bloopsaphone *P;
  int length, capa;
  bloopsanote *notes;
} bloopsasong;

typedef struct {
  bloopsasong *S;
  unsigned char playing;
  float volume;
  int stage, time, length[3];
  double period, maxperiod, slide, dslide;
  float square, sweep;
  int phase, iphase, phasex;
  float fphase, dphase;
  float phaser[1024];
  float noise[32];
  float filter[8];
  float vibe, vspeed, vdelay;
  int repeat, limit;
  double arp;
  int atime, alimit;
} bloopsalive;

typedef struct {
  void *stream;
  float volume;
  bloopsalive *live;
  unsigned char play;
} bloops;

//
// the api
//
bloops *bloops_new();
void bloops_destroy(bloops *);
bloopsaphone *bloops_load(char *);
bloopsalive *bloops_play(bloops *, bloopsaphone *);
void bloops_stop(bloops *, bloopsalive *);
bloopsasong *bloops_song(bloops *, bloopsaphone *, char *, int);
bloopsasong *bloops_song2(bloops *, bloopsaphone *, char *);
char *bloops_song_str(bloopsasong *);
 
#endif
