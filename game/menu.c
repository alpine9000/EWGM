#include "game.h"
// http://www.picturetopeople.org/text_generator/others/3d/3d-perspective-text-effect-creator.html

extern void palette_menuInstall(void);
extern frame_buffer_t menu_frameBuffer;

#define MENU_NUM_ITEMS             6
#define MENU_START_Y               110
#define MENU_TOP_COLOR             0xfb5
#define MENU_BOTTOM_COLOR          0xb72
#define MENU_TOP_COLOR_SELECTED    0xffb
#define MENU_BOTTOM_COLOR_SELECTED 0xfb5

typedef struct {
  uint16_t wait1[2];
  uint16_t color1[2];
  uint16_t wait2[2];
  uint16_t color2[2];
} menu_line_copper_t;

typedef struct {
  uint16_t bpl1[SCREEN_BIT_DEPTH*2*2];
  menu_line_copper_t lines[MENU_NUM_ITEMS];
  menu_line_copper_t lastLine;
  /*uint16_t wrap[2];
    menu_line_copper_t overflowLine;*/
  uint16_t end[2];
} menu_copper_t;

static uint16_t menu_selected = 0;
static uint16_t menu_first = 0;

#define MENU_TEXT_START (0x8bd1+((RASTER_Y_START-0x1d)*0x100))
#define MENU_COPPER_WAIT_TOP(x)     { MENU_TEXT_START + (0x800*(x*2)), 0xfffe}
#define MENU_COPPER_WAIT_BOTTOM(x)  { MENU_TEXT_START + 0x400 + (0x800*(x*2)), 0xfffe}
#define MENU_COPPER_LINE(c1, c2, x) [x] = {	\
    .wait1 = MENU_COPPER_WAIT_TOP(x),\
   .color1 = { COLOR01, c1},\
   .wait2 =  MENU_COPPER_WAIT_BOTTOM(x),\
   .color2 = { COLOR01, c2},\
   }


static uint16_t menuPalette[32] = {
  #include "out/palette_menu.h"
};

static  __section(data_c)  menu_copper_t menu_copper  = {
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
  .lines = {
    MENU_COPPER_LINE(MENU_TOP_COLOR_SELECTED, MENU_BOTTOM_COLOR_SELECTED, 0),
    MENU_COPPER_LINE(MENU_TOP_COLOR, MENU_BOTTOM_COLOR, 1),
    MENU_COPPER_LINE(MENU_TOP_COLOR, MENU_BOTTOM_COLOR, 2),
    MENU_COPPER_LINE(MENU_TOP_COLOR, MENU_BOTTOM_COLOR, 3),
    MENU_COPPER_LINE(MENU_TOP_COLOR, MENU_BOTTOM_COLOR, 4),
    MENU_COPPER_LINE(MENU_TOP_COLOR, MENU_BOTTOM_COLOR, 5),
  },

  .lastLine = {
    .wait1 = MENU_COPPER_WAIT_TOP(6),
   .color1 = { COLOR01, MENU_TOP_COLOR},
    .wait2 =  MENU_COPPER_WAIT_BOTTOM(6),
    .color2 = { COLOR01, MENU_BOTTOM_COLOR},
  },
  /*  .wrap = {0xffdf,0xfffe},
  .overflowLine = {
    .wait1 =  {0x0001, 0xfffe},
    .color1 = { COLOR01, MENU_TOP_COLOR},
    .wait2 =  {0x03df, 0xfffe},
   .color2 = { COLOR01, MENU_BOTTOM_COLOR}, 
   },*/
  .end = {0xFFFF, 0xFFFE}
};

typedef struct {
  char* text;
  menu_command_t command;
  int16_t done;
  void (*callback)(void);
} menu_item_t;


static void
menu_render(void);
static void
menu_toggleMusic(void);
static void
menu_toggleNumPlayers(void);
static void
menu_showHiScores(void);
static void
menu_select(uint16_t i);

static 
menu_mode_t menu_mode = MENU_MODE_INACTIVE;
static
menu_item_t menu_items[MENU_NUM_ITEMS+1] = {
  {
    .text = "PLAY NOW!",
    .command = MENU_COMMAND_PLAY,
    .done = 1,
    .callback = 0
  },
  {
    .text = "SINGLE PLAYER",
    .command = MENU_COMMAND_NUM_PLAYERS,
    .done = 0,
    .callback = menu_toggleNumPlayers
  },
  {
    .text = "PLAY RECORDING",
    .command = MENU_COMMAND_REPLAY,
    .done = 1,
    .callback = 0
  },
  {
    .text = "MUSIC - ON ",
    .command = MENU_COMMAND_PLAY,
    .done = 0,
    .callback = menu_toggleMusic
  },
  {
    .text = "HI SCORES",
    .command = MENU_COMMAND_PLAY,
    .done = 0,
    .callback = menu_showHiScores
  },
  {
    .text = "CREDITS",
    .command = MENU_COMMAND_PLAY,
    .done = 0,
    .callback = 0
  },
  {
    .text = "",
    .command = MENU_COMMAND_PLAY,
    .done = 0,
    .callback = 0
  }
};


