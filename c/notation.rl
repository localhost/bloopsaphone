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

%%{
  machine bloopnotes;

  len = [1-9] [0-9]*;
  up = "+" len?;
  down = "-" len?;
  note = len? [a-gA-G] [b#]? (up | down)?;

  main := |*
    len   => { printf("PAUSE\n"); };
    note  => { printf("NOTE\n"); };
    up    => { printf("UP\n"); };
    down  => { printf("DOWN\n"); };
    space;
  *|;

  write data nofinal;
}%%

void
bloops_song(bloops *B, char *song, int len)
{
  int cs, act;
  char *p, *pe, *ts, *te, *eof = 0;

  p = song;
  pe = song + len + 1;

  %% write init;
  %% write exec;
}

void
bloops_song2(bloops *B, char *song)
{
  bloops_song(B, song, strlen(song));
}
