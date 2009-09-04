#line 1 "c/notation.rl"
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


#line 44 "c/notation.c"
static const char _bloopnotes_actions[] = {
	0, 1, 1, 1, 3, 1, 4, 1, 
	6, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16, 1, 17, 1, 18, 1, 
	19, 1, 20, 1, 21, 1, 22, 1, 
	23, 1, 24, 1, 25, 1, 26, 1, 
	27, 1, 28, 1, 29, 1, 30, 1, 
	31, 1, 32, 1, 33, 1, 34, 1, 
	35, 1, 36, 1, 41, 2, 0, 37, 
	2, 0, 39, 2, 0, 40, 2, 28, 
	39, 2, 29, 40, 2, 35, 0, 2, 
	35, 5, 2, 35, 30, 2, 35, 32, 
	3, 1, 2, 38, 3, 3, 2, 38, 
	3, 30, 2, 38, 3, 32, 2, 38, 
	3, 35, 4, 5, 3, 35, 5, 7, 
	4, 35, 4, 5, 7
};

static const short _bloopnotes_key_offsets[] = {
	0, 0, 8, 11, 12, 20, 27, 31, 
	37, 39, 43, 45, 48, 52, 54, 57, 
	60, 66, 70, 71, 72, 73, 74, 82, 
	83, 84, 85, 86, 94, 95, 96, 97, 
	98, 106, 108, 109, 117, 118, 119, 120, 
	121, 129, 131, 132, 140, 141, 142, 143, 
	144, 152, 155, 156, 157, 158, 166, 167, 
	168, 169, 170, 178, 179, 180, 181, 189, 
	190, 192, 193, 194, 195, 203, 204, 205, 
	206, 207, 208, 209, 217, 220, 221, 222, 
	223, 224, 232, 233, 234, 235, 236, 237, 
	245, 246, 247, 248, 256, 260, 261, 262, 
	263, 264, 272, 273, 274, 282, 283, 284, 
	285, 286, 294, 295, 296, 297, 298, 306, 
	317, 319, 322, 323, 323, 325, 328, 329, 
	329, 336, 341, 345, 350, 353, 354
};

static const char _bloopnotes_trans_keys[] = {
	97, 100, 104, 108, 112, 114, 115, 118, 
	114, 115, 116, 112, 32, 43, 45, 58, 
	9, 13, 48, 57, 32, 43, 45, 9, 
	13, 48, 57, 32, 58, 9, 13, 32, 
	45, 9, 13, 48, 57, 48, 57, 46, 
	93, 48, 57, 48, 57, 93, 48, 57, 
	46, 93, 48, 57, 48, 57, 93, 48, 
	57, 45, 48, 57, 32, 58, 9, 13, 
	48, 57, 43, 45, 48, 57, 112, 101, 
	101, 100, 32, 43, 45, 58, 9, 13, 
	48, 57, 116, 97, 99, 107, 32, 43, 
	45, 58, 9, 13, 48, 57, 101, 99, 
	97, 121, 32, 43, 45, 58, 9, 13, 
	48, 57, 112, 115, 102, 32, 43, 45, 
	58, 9, 13, 48, 57, 119, 101, 101, 
	112, 32, 43, 45, 58, 9, 13, 48, 
	57, 112, 115, 102, 32, 43, 45, 58, 
	9, 13, 48, 57, 119, 101, 101, 112, 
	32, 43, 45, 58, 9, 13, 48, 57, 
	104, 115, 117, 97, 115, 101, 32, 43, 
	45, 58, 9, 13, 48, 57, 119, 101, 
	101, 112, 32, 43, 45, 58, 9, 13, 
	48, 57, 110, 99, 104, 32, 43, 45, 
	58, 9, 13, 48, 57, 101, 112, 115, 
	101, 97, 116, 32, 43, 45, 58, 9, 
	13, 48, 57, 111, 110, 97, 110, 99, 
	101, 32, 43, 45, 58, 9, 13, 48, 
	57, 113, 117, 119, 117, 97, 114, 101, 
	32, 43, 45, 58, 9, 13, 48, 57, 
	115, 116, 97, 105, 110, 32, 43, 45, 
	58, 9, 13, 48, 57, 101, 101, 112, 
	32, 43, 45, 58, 9, 13, 48, 57, 
	100, 105, 111, 115, 101, 108, 97, 121, 
	32, 43, 45, 58, 9, 13, 48, 57, 
	98, 101, 32, 43, 45, 58, 9, 13, 
	48, 57, 108, 117, 109, 101, 32, 43, 
	45, 58, 9, 13, 48, 57, 112, 101, 
	101, 100, 32, 43, 45, 58, 9, 13, 
	48, 57, 32, 43, 45, 9, 13, 49, 
	57, 65, 71, 97, 103, 49, 57, 58, 
	48, 57, 58, 49, 57, 58, 48, 57, 
	58, 58, 48, 57, 65, 71, 97, 103, 
	58, 65, 71, 97, 103, 65, 71, 97, 
	103, 35, 91, 98, 49, 56, 91, 49, 
	56, 91, 91, 0
};

