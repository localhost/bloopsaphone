//
// bloopsaphone.c
// the chiptune maker for portaudio
// (with bindings for ruby)
// 
// (c) 2009 why the lucky stiff
// See COPYING for the license
//
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <portaudio.h>
#include <unistd.h>
#include "bloopsaphone.h"
#include "bloopsaphone-internal.h"

#ifdef PaStream
#error ** Looks like you're linking against PortAudio 1.8!
#error ** Bloopsaphone needs PortAudio 1.9 or greater.
#error ** On Ubuntu, try: aptitude install portaudio19-dev.
#endif

#define SAMPLE_RATE 44100
#define rnd(n) (rand() % (n + 1))
#define tempo2frames(tempo) ((float)SAMPLE_RATE / (tempo / 60.0f))
#define PI 3.14159265f

#define FX(F, V) ({ \
  if (F->mod == '+')      V += F->val; \
  else if (F->mod == '-') V -= F->val; \
  else                    V  = F->val; \
  if (V > 1.0f) \
    V = 1.0f; \
  else if (V < 0.0f) \
    V = 0.0f; \
})

static bloopsalock LOCK;
static bloopsmix *MIXER = NULL;

static void bloops_synth(int, float *);
static int bloops_port_callback(const void *, void *,
  unsigned long, const PaStreamCallbackTimeInfo *,
  PaStreamCallbackFlags, void *);
static void bloops_set_track_at(bloops *B, bloopsatrack *track, int num);

float
frnd(float range)
{
  return (float)rnd(10000) / 10000 * range;
}

static void
bloops_remove(bloops *B)
{
  int i;
  if (MIXER == NULL) return;
  for (i = 0; i < BLOOPS_MAX_CHANNELS; i++) {
    if (MIXER->B[i] == B) {
      MIXER->B[i] = NULL;
      bloops_destroy(B);
    }
  }
}

static void
bloops_reset_voice(bloopsavoice *A)
{
  A->period = 100.0 / (A->params.freq * A->params.freq + 0.001);
  A->maxperiod = 100.0 / (A->params.limit * A->params.limit + 0.001);
  A->slide = 1.0 - pow((double)A->params.slide, 3.0) * 0.01;
  A->dslide = -pow((double)A->params.dslide, 3.0) * 0.000001;
  A->square = 0.5f - A->params.square * 0.5f;
  A->sweep = -A->params.sweep * 0.00005f;
  if (A->params.arp >= 0.0f)
    A->arp = 1.0 - pow((double)A->params.arp, 2.0) * 0.9;
  else
    A->arp = 1.0 + pow((double)A->params.arp, 2.0) * 10.0;
  A->atime = 0;
  A->alimit = (int)(pow(1.0f - A->params.aspeed, 2.0f) * 20000 + 32);
  if (A->params.aspeed == 1.0f)
    A->alimit = 0;
}

static void
bloops_start_voice(bloopsavoice *A) {
  int i = 0;
  A->phase = 0;
  A->filter[0] = 0.0f;
  A->filter[1] = 0.0f;
  A->filter[2] = pow(A->params.lpf, 3.0f) * 0.1f;
  A->filter[3] = 1.0f + A->params.lsweep * 0.0001f;
  A->filter[4] = 5.0f / (1.0f + pow(A->params.resonance, 2.0f) * 20.0f) * (0.01f + A->filter[2]);
  if (A->filter[4] > 0.8f) A->filter[4] = 0.8f;
  A->filter[5] = 0.0f;
  A->filter[6] = pow(A->params.hpf, 2.0f) * 0.1f;
  A->filter[7] = 1.0 + A->params.hsweep * 0.0003f;

  A->vibe = 0.0f;
  A->vspeed = pow(A->params.vspeed, 2.0f) * 0.01f;
  A->vdelay = A->params.vibe * 0.5f;

  A->volume = 0.0f;
  A->stage = 0;
  A->time = 0;
  A->length[0] = (int)(A->params.attack * A->params.attack * 100000.0f);
  A->length[1] = (int)(A->params.sustain * A->params.sustain * 100000.0f);
  A->length[2] = (int)(A->params.decay * A->params.decay * 100000.0f);

  A->fphase = pow(A->params.phase, 2.0f) * 1020.0f;
  if (A->params.phase < 0.0f) A->fphase = -A->fphase;
  A->dphase = pow(A->params.psweep, 2.0f) * 1.0f;
  if (A->params.psweep < 0.0f) A->dphase = -A->dphase;
  A->iphase = abs((int)A->fphase);
  A->phasex = 0;

  memset(A->phaser, 0, 1024 * sizeof(float));
  for (i = 0; i < 32; i++)
    A->noise[i] = frnd(2.0f) - 1.0f;

  A->repeat = 0;
  A->limit = (int)(pow(1.0f - A->params.repeat, 2.0f) * 20000 + 32);
  if (A->params.repeat == 0.0f)
    A->limit = 0;
  A->state = BLOOPS_PLAY;
}

