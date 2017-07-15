#include "game.h"

extern void palette_menuInstall(void);
static frame_buffer_t menu_frameBuffer;

#define MENU_START_Y               102
#define MENU_TOP_COLOR             0xfb5
#define MENU_BOTTOM_COLOR          0xb72
#define MENU_TOP_COLOR_SELECTED    0xffb
#define MENU_BOTTOM_COLOR_SELECTED 0xfb5

#if MUSIC_PARALLEL_LOAD==0
static __SECTION_RANDOM_C uint8_t menu_offscreenBuffer[(SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH*9)+2];
#else
__EXTERNAL void* music_spare_ptr;
#define menu_offscreenBuffer ((uint8_t*)music_spare_ptr)
#endif
static frame_buffer_t menu_offscreen;// = &menu_offscreenBuffer[0];
static frame_buffer_t menu_scrollerFB;
static uint16_t menu_scrollerCounter;
static char* menu_scrollerPtr;
static uint16_t menu_scrollerMode;
static uint8_t menu_lastJoystick;
static char* menu_scrollerText;
static uint16_t menu_demoCount;

static char* menu_creditsText =
  "CREDITS:MUSIC/ART/SFX COPYRIGHT (c) 2017 AMIGA WAVE...  GAME ENGINE COPYRIGHT (c) 2017 ENABLE SOFTWARE PTY LTD...  ALL RIGHTS RESERVED...   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE....            ";

#ifdef GAME_KEYBOARD_ENABLED
static char* menu_helpText = "WELCOME TO ELSE WE GET MAD!... PRESS ESC TO QUIT GAME... PRESS P TO PAUSE... PRESS M TO TOGGLE MUSIC... PRESS D TO WATCH A DEMO...                                            ";
#else
static char* menu_helpText = "WELCOME TO ELSE WE GET MAD!... CD32 VERSION!...                                                ";
#endif




typedef struct {
  uint16_t wait1[2];
  uint16_t color1[2];
  uint16_t wait2[2];
  uint16_t color2[2];
} menu_line_copper_t;

typedef struct {
  uint16_t bpl1[SCREEN_BIT_DEPTH*2*2];
  menu_line_copper_t lines[MENU_MAX_ITEMS];
  uint16_t wrap[2];
  menu_line_copper_t scrollerLine;
  uint16_t end[2];
} menu_copper_t;

static uint16_t menu_selected = 0;
static uint16_t menu_first = 0;

#define MENU_TEXT_START (0x83d1+((RASTER_Y_START-0x1d)*0x100))
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

static  __SECTION_DATA_C  menu_copper_t menu_copper  = {
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
    MENU_COPPER_LINE(MENU_TOP_COLOR, MENU_BOTTOM_COLOR, 6),
#if TRACKLOADER==0
    MENU_COPPER_LINE(MENU_TOP_COLOR, MENU_BOTTOM_COLOR, 7),
#endif
  },

  .wrap = {0xffdf,0xfffe},

  .scrollerLine = {
    .wait1 =  {0x1adf, 0xfffe},
    .color1 = { COLOR01, MENU_TOP_COLOR},
    .wait2 =  {0x1edf, 0xfffe},
    .color2 = { COLOR01, MENU_BOTTOM_COLOR},
  },

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
menu_toggleMode(void);
static void
menu_showHiScores(void);
static void
menu_select(uint16_t i);
static void
menu_credits(void);
static void
menu_scrollerRender(void);

