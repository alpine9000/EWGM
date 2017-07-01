#include "game.h"

#define PLAYER_SELECT_TOP_COLOR 0xffb
#define PLAYER_SELECT_BOTTOM_COLOR 0xfb5
typedef struct {
  uint16_t bpl1[SCREEN_BIT_DEPTH*2*2];
  uint16_t end[2];
} player_select_copper_t;

static uint16_t player_select_palette[32] = {
  #include "out/palette_player_select.h"
};

static  __SECTION_DATA_C  player_select_copper_t player_select_copper  = {
  .bpl1 = {
    BPL1PTL,0x0000,
    BPL1PTH,0x0000,
    BPL2PTL,0x0000,
    BPL2PTH,0x0000,
    BPL3PTL,0x0000,
    BPL3PTH,0x0000,
    BPL4PTL,0x0000,
    BPL4PTH,0x0000,
    BPL5PTL,0x0000,
    BPL5PTH,0x0000,
  },

  .end = {0xFFFF, 0xFFFE}
};

static void 
player_select_pokeCopperList(frame_buffer_t frameBuffer)
{
  uint16_t volatile* copperPtr = (uint16_t*)&player_select_copper;
  /* poke bitplane pointers into copper list */
  uint32_t bitplanesPtr = (uint32_t)frameBuffer;

  for (int16_t i = 0; i < SCREEN_BIT_DEPTH; i++) {
    copperPtr[1] = (uint16_t)bitplanesPtr;
    copperPtr[3] = (uint16_t)(((uint32_t)bitplanesPtr)>>16);
    bitplanesPtr = bitplanesPtr + (MENU_SCREEN_WIDTH_BYTES);
    copperPtr = copperPtr + 4;
  }
}


uint16_t
player_select(void)
{
  static uint8_t lastJoystick;
  uint16_t done;
  uint16_t selectedIndex = 0;

  lastJoystick = 0;  

  extern uint8_t player_selectBitplanes;
#ifdef GAME_COMPRESS_DATA
  extern uint8_t player_selectBitplanesEnd;  
  disk_loadCompressedData((void*)game_menuBuffer, (void*)&player_selectBitplanes, &player_selectBitplanesEnd-&player_selectBitplanes, 0);
#else
  disk_loadData((void*)game_menuBuffer, (void*)&player_selectBitplanes, MENU_SCREEN_WIDTH_BYTES*SCREEN_HEIGHT*SCREEN_BIT_DEPTH);
#endif
  
#ifndef GAME_TRIPLE_BUFFER
  game_scoreboardLoaded = 0;
#endif
  
  
#ifndef GAME_TRIPLE_BUFFER
  game_scoreboardLoaded = 0;
#endif
  
  message_screenOff();

  custom->dmacon = DMAF_RASTER;

  game_switchFrameBuffers();  

  palette_black();

  uint16_t volatile* copperPtr = (uint16_t*)&player_select_copper;

  hw_waitBlitter();  
  custom->dmacon = (DMAF_COPPER|DMAF_RASTER);    
  
  /* set up playfield */
  
  custom->diwstrt = (RASTER_Y_START<<8)|RASTER_X_START;
  custom->diwstop = ((SCREEN_RASTER_Y_STOP-256)<<8)|((RASTER_X_START+SCREEN_WIDTH-16)-256);

  custom->ddfstrt = (RASTER_X_START/2-SCREEN_RES);
  custom->ddfstop = (RASTER_X_START/2-SCREEN_RES)+(8*(((MENU_SCREEN_WIDTH)/16)-1));
  custom->bplcon0 = (SCREEN_BIT_DEPTH<<12)|0x200;
  custom->bplcon1 = 0;
  custom->bpl1mod = (MENU_SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-(MENU_SCREEN_WIDTH_BYTES);
  custom->bpl2mod = (MENU_SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-(MENU_SCREEN_WIDTH_BYTES);

  player_select_pokeCopperList(game_menuBuffer);  
  /* install copper list, then enable dma and selected interrupts */
  custom->cop1lc = (uint32_t)copperPtr;
  //  scratch = custom->copjmp1;

  custom->dmacon = (DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER);

  player_select_palette[1] = PLAYER_SELECT_TOP_COLOR;  
  player_select_palette[2] = PLAYER_SELECT_BOTTOM_COLOR;
  player_select_palette[3] = 0x0;
  player_select_palette[4] = 0x0;  
  
  palette_fadeTo(player_select_palette, 32, 0);

  hw_waitVerticalBlank();

  done = 0;  

  while (!done) {
    hw_readJoystick();
    keyboard_read();
#ifdef DEBUG
#ifdef SCRIPTING
    script_process();
#endif
#endif

    if (game_fire()) {
      done = 1;
    }    

    if (JOYSTICK_LEFT() || JOYSTICK_RIGHT()) {
      if (lastJoystick != hw_joystickPos) {
	sound_playSound(SOUND_MENU);	
	selectedIndex = !selectedIndex;
	if (!selectedIndex) {
	  custom->color[1] = PLAYER_SELECT_TOP_COLOR;
	  custom->color[2] = PLAYER_SELECT_BOTTOM_COLOR;	
	  custom->color[3] = 0x000;
	  custom->color[4] = 0x000;	  
	} else {
	  custom->color[3] = PLAYER_SELECT_TOP_COLOR;
	  custom->color[4] = PLAYER_SELECT_BOTTOM_COLOR;	
	  custom->color[1] = 0x000;
	  custom->color[2] = 0x000;	  
	}

      }
    }

    lastJoystick = hw_joystickPos;
    
    hw_waitVerticalBlank();
  }

  hw_waitVerticalBlank();
  palette_black();
  custom->dmacon = DMAF_RASTER|DMAF_COPPER;
  
  return selectedIndex;
}
	   
