#include "game.h"

__EXTERNAL void
memory_ctor(void)
{
#if TRACKLOADER==1
  extern char startBSS;
  extern char endBSS;
  //  extern char startRandom;
  //  extern char endRandom;

  memset(&startBSS, 0x0, &endBSS-&startBSS);
  //  memset(&startRandom, 0x0, &endRandom-&startRandom);

#if FASTRAM==1
  extern char startBSSChip;
  extern char endBSSChip;
  //  extern char startRandom;
  //  extern char endRandom;

  memset(&startBSSChip, 0x0, &endBSSChip-&startBSSChip);
#endif
#endif
}

__EXTERNAL void
init_amiga(void)
{
  custom->dmacon = 0x7ff;  /* disable all dma */
  custom->intena = 0x7fff; /* disable all interrupts */

  hw_waitVerticalBlank();

  custom->intena = 0x7fff; /* disable all interrupts */
  custom->dmacon = 0x7fff; /* disable all dma */
  custom->intreq = 0x7fff; /* Clear all INT requests */
  custom->intreq = 0x7fff; /* Clear all INT requests */


  /* AGA compatibility stuff */
  custom->fmode = 0;
  custom->bplcon2 = 0x24;
  //custom->bplcon2 = 0x0;
  custom->bplcon3 = 0xc00;
  custom->bplcon4 = 0x11;

  custom->dmacon = (DMAF_BLITTER|DMAF_SETCLR|DMAF_MASTER);

  memory_ctor();
  gfx_ctor();
  text_ctor();

#if TRACKLOADER==0
  akiko_ctor();
#endif

#ifdef GAME_KEYBOARD_ENABLED
  keyboard_ctor();
#endif
}
