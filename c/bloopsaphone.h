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

typedef struct {
  bloopsaphone *P;
  unsigned char playing;
  int volume, stage, time, length[3];
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
void bloops_song(bloops *, char *, int);
void bloops_song2(bloops *, char *);
 
#endif