static const char _bloopnotes_single_lengths[] = {
	0, 8, 3, 1, 4, 3, 2, 2, 
	0, 2, 0, 1, 2, 0, 1, 1, 
	2, 2, 1, 1, 1, 1, 4, 1, 
	1, 1, 1, 4, 1, 1, 1, 1, 
	4, 2, 1, 4, 1, 1, 1, 1, 
	4, 2, 1, 4, 1, 1, 1, 1, 
	4, 3, 1, 1, 1, 4, 1, 1, 
	1, 1, 4, 1, 1, 1, 4, 1, 
	2, 1, 1, 1, 4, 1, 1, 1, 
	1, 1, 1, 4, 3, 1, 1, 1, 
	1, 4, 1, 1, 1, 1, 1, 4, 
	1, 1, 1, 4, 4, 1, 1, 1, 
	1, 4, 1, 1, 4, 1, 1, 1, 
	1, 4, 1, 1, 1, 1, 4, 3, 
	0, 1, 1, 0, 0, 1, 1, 0, 
	1, 1, 0, 3, 1, 1, 1
};

static const char _bloopnotes_range_lengths[] = {
	0, 0, 0, 0, 2, 2, 1, 2, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	2, 1, 0, 0, 0, 0, 2, 0, 
	0, 0, 0, 2, 0, 0, 0, 0, 
	2, 0, 0, 2, 0, 0, 0, 0, 
	2, 0, 0, 2, 0, 0, 0, 0, 
	2, 0, 0, 0, 0, 2, 0, 0, 
	0, 0, 2, 0, 0, 0, 2, 0, 
	0, 0, 0, 0, 2, 0, 0, 0, 
	0, 0, 0, 2, 0, 0, 0, 0, 
	0, 2, 0, 0, 0, 0, 0, 2, 
	0, 0, 0, 2, 0, 0, 0, 0, 
	0, 2, 0, 0, 2, 0, 0, 0, 
	0, 2, 0, 0, 0, 0, 2, 4, 
	1, 1, 0, 0, 1, 1, 0, 0, 
	3, 2, 2, 1, 1, 0, 0
};

static const short _bloopnotes_index_offsets[] = {
	0, 0, 9, 13, 15, 22, 28, 32, 
	37, 39, 43, 45, 48, 52, 54, 57, 
	60, 65, 69, 71, 73, 75, 77, 84, 
	86, 88, 90, 92, 99, 101, 103, 105, 
	107, 114, 117, 119, 126, 128, 130, 132, 
	134, 141, 144, 146, 153, 155, 157, 159, 
	161, 168, 172, 174, 176, 178, 185, 187, 
	189, 191, 193, 200, 202, 204, 206, 213, 
	215, 218, 220, 222, 224, 231, 233, 235, 
	237, 239, 241, 243, 250, 254, 256, 258, 
	260, 262, 269, 271, 273, 275, 277, 279, 
	286, 288, 290, 292, 299, 304, 306, 308, 
	310, 312, 319, 321, 323, 330, 332, 334, 
	336, 338, 345, 347, 349, 351, 353, 360, 
	368, 370, 373, 375, 376, 378, 381, 383, 
	384, 389, 393, 396, 401, 404, 406
};

static const char _bloopnotes_trans_targs[] = {
	2, 28, 33, 41, 49, 63, 76, 92, 
	111, 3, 18, 23, 111, 4, 111, 5, 
	6, 16, 17, 5, 12, 111, 5, 6, 
	16, 5, 12, 111, 7, 15, 7, 111, 
	7, 8, 7, 12, 111, 9, 111, 10, 
	126, 9, 111, 11, 111, 126, 11, 111, 
	13, 126, 12, 111, 14, 111, 126, 14, 
	111, 8, 12, 111, 7, 15, 7, 9, 
	111, 6, 16, 12, 111, 19, 111, 20, 
	111, 21, 111, 22, 111, 5, 6, 16, 
	17, 5, 12, 111, 24, 111, 25, 111, 
	26, 111, 27, 111, 5, 6, 16, 17, 
	5, 12, 111, 29, 111, 30, 111, 31, 
	111, 32, 111, 5, 6, 16, 17, 5, 
	12, 111, 34, 36, 111, 35, 111, 5, 
	6, 16, 17, 5, 12, 111, 37, 111, 
	38, 111, 39, 111, 40, 111, 5, 6, 
	16, 17, 5, 12, 111, 42, 44, 111, 
	43, 111, 5, 6, 16, 17, 5, 12, 
	111, 45, 111, 46, 111, 47, 111, 48, 
	111, 5, 6, 16, 17, 5, 12, 111, 
	50, 54, 59, 111, 51, 111, 52, 111, 
	53, 111, 5, 6, 16, 17, 5, 12, 
	111, 55, 111, 56, 111, 57, 111, 58, 
	111, 5, 6, 16, 17, 5, 12, 111, 
	60, 111, 61, 111, 62, 111, 5, 6, 
	16, 17, 5, 12, 111, 64, 111, 65, 
	69, 111, 66, 111, 67, 111, 68, 111, 
	5, 6, 16, 17, 5, 12, 111, 70, 
	111, 71, 111, 72, 111, 73, 111, 74, 
	111, 75, 111, 5, 6, 16, 17, 5, 
	12, 111, 77, 82, 88, 111, 78, 111, 
	79, 111, 80, 111, 81, 111, 5, 6, 
	16, 17, 5, 12, 111, 83, 111, 84, 
	111, 85, 111, 86, 111, 87, 111, 5, 
	6, 16, 17, 5, 12, 111, 89, 111, 
	90, 111, 91, 111, 5, 6, 16, 17, 
	5, 12, 111, 93, 98, 101, 106, 111, 
	94, 111, 95, 111, 96, 111, 97, 111, 
	5, 6, 16, 17, 5, 12, 111, 99, 
	111, 100, 111, 5, 6, 16, 17, 5, 
	12, 111, 102, 111, 103, 111, 104, 111, 
	105, 111, 5, 6, 16, 17, 5, 12, 
	111, 107, 111, 108, 111, 109, 111, 110, 
	111, 5, 6, 16, 17, 5, 12, 111, 
	111, 112, 116, 111, 120, 123, 123, 0, 
	113, 111, 115, 114, 111, 115, 111, 111, 
	117, 111, 119, 118, 111, 119, 111, 111, 
	122, 121, 123, 123, 111, 122, 123, 123, 
	111, 123, 123, 111, 124, 1, 124, 125, 
	111, 1, 125, 111, 1, 111, 1, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 111, 111, 111, 
	111, 111, 111, 111, 111, 0
};

