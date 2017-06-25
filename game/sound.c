#include "game.h"

#define SOUND_LATCH_SCANLINES 5

#if SFX==1
extern UWORD sound_Terence_punch01, sound_Terence_punch02, sound_Bud_Punch01, sound_enemy01, sound_enemy02, sound_enemy03, sound_pop, sound_coin, sound_rev, sound_motorbike;

static void 
sound_playBud_Punch01(void);
static void 
sound_playTerence_punch01(void);
static void 
sound_playTerence_punch02(void);
static void
sound_playEnemy01(void);
static void
sound_playEnemy02(void);
static void
sound_playEnemy03(void);
static void
sound_playPickup(void);
static void
sound_playPop(void);
static void
sound_playRev(void);
static void
sound_playMotorbike(void);


typedef struct {
  int16_t count;
  int16_t delay;
  int16_t interrupt;
  int16_t loop;
  void (*play)(void);
} sound_config_t;

static sound_config_t sound_queue[] = {
  [SOUND_DIE01] = {
    .count = 0,
    .delay = 2,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playEnemy01
  },
  [SOUND_DIE02] = {
    .count = 0,
    .delay = 2,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playEnemy02
  },
  [SOUND_DIE03] = {
    .count = 0,
    .delay = 2,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playEnemy03
  },  
  [SOUND_BUD_PUNCH01] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playBud_Punch01
  },
  [SOUND_TERENCE_PUNCH01] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playTerence_punch01
  },
  [SOUND_ENEMY_PUNCH01] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playTerence_punch02
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
  [SOUND_REV] = {
    .count = 0,
    .delay = 1,
    .interrupt = 1,
    .loop = 0,
    .play = &sound_playRev
  },

  [SOUND_MOTORBIKE] = {
    .count = 0,
    .delay = 1,
    .interrupt = 0,
    .loop = 1,
    .play = &sound_playMotorbike
  },      
};

static int16_t sound_next = -1;
static int16_t sound_loop = -1;
static int16_t sound_triggered = 0;
static uint32_t sound_loopStart;
//static
__EXTERNAL uint32_t sound_loopLength;

static __SECTION_DATA_C UWORD sound_empty[2] = {0,0};


#define _sound_hwPlay(sample, period, volume, length) \
  volatile struct AudChannel *aud = &custom->aud[3];\
  aud->ac_ptr = &sample;\
  aud->ac_per = period;\
  aud->ac_vol = volume;\
  aud->ac_len = length/2;\
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;\
  sound_loopStart = hw_verticalBlankCount;				\
  uint32_t samplesPerSec = 3546895/period;				\
  uint32_t length_in_100ths_per_sec = (100*(length))/samplesPerSec;	\
  sound_loopLength = (length_in_100ths_per_sec/2);

static void 
sound_playBud_Punch01(void)
{
  uint16_t periodIndex =  hw_verticalBlankCount & 0x3;
  
  switch (periodIndex) {
  case 0:
  case 3:
    {  
      _sound_hwPlay(sound_Bud_Punch01, 321, 64, 4711);
    }
    break;
  case 1:
    {
      _sound_hwPlay(sound_Bud_Punch01, 295, 64, 4711);
    }
    break;
  case 2:
    {
      _sound_hwPlay(sound_Bud_Punch01, 354, 64, 4711);
    }
    break;
  }    
}

static void 
sound_playTerence_punch01(void)
{
  uint16_t periodIndex =  hw_verticalBlankCount & 0x3;

  switch (periodIndex) {
  case 0:
  case 3:
    {  
      _sound_hwPlay(sound_Terence_punch01, 321, 64, 2614);
    }
    break;
  case 1:
    {
      _sound_hwPlay(sound_Terence_punch01, 295, 64, 2614);
    }
    break;
  case 2:
    {
      _sound_hwPlay(sound_Terence_punch01, 354, 64, 2614);
    }
    break;
  }  
}

static void 
sound_playTerence_punch02(void)
{
  uint16_t periodIndex =  hw_verticalBlankCount & 0x3;

  switch (periodIndex) {
  case 0:
  case 3:
    {
      _sound_hwPlay(sound_Terence_punch02, 321, 64, 2851);
    }
    break;
  case 1:
    {
      _sound_hwPlay(sound_Terence_punch02, 295, 64, 2851);
    }
    break;
  case 2:
    {
      _sound_hwPlay(sound_Terence_punch02, 354, 64, 2851);
    }
    break;
  }
}

static void 
sound_playEnemy01(void)
{
  _sound_hwPlay(sound_Bud_Punch01, 321, 64, (4711/2)+(4410/2)*2);  
}

static void 
sound_playEnemy02(void)
{
  _sound_hwPlay(sound_Terence_punch01, 321, 64, (3503/2)+(2614/2)*2);
}


static void 
sound_playEnemy03(void)
{
  _sound_hwPlay(sound_Terence_punch02, 321, 64, (4063/2)+(2851/2)*2);      
  /*
  volatile struct AudChannel *aud = &custom->aud[3];  
  aud->ac_ptr = &sound_Terence_punch02;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 4063/2 + 2851/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
  */
}


static void 
sound_playPop(void)
{
  _sound_hwPlay(sound_pop, 321, 64, 224);    
}

static void 
sound_playRev(void)
{
  _sound_hwPlay(sound_rev, 321, 64, 7670);    
}

static void 
sound_playMotorbike(void)
{
  _sound_hwPlay(sound_motorbike, 420, 64, 11942);  
}

static void 
sound_playPickup(void)
{
  _sound_hwPlay(sound_coin, 321, 64, 3919);
}

static void
sound_resetSound(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];    
  aud->ac_len = 2;
  //    aud->ac_per = 1;
  aud->ac_ptr = &sound_empty[0];
}


void
sound_vbl(void)
{ 
  if (/*sound_loop == -1 &&*/ sound_triggered) {
    sound_triggered = 0;
    sound_resetSound();
    hw_waitScanLines(SOUND_LATCH_SCANLINES);    
  }
}


static void
sound_interrupt(void)
{
  sound_triggered = 0;
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
      sound_triggered = 1;      
      if (sptr->loop) {
	sound_loop = sound_next;
	sound_triggered = 0;      	
      }
      sound_next = -1;      
    }
  } else if (sound_loop != -1 && hw_verticalBlankCount > (sound_loopStart + sound_loopLength)) {  
    sound_interrupt();    
    //    hw_waitScanLines(SOUND_LATCH_SCANLINES);
    (*sound_queue[sound_loop].play)();
    sound_loopStart = hw_verticalBlankCount;
    sound_next = -1;    
    sound_triggered = 0;
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


__NOINLINE void
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


__NOINLINE void
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
    sound_triggered = 1;    
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
