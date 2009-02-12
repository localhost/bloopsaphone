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
    bloopsaphone *P = bloops_load("tone.bloo");
    bloopsaphone *P2 = bloops_load("shot.bloo");
    bloopsaphone *P3 = bloops_load("jump.bloo");
    bloopsatrack *track = bloops_track2(B, P, "C 4 A A B 4 A A");
    bloopsatrack *track2 = bloops_track2(B, P2, "4 C 4 A 4 B 4 A");
    bloopsatrack *track3 = bloops_track2(B, P3, "A C A 4 4 4 A A");
    printf("%s\n", bloops_track_str(track));
    bloops_track_at(B, track, 0);
    bloops_track_at(B, track2, 1);
    bloops_track_at(B, track3, 2);
    bloops_play(B);
    sleep(8);
    bloops_destroy(B);
    return 0;
  }

  usage();
  return 0;
}