static const char _bloopnotes_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	67, 0, 0, 0, 67, 0, 67, 39, 
	39, 39, 39, 39, 39, 67, 0, 0, 
	0, 0, 0, 67, 55, 55, 55, 67, 
	0, 0, 0, 0, 67, 0, 67, 5, 
	120, 0, 67, 7, 67, 116, 0, 67, 
	5, 112, 0, 67, 7, 67, 87, 0, 
	67, 0, 0, 67, 55, 55, 55, 0, 
	67, 0, 0, 0, 67, 0, 67, 0, 
	67, 0, 67, 0, 67, 41, 41, 41, 
	41, 41, 41, 67, 0, 67, 0, 67, 
	0, 67, 0, 67, 13, 13, 13, 13, 
	13, 13, 67, 0, 67, 0, 67, 0, 
	67, 0, 67, 17, 17, 17, 17, 17, 
	17, 67, 0, 0, 67, 0, 67, 35, 
	35, 35, 35, 35, 35, 67, 0, 67, 
	0, 67, 0, 67, 0, 67, 37, 37, 
	37, 37, 37, 37, 67, 0, 0, 67, 
	0, 67, 29, 29, 29, 29, 29, 29, 
	67, 0, 67, 0, 67, 0, 67, 0, 
	67, 31, 31, 31, 31, 31, 31, 67, 
	0, 0, 0, 67, 0, 67, 0, 67, 
	0, 67, 43, 43, 43, 43, 43, 43, 
	67, 0, 67, 0, 67, 0, 67, 0, 
	67, 45, 45, 45, 45, 45, 45, 67, 
	0, 67, 0, 67, 0, 67, 11, 11, 
	11, 11, 11, 11, 67, 0, 67, 0, 
	0, 67, 0, 67, 0, 67, 0, 67, 
	47, 47, 47, 47, 47, 47, 67, 0, 
	67, 0, 67, 0, 67, 0, 67, 0, 
	67, 0, 67, 33, 33, 33, 33, 33, 
	33, 67, 0, 0, 0, 67, 0, 67, 
	0, 67, 0, 67, 0, 67, 19, 19, 
	19, 19, 19, 19, 67, 0, 67, 0, 
	67, 0, 67, 0, 67, 0, 67, 15, 
	15, 15, 15, 15, 15, 67, 0, 67, 
	0, 67, 0, 67, 21, 21, 21, 21, 
	21, 21, 67, 0, 0, 0, 0, 67, 
	0, 67, 0, 67, 0, 67, 0, 67, 
	27, 27, 27, 27, 27, 27, 67, 0, 
	67, 0, 67, 23, 23, 23, 23, 23, 
	23, 67, 0, 67, 0, 67, 0, 67, 
	0, 67, 9, 9, 9, 9, 9, 9, 
	67, 0, 67, 0, 67, 0, 67, 0, 
	67, 25, 25, 25, 25, 25, 25, 67, 
	65, 0, 0, 65, 0, 63, 63, 0, 
	49, 78, 0, 0, 72, 0, 72, 72, 
	51, 81, 0, 0, 75, 0, 75, 75, 
	0, 0, 84, 84, 69, 0, 84, 84, 
	69, 84, 84, 69, 93, 57, 93, 93, 
	108, 53, 90, 104, 1, 96, 3, 100, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 67, 67, 
	67, 67, 67, 67, 67, 67, 78, 72, 
	72, 72, 81, 75, 75, 75, 69, 69, 
	69, 108, 104, 96, 100, 0
};