static 
menu_mode_t menu_mode = MENU_MODE_INACTIVE;
static
menu_item_t menu_items[MENU_MAX_ITEMS] = {
  {
    .text = I18N_PLAY_NOW,
    .command = MENU_COMMAND_PLAY,
    .done = 1,
    .callback = 0
  },
  {
    .text = I18N_SINGLE_PLAYER,
    .command = MENU_COMMAND_NUM_PLAYERS,
    .done = 0,
    .callback = menu_toggleNumPlayers
  },
  {
    .text = I18N_MUSIC_ON,
    .command = MENU_COMMAND_PLAY,
    .done = 0,
    .callback = menu_toggleMusic
  },
  {
    .text = I18N_MODE_HARD,
    .command = MENU_COMMAND_MODE,
    .done = 0,
    .callback = menu_toggleMode
  },
  {
    .text = I18N_HI_SCORES,
    .command = MENU_COMMAND_PLAY,
    .done = 0,
    .callback = menu_showHiScores
  },
  {
    .text = I18N_CREDITS,
    .command = MENU_COMMAND_PLAY,
    .done = 0,
    .callback = menu_credits
  },

#if TRACKLOADER==0
  {
    .text = "QUIT",
    .command = MENU_COMMAND_EXIT,
    .done = 1,
    .callback = 0
  },
#endif
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


static int16_t
menu_processKeyboard(void)
{
  if (keyboard_key) {
    menu_demoCount = 0;
  } else {
    menu_demoCount++;
  }

  if (menu_demoCount == 50*28) {
    if (menu_scrollerText != menu_creditsText) {
      game_demo = 1;
      return MENU_COMMAND_REPLAY;
    } else {
      menu_demoCount = 0;
    }
  }
  
  switch (keyboard_key) {
#ifdef DEBUG
  case ' ':
    menu_scrollerMode = 0xff;
    return -1;
    break;
  case 'Z':
    music_next();
    return -1;
    break;
#endif
  case 'D':
    game_demo = '1';
    return MENU_COMMAND_REPLAY;
    break;
  case 'L':
    game_numPlayers = 1;
    return MENU_COMMAND_REPLAY;
    break;
  case 'R':
    return MENU_COMMAND_RECORD;
    break;
  case '1':
    game_numPlayers = 1;    
    //return MENU_COMMAND_LEVEL;
    game_startLevelIndex = 0;
    return -1;
    break;
  case '2':
    game_numPlayers = 1;
    //return MENU_COMMAND_LEVEL;
    game_startLevelIndex = 1;
    return -1;    
    break;
#if TRACKLOADER==0
  case KEYBOARD_CODE_ESC:
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
  
  
  if (len == 0) {
    gfx_screenWidthBitBlitNoMask(fb, game_messageBuffer, 0, y-(SCREEN_HEIGHT-PLAYAREA_HEIGHT), 0, y, SCREEN_WIDTH, 9);    
    return;
  }

  gfx_screenWidthBitBlitNoMask(menu_offscreen, game_messageBuffer, 0, y-(SCREEN_HEIGHT-PLAYAREA_HEIGHT), 0, 0, SCREEN_WIDTH, 9);    

  fb = menu_offscreen;
  int16_t _y = 0;
  
  text_drawMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, _y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_drawMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, _y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, _y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, _y+1);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2)+1, _y+1);


  fb = menu_offscreen;
  
  text_drawMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), _y);
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), _y);  
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), _y);  
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), _y);  
  fb += MENU_SCREEN_WIDTH_BYTES;
  text_clearMaskedText8Blitter(fb, text, (MENU_SCREEN_WIDTH/2)-(len<<2), _y);


  gfx_screenWidthBitBlitNoMask(_fb, menu_offscreen, 0, 0, 0, y, SCREEN_WIDTH, 9);    
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
  frame_buffer_t fb = game_menuBuffer;
  int16_t y = MENU_START_Y + (i*16);

  if (menu_mode == MENU_MODE_MENU) {
    menu_renderText(fb, menu_items[i].text, y);
  } else {
    if (i == 0) {
      menu_renderText(fb, I18N_HI_SCORES, y);
    } else {
      if (i-1 < HISCORE_NUM_SCORES) {
	menu_renderText(fb, menu_hiscores[i-1].text, y);
      } else {
	menu_renderText(fb, I18N_BLANK_GAME_OVER, y);
      }
    }
  }
}

