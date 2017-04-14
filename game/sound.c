#include "game.h"

#define SOUND_LATCH_SCANLINES 5

#if SFX==1
extern UWORD sound_Terence_punch01, sound_Terence_punch02, sound_Bud_Punch01, sound_enemy01, sound_enemy02, sound_enemy03, sound_pop, sound_coin;

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
};

static int16_t sound_next = -1;
static int16_t sound_loop = -1;


static void 
sound_playBud_Punch01(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  static uint16_t periods[] = {321, 295, 354, 321};
  uint16_t periodIndex =  hw_verticalBlankCount & 0x3;
  
  aud->ac_ptr = &sound_Bud_Punch01;
  aud->ac_per = periods[periodIndex];
  aud->ac_vol = 64;
  aud->ac_len = 4711/2;//3509/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}

static void 
sound_playTerence_punch01(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  static uint16_t periods[] = {321, 295, 354, 321};
  uint16_t periodIndex =  hw_verticalBlankCount & 0x3;
  
  aud->ac_ptr = &sound_Terence_punch01;
  aud->ac_per = periods[periodIndex];
  aud->ac_vol = 64;
  aud->ac_len = 2614/2;//3509/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}

static void 
sound_playTerence_punch02(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  static uint16_t periods[] = {321, 295, 354, 321};
  uint16_t periodIndex =  hw_verticalBlankCount & 0x3;
  
  aud->ac_ptr = &sound_Terence_punch02;
  aud->ac_per = periods[periodIndex];
  aud->ac_vol = 64;
  aud->ac_len = 2851/2;//3509/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}

static void 
sound_playEnemy01(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  
  aud->ac_ptr = &sound_Bud_Punch01;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 4711/2 + 4410/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}

static void 
sound_playEnemy02(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  
  aud->ac_ptr = &sound_Terence_punch01;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 3503/2 + 2614/2;
  custom->dmacon = DMAF_AUD3|DMAF_SETCLR;
}


static void 
sound_playEnemy03(void)
{
  volatile struct AudChannel *aud = &custom->aud[3];
  
  aud->ac_ptr = &sound_Terence_punch02;
  aud->ac_per = 321;
  aud->ac_vol = 64;
  aud->ac_len = 4063/2 + 2851/2;
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