static const char _bloopnotes_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 59, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _bloopnotes_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 61, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const short _bloopnotes_eof_trans[] = {
	0, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 518, 
	518, 518, 518, 518, 518, 518, 518, 0, 
	519, 522, 522, 522, 523, 526, 526, 526, 
	529, 529, 529, 530, 531, 532, 533
};

static const int bloopnotes_start = 111;
static const int bloopnotes_error = 0;

static const int bloopnotes_en_main = 111;

#line 154 "c/notation.rl"


extern void _bloops_track_add(bloops *B, bloopsatrack *track);

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

  
#line 413 "c/notation.c"
	{
	cs = bloopnotes_start;
	ts = 0;
	te = 0;
	act = 0;
	}
#line 176 "c/notation.rl"
  
#line 422 "c/notation.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _bloopnotes_actions + _bloopnotes_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 34:
#line 1 "c/notation.rl"
	{ts = p;}
	break;
#line 443 "c/notation.c"
		}
	}

	_keys = _bloopnotes_trans_keys + _bloopnotes_key_offsets[cs];
	_trans = _bloopnotes_index_offsets[cs];

	_klen = _bloopnotes_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _bloopnotes_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
_eof_trans:
	cs = _bloopnotes_trans_targs[_trans];

	if ( _bloopnotes_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _bloopnotes_actions + _bloopnotes_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 44 "c/notation.rl"
	{
    len = ATOI(ts, p - ts);
  }
	break;
	case 1:
#line 48 "c/notation.rl"
	{
    oct = ATOI(p - 1, 1);
  }
	break;
	case 2:
#line 52 "c/notation.rl"
	{
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
	break;
	case 3:
#line 92 "c/notation.rl"
	{
    bloopsafx *fx = (bloopsafx *)malloc(sizeof(bloopsafx));
    fx->next = NOTE.FX;
    fx->cmd = fxcmd;
    fx->val = fxval;
    fx->mod = fxmod;
    fxval = fxmod = 0;
    NOTE.FX = fx;
  }
	break;
	case 4:
#line 102 "c/notation.rl"
	{
    fxval = atoi(p-1) * 1.0f;
  }
	break;
	case 5:
#line 106 "c/notation.rl"
	{
    fxval += ATOI(pf, p - pf) * pow(0.1f, p - pf);
  }
	break;
	case 6:
#line 110 "c/notation.rl"
	{ pf = p; }
	break;
	case 7:
#line 111 "c/notation.rl"
	{ fxval *= -1.0f; }
	break;
	case 8:
#line 112 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_VOLUME; }
	break;
	case 9:
#line 113 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_PUNCH; }
	break;
	case 10:
#line 114 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_ATTACK; }
	break;
	case 11:
#line 115 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_SUSTAIN; }
	break;
	case 12:
#line 116 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_DECAY; }
	break;
	case 13:
#line 117 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_SQUARE; }
	break;
	case 14:
#line 118 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_SWEEP; }
	break;
	case 15:
#line 119 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_VIBE; }
	break;
	case 16:
#line 120 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_VSPEED; }
	break;
	case 17:
#line 121 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_VDELAY; }
	break;
	case 18:
#line 122 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_LPF; }
	break;
	case 19:
#line 123 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_LSWEEP; }
	break;
	case 20:
#line 124 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_RESONANCE; }
	break;
	case 21:
#line 125 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_HPF; }
	break;
	case 22:
#line 126 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_HSWEEP; }
	break;
	case 23:
#line 127 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_ARP; }
	break;
	case 24:
#line 128 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_ASPEED; }
	break;
	case 25:
#line 129 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_PHASE; }
	break;
	case 26:
#line 130 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_PSWEEP; }
	break;
	case 27:
#line 131 "c/notation.rl"
	{ fxcmd = BLOOPS_FX_REPEAT; }
	break;
	case 28:
#line 134 "c/notation.rl"
	{ len = 1; }
	break;
	case 29:
#line 135 "c/notation.rl"
	{ len = 1; }
	break;
	case 30:
#line 136 "c/notation.rl"
	{ mod = p[-1]; }
	break;
	case 31:
#line 138 "c/notation.rl"
	{ fxmod = p[-1]; }
	break;
	case 32:
#line 140 "c/notation.rl"
	{ tone = p[-1]; }
	break;
	case 35:
#line 1 "c/notation.rl"
	{te = p+1;}
	break;
	case 36:
#line 150 "c/notation.rl"
	{te = p+1;}
	break;
	case 37:
#line 143 "c/notation.rl"
	{te = p;p--;{
      NOTE.tone = 0;
      NEXT();
    }}
	break;
	case 38:
#line 147 "c/notation.rl"
	{te = p;p--;{ NEXT(); }}
	break;
	case 39:
#line 148 "c/notation.rl"
	{te = p;p--;{ oct++; len = 4; }}
	break;
	case 40:
#line 149 "c/notation.rl"
	{te = p;p--;{ oct--; len = 4; }}
	break;
	case 41:
#line 147 "c/notation.rl"
	{{p = ((te))-1;}{ NEXT(); }}
	break;
#line 725 "c/notation.c"
		}
	}