static void menu_refresh(void)
{
  for (uint16_t i = 0; i < MENU_MAX_ITEMS; i++) {
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
  for (uint16_t i = 0; i < MENU_MAX_ITEMS; i++) {
    if (menu_items[i].callback == menu_toggleMusic) {
      if (music_enabled()) {
	menu_items[i].text = I18N_MUSIC_ON;
      } else {
	menu_items[i].text = I18N_MUSIC_OFF;
      }
      return;
    }
  }
}


static void
menu_updateNumPlayersMenu(void)
{
  for (uint16_t i = 0; i < MENU_MAX_ITEMS; i++) {
    if (menu_items[i].callback == menu_toggleNumPlayers) {
      if (game_numPlayers == 1) {
	menu_items[i].text = I18N_SINGLE_PLAYER;
      } else {
	menu_items[i].text = I18N_2_PLAYERS;
      }
      return;
    }
  }
}


static void
menu_updateModeMenu(void)
{
  for (uint16_t i = 0; i < MENU_MAX_ITEMS; i++) {
    if (menu_items[i].callback == menu_toggleMode) {
      if (game_difficulty == GAME_DIFFICULTY_EASY) {
	menu_items[i].text = I18N_MODE_EASY;
      } else {
	menu_items[i].text = I18N_MODE_HARD;
      }
      return;
    }
  }
}


static void
menu_render(void)
{
  menu_update_music_menu();
  menu_updateNumPlayersMenu();
  menu_updateModeMenu();

  frame_buffer_t fb = game_menuBuffer;
  uint16_t y = MENU_START_Y;

  switch (menu_mode) {
  case MENU_MODE_MENU:
    for (uint16_t i = 0; i < MENU_NUM_ITEMS; i++) {
      menu_renderText(fb, menu_items[i].text, y);
      y+= 16;
    }
    break;
  case MENU_MODE_HISCORES:
    menu_renderText(fb, I18N_HI_SCORES, y);
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
  sound_playSound(SOUND_MENU);  
  menu_redraw(menu_selected);
}

static void
menu_toggleNumPlayers(void)
{
  game_numPlayers = game_numPlayers == 1 ? 2 : 1;
  menu_updateNumPlayersMenu();
  sound_playSound(SOUND_MENU);  
  menu_redraw(menu_selected);
}


static void
menu_toggleMode(void)
{
  game_difficulty = game_difficulty == GAME_DIFFICULTY_HARD ? GAME_DIFFICULTY_EASY : GAME_DIFFICULTY_HARD;
  menu_updateModeMenu();
  sound_playSound(SOUND_MENU);  
  menu_redraw(menu_selected);
}


static void
menu_select(uint16_t i)
{
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
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR;
    menu_selected--;
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR_SELECTED;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR_SELECTED;
    menu_demoCount = 0;
  }
}

static void
menu_down(void)
{
  if (menu_mode == MENU_MODE_MENU && menu_selected < MENU_NUM_ITEMS-1) {
    sound_playSound(SOUND_MENU);
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR;
    menu_selected++;
    menu_copper.lines[menu_selected].color1[1] = MENU_TOP_COLOR_SELECTED;
    menu_copper.lines[menu_selected].color2[1] = MENU_BOTTOM_COLOR_SELECTED;
    menu_demoCount = 0;    
  }
}


static void
menu_scroller(char* text)
{
  switch (menu_scrollerMode) {
  case 0:
  case 2:
    menu_scrollerMode = 1;
    menu_scrollerFB = game_menuBuffer;  
    menu_scrollerFB += (((SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)*256));
    menu_scrollerCounter = 0;
    menu_scrollerPtr = text;
    menu_scrollerText = text;
    break;
  case 1:
    menu_scrollerMode = 2;
    menu_scrollerPtr = text;
    menu_scrollerText = text;
    menu_scrollerCounter = 0;
    break;
  } 
}


__EXTERNAL menu_command_t
menu_loop(menu_mode_t mode)
{
  menu_offscreen = &menu_offscreenBuffer[0];  
  extern uint8_t menu_menuBitplanes;
  menu_frameBuffer = &menu_menuBitplanes;
  menu_command_t command;
  uint16_t done;

  game_demo = 0;
  menu_demoCount = 0;
  menu_lastJoystick = 0;  
  menu_scrollerMode = 0;
  menu_scroller(menu_helpText);
#ifdef GAME_STARS
  custom->dmacon = DMAF_SPRITE;
#endif
  
  menu_mode = mode;

  menu_hiscores = hiscore_render();

  sound_init();

  uint32_t startFrame = 0;

  if (!menu_first) {
    logo_clear();
    startFrame = hw_verticalBlankCount;
    message_screenOn(I18N_PRESENTS);
  } else {
    message_loading(I18N_LOADING);
  }

  game_switchFrameBuffers();


#ifdef GAME_COMPRESS_DATA
  extern uint8_t menu_menuBitplanes;
  extern uint8_t menu_menuBitplanesEnd;  
  disk_loadCompressedData((void*)game_menuBuffer, (void*)menu_frameBuffer, &menu_menuBitplanesEnd-&menu_menuBitplanes, 0);
#else
  disk_loadData((void*)game_menuBuffer, (void*)menu_frameBuffer, MENU_SCREEN_WIDTH_BYTES*SCREEN_HEIGHT*SCREEN_BIT_DEPTH);
#endif
  
#ifndef GAME_TRIPLE_BUFFER
  game_scoreboardLoaded = 0;
#endif
  
  if (!menu_first) {
    while (hw_verticalBlankCount < startFrame+200) {
      hw_waitVerticalBlank();
    }
    menu_first = 1;
  }
  
  message_screenOff();

  custom->dmacon = DMAF_RASTER;

  game_switchFrameBuffers();  

  for (uint16_t y = 0; y < PLAYAREA_HEIGHT; y += PLAYAREA_HEIGHT/8) {
    gfx_screenWidthBitBlitNoMask(game_messageBuffer, game_menuBuffer, 0, y+(SCREEN_HEIGHT-PLAYAREA_HEIGHT), 0, y, SCREEN_WIDTH, PLAYAREA_HEIGHT/8);
  }

  palette_black();

  uint16_t volatile* copperPtr = (uint16_t*)&menu_copper;

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

  menu_pokeCopperList(game_menuBuffer);  
  /* install copper list, then enable dma and selected interrupts */
  custom->cop1lc = (uint32_t)copperPtr;
  //  scratch = custom->copjmp1;

  custom->dmacon = (DMAF_SETCLR|DMAF_COPPER|DMAF_RASTER);

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

#ifdef DEBUG
    if (game_startReplay) {
      game_difficulty = GAME_DIFFICULTY_HARD;
      game_scoreboardLoaded = 0;
      game_numPlayers = 1;
      command = MENU_COMMAND_REPLAY;
      done = 1;
    }
#endif
    
      
    if (game_fire()) {
      if (menu_mode == MENU_MODE_HISCORES) {
	menu_mode = MENU_MODE_MENU;
	menu_refresh();
      } else {
	menu_demoCount = 0;
	command = menu_items[menu_selected].command;
	done = menu_items[menu_selected].done;
	if (menu_items[menu_selected].callback != 0) {
	  menu_items[menu_selected].callback();
	}
      }
    }    

    
    if (JOYSTICK_DOWN()) {
      if (menu_lastJoystick != JOYSTICK_POS_DOWN) {
	menu_down();
      }
    } else if (JOYSTICK_UP()) {
      if (menu_lastJoystick != JOYSTICK_POS_UP) {      
	menu_up();
      }
    }

    menu_lastJoystick = hw_joystickPos;
    
    int16_t kbCommand;
    if ((kbCommand = menu_processKeyboard()) != -1) {
      command = kbCommand;
      done = 1;
    }

    if (menu_scrollerMode == 0) {
      hw_waitVerticalBlank();
    } else {
      menu_scrollerRender();
    }
  }

  hw_waitVerticalBlank();
  palette_black();
  custom->dmacon = DMAF_RASTER|DMAF_COPPER;


  menu_mode = MENU_MODE_INACTIVE;

  return command;
}