hiscore_t* menu_hiscores;  

static void 
menu_pokeCopperList(frame_buffer_t frameBuffer)
{
  uint16_t volatile* copperPtr = (uint16_t*)&menu_copper;
  /* poke bitplane pointers into copper list */
  uint32_t bitplanesPtr = (uint32_t)frameBuffer;

  for (int16_t i = 0; i < SCREEN_BIT_DEPTH; i++) {
    copperPtr[1] = (uint16_t)bitplanesPtr;
    copperPtr[3] = (uint16_t)(((uint32_t)bitplanesPtr)>>16);
    bitplanesPtr = bitplanesPtr + (MENU_SCREEN_WIDTH_BYTES);
    copperPtr = copperPtr + 4;
  }
}

#if TRACKLOADER==1
#ifdef DEBUG
static inline void
debug_hiscoreStress(void)
{
  for (int16_t i = 0; i < 100; i++) {
    hiscore_saveData(1);
    hw_waitVerticalBlank();
    message_loading("Loading hiscore...");
    if (hiscore_load(1)) {
      message_loading("Loading failed...");
      hw_waitScanLines(200);
    }
    hw_waitVerticalBlank();
  }
  
  message_screenOff();
}
#endif
#endif

static int16_t
menu_processKeyboard(void)
{
  switch (keyboard_key) {
  case 'Z':
    music_next();
    return -1;
    break;
#if TRACKLOADER==1
#ifdef DEBUG
  case 'Y':
    debug_hiscoreStress();
    return MENU_COMMAND_LEVEL;
    break;
#endif
#endif
  case 'A':
    if (message_ask("ask test, menu? y/n")) {
      return MENU_COMMAND_MENU;
    }
    return MENU_COMMAND_LEVEL;
    break;
  case 'P':
    return MENU_COMMAND_REPLAY;
    break;
  case 'R':
    return MENU_COMMAND_RECORD;
    break;
  case '1':
    game_numPlayers = 1;    
    return MENU_COMMAND_LEVEL;
    break;
  case '2':
    game_numPlayers = 2;
    return MENU_COMMAND_LEVEL;
    break;
#if TRACKLOADER==0
  case 'Q':
    return MENU_COMMAND_EXIT;
    break;
#endif
  default:
    return -1;
  }
}


static void
menu_renderText(frame_buffer_t _fb, char* text, uint16_t y)
{
  uint16_t len = strlen(text);

  frame_buffer_t fb = _fb;
  
  gfx_screenWidthBitBlitNoMask(fb, game_offScreenBuffer, 0, y, 0, y, SCREEN_WIDTH, 9);  

  if (len == 0) {
    return;
  }
  
  text_drawMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_drawMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, y+1);


  fb = _fb;
  
  text_drawMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), y);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), y);  
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), y);  
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), y);  
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), y);

}