_again:
	_acts = _bloopnotes_actions + _bloopnotes_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 33:
#line 1 "c/notation.rl"
	{ts = 0;}
	break;
#line 738 "c/notation.c"
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _bloopnotes_eof_trans[cs] > 0 ) {
		_trans = _bloopnotes_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}
#line 177 "c/notation.rl"

  memcpy(&S->params, &P->params, sizeof(bloopsaparams));

  _bloops_track_add(B, S);

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

#define KEY(name) key = (void *)&P->params.name


#line 975 "c/notation.c"
static const char _bloopserial_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	5, 1, 6, 1, 7, 1, 8, 1, 
	9, 1, 10, 1, 11, 1, 12, 1, 
	13, 1, 14, 1, 15, 1, 16, 1, 
	17, 1, 18, 1, 19, 1, 20, 1, 
	21, 1, 22, 1, 23, 1, 24, 1, 
	25, 1, 26, 1, 27, 1, 29, 1, 
	30, 1, 31, 1, 32, 1, 33, 1, 
	34, 1, 35, 1, 36, 2, 1, 3, 
	2, 1, 35, 2, 4, 28, 3, 1, 
	3, 35
};

static const unsigned char _bloopserial_key_offsets[] = {
	0, 0, 3, 4, 7, 13, 15, 18, 
	20, 23, 25, 26, 27, 28, 29, 32, 
	33, 34, 35, 36, 39, 41, 42, 43, 
	44, 47, 48, 49, 50, 51, 54, 55, 
	56, 57, 60, 62, 63, 66, 67, 68, 
	69, 70, 73, 76, 77, 78, 79, 82, 
	83, 86, 87, 88, 89, 90, 93, 96, 
	97, 98, 99, 102, 103, 104, 105, 106, 
	109, 110, 111, 112, 115, 116, 118, 119, 
	120, 121, 124, 125, 126, 127, 128, 129, 
	130, 133, 137, 138, 139, 140, 143, 144, 
	145, 146, 147, 150, 151, 152, 153, 154, 
	155, 158, 159, 160, 161, 164, 165, 166, 
	167, 170, 175, 176, 177, 178, 179, 182, 
	183, 184, 185, 186, 187, 188, 189, 190, 
	191, 192, 193, 194, 198, 199, 200, 201, 
	202, 205, 206, 207, 210, 211, 212, 213, 
	214, 217, 218, 219, 220, 221, 224, 237, 
	240, 245, 248
};

static const char _bloopserial_trans_keys[] = {
	114, 115, 116, 112, 32, 9, 13, 32, 
	45, 9, 13, 48, 57, 48, 57, 46, 
	48, 57, 48, 57, 46, 48, 57, 48, 
	57, 112, 101, 101, 100, 32, 9, 13, 
	116, 97, 99, 107, 32, 9, 13, 101, 
	115, 99, 97, 121, 32, 9, 13, 108, 
	105, 100, 101, 32, 9, 13, 114, 101, 
	113, 32, 9, 13, 112, 115, 102, 32, 
	9, 13, 119, 101, 101, 112, 32, 9, 
	13, 105, 112, 115, 109, 105, 116, 32, 
	9, 13, 102, 32, 9, 13, 119, 101, 
	101, 112, 32, 9, 13, 104, 115, 117, 
	97, 115, 101, 32, 9, 13, 119, 101, 
	101, 112, 32, 9, 13, 110, 99, 104, 
	32, 9, 13, 101, 112, 115, 101, 97, 
	116, 32, 9, 13, 111, 110, 97, 110, 
	99, 101, 32, 9, 13, 108, 113, 117, 
	119, 105, 100, 101, 32, 9, 13, 117, 
	97, 114, 101, 32, 9, 13, 115, 116, 
	97, 105, 110, 32, 9, 13, 101, 101, 
	112, 32, 9, 13, 121, 112, 101, 32, 
	9, 13, 32, 110, 115, 9, 13, 111, 
	105, 115, 101, 97, 105, 113, 119, 116, 
	111, 111, 116, 104, 110, 101, 117, 97, 
	114, 101, 100, 105, 111, 115, 101, 108, 
	97, 121, 32, 9, 13, 98, 101, 32, 
	9, 13, 108, 117, 109, 101, 32, 9, 
	13, 112, 101, 101, 100, 32, 9, 13, 
	32, 97, 100, 102, 104, 108, 112, 114, 
	115, 116, 118, 9, 13, 32, 9, 13, 
	32, 9, 13, 48, 57, 32, 9, 13, 
	32, 9, 13, 48, 57, 0
};

static const char _bloopserial_single_lengths[] = {
	0, 3, 1, 1, 2, 0, 1, 0, 
	1, 0, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 2, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 2, 1, 1, 1, 1, 1, 
	1, 1, 3, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 3, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 2, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 4, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 3, 1, 1, 1, 1, 3, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 4, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 11, 1, 
	1, 1, 1
};