void
bloops_clear(bloops *B)
{
  int i;
  for (i = 0; i < BLOOPS_MAX_TRACKS; i++) {
    bloops_set_track_at(B, NULL, i);
  }
}

void
bloops_tempo(bloops *B, int tempo)
{
  B->tempo = tempo;
}

void
bloops_set_track_at(bloops *B, bloopsatrack *track, int num)
{
  bloopsavoice *voice;
  bloopsatrack *old_track;
  voice = &B->voices[num];
  old_track = voice->track;
  voice->track = track;
  if (track != NULL) {
    bloops_track_ref(track);
  }
  if (old_track != NULL) {
    bloops_track_destroy(old_track);
  }
  voice->state = BLOOPS_STOP;
  if (track != NULL) {
    memcpy(&voice->params, &track->params, sizeof(bloopsaparams));
  }
  voice->frames = 0;
  voice->nextnote[0] = 0;
  voice->nextnote[1] = 0;
}

void
_bloops_track_add(bloops *B, bloopsatrack *track) {
  int i;
  for (i = 0; i < BLOOPS_MAX_TRACKS; i++) {
    if (B->voices[i].track == NULL) {
      bloops_set_track_at(B, track, i);
      break;
    }
  }
}

int
bloops_is_done(bloops *B)
{
  return B->state == BLOOPS_STOP;
}