void
menu_fillRect(frame_buffer_t fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  static custom_t _custom = CUSTOM;
  static uint16_t startBitPatterns[] = { 0xffff,
			       0x7fff, 0x3fff, 0x1fff, 0x0fff, 
			       0x07ff, 0x03ff, 0x01ff, 0x00ff,
			       0x007f, 0x003f, 0x001f, 0x000f,
			       0x0007, 0x0003, 0x0001, 0x0000 };

  static uint16_t endBitPatterns[] = { 0xffff, 
				    0x8000, 0xc000, 0xe000, 0xf000,
				    0xf800, 0xfc00, 0xfe00, 0xff00,
				    0xff80, 0xffc0, 0xffe0, 0xfff0,
				    0xfff8, 0xfffc, 0xfffe, 0xffff};

  uint16_t startMask = startBitPatterns[x & 0xf]; 
  uint16_t endMask = endBitPatterns[(x+w) & 0xf]; 
  uint32_t widthWords = (((x&0x0f)+w)+15)>>4;
  
  if (widthWords == 1) {
    startMask &= endMask;
  }
  
  fb += y*(MENU_SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)/*gfx_dyOffsetsLUT[y]*/ + (x>>3);

  int32_t colorInPlane;
  for (int32_t plane = 0; plane < SCREEN_BIT_DEPTH; plane++) {
    colorInPlane = (1<<plane) & color;
    hw_waitBlitter();
    
    _custom->bltcon0 = (SRCC|DEST|0xca);
    _custom->bltcon1 = 0;
    _custom->bltafwm = 0xffff;
    _custom->bltalwm = 0xffff;
    _custom->bltdmod = (MENU_SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(MENU_SCREEN_WIDTH_BYTES-2);
    _custom->bltcmod = (MENU_SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(MENU_SCREEN_WIDTH_BYTES-2);
    _custom->bltbmod = 0;
    _custom->bltamod = 0;
    _custom->bltadat = startMask;
    _custom->bltbdat = colorInPlane ? 0xffff : 0x0;
    _custom->bltcpt = (uint8_t*)fb;
    _custom->bltdpt = (uint8_t*)fb;
    _custom->bltsize = h<<6 | 1;
    
    if (widthWords > 1) {
      hw_waitBlitter();    
      _custom->bltcon0 = (SRCC|DEST|0xca);
      _custom->bltadat = endMask;
      _custom->bltcpt = (uint8_t*)fb+((widthWords-1)<<1);
      _custom->bltdpt = (uint8_t*)fb+((widthWords-1)<<1);
      _custom->bltsize = h<<6 | 1;
    }
    
    if (widthWords > 2) {
      hw_waitBlitter();    
      _custom->bltcon0 = (DEST|(colorInPlane ? 0xff : 0x00));
      _custom->bltdmod = (MENU_SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(MENU_SCREEN_WIDTH_BYTES-((widthWords-2)<<1));
      _custom->bltdpt = (uint8_t*)fb+2;
      _custom->bltsize = h<<6 | (widthWords-2);
    }    

    fb += MENU_SCREEN_WIDTH_BYTES;
  }
}

static void
menu_redraw(uint16_t i)
{
  frame_buffer_t fb = game_onScreenBuffer;
  int16_t y = MENU_START_Y + (i*16);


  hw_waitVerticalBlank();

  if (menu_mode == MENU_MODE_MENU) {
    menu_renderText(fb, menu_items[i].text, y);
  } else {
    if (i == 0) {
      menu_renderText(fb, "HI SCORES", y);
    } else {
      menu_renderText(fb, menu_hiscores[i-1].text, y);
    }
  }
}

static void menu_refresh(void)
{
  for (uint16_t i = 0; i < MENU_NUM_ITEMS+1; i++) {
    menu_redraw(i);
  }    
}

static void
menu_showHiScores(void)
{
  menu_mode = MENU_MODE_HISCORES;
  menu_select(0);
  menu_refresh();
}

static void
menu_update_music_menu(void)
{
  for (uint16_t i = 0; i < MENU_NUM_ITEMS; i++) {
    if (menu_items[i].callback == menu_toggleMusic) {
      if (music_enabled()) {
	menu_items[i].text = "MUSIC - ON ";
      } else {
	menu_items[i].text = "MUSIC - OFF";
      }
      return;
    }
  }
}


static void
menu_updateNumPlayersMenu(void)
{
  for (uint16_t i = 0; i < MENU_NUM_ITEMS; i++) {
    if (menu_items[i].callback == menu_toggleNumPlayers) {
      if (game_numPlayers == 1) {
	menu_items[i].text = "SINGLE PLAYER";
      } else {
	menu_items[i].text = "  2 PLAYERS  ";
      }
      return;
    }
  }
}


static void
menu_render(void)
{
  menu_update_music_menu();

  frame_buffer_t fb = game_onScreenBuffer;
  //  fb += 2*MENU_SCREEN_WIDTH_BYTES;
  uint16_t y = MENU_START_Y;

  switch (menu_mode) {
  case MENU_MODE_MENU:
    for (uint16_t i = 0; i < MENU_NUM_ITEMS; i++) {
      menu_renderText(fb, menu_items[i].text, y);
      y+= 16;
    }
    break;
  case MENU_MODE_HISCORES:
    menu_renderText(fb, "HI SCORES", y);
    y+= 16;
    for (uint16_t i = 0; i < HISCORE_NUM_SCORES; i++) {
      menu_renderText(fb, menu_hiscores[i].text, y);
      y+= 16;
    }
    break;
  default:
    break;
  }
}


static void
menu_toggleMusic(void)
{
  music_toggle();
  menu_update_music_menu();
  menu_redraw(menu_selected);
}

static void
menu_toggleNumPlayers(void)
{
  game_numPlayers = game_numPlayers == 1 ? 2 : 1;
  menu_updateNumPlayersMenu();
  menu_redraw(menu_selected);
}


static void
menu_select(uint16_t i)
{
  hw_waitVerticalBlank();
  menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR;
  menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR;
  menu_selected = i;
  menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR_SELECTED;
  menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR_SELECTED;    
}

static void
menu_up(void)
{
  if (menu_mode == MENU_MODE_MENU && menu_selected > 0) {
    sound_playSound(SOUND_MENU);
    hw_waitVerticalBlank();
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR;
    menu_selected--;
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR_SELECTED;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR_SELECTED;    
    do {
      hw_readJoystick();
    } while (JOYSTICK_UP());
  }
}

static void
menu_down(void)
{
  if (menu_mode == MENU_MODE_MENU && menu_selected < MENU_NUM_ITEMS-1) {
    sound_playSound(SOUND_MENU);    
    hw_waitVerticalBlank();
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR;
    menu_selected++;
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR_SELECTED;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR_SELECTED;    

    do {
      hw_readJoystick();
    } while (JOYSTICK_DOWN());
  }
}


__EXTERNAL menu_command_t
menu_loop(menu_mode_t mode)
{
  menu_command_t command;
  uint16_t done;
  volatile uint16_t scratch;

  menu_mode = mode;

  menu_hiscores = hiscore_render();

  sound_init();

  uint32_t startFrame = 0;

  if (!menu_first) {
    logo_clear();
    startFrame = hw_verticalBlankCount;
    message_screenOn("PRESENTS");
  } else {
    message_loading("LOADING...");
  }

  game_switchFrameBuffers();    
  
  disk_loadData((void*)game_offScreenBuffer, (void*)menu_frameBuffer, MENU_SCREEN_WIDTH_BYTES*SCREEN_HEIGHT*SCREEN_BIT_DEPTH);

  //memset((void*)game_offScreenBuffer, 0, MENU_SCREEN_WIDTH_BYTES*SCREEN_HEIGHT*SCREEN_BIT_DEPTH);

  if (!menu_first) {
    while (hw_verticalBlankCount < startFrame+200) {
      hw_waitVerticalBlank();
    }
    menu_first = 1;
  }
  
  message_screenOff();

  custom->dmacon = DMAF_RASTER;

  game_switchFrameBuffers();  

  for (uint16_t y = 0; y < SCREEN_HEIGHT; y += SCREEN_HEIGHT/8) {
    gfx_screenWidthBitBlitNoMask(game_offScreenBuffer, game_onScreenBuffer, 0, y, 0, y, SCREEN_WIDTH, SCREEN_HEIGHT/8);
  }

  palette_black();

  uint16_t volatile* copperPtr = (uint16_t*)&menu_copper;

  hw_waitBlitter();  
  custom->dmacon = (DMAF_BLITTER|DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);    
  
  /* set up playfield */
  
  custom->diwstrt = (RASTER_Y_START<<8)|RASTER_X_START;
  custom->diwstop = ((SCREEN_RASTER_Y_STOP-256)<<8)|(RASTER_X_STOP-256);

  custom->ddfstrt = (RASTER_X_START/2-SCREEN_RES);
  custom->ddfstop = (RASTER_X_START/2-SCREEN_RES)+(8*((MENU_SCREEN_WIDTH/16)-1));
  custom->bplcon0 = (SCREEN_BIT_DEPTH<<12)|0x200;
  custom->bplcon1 = 0;
  custom->bpl1mod = (MENU_SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-MENU_SCREEN_WIDTH_BYTES;
  custom->bpl2mod = (MENU_SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-MENU_SCREEN_WIDTH_BYTES;

  menu_pokeCopperList(game_onScreenBuffer);  
  /* install copper list, then enable dma and selected interrupts */
  custom->cop1lc = (uint32_t)copperPtr;
  //  scratch = custom->copjmp1;
  USE(scratch);

  custom->dmacon = (DMAF_BLITTER|DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER|DMAF_MASTER);

  palette_fadeTo(menuPalette, 32, 0);

  menu_render();

  hw_waitVerticalBlank();

  command = MENU_COMMAND_PLAY;
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
      if (menu_mode == MENU_MODE_HISCORES) {
	menu_mode = MENU_MODE_MENU;
	menu_refresh();
      } else {
	command = menu_items[menu_selected].command;
	done = menu_items[menu_selected].done;
	if (menu_items[menu_selected].callback != 0) {
	  menu_items[menu_selected].callback();
	}
      }
    }
    if (JOYSTICK_DOWN()) {
      menu_down();
    } else if (JOYSTICK_UP()) {
      menu_up();
    }
    int16_t kbCommand;
    if ((kbCommand = menu_processKeyboard()) != -1) {
      command = kbCommand;
      done = 1;
    }
    hw_waitVerticalBlank();
  }

  hw_waitVerticalBlank();
  custom->dmacon = DMAF_RASTER;
  palette_black();

  menu_mode = MENU_MODE_INACTIVE;

  return command;
}