static const char _bloopserial_range_lengths[] = {
	0, 0, 0, 1, 2, 1, 1, 1, 
	1, 1, 0, 0, 0, 0, 1, 0, 
	0, 0, 0, 1, 0, 0, 0, 0, 
	1, 0, 0, 0, 0, 1, 0, 0, 
	0, 1, 0, 0, 1, 0, 0, 0, 
	0, 1, 0, 0, 0, 0, 1, 0, 
	1, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 1, 0, 0, 0, 0, 1, 
	0, 0, 0, 1, 0, 0, 0, 0, 
	0, 1, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 0, 0, 1, 0, 0, 
	0, 0, 1, 0, 0, 0, 0, 0, 
	1, 0, 0, 0, 1, 0, 0, 0, 
	1, 1, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 1, 0, 0, 0, 0, 
	1, 0, 0, 0, 0, 1, 1, 1, 
	2, 1, 2
};

static const short _bloopserial_index_offsets[] = {
	0, 0, 4, 6, 9, 14, 16, 19, 
	21, 24, 26, 28, 30, 32, 34, 37, 
	39, 41, 43, 45, 48, 51, 53, 55, 
	57, 60, 62, 64, 66, 68, 71, 73, 
	75, 77, 80, 83, 85, 88, 90, 92, 
	94, 96, 99, 103, 105, 107, 109, 112, 
	114, 117, 119, 121, 123, 125, 128, 132, 
	134, 136, 138, 141, 143, 145, 147, 149, 
	152, 154, 156, 158, 161, 163, 166, 168, 
	170, 172, 175, 177, 179, 181, 183, 185, 
	187, 190, 195, 197, 199, 201, 204, 206, 
	208, 210, 212, 215, 217, 219, 221, 223, 
	225, 228, 230, 232, 234, 237, 239, 241, 
	243, 246, 251, 253, 255, 257, 259, 263, 
	265, 267, 269, 271, 273, 275, 277, 279, 
	281, 283, 285, 287, 292, 294, 296, 298, 
	300, 303, 305, 307, 310, 312, 314, 316, 
	318, 321, 323, 325, 327, 329, 332, 345, 
	348, 352, 355
};

static const unsigned char _bloopserial_trans_targs[] = {
	2, 10, 15, 0, 3, 0, 4, 4, 
	0, 4, 5, 4, 8, 0, 6, 0, 
	7, 6, 0, 144, 0, 9, 8, 0, 
	146, 0, 11, 0, 12, 0, 13, 0, 
	14, 0, 4, 4, 0, 16, 0, 17, 
	0, 18, 0, 19, 0, 4, 4, 0, 
	21, 25, 0, 22, 0, 23, 0, 24, 
	0, 4, 4, 0, 26, 0, 27, 0, 
	28, 0, 29, 0, 4, 4, 0, 31, 
	0, 32, 0, 33, 0, 4, 4, 0, 
	35, 37, 0, 36, 0, 4, 4, 0, 
	38, 0, 39, 0, 40, 0, 41, 0, 
	4, 4, 0, 43, 47, 49, 0, 44, 
	0, 45, 0, 46, 0, 4, 4, 0, 
	48, 0, 4, 4, 0, 50, 0, 51, 
	0, 52, 0, 53, 0, 4, 4, 0, 
	55, 59, 64, 0, 56, 0, 57, 0, 
	58, 0, 4, 4, 0, 60, 0, 61, 
	0, 62, 0, 63, 0, 4, 4, 0, 
	65, 0, 66, 0, 67, 0, 4, 4, 
	0, 69, 0, 70, 74, 0, 71, 0, 
	72, 0, 73, 0, 4, 4, 0, 75, 
	0, 76, 0, 77, 0, 78, 0, 79, 
	0, 80, 0, 4, 4, 0, 82, 86, 
	91, 97, 0, 83, 0, 84, 0, 85, 
	0, 4, 4, 0, 87, 0, 88, 0, 
	89, 0, 90, 0, 4, 4, 0, 92, 
	0, 93, 0, 94, 0, 95, 0, 96, 
	0, 4, 4, 0, 98, 0, 99, 0, 
	100, 0, 4, 4, 0, 102, 0, 103, 
	0, 104, 0, 105, 105, 0, 105, 106, 
	110, 105, 0, 107, 0, 108, 0, 109, 
	0, 142, 0, 111, 117, 119, 0, 112, 
	0, 113, 0, 114, 0, 115, 0, 116, 
	0, 142, 0, 118, 0, 142, 0, 120, 
	0, 121, 0, 122, 0, 142, 0, 124, 
	129, 132, 137, 0, 125, 0, 126, 0, 
	127, 0, 128, 0, 4, 4, 0, 130, 
	0, 131, 0, 4, 4, 0, 133, 0, 
	134, 0, 135, 0, 136, 0, 4, 4, 
	0, 138, 0, 139, 0, 140, 0, 141, 
	0, 4, 4, 0, 143, 1, 20, 30, 
	34, 42, 54, 68, 81, 101, 123, 143, 
	0, 143, 143, 142, 145, 145, 144, 142, 
	145, 145, 142, 145, 145, 146, 142, 142, 
	142, 142, 142, 0
};

