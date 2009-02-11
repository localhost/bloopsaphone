//
// bloopsawhat.c
// a simple commandline player
//
#include <stdio.h>
#include "bloopsaphone.h"

static void
usage()
{
  printf("usage: bloopsawhat notes\n"
         " (ex.: bloopsawhat \"a b c d e f g + a b c\"\n");
}

int
main(int argc, char *argv[])
{
  if (argc > 1) {
    bloops *B = bloops_new();
    bloopsaphone *P = bloops_load("tone.sfx");
    bloops_play(B, P);
    sleep(2);
    // bloops_song2(B, argv[1]);
    bloops_destroy(B);
    return 0;
  }

  usage();
  return 0;
}
