#include "game.h"

#define SOUND_LATCH_SCANLINES 5

#if SFX==1
extern UWORD sound_land, sound_coin, sound_pop, sound_kill, sound_falling;
#ifdef GAME_JETPACK
extern UWORD  sound_jetpack, sound_jetpack_coin;
#endif

static void 
sound_playLand(void);
static void
sound_playHeadSmash(void);
static void
sound_playPickup(void);
static void
sound_playKill(void);
static void
sound_playFalling(void);
#ifdef GAME_JETPACK
static void
sound_playJetpack(void);
#endif

typedef struct {
  int16_t count;
  int16_t delay;
  int16_t interrupt;
  int16_t loop;
  void (*play)(void);
} sound_config_t;

static sound_config_t sound_queue[] = {
  [SOUND_HEADSMASH] = {
    .count = 0,
    .delay = 2,
    .interrupt = 0,
    .loop = 0,
    .play = &sound_playHeadSmash
  },
  [SOUND_LAND] = {
    .count = 0,
    .delay = 2,
    .interrupt = 0,
    .loop = 0,
    .play = &sound_playLand
  },
  [SOUND_KILL] = {
    .count = 0,
    .delay = 1,
    .interrupt = 0,
    .loop = 0,
    .play = &sound_playKill
  },
  [SOUND_PICKUP] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playPickup
  },
  [SOUND_FALLING] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playFalling
  },
  [SOUND_MENU] = {
    .count = 0,
    .delay = 0,
    .interrupt = 0,
    .loop = 0,
    .play = &sound_playHeadSmash
  },
#ifdef GAME_JETPACK
  [SOUND_JETPACK] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 1,
    .play = &sound_playJetpack
  }
#endif
};

static int16_t sound_next = -1;
static int16_t sound_loop = -1;

static void 
sound_playLand(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  
  aud->ac_ptr = &sound_land;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 914/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playKill(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  
  aud->ac_ptr = &sound_kill;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 3509/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playHeadSmash(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];

  aud->ac_ptr = &sound_pop;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 224/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playPickup(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];

#ifdef GAME_JETPACK
  aud->ac_ptr = sound_loop == SOUND_JETPACK ? &sound_jetpack_coin : &sound_coin;
#else
  aud->ac_ptr = &sound_coin;
#endif
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 3919/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playFalling(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];

  aud->ac_ptr = &sound_falling;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 10506/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


#ifdef GAME_JETPACK
static void 
sound_playJetpack(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];

  aud->ac_ptr = &sound_jetpack;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 3919/2;//9975/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}
#endif


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
