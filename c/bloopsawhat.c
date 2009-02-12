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
  char *str;
  if (argc > 1) {
    bloops *B = bloops_new();
    bloops_tempo(B, 320);
    bloopsaphone *P = bloops_square();
    bloopsatrack *track = bloops_track2(B, P, "c5 c6 b4 b5 d5 d6 e5 e6");
    printf("%s\n", str = bloops_track_str(track));
    bloops_track_at(B, track, 0);
    bloops_play(B);
    while (!bloops_is_done(B))
      sleep(1);
    free(P); free(str);
    bloops_track_destroy(track);
    bloops_destroy(B);
    return 0;
  }

  usage();
  return 0;
}