static void
bloops_synth(int length, float* buffer)
{
  int bi, t, i, si;

  while (length--)
  {
    int samplecount = 0;
    float allsample = 0.0f;

    for (bi = 0; bi < BLOOPS_MAX_CHANNELS; bi++)
    {
      int moreframes = 0;
      bloops *B = MIXER->B[bi];
      if (B == NULL)
        continue;
      for (t = 0; t < BLOOPS_MAX_TRACKS; t++)
      {
        bloopsavoice *A = &B->voices[t];
        bloopsatrack *track = A->track;
        if (track == NULL)
          continue;

        if (track->notes)
        {
          if (A->frames == A->nextnote[0])
          {
            if (A->nextnote[1] < track->nlen)
            {
              bloopsanote *note = &track->notes[A->nextnote[1]];
              float freq = A->params.freq;
              if (note->tone != 'n')
                freq = bloops_note_freq(note->tone, (int)note->octave);
              if (freq == 0.0f) {
                A->period = 0.0f;
                A->state = BLOOPS_STOP;
              } else {
                bloopsanote *note = &track->notes[A->nextnote[1]];
                bloopsafx *fx = note->FX;
                while (fx) {
                  switch (fx->cmd) {
                    case BLOOPS_FX_VOLUME:    FX(fx, A->params.volume);     break;
                    case BLOOPS_FX_PUNCH:     FX(fx, A->params.punch);      break;
                    case BLOOPS_FX_ATTACK:    FX(fx, A->params.attack);     break;
                    case BLOOPS_FX_SUSTAIN:   FX(fx, A->params.sustain);    break;
                    case BLOOPS_FX_DECAY:     FX(fx, A->params.decay);      break;
                    case BLOOPS_FX_SQUARE:    FX(fx, A->params.square);     break;
                    case BLOOPS_FX_SWEEP:     FX(fx, A->params.sweep);      break;
                    case BLOOPS_FX_VIBE:      FX(fx, A->params.vibe);       break;
                    case BLOOPS_FX_VSPEED:    FX(fx, A->params.vspeed);     break;
                    case BLOOPS_FX_VDELAY:    FX(fx, A->params.vdelay);     break;
                    case BLOOPS_FX_LPF:       FX(fx, A->params.lpf);        break;
                    case BLOOPS_FX_LSWEEP:    FX(fx, A->params.lsweep);     break;
                    case BLOOPS_FX_RESONANCE: FX(fx, A->params.resonance);  break;
                    case BLOOPS_FX_HPF:       FX(fx, A->params.hpf);        break;
                    case BLOOPS_FX_HSWEEP:    FX(fx, A->params.hsweep);     break;
                    case BLOOPS_FX_ARP:       FX(fx, A->params.arp);        break;
                    case BLOOPS_FX_ASPEED:    FX(fx, A->params.aspeed);     break;
                    case BLOOPS_FX_PHASE:     FX(fx, A->params.phase);      break;
                    case BLOOPS_FX_PSWEEP:    FX(fx, A->params.psweep);     break;
                    case BLOOPS_FX_REPEAT:    FX(fx, A->params.repeat);     break;
                  }
                  fx = fx->next;
                }

                bloops_reset_voice(A);
                bloops_start_voice(A);
                A->period = 100.0 / (freq * freq + 0.001);
              }

              A->nextnote[0] += (int)(tempo2frames(B->tempo) * (4.0f / note->duration));
            }
            A->nextnote[1]++;
          }

          if (A->nextnote[1] <= track->nlen)
            moreframes++;
        }
        else
        {
          moreframes++;
        }

        A->frames++;

        if (A->state == BLOOPS_STOP)
          continue;

        samplecount++;
        A->repeat++;
        if (A->limit != 0 && A->repeat >= A->limit)
        {
          A->repeat = 0;
          bloops_reset_voice(A);
        }

        A->atime++;
        if (A->alimit != 0 && A->atime >= A->alimit)
        {
          A->alimit = 0;
          A->period *= A->arp;
        }

        A->slide += A->dslide;
        A->period *= A->slide;
        if (A->period > A->maxperiod)
        {
          A->period = A->maxperiod;
          if (A->params.limit > 0.0f)
            A->state = BLOOPS_STOP;
        }

        float rfperiod = A->period;
        if (A->vdelay > 0.0f)
        {
          A->vibe += A->vspeed;
          rfperiod = A->period * (1.0 + sin(A->vibe) * A->vdelay);
        }

        int period = (int)rfperiod;
        if (period < 8) period = 8;
        A->square += A->sweep;
        if(A->square < 0.0f) A->square = 0.0f;
        if(A->square > 0.5f) A->square = 0.5f;    

        A->time++;
        while (A->time >= A->length[A->stage])
        {
          A->time = 0;
          A->stage++;
          if (A->stage == 3)
            A->state = BLOOPS_STOP;
        }

        switch (A->stage) {
          case 0:
            A->volume = (float)A->time / A->length[0];
          break;
          case 1:
            A->volume = 1.0f + (1.0f - (float)A->time / A->length[1]) * 2.0f * A->params.punch;
          break;
          case 2:
            A->volume = 1.0f - (float)A->time / A->length[2];
          break;
        }

        A->fphase += A->dphase;
        A->iphase = abs((int)A->fphase);
        if (A->iphase > 1023) A->iphase = 1023;

        if (A->filter[7] != 0.0f)
        {
          A->filter[6] *= A->filter[7];
          if (A->filter[6] < 0.00001f) A->filter[6] = 0.00001f;
          if (A->filter[6] > 0.1f)     A->filter[6] = 0.1f;
        }

        float ssample = 0.0f;
        for (si = 0; si < 8; si++)
        {
          float sample = 0.0f;
          A->phase++;
          if (A->phase >= period)
          {
            A->phase %= period;
            if (A->params.type == BLOOPS_NOISE)
              for (i = 0; i < 32; i++)
                A->noise[i] = frnd(2.0f) - 1.0f;
          }

          float fp = (float)A->phase / period;
          switch (A->params.type)
          {
            case BLOOPS_SQUARE:
              if (fp < A->square)
                sample = 0.5f;
              else
                sample = -0.5f;
            break;
            case BLOOPS_SAWTOOTH:
              sample = 1.0f - fp * 2;
            break;
            case BLOOPS_SINE:
              sample = (float)sin(fp * 2 * PI);
            break;
            case BLOOPS_NOISE:
              sample = A->noise[A->phase * 32 / period];
            break;
          }

          float pp = A->filter[0];
          A->filter[2] *= A->filter[3];
          if (A->filter[2] < 0.0f) A->filter[2] = 0.0f;
          if (A->filter[2] > 0.1f) A->filter[2] = 0.1f;
          if (A->params.lpf != 1.0f)
          {
            A->filter[1] += (sample - A->filter[0]) * A->filter[2];
            A->filter[1] -= A->filter[1] * A->filter[4];
          }
          else
          {
            A->filter[0] = sample;
            A->filter[1] = 0.0f;
          }
          A->filter[0] += A->filter[1];

          A->filter[5] += A->filter[0] - pp;
          A->filter[5] -= A->filter[5] * A->filter[6];
          sample = A->filter[5];

          A->phaser[A->phasex & 1023] = sample;
          sample += A->phaser[(A->phasex - A->iphase + 1024) & 1023];
          A->phasex = (A->phasex + 1) & 1023;

          ssample += sample * A->volume;
        }
        ssample = ssample / 8 * B->volume;
        ssample *= 2.0f * A->params.volume;

        if (ssample > 1.0f)  ssample = 1.0f;
        if (ssample < -1.0f) ssample = -1.0f;
        allsample += ssample;
      }
      if (moreframes == 0)
        B->state = BLOOPS_STOP;
    }

    *buffer++ = allsample;
  }
}