static const char _bloopserial_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 7, 7, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 0, 0, 5, 0, 1, 0, 0, 
	5, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 9, 9, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 11, 11, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 13, 13, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 15, 15, 0, 0, 
	0, 0, 0, 0, 0, 17, 17, 0, 
	0, 0, 0, 0, 0, 19, 19, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	21, 21, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 23, 23, 0, 
	0, 0, 25, 25, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 27, 27, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 29, 29, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 31, 31, 0, 
	0, 0, 0, 0, 0, 0, 45, 45, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 33, 33, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 35, 35, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 37, 37, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 39, 39, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 41, 41, 0, 0, 0, 0, 0, 
	0, 0, 43, 43, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 63, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 59, 0, 0, 0, 61, 0, 0, 
	0, 0, 0, 0, 0, 57, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 51, 51, 0, 0, 
	0, 0, 0, 47, 47, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 75, 75, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 49, 49, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 67, 69, 69, 0, 78, 
	0, 0, 65, 3, 3, 0, 72, 67, 
	78, 65, 72, 0
};

static const char _bloopserial_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 53, 0, 
	0, 0, 0
};

static const char _bloopserial_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 55, 0, 
	0, 0, 0
};

static const short _bloopserial_eof_trans[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 360, 
	361, 362, 363
};

static const int bloopserial_start = 142;
static const int bloopserial_error = 0;

static const int bloopserial_en_main = 142;

#line 446 "c/notation.rl"


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
  
#line 1314 "c/notation.c"
	{
	cs = bloopserial_start;
	ts = 0;
	te = 0;
	act = 0;
	}
#line 477 "c/notation.rl"
  
#line 1323 "c/notation.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _bloopserial_actions + _bloopserial_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 30:
#line 1 "c/notation.rl"
	{ts = p;}
	break;
#line 1344 "c/notation.c"
		}
	}

	_keys = _bloopserial_trans_keys + _bloopserial_key_offsets[cs];
	_trans = _bloopserial_index_offsets[cs];

	_klen = _bloopserial_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _bloopserial_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
_eof_trans:
	cs = _bloopserial_trans_targs[_trans];

	if ( _bloopserial_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _bloopserial_actions + _bloopserial_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 400 "c/notation.rl"
	{
    fval = ATOI(ts, p - ts) * 1.0f;
  }
	break;
	case 1:
#line 404 "c/notation.rl"
	{
    fval = ATOI(pf, p - pf) * pow(0.1f, p - pf);
  }
	break;
	case 2:
#line 408 "c/notation.rl"
	{ pf = p; }
	break;
	case 3:
#line 409 "c/notation.rl"
	{ fval *= -1.0f; }
	break;
	case 4:
#line 410 "c/notation.rl"
	{ KEY(volume); }
	break;
	case 5:
#line 411 "c/notation.rl"
	{ KEY(arp); }
	break;
	case 6:
#line 412 "c/notation.rl"
	{ KEY(aspeed); }
	break;
	case 7:
#line 413 "c/notation.rl"
	{ KEY(attack); }
	break;
	case 8:
#line 414 "c/notation.rl"
	{ KEY(decay); }
	break;
	case 9:
#line 415 "c/notation.rl"
	{ KEY(dslide); }
	break;
	case 10:
#line 416 "c/notation.rl"
	{ KEY(freq); }
	break;
	case 11:
#line 417 "c/notation.rl"
	{ KEY(hpf); }
	break;
	case 12:
#line 418 "c/notation.rl"
	{ KEY(hsweep); }
	break;
	case 13:
#line 419 "c/notation.rl"
	{ KEY(limit); }
	break;
	case 14:
#line 420 "c/notation.rl"
	{ KEY(lpf); }
	break;
	case 15:
#line 421 "c/notation.rl"
	{ KEY(lsweep); }
	break;
	case 16:
#line 422 "c/notation.rl"
	{ KEY(phase); }
	break;
	case 17:
#line 423 "c/notation.rl"
	{ KEY(psweep); }
	break;
	case 18:
#line 424 "c/notation.rl"
	{ KEY(repeat); }
	break;
	case 19:
#line 425 "c/notation.rl"
	{ KEY(resonance); }
	break;
	case 20:
#line 426 "c/notation.rl"
	{ KEY(slide); }
	break;
	case 21:
#line 427 "c/notation.rl"
	{ KEY(square); }
	break;
	case 22:
#line 428 "c/notation.rl"
	{ KEY(sustain); }
	break;
	case 23:
#line 429 "c/notation.rl"
	{ KEY(sweep); }
	break;
	case 24:
#line 430 "c/notation.rl"
	{ KEY(punch); }
	break;
	case 25:
#line 431 "c/notation.rl"
	{ KEY(vibe); }
	break;
	case 26:
#line 432 "c/notation.rl"
	{ KEY(vspeed); }
	break;
	case 27:
#line 433 "c/notation.rl"
	{ KEY(vdelay); }
	break;
	case 28:
#line 434 "c/notation.rl"
	{ KEY(volume); }
	break;
	case 31:
#line 438 "c/notation.rl"
	{te = p+1;{ P->params.type = BLOOPS_SQUARE; }}
	break;
	case 32:
#line 439 "c/notation.rl"
	{te = p+1;{ P->params.type = BLOOPS_SAWTOOTH; }}
	break;
	case 33:
#line 440 "c/notation.rl"
	{te = p+1;{ P->params.type = BLOOPS_SINE; }}
	break;
	case 34:
#line 441 "c/notation.rl"
	{te = p+1;{ P->params.type = BLOOPS_NOISE; }}
	break;
	case 35:
#line 437 "c/notation.rl"
	{te = p;p--;{ *((float *)key) = fval; }}
	break;
	case 36:
#line 442 "c/notation.rl"
	{te = p;p--;}
	break;
#line 1553 "c/notation.c"
		}
	}