static void
menu_scrollerRender(void)
{
  volatile struct Custom* _custom = CUSTOM;    
  int32_t shift = 1;

  if (menu_scrollerMode == 0xff) {
    shift = 15;
  } else if (menu_scrollerMode == 2) {
    shift = 15;
    if (menu_scrollerCounter++ > 25) {
      menu_scrollerMode = 1;
      menu_scrollerCounter = 0;
    }
  } else {  
    if (menu_scrollerCounter == 7) {
      char buffer[2] = {0, 0};
      if (*menu_scrollerPtr == 0) {
	menu_scrollerPtr = menu_scrollerText;
      }
      buffer[0] = *menu_scrollerPtr;
      menu_scrollerPtr++;
      if (menu_scrollerMode == 1) {
	text_drawMaskedText8Blitter(game_menuBuffer, buffer, SCREEN_WIDTH-8, SCREEN_HEIGHT-11);
      }
      menu_scrollerCounter = 0;
    } else {
      menu_scrollerCounter++;
    }
  }
  
  hw_waitBlitter();   
  _custom->bltafwm = 0x0;
  _custom->bltalwm = 0xffff;
  _custom->bltcon0 = (SRCA|DEST|0xf0|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT|BC1F_DESC;
  _custom->bltamod = 0;
  _custom->bltdmod = 0;
  
  _custom->bltapt = (uint8_t*)menu_scrollerFB;
  _custom->bltdpt = (uint8_t*)menu_scrollerFB;
  
  hw_waitVerticalBlank();    
  _custom->bltsize = ((10*SCREEN_BIT_DEPTH)<<6) | (SCREEN_WIDTH_WORDS);
}


void
menu_credits(void)
{
  if (menu_scrollerMode != 2) {
    if (menu_scrollerText == menu_creditsText) {
      menu_scroller(menu_helpText);
    } else {
      menu_scroller(menu_creditsText);    
    }
  }
}