static int bloops_port_callback(const void *inputBuffer, void *outputBuffer,
  unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
  PaStreamCallbackFlags statusFlags, void *data)
{
  float *out = (float*)outputBuffer;
  bloops_synth(framesPerBuffer, out);
  return paContinue;
}

void
bloops_play(bloops *B)
{
  int i;

  for (i = 0; i < BLOOPS_MAX_TRACKS; i++) {
    bloopsavoice *A;
    A = &B->voices[i];
    if (A->track != NULL) {
      memcpy(&A->params, &A->track->params, sizeof(bloopsaparams));
      bloops_reset_voice(A);
      bloops_start_voice(A);
      A->frames = 0;
      A->nextnote[0] = 0;
      A->nextnote[1] = 0;
    }
  }

  bloops_remove(B);
  for (i = 0; i < BLOOPS_MAX_CHANNELS; i++) {
    if (MIXER->B[i] == NULL || MIXER->B[i]->state == BLOOPS_STOP) {
      bloops_ref(B);
      if (MIXER->B[i] != NULL) {
        bloops_destroy(MIXER->B[i]);
      }
      MIXER->B[i] = B;
      break;
    }
  }

  B->state = BLOOPS_PLAY;
  if (MIXER->stream == NULL) {
    Pa_OpenDefaultStream(&MIXER->stream, 0, 1, paFloat32,
      SAMPLE_RATE, 512, bloops_port_callback, B);
    Pa_StartStream(MIXER->stream);
  }
}

void
bloops_stop(bloops *B)
{
  int i, stopall = 1;
  B->state = BLOOPS_STOP;
  for (i = 0; i < BLOOPS_MAX_CHANNELS; i++)
    if (MIXER->B[i] != NULL && MIXER->B[i]->state != BLOOPS_STOP)
      stopall = 0;

  if (stopall)
  {
    Pa_StopStream(MIXER->stream);
    Pa_CloseStream(MIXER->stream);
    MIXER->stream = NULL;
  }
}

bloopsaphone *
bloops_square()
{
  bloopsaphone *P = (bloopsaphone *)calloc(sizeof(bloopsaphone), 1);
  P->refcount = 1;
  P->params.type = BLOOPS_SQUARE;
  P->params.volume = 0.5f;
  P->params.sustain = 0.3f;
  P->params.decay = 0.4f;
  P->params.freq = 0.3f;
  P->params.lpf = 1.0f;
  return P;
}

static int bloops_open = 0;

bloops *
bloops_new()
{
  int i;
  bloops *B = (bloops *)malloc(sizeof(bloops));
  B->refcount = 1;
  B->volume = 0.10f;
  B->tempo = 120;
  B->state = BLOOPS_STOP;
  for (i = 0; i < BLOOPS_MAX_TRACKS; i++) {
    B->voices[i].track = NULL;
  }

  if (MIXER == NULL)
    MIXER = (bloopsmix *)calloc(sizeof(bloopsmix), 1);

  if (!bloops_open++)
  {
    srand(time(NULL));
    bloops_lock_init(&LOCK);
    Pa_Initialize();
  }

  return B;
}

void
bloops_ref(bloops *B)
{
  B->refcount++;
}

void
bloops_destroy(bloops *B)
{
  if (--B->refcount) {
    return;
  }

  bloops_remove(B);
  free((void *)B);

  if (!--bloops_open)
  {
    Pa_Terminate();
    bloops_lock_finalize(&LOCK);
    if (MIXER != NULL)
      free(MIXER);
    MIXER = NULL;
  }
}

static void bloops_notes_destroy(bloopsanote *notes, int nlen)
{
  bloopsafx *fx, *n;
  int i;

  for (i = 0; i < nlen; i++) {
    n = fx = notes[i].FX;
    while ((fx = n)) {
      n = fx->next;
      free(fx);
    }
  }

  free(notes);
}

void
bloops_track_ref(bloopsatrack *track)
{
  track->refcount++;
}

void
bloops_track_destroy(bloopsatrack *track)
{
  if (--track->refcount) {
    return;
  }
  if (track->notes != NULL) {
    bloops_notes_destroy(track->notes, track->nlen);
  }
  free(track);
}

void bloops_sound_copy(bloopsaphone *dest, bloopsaphone const *src) {
  memcpy(&dest->params, &src->params, sizeof(bloopsaparams));
}

void bloops_sound_ref(bloopsaphone *sound) {
  sound->refcount++;
}

void bloops_sound_destroy(bloopsaphone *sound) {
  if (--sound->refcount) {
    return;
  }
  free(sound);
}