_again:
	_acts = _bloopserial_actions + _bloopserial_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 29:
#line 1 "c/notation.rl"
	{ts = 0;}
	break;
#line 1566 "c/notation.c"
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _bloopserial_eof_trans[cs] > 0 ) {
		_trans = _bloopserial_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}
#line 478 "c/notation.rl"

done:
  fclose(fp);
  return P;
}

char *
bloops_sound_str(bloopsaphone *P)
{
  char *lines = (char *)malloc(4096), *str = lines;
  bloopsaphone *sq = bloops_square();
  if (P->params.type == BLOOPS_SQUARE)
    str += sprintf(str, "type square\n");
  else if (P->params.type == BLOOPS_SAWTOOTH)
    str += sprintf(str, "type sawtooth\n");
  else if (P->params.type == BLOOPS_SINE)
    str += sprintf(str, "type sine\n");
  else if (P->params.type == BLOOPS_NOISE)
    str += sprintf(str, "type noise\n");

  if (P->params.volume != sq->params.volume)
    str += sprintf(str, "volume %0.3f\n", P->params.volume);
  if (P->params.punch != sq->params.punch)
    str += sprintf(str, "punch %0.3f\n", P->params.punch);
  if (P->params.attack != sq->params.attack)
    str += sprintf(str, "attack %0.3f\n", P->params.attack);
  if (P->params.sustain != sq->params.sustain)
    str += sprintf(str, "sustain %0.3f\n", P->params.sustain);
  if (P->params.decay != sq->params.decay)
    str += sprintf(str, "decay %0.3f\n", P->params.decay);
  if (P->params.freq != sq->params.freq)
    str += sprintf(str, "freq %0.3f\n", P->params.freq);
  if (P->params.limit != sq->params.limit)
    str += sprintf(str, "limit %0.3f\n", P->params.limit);
  if (P->params.slide != sq->params.slide)
    str += sprintf(str, "slide %0.3f\n", P->params.slide);
  if (P->params.dslide != sq->params.dslide)
    str += sprintf(str, "dslide %0.3f\n", P->params.dslide);
  if (P->params.square != sq->params.square)
    str += sprintf(str, "square %0.3f\n", P->params.square);
  if (P->params.sweep != sq->params.sweep)
    str += sprintf(str, "sweep %0.3f\n", P->params.sweep);
  if (P->params.vibe != sq->params.vibe)
    str += sprintf(str, "vibe %0.3f\n", P->params.vibe);
  if (P->params.vspeed != sq->params.vspeed)
    str += sprintf(str, "vspeed %0.3f\n", P->params.vspeed);
  if (P->params.vdelay != sq->params.vdelay)
    str += sprintf(str, "vdelay %0.3f\n", P->params.vdelay);
  if (P->params.lpf != sq->params.lpf)
    str += sprintf(str, "lpf %0.3f\n", P->params.lpf);
  if (P->params.lsweep != sq->params.lsweep)
    str += sprintf(str, "lsweep %0.3f\n", P->params.lsweep);
  if (P->params.resonance != sq->params.resonance)
    str += sprintf(str, "resonance %0.3f\n", P->params.resonance);
  if (P->params.hpf != sq->params.hpf)
    str += sprintf(str, "hpf %0.3f\n", P->params.hpf);
  if (P->params.hsweep != sq->params.hsweep)
    str += sprintf(str, "hsweep %0.3f\n", P->params.hsweep);
  if (P->params.arp != sq->params.arp)
    str += sprintf(str, "arp %0.3f\n", P->params.arp);
  if (P->params.aspeed != sq->params.aspeed)
    str += sprintf(str, "aspeed %0.3f\n", P->params.aspeed);
  if (P->params.phase != sq->params.phase)
    str += sprintf(str, "phase %0.3f\n", P->params.phase);
  if (P->params.psweep != sq->params.psweep)
    str += sprintf(str, "psweep %0.3f\n", P->params.psweep);
  if (P->params.repeat != sq->params.repeat)
    str += sprintf(str, "repeat %0.3f\n", P->params.repeat);

  bloops_sound_destroy(sq);
  return lines;
}
