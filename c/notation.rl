//
// notation.rl
// the musical notation parser
//
// (c) 2008 why the lucky stiff
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define NOTE S->notes[S->length]

#define NEXT() \
  NOTE.duration = len; \
  NOTE.octave = oct; \
  mod = 0; \
  tone = 0; \
  len = 4; \
  S->length++

%%{
  machine bloopnotes;

  action Alen {
    len = ATOI(ts, p - ts);
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

  len = [1-9] [0-9]? %Alen;
  up = "+" %{ len = 1; } len?;
  down = "-" %{ len = 1; } len?;
  mod = [b#] %{ mod = p[-1]; };
  note = (len ":")? [a-gA-G] %{ tone = p[-1]; } mod? (up | down)? %Anote;

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

bloopsasong *
bloops_song(bloops *B, bloopsaphone *P, char *song, int songlen)
{
  int cs, act, oct = 4, len = 4;
  bloopsasong *S = (bloopsasong *)malloc(sizeof(bloopsasong));
  char tone, mod, *p, *pe, *ts, *te, *eof = 0;

  S->P = P;
  S->length = 0;
  S->capa = 1024;
  S->notes = (bloopsanote *)calloc(sizeof(bloopsanote), 1024);

  printf("START\n");
  p = song;
  pe = song + songlen + 1;

  %% write init;
  %% write exec;

  return S;
}

bloopsasong *
bloops_song2(bloops *B, bloopsaphone *P, char *song)
{
  return bloops_song(B, P, song, strlen(song));
}

char *
bloops_song_str(bloopsasong *song)
{
  char *str = (char *)malloc(sizeof(char) * song->length * 6), *ptr = str;
  int i, adv;

  for (i = 0; i < song->length; i++)
  {
    if (ptr > str)
      strcat(ptr++, " ");

    if (song->notes[i].duration != 4)
    {
      adv = sprintf(ptr, "%d:", (int)song->notes[i].duration);
      ptr += adv;
    }

    if (song->notes[i].tone)
    {
      char tone[3] = "\0\0\0";
      tone[0] = song->notes[i].tone;
      switch (tone[0]) {
        case 'a': tone[0] = 'A'; tone[1] = 'b'; break;
        case 'b': tone[0] = 'B'; tone[1] = 'b'; break;
        case 'd': tone[0] = 'C'; tone[1] = '#'; break;
        case 'e': tone[0] = 'E'; tone[1] = 'b'; break;
        case 'g': tone[0] = 'F'; tone[1] = '#'; break;
      }
      adv = sprintf(ptr, "%s", tone);
      ptr += adv;
    }

    adv = sprintf(ptr, "%d", (int)song->notes[i].octave);
    ptr += adv;
  }

  return str;
}

float
bloops_note_freq(char note, int octave)
{
  switch (note)
  {
    case 'A': // A
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.121;
      else if (octave == 3) return 0.175;
      else if (octave == 4) return 0.248;
      else if (octave == 5) return 0.353;
      else if (octave == 6) return 0.500;
    break;

    case 'b': // A# or Bb
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.125;
      else if (octave == 3) return 0.181;
      else if (octave == 4) return 0.255;
      else if (octave == 5) return 0.364;
      else if (octave == 6) return 0.515;
    break;

    case 'B': // B
      if (octave <= 1)      return 0.0;
      else if (octave == 2) return 0.129;
      else if (octave == 3) return 0.187;
      else if (octave == 4) return 0.263;
      else if (octave == 5) return 0.374;
      else if (octave == 6) return 0.528;
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
