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
  len = 4; \
  S->length++

%%{
  machine bloopnotes;

  action Alen {
    len = ATOI(ts, p - ts);
  }

  action Anote {
    switch (p[-1]) {
      case 'a': case 'A':
        NOTE.tone = 'a';
      break;
      case 'b': case 'B':
        NOTE.tone = 'b';
      break;
      case 'c': case 'C':
        NOTE.tone = 'c';
      break;
      case 'd': case 'D':
        NOTE.tone = 'd';
      break;
      case 'e': case 'E':
        NOTE.tone = 'e';
      break;
      case 'f': case 'F':
        NOTE.tone = 'f';
      break;
      case 'g': case 'G':
        NOTE.tone = 'g';
      break;
    }
  }

  len = [1-9] [0-9]? %Alen;
  up = "+" %{ len = 1; } len?;
  down = "-" %{ len = 1; } len?;
  note = (len ":")? [a-gA-G] %Anote [b#]? (up | down)?;

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
  char *p, *pe, *ts, *te, *eof = 0;

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
      adv = sprintf(ptr, "%c", (int)song->notes[i].tone);
      ptr += adv;
    }

    adv = sprintf(ptr, "%d", (int)song->notes[i].octave);
    ptr += adv;
  }

  return str;
}
