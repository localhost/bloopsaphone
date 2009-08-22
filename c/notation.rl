//
// notation.rl
// the musical notation parser
//
// (c) 2009 why the lucky stiff
// See COPYING for the license
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include "bloopsaphone.h"

#define ATOI(X,N) ({ \
  char *Ap = X; \
  int Ai = 0; \
  size_t Al = N; \
  while (Al--) { \
    if ((*Ap >= '0') && (*Ap <= '9')) { \
      Ai = (Ai * 10) + (*Ap - '0'); \
      Ap++; \
    } \
    else break; \
  } \
  Ai; \
})

#define NOTE S->notes[S->nlen]

#define NEXT() \
  NOTE.duration = len; \
  NOTE.octave = oct; \
  mod = 0; \
  tone = 0; \
  len = 4; \
  fxval = 0; \
  fxmod = 0; \
  S->nlen++

%%{
  machine bloopnotes;

  action Alen {
    len = ATOI(ts, p - ts);
  }

  action Aoct {
    oct = ATOI(p - 1, 1);
  }

  action Anote {
    switch (tone) {
      case 'a': case 'A':
        if (mod == 'b')      NOTE.tone = 'a';
        else if (mod == '#') NOTE.tone = 'b';
        else                 NOTE.tone = 'A';
      break;
      case 'b': case 'B':
        if (mod == 'b')      NOTE.tone = 'b';
        else if (mod == '#') NOTE.tone = 'C';
        else                 NOTE.tone = 'B';
      break;
      case 'c': case 'C':
        if (mod == 'b')      NOTE.tone = 'B';
        else if (mod == '#') NOTE.tone = 'd';
        else                 NOTE.tone = 'C';
      break;
      case 'd': case 'D':
        if (mod == 'b')      NOTE.tone = 'd';
        else if (mod == '#') NOTE.tone = 'e';
        else                 NOTE.tone = 'D';
      break;
      case 'e': case 'E':
        if (mod == 'b')      NOTE.tone = 'e';
        else if (mod == '#') NOTE.tone = 'F';
        else                 NOTE.tone = 'E';
      break;
      case 'f': case 'F':
        if (mod == 'b')      NOTE.tone = 'E';
        else if (mod == '#') NOTE.tone = 'g';
        else                 NOTE.tone = 'F';
      break;
      case 'g': case 'G':
        if (mod == 'b')      NOTE.tone = 'g';
        else if (mod == '#') NOTE.tone = 'a';
        else                 NOTE.tone = 'G';
      break;
    }
  }

  action Afx {
    bloopsafx *fx = (bloopsafx *)malloc(sizeof(bloopsafx));
    fx->next = NOTE.FX;
    fx->cmd = fxcmd;
    fx->val = fxval;
    fx->mod = fxmod;
    fxval = fxmod = 0;
    NOTE.FX = fx;
  }

  action fxval1 {
    fxval = atoi(p-1) * 1.0f;
  }

  action fxval2 {
    fxval += ATOI(pf, p - pf) * pow(0.1f, p - pf);
  }

  dec = digit+ %fxval1 ("." %{ pf = p; } digit+ )? %fxval2;
  float = ("-" dec %{ fxval *= -1.0f; } | dec);
  fxcmd = "volume" %{ fxcmd = BLOOPS_FX_VOLUME; } |
          "punch" %{ fxcmd = BLOOPS_FX_PUNCH; } |
          "attack" %{ fxcmd = BLOOPS_FX_ATTACK; } |
          "sustain" %{ fxcmd = BLOOPS_FX_SUSTAIN; } |
          "decay" %{ fxcmd = BLOOPS_FX_DECAY; } |
          "square" %{ fxcmd = BLOOPS_FX_SQUARE; } |
          "sweep" %{ fxcmd = BLOOPS_FX_SWEEP; } |
          "vibe" %{ fxcmd = BLOOPS_FX_VIBE; } |
          "vspeed" %{ fxcmd = BLOOPS_FX_VSPEED; } |
          "vdelay" %{ fxcmd = BLOOPS_FX_VDELAY; } |
          "lpf" %{ fxcmd = BLOOPS_FX_LPF; } |
          "lsweep" %{ fxcmd = BLOOPS_FX_LSWEEP; } |
          "resonance" %{ fxcmd = BLOOPS_FX_RESONANCE; } |
          "hpf" %{ fxcmd = BLOOPS_FX_HPF; } |
          "hsweep" %{ fxcmd = BLOOPS_FX_HSWEEP; } |
          "arp" %{ fxcmd = BLOOPS_FX_ARP; } |
          "aspeed" %{ fxcmd = BLOOPS_FX_ASPEED; } |
          "phase" %{ fxcmd = BLOOPS_FX_PHASE; } |
          "psweep" %{ fxcmd = BLOOPS_FX_PSWEEP; } |
          "repeat" %{ fxcmd = BLOOPS_FX_REPEAT; };

  len = [1-9] [0-9]? ":"? %Alen;
  up = "+" %{ len = 1; } len?;
  down = "-" %{ len = 1; } len?;
  mod = [b#] %{ mod = p[-1]; };
  oct = [1-8] %Aoct;
  fxmod = ( ("+"|"-") %{ fxmod = p[-1]; } (":"|space+) )?;
  fx = ("[" fxcmd (":"|space*) fxmod float "]" %Afx );
  note = len? [a-gA-G] %{ tone = p[-1]; } mod? oct? fx* %Anote;

  main := |*
    len => {
      NOTE.tone = 0;
      NEXT();
    };
    note => { NEXT(); };
    up   => { oct++; len = 4; };
    down => { oct--; len = 4; };
    space;
  *|;

  write data nofinal;
}%%

bloopsatrack *
bloops_track(bloops *B, bloopsaphone *P, char *track, int tracklen)
{
  int cs, act, oct = 4, len = 4;
  bloopsatrack *S = (bloopsatrack *)malloc(sizeof(bloopsatrack));
  char tone, mod, fxmod, *p, *pe, *pf, *ts, *te, *eof = 0;
  bloopsafxcmd fxcmd = (bloopsafxcmd)0;
  float fxval = 0;

  S->refcount = 1;
  S->nlen = 0;
  S->capa = 1024;
  S->notes = (bloopsanote *)calloc(sizeof(bloopsanote), 1024);

  p = track;
  pe = track + tracklen + 1;

  %% write init;
  %% write exec;

  S->P = P;
  bloops_sound_ref(P);

  return S;
}

bloopsatrack *
bloops_track2(bloops *B, bloopsaphone *P, char *track)
{
  return bloops_track(B, P, track, strlen(track));
}

char *
bloops_track_str(bloopsatrack *track)
{
  int bufsize = sizeof(char) * (track->nlen * 6 + 1024);
  char *str = (char *)malloc(bufsize), *ptr = str;
  int i, adv = 0;

  for (i = 0; i < track->nlen; i++)
  {
    if (ptr - str + adv + sizeof(char) * 256 > bufsize) {
      char *new_str;
      bufsize += sizeof(char) * 1024;
      new_str = realloc(str, bufsize);
      if (new_str == NULL) {
        free(str);
        return NULL;
      }
    }

    if (ptr > str)
      strcat(ptr++, " ");

    if (track->notes[i].duration != 4)
    {
      adv = sprintf(ptr, "%d:", (int)track->notes[i].duration);
      ptr += adv;
    }

    if (track->notes[i].tone)
    {
      char tone[3] = "\0\0\0";
      tone[0] = track->notes[i].tone;
      switch (tone[0]) {
        case 'a': tone[0] = 'A'; tone[1] = 'b'; break;
        case 'b': tone[0] = 'B'; tone[1] = 'b'; break;
        case 'd': tone[0] = 'C'; tone[1] = '#'; break;
        case 'e': tone[0] = 'E'; tone[1] = 'b'; break;
        case 'g': tone[0] = 'F'; tone[1] = '#'; break;
      }
      adv = sprintf(ptr, "%s", tone);
      ptr += adv;

      adv = sprintf(ptr, "%d", (int)track->notes[i].octave);
      ptr += adv;
      bloopsafx *fx = (bloopsafx *)track->notes[i].FX;
      while (fx) {
        if (fx->mod == 0)
          adv = sprintf(ptr, "[%s %0.3f]", bloops_fxcmd_name(fx->cmd), fx->val);
        else
          adv = sprintf(ptr, "[%s %c %0.3f]", bloops_fxcmd_name(fx->cmd), fx->mod, fx->val);
        ptr += adv;
        fx = (bloopsafx *)fx->next;
      }
    }
  }

  return str;
}

char *
bloops_fxcmd_name(bloopsafxcmd fxcmd) {
  char *fxname = "\0";
  switch (fxcmd) {
    case BLOOPS_FX_VOLUME:    fxname = "volume"; break;
    case BLOOPS_FX_PUNCH:     fxname = "punch"; break;
    case BLOOPS_FX_ATTACK:    fxname = "attack"; break;
    case BLOOPS_FX_SUSTAIN:   fxname = "sustain"; break;
    case BLOOPS_FX_DECAY:     fxname = "decay"; break;
    case BLOOPS_FX_SQUARE:    fxname = "square"; break;
    case BLOOPS_FX_SWEEP:     fxname = "sweep"; break;
    case BLOOPS_FX_VIBE:      fxname = "vibe"; break;
    case BLOOPS_FX_VSPEED:    fxname = "vspeed"; break;
    case BLOOPS_FX_VDELAY:    fxname = "vdelay"; break;
    case BLOOPS_FX_LPF:       fxname = "lpf"; break;
    case BLOOPS_FX_LSWEEP:    fxname = "lsweep"; break;
    case BLOOPS_FX_RESONANCE: fxname = "resonance"; break;
    case BLOOPS_FX_HPF:       fxname = "hpf"; break;
    case BLOOPS_FX_HSWEEP:    fxname = "hsweep"; break;
    case BLOOPS_FX_ARP:       fxname = "arp"; break;
    case BLOOPS_FX_ASPEED:    fxname = "aspeed"; break;
    case BLOOPS_FX_PHASE:     fxname = "phase"; break;
    case BLOOPS_FX_PSWEEP:    fxname = "psweep"; break;
    case BLOOPS_FX_REPEAT:    fxname = "repeat"; break;
  }
  return fxname;
}

float
bloops_note_freq(char note, int octave)
{
  switch (note)
  {
    case 'A': // A
      if (octave <= 0)      return 0.0;
      else if (octave == 1) return 0.121;
      else if (octave == 2) return 0.175;
      else if (octave == 3) return 0.248;
      else if (octave == 4) return 0.353;
      else if (octave == 5) return 0.500;
    break;

    case 'b': // A# or Bb
      if (octave <= 0)      return 0.0;
      else if (octave == 1) return 0.125;
      else if (octave == 2) return 0.181;
      else if (octave == 3) return 0.255;
      else if (octave == 4) return 0.364;
      else if (octave == 5) return 0.515;
    break;

    case 'B': // B
      if (octave <= 0)      return 0.0;
      else if (octave == 1) return 0.129;
      else if (octave == 2) return 0.187;
      else if (octave == 3) return 0.263;
      else if (octave == 4) return 0.374;
      else if (octave == 5) return 0.528;
    break;

    case 'C': // C
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.133;
      else if (octave == 3) return 0.192;
      else if (octave == 4) return 0.271;
      else if (octave == 5) return 0.385;
      else if (octave == 6) return 0.544;
    break;

    case 'd': // C# or Db
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.138;
      else if (octave == 3) return 0.198;
      else if (octave == 4) return 0.279;
      else if (octave == 5) return 0.395;
      else if (octave == 6) return 0.559;
    break;

    case 'D': // D
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.143;
      else if (octave == 3) return 0.202;
      else if (octave == 4) return 0.287;
      else if (octave == 5) return 0.406;
      else if (octave == 6) return 0.575;
    break;

    case 'e': // D# or Eb
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.148;
      else if (octave == 3) return 0.208;
      else if (octave == 4) return 0.296;
      else if (octave == 5) return 0.418;
      else if (octave == 6) return 0.593;
    break;

    case 'E': // E
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.152;
      else if (octave == 3) return 0.214;
      else if (octave == 4) return 0.305;
      else if (octave == 5) return 0.429;
      else if (octave == 6) return 0.608;
    break;

    case 'F': // F
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.155;
      else if (octave == 3) return 0.220;
      else if (octave == 4) return 0.314;
      else if (octave == 5) return 0.441;
    break;

    case 'g': // F# or Gb
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.160;
      else if (octave == 3) return 0.227;
      else if (octave == 4) return 0.323;
      else if (octave == 5) return 0.454;
    break;

    case 'G': // G
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.164;
      else if (octave == 3) return 0.234;
      else if (octave == 4) return 0.332;
      else if (octave == 5) return 0.468;
    break;

    case 'a': // G# or Ab
      if (octave <= 1)      return 0.117;
      else if (octave == 2) return 0.170;
      else if (octave == 3) return 0.242;
      else if (octave == 4) return 0.343;
      else if (octave == 5) return 0.485;
    break;
  }

  return 0.0;
}

#define KEY(name) key = (void *)&P->name

%%{
  machine bloopserial;

  action ival {
    ival = ATOI(ts, p - ts);
  }

  action fval1 {
    fval = ATOI(ts, p - ts) * 1.0f;
  }

  action fval2 {
    fval = ATOI(pf, p - pf) * pow(0.1f, p - pf);
  }

  dec = [0-9]+ %fval1 "." %{ pf = p; } [0-9]+ %fval2;
  float = ("-" dec %{ fval *= -1.0f; } | dec);
  key = "volume" %{ KEY(volume); } |
        "arp" %{ KEY(arp); } |
        "aspeed" %{ KEY(aspeed); } |
        "attack" %{ KEY(attack); } |
        "decay" %{ KEY(decay); } |
        "dslide" %{ KEY(dslide); } |
        "freq" %{ KEY(freq); } |
        "hpf" %{ KEY(hpf); } |
        "hsweep" %{ KEY(hsweep); } |
        "limit" %{ KEY(limit); } |
        "lpf" %{ KEY(lpf); } |
        "lsweep" %{ KEY(lsweep); } |
        "phase" %{ KEY(phase); } |
        "psweep" %{ KEY(psweep); } |
        "repeat" %{ KEY(repeat); } |
        "resonance" %{ KEY(resonance); } |
        "slide" %{ KEY(slide); } |
        "square" %{ KEY(square); } |
        "sustain" %{ KEY(sustain); } |
        "sweep" %{ KEY(sweep); } |
        "punch" %{ KEY(punch); } |
        "vibe" %{ KEY(vibe); } |
        "vspeed" %{ KEY(vspeed); } |
        "vdelay" %{ KEY(vdelay); } |
        "volume" %{ KEY(volume); };

  main := |*
    key space+ float space*   => { *((float *)key) = fval; };
    "type" space+ "square"    => { P->type = BLOOPS_SQUARE; };
    "type" space+ "sawtooth"  => { P->type = BLOOPS_SAWTOOTH; };
    "type" space+ "sine"      => { P->type = BLOOPS_SINE; };
    "type" space+ "noise"     => { P->type = BLOOPS_NOISE; };
    space+;
  *|;

  write data nofinal;
}%%

bloopsaphone *
bloops_sound_file(bloops *B, char *fname)
{
  FILE *fp;
  struct stat stats;
  int cs, act, len;
  float fval;
  void *key;
  char *str, *p, *pe, *pf, *ts, *te, *eof = 0;
  bloopsaphone *P;

  if (stat(fname, &stats) == -1)
    return NULL;

  fp = fopen(fname, "rb");
  if (!fp)
    return NULL;

  len = stats.st_size;
  str = (char *)malloc(stats.st_size + 1);
  if (fread(str, 1, stats.st_size, fp) != stats.st_size)
    goto done;

  p = str;
  pe = str + len + 1;
  p[len] = '\0';

  P = bloops_square();
  %% write init;
  %% write exec;

done:
  fclose(fp);
  return P;
}

char *
bloops_sound_str(bloopsaphone *P)
{
  char *lines = (char *)malloc(4096), *str = lines;
  bloopsaphone *sq = bloops_square();
  if (P->type == BLOOPS_SQUARE)
    str += sprintf(str, "type square\n");
  else if (P->type == BLOOPS_SAWTOOTH)
    str += sprintf(str, "type sawtooth\n");
  else if (P->type == BLOOPS_SINE)
    str += sprintf(str, "type sine\n");
  else if (P->type == BLOOPS_NOISE)
    str += sprintf(str, "type noise\n");

  if (P->volume != sq->volume)
    str += sprintf(str, "volume %0.3f\n", P->volume);
  if (P->punch != sq->punch)
    str += sprintf(str, "punch %0.3f\n", P->punch);
  if (P->attack != sq->attack)
    str += sprintf(str, "attack %0.3f\n", P->attack);
  if (P->sustain != sq->sustain)
    str += sprintf(str, "sustain %0.3f\n", P->sustain);
  if (P->decay != sq->decay)
    str += sprintf(str, "decay %0.3f\n", P->decay);
  if (P->freq != sq->freq)
    str += sprintf(str, "freq %0.3f\n", P->freq);
  if (P->limit != sq->limit)
    str += sprintf(str, "limit %0.3f\n", P->limit);
  if (P->slide != sq->slide)
    str += sprintf(str, "slide %0.3f\n", P->slide);
  if (P->dslide != sq->dslide)
    str += sprintf(str, "dslide %0.3f\n", P->dslide);
  if (P->square != sq->square)
    str += sprintf(str, "square %0.3f\n", P->square);
  if (P->sweep != sq->sweep)
    str += sprintf(str, "sweep %0.3f\n", P->sweep);
  if (P->vibe != sq->vibe)
    str += sprintf(str, "vibe %0.3f\n", P->vibe);
  if (P->vspeed != sq->vspeed)
    str += sprintf(str, "vspeed %0.3f\n", P->vspeed);
  if (P->vdelay != sq->vdelay)
    str += sprintf(str, "vdelay %0.3f\n", P->vdelay);
  if (P->lpf != sq->lpf)
    str += sprintf(str, "lpf %0.3f\n", P->lpf);
  if (P->lsweep != sq->lsweep)
    str += sprintf(str, "lsweep %0.3f\n", P->lsweep);
  if (P->resonance != sq->resonance)
    str += sprintf(str, "resonance %0.3f\n", P->resonance);
  if (P->hpf != sq->hpf)
    str += sprintf(str, "hpf %0.3f\n", P->hpf);
  if (P->hsweep != sq->hsweep)
    str += sprintf(str, "hsweep %0.3f\n", P->hsweep);
  if (P->arp != sq->arp)
    str += sprintf(str, "arp %0.3f\n", P->arp);
  if (P->aspeed != sq->aspeed)
    str += sprintf(str, "aspeed %0.3f\n", P->aspeed);
  if (P->phase != sq->phase)
    str += sprintf(str, "phase %0.3f\n", P->phase);
  if (P->psweep != sq->psweep)
    str += sprintf(str, "psweep %0.3f\n", P->psweep);
  if (P->repeat != sq->repeat)
    str += sprintf(str, "repeat %0.3f\n", P->repeat);

  free(sq);
  return lines;
}
