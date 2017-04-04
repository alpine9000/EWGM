#include "game.h"

#define SOUND_LATCH_SCANLINES 5

#if SFX==1
extern UWORD sound_finehit, sound_fathit, sound_coin, sound_pop;

static void 
sound_playFineHit(void);
static void
sound_playFatHit(void);
static void
sound_playPickup(void);
static void
sound_playPop(void);

typedef struct {
  int16_t count;
  int16_t delay;
  int16_t interrupt;
  int16_t loop;
  void (*play)(void);
} sound_config_t;

static sound_config_t sound_queue[] = {
  [SOUND_FATHIT] = {
    .count = 0,
    .delay = 2,
    .interrupt = 0,
    .loop = 0,
    .play = &sound_playFatHit
  },
  [SOUND_FINEHIT] = {
    .count = 0,
    .delay = 2,
    .interrupt = 0,
    .loop = 0,
    .play = &sound_playFineHit
  },
  [SOUND_PICKUP] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playPickup
  },
  [SOUND_MENU] = {
    .count = 0,
    .delay = 0,
    .interrupt = 0,
    .loop = 0,
    .play = &sound_playPop
  },
};

static int16_t sound_next = -1;
static int16_t sound_loop = -1;

static void 
sound_playFineHit(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  
  aud->ac_ptr = &sound_finehit;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 4384/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playPop(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];

  aud->ac_ptr = &sound_pop;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 224/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playFatHit(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  
  aud->ac_ptr = &sound_fathit;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 4036/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playPickup(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];

  aud->ac_ptr = &sound_coin;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 3919/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void
sound_resetSound(void)
{
  static UWORD empty[2] = {0,0};
  volatile struct AudChannel *aud = &custom->aud[3];    
  aud->ac_len = 2;
  //    aud->ac_per = 1;
  aud->ac_ptr = &empty[0];
}


void
sound_vbl(void)
{ 
  if (sound_loop == -1) {
    sound_resetSound();
  }
}


static void
sound_interrupt(void)
{
  custom->dmacon = DMAF_AUD3;
  sound_resetSound();
  hw_waitScanLines(SOUND_LATCH_SCANLINES);
}


void
sound_schedule(void)
{
  if (sound_next >= 0) {
    if (--sound_queue[sound_next].count == 0) {
      sound_config_t *sptr = &sound_queue[sound_next];
      if (sptr->interrupt) {
	sound_interrupt();
      }
      (*sptr->play)();
      if (sptr->loop) {
	sound_loop = sound_next;
      } else  if (sound_loop != -1) {       
	hw_waitScanLines(SOUND_LATCH_SCANLINES);
	(*sound_queue[sound_loop].play)();
      }
      sound_next = -1;
    }
  }
}


static void
sound_doQueue(sound_t sound) 
{
  if ((int16_t)sound >= sound_next) {
    sound_config_t* sptr = &sound_queue[sound];
    if (sptr->delay == 0) {
      if (sptr->interrupt) {
	sound_interrupt();
      }
      (*sptr->play)();      
      sound_next = -1;      
    } else {
      sound_queue[sound].count = sound_queue[sound].delay;
      sound_next = sound;
    }
  }
}


void
sound_playSound(sound_t sound)
{
  sound_config_t* sptr = &sound_queue[sound];
  sound_interrupt(); 
  (*sptr->play)();      
  hw_waitScanLines(SOUND_LATCH_SCANLINES);
  sound_resetSound();
  sound_next = -1;
  sound_loop = -1;
}


void
sound_queueSound(sound_t sound)
{
  sound_doQueue(sound);
}


void
sound_endLoop(void)
{
  if (sound_loop != -1) {
    sound_loop = -1;
    hw_waitScanLines(SOUND_LATCH_SCANLINES);
    sound_vbl();
  }
}

void
sound_init(void)
{
  sound_interrupt();
  sound_loop = -1;
  sound_next = -1;
}
#endif
