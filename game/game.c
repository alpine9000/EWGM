#include "game.h"

#define GAME_SCORE_RASTERLINE_CUTOFF 330

#ifdef DEBUG
#define GAME_RASTERAVERAGE_LENGTH 16
#endif

static void
game_startLevel(menu_command_t command);
static void
game_loadLevel(menu_command_t command);
static void
game_render(uint16_t deltaT);
static void
game_scrollBackground(void);

frame_buffer_t game_offScreenBuffer;
frame_buffer_t game_onScreenBuffer;
frame_buffer_t game_menuBuffer;
frame_buffer_t game_messageBuffer;
frame_buffer_t game_scoreBoardFrameBuffer;
#ifdef GAME_TRIPLE_BUFFER
frame_buffer_t game_backScreenBuffer;
#endif
uint16_t game_numPlayers;
uint16_t game_bplcon1;
int16_t game_cameraX;
int16_t game_screenScrollX;
uint16_t game_level;
uint16_t game_over;
uint16_t game_loopControl;
uint32_t game_paused;
object_t* game_player1;
object_t* game_player2;
uint32_t game_player1Score;
uint32_t game_player2Score;
uint16_t game_difficulty;
uint16_t game_killScore;
uint16_t game_scoreboardLoaded;
uint16_t player1_character;
uint16_t game_demo;
uint16_t game_maxScrollX;
#ifdef DEBUG
uint16_t game_startReplay;
uint16_t game_collisions;
#endif
#ifndef RELEASE
uint16_t game_startLevelIndex;
#endif

static menu_command_t menuCommand;

static volatile __SECTION_RANDOM_C struct framebuffeData {
#ifdef DEBUG
  uint32_t canary1;
#endif
  // uint32_t overdraw1;
  uint8_t frameBuffer1[FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH*(FRAME_BUFFER_NUM_LINES)];
  //  uint32_t overdraw2;
  uint8_t frameBuffer2[FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH*(FRAME_BUFFER_NUM_LINES)];
#ifdef GAME_TRIPLE_BUFFER
  uint8_t frameBuffer3[FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH*(FRAME_BUFFER_NUM_LINES)];
#endif
  uint8_t scoreBoardBuffer[SCOREBOARD_BUFFER_SIZE_BYTES];
#ifdef DEBUG
  uint32_t canary2;
#endif
} game_frameBufferData;

typedef union {
  struct {
    uint16_t min;
    uint8_t sec10;
    uint8_t sec;
  };
  uint32_t value;
} time_t;

static int16_t game_lastPlayer1Health;
static int16_t game_lastPlayer2Health;
static uint32_t game_lastPlayer1Score;
static uint32_t game_lastPlayer2Score;
static int16_t game_scroll;
#ifdef DEBUG
static uint16_t game_singleStep;
#endif
static int16_t game_targetCameraX;
static uint32_t game_lastScrollFrame;
static uint16_t game_lastTileX;
static uint16_t game_deltaT;
static time_t game_levelTime;
static time_t game_lastLevelTime;
static uint16_t game_levelTicCounter;
static uint32_t game_lastVerticalBlankCount;
static uint16_t game_calculated25fps;
static uint16_t game_25fps;
static uint16_t game_newGame;

#ifdef GAME_TURTLE
static int16_t game_turtle;
__EXTERNAL int16_t game_missedFrameCount;
#endif

#ifdef DEBUG
static uint16_t game_maxRasterLine;
static uint16_t game_collectTotal;
static uint32_t game_total;
static uint32_t game_frame;
static uint16_t game_average;
static uint16_t game_lastAverage;
static uint16_t game_lastMaxRasterLine;
static int16_t game_lastEnemyCount;
static int16_t game_lastItemCount;
static int16_t game_scoreBoardMode = 0;
static int16_t game_debugRenderFrame;
#endif


__SECTION_DATA_C  copper_t copper  = {
   .bplcon1 = {
     BPLCON1,0xff
   },
   .ddfstrt = {
     DDFSTRT, (RASTER_X_START/2-SCREEN_RES)-8,
   },
   .bpl1mod = {
     BPL1MOD, (FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH-SCREEN_WIDTH_BYTES)-2,
   },
   .bpl2mod = {
     BPL2MOD, (FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH-SCREEN_WIDTH_BYTES)-2,
   },
   .bpl = {
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
   .wait = {
#if 1
     0x00e1|1, 0xfffe,
     ((GAME_RASTER_Y_STOP-SCOREBOARD_HEIGHT-1)<<8)|0xe1, 0xfffe,
#else
     0xffdf, 0xfffe,
     ((GAME_RASTER_Y_STOP-SCOREBOARD_HEIGHT-256)<<8)|0xe1, 0xfffe,
#endif
    },
   .sb_ddfstrt = {
     DDFSTRT, (RASTER_X_START/2-SCREEN_RES),
   },
   .sb_bplcon1 = {
     BPLCON1, 0x0
   },
   .sb_bpl1mod = {
     BPL1MOD, ((SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-SCREEN_WIDTH_BYTES),
   },
   .sb_bpl2mod = {
     BPL2MOD, ((SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)-SCREEN_WIDTH_BYTES),
     },
   .sb_bpl= {
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

#ifdef GAME_STARS
 .sprpt = {
    SPR0PTL,0x0000,
    SPR0PTH,0x0000,
    SPR1PTL,0x0000,
    SPR1PTH,0x0000,
    SPR2PTL,0x0000,
    SPR2PTH,0x0000,
    SPR3PTL,0x0000,
    SPR3PTH,0x0000,
    SPR4PTL,0x0000,
    SPR4PTH,0x0000,
    SPR5PTL,0x0000,
    SPR5PTH,0x0000,
    SPR6PTL,0x0000,
    SPR6PTH,0x0000,
    SPR7PTL,0x0000,
    SPR7PTH,0x0000
  },
#endif

   .fontColorWait = {
    0xffdf, 0xfffe,
    0xffff, 0xfffe,
  },
   .fontColor = {
    COLOR28,GAME_COUNTDOWN_COLOR_BOTTOM_OK
  },
   .fontColorWaitEnd = {
    ((28)<<8)|0xe1, 0xfffe,
  },
   .fontResetColor = {
    COLOR28,GAME_COUNTDOWN_COLOR_TOP_OK
  },

   .end = {0xFFFF, 0xFFFE}
};


static void
game_enableCopperEffects(void)
{
  copper.fontColorWait[2] = ((14)<<8)|0xe1;
}


static void
game_disableCopperEffects(void)
{
  hw_waitVerticalBlank();
  copper.fontColorWait[2] = 0xffff;
}


static void
game_setBigFontColor(uint16_t topColor, uint16_t bottomColor)
{
  copper.fontResetColor[1] = topColor;
  copper.fontColor[1] = bottomColor;
}


static uint16_t __NOINLINE
game_check25fps(void)
{
  volatile uint32_t count = hw_verticalBlankCount;

  for (volatile int32_t i = 0; i < 100000; i++);

  volatile uint32_t difference = hw_verticalBlankCount - count;

  volatile uint16_t slow = difference > 15;

  return slow;
}


static void
game_ctor(void)
{
  game_difficulty = GAME_DIFFICULTY_HARD;

  game_demo = 0;
  game_numPlayers = 1;
#ifdef DEBUG
#ifdef GAME_AUTOSTART_REPLAY
  game_startReplay = 1;
#endif
#endif

  game_scoreboardLoaded = 0;
  game_onScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer2;
  game_offScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer1;
  game_messageBuffer  = (frame_buffer_t)&game_frameBufferData.frameBuffer1;
  game_menuBuffer  = (frame_buffer_t)&game_frameBufferData.frameBuffer2;
#ifdef GAME_TRIPLE_BUFFER
  game_backScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer3;
#endif

  game_scoreBoardFrameBuffer = (frame_buffer_t)&game_frameBufferData.scoreBoardBuffer;
}


#ifdef DEBUG
void __NOINLINE
game_checkCanary(void)
{
  if (game_frameBufferData.canary1 != 0x55555555) {
    PANIC("game_finish: dead canary 1");
  }
  if (game_frameBufferData.canary2 != 0xaaaaaaaa) {
    PANIC("game_finish: dead canary 2");
  }
}
#endif

#ifdef GAME_RECORDING
static void
game_startRecord(void)
{
  game_numPlayers = 1;
  random_seed(1);
  palette_black();
  game_loadLevel(MENU_COMMAND_RECORD);
  //  record_setState(RECORD_RECORD);
  music_restart();
  hw_waitVerticalBlank();
  hw_verticalBlankCount = 0;
  game_lastVerticalBlankCount = 0;
}

#endif

static void
game_init(menu_command_t command)
{
  game_onScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer2;
  game_offScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer1;
#ifdef GAME_TRIPLE_BUFFER
  game_backScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer3;
#endif

  hw_waitVerticalBlank();
  palette_black();

  screen_pokeCopperList(game_scoreBoardFrameBuffer, copper.sb_bpl, SCREEN_WIDTH_BYTES);
  screen_setup((uint16_t*)&copper);

#ifdef DEBUG
  game_frameBufferData.canary1 = 0x55555555;
  game_frameBufferData.canary2 = 0xaaaaaaaa;
#endif

  if (command == MENU_COMMAND_RECORD) {
    game_startRecord();
  } else {
    game_startLevel(command);
  }
}


static void
game_complete(void)
{
  game_loopControl = GAME_LOOP_CONTROL_GOTO_MENU;

  game_disableCopperEffects();
  palette_fadeFrom(levelFast.palette, 32, 0, 32);

#ifdef GAME_KEYBOARD_ENABLED
  if (game_player1Score > 0) {
    hiscore_addScore(1, game_player1Score);
  }
  if (game_player2Score > 0) {
    hiscore_addScore(2, game_player2Score);
  }
#endif

  hw_waitVerticalBlank();
  palette_black();
  hw_waitVerticalBlank();

  custom->dmacon = (DMAF_COPPER|DMAF_RASTER);
  //  message_screenOn(" "); // disables game copper list, prevents any artifacts during last exit loop


#ifdef DEBUG
  game_checkCanary();
#endif
}


static void
game_scoreBoardPlayer2Score(char* text)
{
  text_drawText8(game_scoreBoardFrameBuffer, text, 48, 8);
}


static void
game_scoreBoardPlayer1Score(char* text)
{
  text_drawText8(game_scoreBoardFrameBuffer, text, 224-3*8, 8);
}


#ifdef DEBUG
static void
game_refreshDebugScoreboard(void)
{
  gfx_fillRectSmallScreen(game_scoreBoardFrameBuffer, 0, 0, SCREEN_WIDTH, SCOREBOARD_HEIGHT, 0);
  game_lastAverage = -1;
  game_lastMaxRasterLine = -1;
  game_lastEnemyCount = -1;
  game_lastItemCount = -1;

  text_drawScoreBoard("s/l:", 0, 0);
  text_drawScoreBoard("tics:", 14*8, 0);
  text_drawScoreBoard("vbi:", 27*8, 0);

  text_drawScoreBoard("skip:", 0, 9);
  text_drawScoreBoard("asl:", 9*8, 9);
  text_drawScoreBoard("msl:", 18*8, 9);
  text_drawScoreBoard("dt:", 27*8, 9);
  text_drawScoreBoard("obj:", 33*8, 9);

  text_drawScoreBoard("ec:", 0, 18);
  text_drawScoreBoard("ci:", 5*8, 18);
#ifdef GAME_STARS
  text_drawScoreBoard("sc:", 12*8, 18);
#endif
}
#endif


static void
game_renderCounter(void)
{
  uint16_t x = (SCREEN_WIDTH/2)-(((GAME_BIG_FONT_GAP*3)+GAME_BIG_FONT_COLON_GAP)/2);
  uint16_t y = 19;
  frame_buffer_t fb = game_scoreBoardFrameBuffer+(SCREEN_WIDTH_BYTES*3);
  text_clrBlit(fb, x, y, (GAME_BIG_FONT_GAP*3), GAME_BIG_FONT_HEIGHT);
  text_drawBigNumeral(fb, game_levelTime.sec, x+GAME_BIG_FONT_COLON_GAP+GAME_BIG_FONT_GAP+GAME_BIG_FONT_GAP, y, GAME_BIG_FONT_WIDTH, GAME_BIG_FONT_HEIGHT);
  text_drawBigNumeral(fb, game_levelTime.sec10, x+GAME_BIG_FONT_COLON_GAP+GAME_BIG_FONT_GAP, y, GAME_BIG_FONT_WIDTH, GAME_BIG_FONT_HEIGHT);
  text_drawBigNumeral(fb, game_levelTime.min, x, y, GAME_BIG_FONT_WIDTH, GAME_BIG_FONT_HEIGHT);
  text_drawBigNumeral(fb, 10, x+GAME_BIG_FONT_GAP, y, GAME_BIG_FONT_WIDTH, GAME_BIG_FONT_HEIGHT);
}


static void
game_refreshScoreboard(void)
{
#ifdef DEBUG
  if (game_scoreBoardMode == 0) {
#endif
    game_scoreBoardPlayer1Text(I18N_BLANK_GAME_OVER);
    game_scoreBoardPlayer2Text(I18N_BLANK_GAME_OVER);
    game_scoreBoardPlayer1Score(I18N_BLANK_GAME_OVER);
    game_scoreBoardPlayer2Score(I18N_BLANK_GAME_OVER);

    if (game_player1) {
      text_drawText8(game_scoreBoardFrameBuffer, itoan(game_player1Score, 6), 224, 8);
      game_updatePlayerHealth(GAME_PLAYER1_HEALTH_SCOREBOARD_X, PLAYER_INITIAL_HEALTH);
    }

    if (game_player2) {
      text_drawText8(game_scoreBoardFrameBuffer, itoan(game_player2Score, 6), 48, 8);
      game_updatePlayerHealth(GAME_PLAYER2_HEALTH_SCOREBOARD_X, PLAYER_INITIAL_HEALTH);
    }

    if (!game_player2) {
      game_updatePlayerHealth(GAME_PLAYER2_HEALTH_SCOREBOARD_X, 0);
      if (game_numPlayers == 1) {
	game_scoreBoardPlayer2Score(I18N_PRESS_2);
	game_scoreBoardPlayer2Text(I18N_TO_PLAY);
      }
    }

    if (!game_player1) {
      game_updatePlayerHealth(GAME_PLAYER1_HEALTH_SCOREBOARD_X, 0);
      if (game_numPlayers == 1) {
	game_scoreBoardPlayer1Score(I18N_PRESS_2);
	game_scoreBoardPlayer1Text(I18N_TO_PLAY);
      }
    }

#ifndef RELEASE
    if (!game_25fps) {
      text_drawText8(game_scoreBoardFrameBuffer, "50", (SCREEN_WIDTH/2)-8, 8);
    } else {
      text_drawText8(game_scoreBoardFrameBuffer, "25", (SCREEN_WIDTH/2)-8, 8);
    }
#endif

    game_renderCounter();

#ifdef DEBUG
  }
#endif
}


//__NOINLINE
void
game_scoreBoardPlayer1Text(char* text)
{
    text_drawText8(game_scoreBoardFrameBuffer, text, 224-3*8, 17);
}

//__NOINLINE
void
game_scoreBoardPlayer2Text(char* text)
{
  text_drawText8(game_scoreBoardFrameBuffer, text, 48, 17);
}


void
game_updatePlayerHealth(uint16_t x, int16_t health)
{
  uint16_t score;

  for (score = 0; score < health; score+=10, x+= 5) {
    gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 288, x, 30, 16, 8);
  }

  for (; score < 100; score+= 10, x+= 5) {
    gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 296, x, 30, 16, 8);
  }
}


void
game_updatePlayer1Health(uint16_t x, int16_t health)
{
  uint16_t score;
  if (health > game_lastPlayer1Health) {
    game_updatePlayerHealth(x, health);
    game_lastPlayer1Health = health;
    return;
  }

  for (score = 0; score < health; score+=10, x+= 5) {
    // gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 288, x, 30, 16, 8);
  }

  for (; score < game_lastPlayer1Health; score+= 10, x+= 5) {
    gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 296, x, 30, 16, 8);
  }

  game_lastPlayer1Health = health;
}

void
game_updatePlayer2Health(uint16_t x, int16_t health)
{
  uint16_t score;

  if (health > game_lastPlayer2Health) {
    game_updatePlayerHealth(x, health);
    game_lastPlayer2Health = health;
    return;
  }

  for (score = 0; score < health; score+=10, x+= 5) {
    //gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 288, x, 30, 16, 8);
  }

  for (; score < game_lastPlayer2Health; score+= 10, x+= 5) {
    gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 296, x, 30, 16, 8);
  }

  game_lastPlayer2Health = health;
}



static void
game_startLevel(menu_command_t command)
{
  hw_waitVerticalBlank();
  game_deltaT = 0;
  game_lastPlayer1Score = 0;
  game_lastPlayer2Score = 0;
  //  game_lastPlayer1Health = 0;
  //  game_lastPlayer2Health = 0;
  game_levelTime.min = 8;
  game_levelTime.sec10 = 0;
  game_levelTime.sec= 0;
  game_lastLevelTime.value = 0;
  game_levelTicCounter = 0;

  game_killScore = game_difficulty == GAME_DIFFICULTY_HARD ? 1000 : 500;

  if (command >= MENU_COMMAND_LEVEL) {
    game_level = command - MENU_COMMAND_LEVEL;
    command = MENU_COMMAND_PLAY;
  }

  if (command == MENU_COMMAND_REPLAY || command == MENU_COMMAND_RECORD) {
    random_seed(1);
    //    music_restart();
  } else {
    random_seed(hw_getRasterLine() | *((uint16_t*)0xbfd800));
  }
  game_loadLevel(command);
}


static void
game_loadLevel(menu_command_t command)
{
  custom->bltafwm = 0xffff;

  game_bplcon1 = 0xff;
  game_cameraX = 0;
  game_lastTileX = 0;
  game_targetCameraX = 0;
  game_scroll = 0;
#ifdef DEBUG
  game_singleStep = 0;
#endif
  game_paused = 0;
  game_over = 0;
  game_loopControl = GAME_LOOP_CONTROL_RUN;
  game_screenScrollX = 0xf;
  //  game_requestCameraX(0);
  game_lastScrollFrame = 0;

#ifdef GAME_TURTLE
  game_turtle = 0;
  game_missedFrameCount = 0;
#endif

#ifdef DEBUG
  game_collisions = 1;
  game_collectTotal = 1;
  game_total = 0;
  game_frame  = 0;
  game_average = 0;
  game_maxRasterLine = 0;
  game_lastAverage = -1;
  game_lastMaxRasterLine = -1;
  game_lastEnemyCount = -1;
  game_lastItemCount = -1;
  game_scoreBoardMode = 0;
  game_debugRenderFrame = 0;
#endif

  game_switchFrameBuffers();

  sound_init();

  if (game_numPlayers == 1) {
#ifdef DEBUG
    if (command != MENU_COMMAND_REPLAY && command != MENU_COMMAND_RECORD) {
#endif
      if (game_newGame && !game_demo) {
	player1_character = player_select();
      }
#ifdef DEBUG
    } else {
      game_startReplay = 0;
      player1_character = 1;
    }
#endif
  } else {
    player1_character = 1;
  }


  level_load(game_level);

  game_requestCameraX(0);

  tile_init();
#ifdef GAME_TRIPLE_BUFFER
  tile_renderScreen(game_offScreenBuffer, game_onScreenBuffer, game_backScreenBuffer);
#else
  tile_renderScreen(game_offScreenBuffer, game_onScreenBuffer);
#endif

#ifdef GAME_RECORDING
  switch (command) {
  case MENU_COMMAND_REPLAY:
    game_25fps = 1;
    record_setState(RECORD_PLAYBACK);
    break;
  case MENU_COMMAND_RECORD:
    game_25fps = 1;
    record_setState(RECORD_RECORD);
    break;
  case MENU_COMMAND_PLAY:
  default:
    game_25fps = game_calculated25fps;
    record_setState(RECORD_IDLE);
    break;
  }
#else
  __USE(command);
#endif

  alarm_init();
  object_init();
  fighter_init();
  hand_init();
  thing_init();
#ifdef GAME_STARS
  star_init();
#endif


  if (game_numPlayers == 2 || player1_character == 1) {
    //    game_player1 = (game_newGame || game_player1) ? player_init(OBJECT_ID_PLAYER1, OBJECT_ANIM_PLAYER1_STAND_RIGHT, 80, game_lastPlayer1Health) : 0;
    game_player1 = (game_newGame || game_player1) ? player_init(OBJECT_ID_PLAYER1, OBJECT_ANIM_ENEMY_LEVEL4_3_STAND_RIGHT, 80, game_lastPlayer1Health) : 0;
    if (game_player1) {
      game_player1->joystickPos = &hw_joystickPos;
      game_player1->joystickButton = &hw_joystickButton;
    }
  } else {
    game_player1 = 0;
  }

  if (game_numPlayers == 2 || player1_character == 0) {
    game_player2 = (game_newGame || game_player2) ? player_init(OBJECT_ID_PLAYER2, OBJECT_ANIM_PLAYER2_STAND_RIGHT, SCREEN_WIDTH-80, game_lastPlayer2Health) : 0;
    if (game_player2) {
      game_player2->joystickPos = &hw_joystickPos;
      game_player2->joystickButton = &hw_joystickButton;
    }
  } else {
    game_player2 = 0;
  }

  if (game_numPlayers == 2 && player1_character == 0) {
    if (game_player2) {
      game_player2->joystickPos = &hw_joystick2Pos;
      game_player2->joystickButton = &hw_joystick2Button;
    }
    if (game_player1) {
       game_player1->joystickPos = &hw_joystickPos;
       game_player1->joystickButton = &hw_joystickButton;
    }
  } else if (game_numPlayers == 2 && player1_character == 1) {
    if (game_player1) {
      game_player1->joystickPos = &hw_joystick2Pos;
      game_player1->joystickButton = &hw_joystick2Button;
    }
    if (game_player2) {
       game_player2->joystickPos = &hw_joystickPos;
       game_player2->joystickButton = &hw_joystickButton;
    }
  }

  if (game_newGame) {
    game_refreshScoreboard();
  }


  enemy_init();

  conductor_init(levelFast.instructions);

#ifdef OBJECT_Z_BUFFER_COLLISION
  object_initZbuffer();
#endif

  hw_waitBlitter();

  game_render(0);

  game_switchFrameBuffers();

  game_render(0);

  hw_waitVerticalBlank();

  palette_fadeTo(levelFast.palette, 32, 0);

  hw_waitVerticalBlank();
  game_setBigFontColor(GAME_COUNTDOWN_COLOR_TOP_OK, GAME_COUNTDOWN_COLOR_BOTTOM_OK);
  game_enableCopperEffects();

  game_newGame = 0;
  hw_waitVerticalBlank();
  hw_verticalBlankCount = 0;
  game_lastVerticalBlankCount = 0;
}


void
game_switchFrameBuffers(void)
{
  custom->dmacon = (DMAF_COPPER);
  copper.bplcon1[1] = game_bplcon1;
  screen_pokeCopperList(game_offScreenBuffer, copper.bpl, FRAME_BUFFER_WIDTH_BYTES);

  frame_buffer_t save = game_onScreenBuffer;
  game_onScreenBuffer = game_offScreenBuffer;
  game_offScreenBuffer = save;
  custom->dmacon = (DMAF_COPPER|DMAF_SETCLR);
}


static void
game_scrollBackground(void)
{
  gfx_setupRenderTile();

  int16_t lastCameraX = game_cameraX;

  if (game_targetCameraX-game_cameraX < 2) {
    game_cameraX += 1;
  } else {
    game_cameraX += 2;
  }


  uint16_t tile = game_cameraX >> 4;
  uint16_t scroll = (game_cameraX % 16);
  game_screenScrollX = 0xf - scroll;
  game_bplcon1 = (game_screenScrollX << 4) | game_screenScrollX;

  if (game_lastTileX != tile) {
    game_lastTileX = tile;
    game_offScreenBuffer += 2;
    game_onScreenBuffer += 2;
#ifdef GAME_TRIPLE_BUFFER
    game_backScreenBuffer += 2;
#endif
  }

  uint16_t screenX = FRAME_BUFFER_WIDTH-TILE_WIDTH*2;
  uint16_t count = game_cameraX-lastCameraX;

  for (int16_t i = 0; i < count; i++) {
    uint16_t x = (uint32_t)game_cameraX+(uint32_t)screenX;
    uint16_t c = i+scroll;
    if (c < MAP_TILE_HEIGHT) {
      uint16_t offset = levelFast.tileAddresses[x>>4][c];
      gfx_quickRenderTile(game_offScreenBuffer, screenX, c << 4, levelChip.tileBitplanes+offset);
      gfx_quickRenderTile(game_onScreenBuffer, screenX, c << 4, levelChip.tileBitplanes+offset);
#ifdef GAME_TRIPLE_BUFFER
      gfx_quickRenderTile(game_backScreenBuffer, screenX, c << 4, levelChip.tileBitplanes+offset);
#endif
    }
  }
}


#ifdef DEBUG
void
debug_mode(void)
{
  switch (game_debugRenderFrame) {
  case 0:
    text_drawScoreBoard(itoan(game_total, 8), 5*8, 0);
    break;
  case 1:
    text_drawScoreBoard(itoan(game_frame, 6), 20*8, 0);
    break;
  case 2:
    {
      uint32_t frame = hw_verticalBlankCount;
      text_drawScoreBoard(itoan(frame, 6), 32*8, 0);
    }
    break;
#ifdef GAME_TURTLE
  case 3:
    text_drawScoreBoard(itoan(game_missedFrameCount, 3),5*8, 9);
    break;
#endif
  case 4:
    if (game_average != game_lastAverage) {
      text_drawScoreBoard(itoan(game_average, 4), 13*8, 9);
      game_lastAverage = game_average;
    }
    break;
  case 5:
    if (game_maxRasterLine != game_lastMaxRasterLine) {
      text_drawScoreBoard(itoan(game_maxRasterLine, 4), 22*8, 9);
      game_lastMaxRasterLine = game_maxRasterLine;
    }
  case 6:
      text_drawScoreBoard(itoan(game_deltaT, 2), 30*8, 9);
    break;
  case 7:
      text_drawScoreBoard(itoan(object_count, 2), 37*8, 9);
    break;
  case 8:
      text_drawScoreBoard(itoan(enemy_count, 1), 3*8, 18);
    break;
  case 9:
      text_drawScoreBoard(itoan(conductor_instructionIndex, 3), 8*8, 18);
    break;
#ifdef GAME_STARS
  case 10:
      text_drawScoreBoard(itoan(star_count, 3), 15*8, 18);
    break;
#endif
  default:
    break;
  }
  game_debugRenderFrame++;
  if (game_debugRenderFrame > 10) {
    game_debugRenderFrame = 0;
  }
}


static void
debug_showRasterLine(void)
{
  if (game_scoreBoardMode && ((hw_getRasterLine() < GAME_SCORE_RASTERLINE_CUTOFF))) {
    debug_mode();
  }

  int16_t line = hw_getRasterLine();

  if (hw_verticalBlankCount-game_lastVerticalBlankCount > 1) {
    line = hw_getRasterLine() + 312;
  } else if (hw_verticalBlankCount == game_lastVerticalBlankCount) {
    line = line - 285;
  }

  if (line > game_maxRasterLine) {
    game_maxRasterLine = line;
  }

  game_average = line;

  if (game_collectTotal) {
    game_total += line;
    game_frame++;
  }

  return;
}
#endif


static void
game_updateWave(void)
{
  if (game_cameraX < conductor_scrollTarget) {
    if ((!game_player1 || object_x(game_player1)-game_cameraX > SCREEN_WIDTH/3) &&
	(!game_player2 || object_x(game_player2)-game_cameraX > SCREEN_WIDTH/3)) {
      if ((game_player1 && object_x(game_player1)-game_cameraX > (SCREEN_WIDTH-128)) ||
	  (game_player2 && object_x(game_player2)-game_cameraX > (SCREEN_WIDTH-128))) {
	game_requestCameraX(min(game_cameraX+(SCREEN_WIDTH/3), conductor_scrollTarget));
	hand_hide();
      }
    }
  }
}


static void
game_render(uint16_t deltaT)
{
  alarm_process(game_deltaT);
  object_render(game_offScreenBuffer, deltaT);
#ifdef GAME_STARS
  star_update(deltaT);
#endif
  game_updateWave();
}


uint16_t
game_requestCameraX(int16_t targetCameraX)
{
  game_targetCameraX = targetCameraX;

  if (game_targetCameraX < 0) {
    game_targetCameraX = 0;
  } else if (game_targetCameraX >= game_maxScrollX /*GAME_WORLD_WIDTH-SCREEN_WIDTH*/) {
    game_targetCameraX = game_maxScrollX-1;//GAME_WORLD_WIDTH-SCREEN_WIDTH-1;
  }

  return targetCameraX;
}


static void
game_updateScoreboard(void)
{
  static uint16_t game_scoreBoardTic = 0;
#ifdef DEBUG
    if (!game_scoreBoardMode) {
#endif
    if (++game_scoreBoardTic%8 != 0) {
	return;
    }
    if (game_levelTime.value != game_lastLevelTime.value) {
      game_renderCounter();
      game_lastLevelTime = game_levelTime;
     } else if (game_lastPlayer1Score != game_player1Score) {
      frame_buffer_t fb = game_scoreBoardFrameBuffer;
      text_drawText8(fb, itoan(game_player1Score, 6), 224, 8);
      game_lastPlayer1Score = game_player1Score;
      custom->bltafwm = 0xffff;
    } else if (game_lastPlayer2Score != game_player2Score) {
      frame_buffer_t fb = game_scoreBoardFrameBuffer;
      text_drawText8(fb, itoan(game_player2Score, 6), 48, 8);
      game_lastPlayer2Score = game_player2Score;
      custom->bltafwm = 0xffff;
    }

#ifdef DEBUG
    }
#endif
}

static void
game_waitForNextFrame(void)
{
  hw_waitVerticalBlank();

  sound_vbl();

  switch (hw_verticalBlankCount-game_lastVerticalBlankCount) {
    case 0:
      if (game_25fps) {
	hw_waitVerticalBlank();
	hw_waitVerticalBlank();
      } else {
	hw_waitVerticalBlank();
      }
      break;
    case 1:
      if (game_25fps) {
	hw_waitVerticalBlank();
      }
      break;
    case 2:
      if (game_25fps) {
	break;
      }
#ifdef GAME_TURTLE
    default:
#ifdef DEBUG
      if (game_collectTotal) {
#endif
	if (!game_paused) {
	  game_turtle = 5;
	}
      game_missedFrameCount++;
#ifdef DEBUG
      }
#endif
#endif
    }

    game_lastVerticalBlankCount = hw_verticalBlankCount;
}


void
game_showDeathMatch(void)
{
  if (!game_player1 || !game_player2) {
    return;
  }
  fighter_intelligence_functor player1_intelligence = fighter_data(game_player1)->intelligence;
  fighter_intelligence_functor player2_intelligence = fighter_data(game_player2)->intelligence;
  game_player1->velocity.x = 0;
  game_player1->velocity.y = 0;
  game_player2->velocity.x = 0;
  game_player2->velocity.y = 0;
  fighter_data(game_player1)->intelligence = fighter_nullIntelligence;
  fighter_data(game_player2)->intelligence = fighter_nullIntelligence;

  object_t* dm = object_add(OBJECT_ID_DEATHMATCH, 0, game_cameraX+(SCREEN_WIDTH/2-48), -4, 0,OBJECT_ANIM_DEATHMATCH, 0, 0, 0, 0);
  object_set_z(dm, 4096);
  uint32_t frame, lastFrame = hw_verticalBlankCount;
  int16_t y;
  for (y = -16; y <= (PLAYAREA_HEIGHT/2)-4; y+=4) {
    object_set_py_no_checks(dm, y*OBJECT_PHYSICS_FACTOR);
    frame = hw_verticalBlankCount;
    game_deltaT = frame-lastFrame;
    lastFrame = frame;
    game_render(game_deltaT);
    game_waitForNextFrame();
    game_switchFrameBuffers();
  }

  for (uint16_t i = 0; i < 7; i++) {
    game_render(1);
    game_waitForNextFrame();
    game_switchFrameBuffers();
    game_waitForNextFrame();
    game_waitForNextFrame();
    game_waitForNextFrame();
    game_waitForNextFrame();
    game_waitForNextFrame();
    game_waitForNextFrame();
    dm->visible = !dm->visible;
  }

  dm->visible = 1;
  lastFrame = hw_verticalBlankCount;
  for (y = y - 2; y >= -8; y-=4) {
    object_set_py_no_checks(dm, y*OBJECT_PHYSICS_FACTOR);
    frame = hw_verticalBlankCount;
    game_deltaT = frame-lastFrame;
    lastFrame = frame;
    game_render(game_deltaT);
    game_waitForNextFrame();
    game_switchFrameBuffers();
  }
  game_lastVerticalBlankCount = hw_verticalBlankCount;

  fighter_data(game_player1)->intelligence = player1_intelligence;
  fighter_data(game_player2)->intelligence = player2_intelligence;
}

static void __NOINLINE
game_pauseToggle(void)
{
  static uint16_t music;
  if (!game_paused) {
    music = music_enabled();
    if (music) {
      music_toggle();
    }
    game_disableCopperEffects();
    palette_fade(levelFast.palette, levelFast.greyPalette, 32, 16);
    game_paused = 1;
  } else {
    if (music) {
      music_toggle();
    }
    palette_fade(levelFast.greyPalette, levelFast.palette, 32, 16);
    game_enableCopperEffects();
    game_paused = 0;
    game_levelTicCounter = 0;
    game_lastScrollFrame = game_lastVerticalBlankCount = hw_verticalBlankCount;
  }
}

void
game_maxHealth(void)
{
  if (game_player1) {
    fighter_data(game_player1)->health = PLAYER_INITIAL_HEALTH;
    game_updatePlayer1Health(GAME_PLAYER1_HEALTH_SCOREBOARD_X, fighter_data(game_player1)->health);
  }

  if (game_player2) {
    fighter_data(game_player2)->health = PLAYER_INITIAL_HEALTH;
    game_updatePlayer2Health(GAME_PLAYER2_HEALTH_SCOREBOARD_X, fighter_data(game_player2)->health);
  }
}

static int16_t
game_processKeyboard()
{
  switch (keyboard_key) {
#ifdef DEBUG
  case 'H':
    game_maxHealth();
    break;
  case 'C':
    game_setGameComplete();
    break;
  case 'U':
    game_level = LEVEL_NUM_LEVELS;
    game_setGameComplete();
    break;
  case 'K':
    game_showDeathMatch();
    break;
  case 'Y':
    game_difficulty = game_difficulty == GAME_DIFFICULTY_HARD ? GAME_DIFFICULTY_EASY : GAME_DIFFICULTY_HARD;
    break;
  case 'E':
    enemy_pause = !enemy_pause;
    break;
  case 'O':
    game_setGameOver();
    break;
  case 'D':
    game_scoreboardLoaded = 0;
    game_scoreBoardMode = !game_scoreBoardMode;
    if (game_scoreBoardMode) {
      game_refreshDebugScoreboard();
    } else {
      game_refreshScoreboard();
    }
    break;
  case 'F':
    {
      static uint16_t sfxId = 0;
      sound_endLoop();
      sound_queueSound(sfxId);
      sfxId++;
      if (sfxId == SOUND_LAST) {
	sfxId = 0;
      }
    }
    break;
  case 'X':
    game_collectTotal = !game_collectTotal;
    break;
  case 'T':
    if (!game_paused) {
      game_paused = 1;
    } else {
      game_singleStep = 1;
    }
    break;
  case 'Z':
    music_next();
    break;
  case '1':
    game_numPlayers = 1;
    game_init(MENU_COMMAND_PLAY);
    break;
  case '5':
    game_25fps = !game_25fps;
    break;
  case ' ':
    game_paused = 0;
    // game_lastVerticalBlankCount = hw_verticalBlankCount;
    game_lastScrollFrame = game_lastVerticalBlankCount = hw_verticalBlankCount;
    break;
#endif

#ifdef GAME_RECORDING
  case 'R':
    game_startRecord();
    break;
  case 'S':
    record_setState(RECORD_IDLE);
#ifdef DEBUG
    game_collectTotal = !game_collectTotal;
#endif
    break;
#ifdef DEBUG
  case 'L':
    game_startReplay = 1;
    return 1;
    break;
#endif
#endif
  case 'P':
    game_pauseToggle();
    break;
  case 'M':
    music_toggle();
    break;
  case KEYBOARD_CODE_ESC:
  case 'Q':
    if (game_paused) {
      game_pauseToggle();
    }
    if (game_demo) {
      game_numPlayers = 1;
    }
    game_disableCopperEffects();
    palette_fadeFrom(levelFast.palette, 32, 0, 32);
    return 1;
    break;
  case '2':
    if (!game_over && game_player2 == 0 && game_numPlayers == 1 && player1_character == 1) {
      game_numPlayers = 2;
      game_scoreBoardPlayer2Score(I18N_BLANK_GAME_OVER);
      game_lastPlayer2Score = 0xffffffff;
      game_scoreBoardPlayer2Text(I18N_BLANK_GAME_OVER);
      game_player2 = player_init(OBJECT_ID_PLAYER2, OBJECT_ANIM_PLAYER2_STAND_RIGHT, game_cameraX+SCREEN_WIDTH-80, game_lastPlayer2Health);
      game_player2->joystickPos = &hw_joystick2Pos;
      game_player2->joystickButton = &hw_joystick2Button;
      game_updatePlayerHealth(GAME_PLAYER2_HEALTH_SCOREBOARD_X, PLAYER_INITIAL_HEALTH);
    } else if (!game_over && game_player1 == 0 && game_numPlayers == 1 && player1_character == 0) {
      game_numPlayers = 2;
      game_scoreBoardPlayer1Score(I18N_BLANK_GAME_OVER);
      game_lastPlayer1Score = 0xffffffff;
      game_scoreBoardPlayer1Text(I18N_BLANK_GAME_OVER);
      game_player1 = player_init(OBJECT_ID_PLAYER1, OBJECT_ANIM_PLAYER1_STAND_RIGHT, game_cameraX+80, game_lastPlayer1Health);
      game_player1->joystickPos = &hw_joystick2Pos;
      game_player1->joystickButton = &hw_joystick2Button;
      game_updatePlayerHealth(GAME_PLAYER1_HEALTH_SCOREBOARD_X, PLAYER_INITIAL_HEALTH);
    }
    break;
  }

  return 0;
}



static void
game_waitForMenuExit(int16_t messageAnimId, int16_t offset)
{
  if (game_player1) {
    fighter_data(game_player1)->intelligence = fighter_nullIntelligence;
  }

  if (game_player2) {
    fighter_data(game_player2)->intelligence = fighter_nullIntelligence;
  }

  object_t* ptr = object_activeList;

  while (ptr) {
    if (ptr->dataType == OBJECT_DATA_TYPE_FIGHTER) {
      ptr->velocity.x = 0;
      ptr->velocity.y = 0;
      fighter_data(ptr)->intelligence = fighter_nullIntelligence;
    }
    ptr = ptr->next;
  }


  object_t* gameOver = object_add(OBJECT_ID_GAMEOVER, 0, game_cameraX+(SCREEN_WIDTH/2-offset), -4, 0, messageAnimId, 0, 0, 0, 0);
  object_set_z(gameOver, 4096);

  uint32_t frame, lastFrame = hw_verticalBlankCount;
  for (int16_t y = -16; y <= (PLAYAREA_HEIGHT/2)-30; y+=2) {
    object_set_py_no_checks(gameOver, y*OBJECT_PHYSICS_FACTOR);
    frame = hw_verticalBlankCount;
    game_deltaT = frame-lastFrame;
    lastFrame = frame;
    game_render(game_deltaT);
    game_waitForNextFrame();
    game_switchFrameBuffers();
  }

  object_t* joystick = object_add(OBJECT_ID_JOYSTICK, 0, game_cameraX+(SCREEN_WIDTH/2-16),PLAYAREA_HEIGHT-32, 0, OBJECT_ANIM_JOYSTICK, 0, 0, 0, 0);
  object_set_z(joystick, 4096);

  for (int16_t y = PLAYAREA_HEIGHT-32; y >  (PLAYAREA_HEIGHT/2)+32; y-=8) {
    object_set_py_no_checks(joystick, y*OBJECT_PHYSICS_FACTOR);
    frame = hw_verticalBlankCount;
    game_deltaT = frame-lastFrame;
    lastFrame = frame;
    game_render(game_deltaT);
    game_waitForNextFrame();
    game_switchFrameBuffers();
  }


  do {
    frame = hw_verticalBlankCount;
    game_deltaT = frame-lastFrame;
    lastFrame = frame;
    game_render(game_deltaT);
    game_waitForNextFrame();
    game_switchFrameBuffers();
    keyboard_read();
    hw_readJoystick();
    hw_readJoystick2();
#ifdef DEBUG
#ifdef SCRIPTING
    script_process();
#endif
#endif
  } while(!game_fire());
}


void
game_setGameOver(void)
{
  if (!game_over) {
    game_over = 1;
    game_loopControl = GAME_LOOP_CONTROL_GAME_OVER;
    music_play(MUSIC_GAME_OVER);
  }
}

void
game_setGameComplete(void)
{
  if (!game_over) {
    game_over = 1;
    game_loopControl = GAME_LOOP_CONTROL_GAME_COMPLETE;
  }
}


static void
game_gameOverSequence(void)
{
  game_scoreBoardPlayer1Text(I18N_GAME_OVER);
  game_scoreBoardPlayer2Text(I18N_GAME_OVER);

  if (game_numPlayers == 1) {
    game_scoreBoardPlayer2Score(I18N_BLANK_GAME_OVER);
  }

  game_waitForMenuExit(OBJECT_ANIM_GAMEOVER, 35);
  game_complete();
}


static void
game_gameCompleteSequence(void)
{
  music_play(MUSIC_BOSS_COMPLETE);
  game_scoreBoardPlayer1Text(I18N_GAME_OVER);
  game_scoreBoardPlayer2Text(I18N_GAME_OVER);

  game_waitForMenuExit(OBJECT_ANIM_GAMECOMPLETE, 55);
  game_complete();
}


static void
game_levelCompleteSequence(void)
{
  music_play(MUSIC_BOSS_COMPLETE);

  game_waitForMenuExit(OBJECT_ANIM_LEVELCOMPLETE, 55);
}


static void
game_decrementTime(void)
{
  if (!game_over && !game_paused) {
    if (game_levelTime.sec == 0) {
      game_levelTime.sec = 9;
      if (game_levelTime.sec10 == 0) {
	game_levelTime.sec10 = 5;
	game_levelTime.min--;
      } else {
	game_levelTime.sec10--;
      }
    } else {
      game_levelTime.sec--;
    }
    if (game_levelTime.value == 0) {
      game_renderCounter();
      game_setGameOver();
    } else if (game_levelTime.min == 0 &&
	       game_levelTime.sec10 == 3 &&
	       game_levelTime.sec == 0) {
      game_setBigFontColor(GAME_COUNTDOWN_COLOR_TOP_WARN, GAME_COUNTDOWN_COLOR_BOTTOM_WARN);
    }
  }
}


#ifdef DEBUG
void
game_checkStack(void)
{
#if TRACKLOADER==1
  extern char startUserstack, userstack;
  uint32_t sp = hw_getsp();
  if (sp <= (uint32_t)&startUserstack || sp >= (uint32_t)&userstack) {
    PANIC("STACK OVERFLOW");
  }
#endif
}
#endif



__EXTERNAL void
game_loop()
{
  hw_verticalBlankCount = 0;
  P61_Target = 0;
  P61_Master = 0;

#ifdef DEBUG
  game_frameBufferData.canary1 = 0x55555555;
  game_frameBufferData.canary2 = 0xaaaaaaaa;
#endif

  custom->color[0] = 0;

  game_ctor();

#if FASTRAM==1

#endif

  logo_load();

  music_play(MUSIC_INTRO);

  hw_interruptsInit(); // Don't enable interrupts until music is set up
  custom->intena = INTF_SETCLR|INTF_VERTB|INTF_INTEN;

  game_calculated25fps = game_25fps = game_check25fps();

  logo_display();

  music_toggle();

  hiscore_ctor();


 menu:
#ifdef DEBUG
  game_checkStack();
#endif
  game_level = 0;
  game_disableCopperEffects();
  if ((menuCommand = menu_loop(game_over == 1 ? MENU_MODE_HISCORES : MENU_MODE_MENU)) == MENU_COMMAND_EXIT) {
#if TRACKLOADER==0
    goto done;
#endif
  }

  if (menuCommand == MENU_COMMAND_MENU) {
    goto menu;
  }

  game_newGame = 1;
#ifndef RELEASE
  game_level = game_startLevelIndex;
#else
  game_level = 0;
#endif
  game_player1Score = 0;
  game_player2Score = 0;
  game_lastPlayer1Health = 0;
  game_lastPlayer2Health = 0;

 restart:
  game_disableCopperEffects();
  game_init(menuCommand);

  for (;;) {
#ifdef DEBUG
    game_checkStack();
#endif
    keyboard_read();
    hw_readJoystick();
    hw_readJoystick2();

    if (!game_paused) {
      record_process();
    }

#ifdef DEBUG
#ifdef SCRIPTING
    script_process();
#endif
#endif

    if (game_processKeyboard()) {
#ifdef DEBUG
      game_checkCanary();
#endif
      goto menu;
    }

    if (game_paused
#ifdef DEBUG
	&& game_singleStep != 1
#endif
	) {
      goto skip;
    }


#ifdef GAME_TURTLE
    if (game_turtle > 1) {
#ifndef RELEASE
      custom->color[21] = 0xf00;
#endif
      game_turtle--;
    } else if (game_turtle == 1) {
      custom->color[21] = levelFast.palette[21];
      game_turtle--;
    }
#endif

    game_updateScoreboard();

#ifdef DEBUG
    debug_showRasterLine();
#endif

    sound_schedule();

    if (game_loopControl != 0) {
      if (game_loopControl == GAME_LOOP_CONTROL_DISPLAY_DEATHMATCH) {
	game_waitForNextFrame();
	game_switchFrameBuffers();
	game_showDeathMatch();
	game_loopControl = GAME_LOOP_CONTROL_DEATHMATCH;
      } else if (game_loopControl == GAME_LOOP_CONTROL_GOTO_MENU) {
	goto menu;
      } else if (game_loopControl == GAME_LOOP_CONTROL_GAME_OVER) {
	game_waitForNextFrame();
	game_switchFrameBuffers();
	game_gameOverSequence();
      } else if (game_loopControl == GAME_LOOP_CONTROL_GAME_COMPLETE) {
	game_waitForNextFrame();
	game_switchFrameBuffers();
	if (game_level + 1 >= LEVEL_NUM_LEVELS) {
	  game_gameCompleteSequence();
	} else {
	  game_levelCompleteSequence();
	  game_level++;
	  goto restart;
	}
      }
    }

    game_waitForNextFrame();

    game_switchFrameBuffers();


    uint32_t frame = hw_verticalBlankCount;
    game_deltaT = hw_verticalBlankCount-game_lastScrollFrame;

    game_levelTicCounter += game_deltaT;
    while (game_levelTicCounter >= 50) {
      game_levelTicCounter-=50;
      game_decrementTime();
    }

#ifdef DEBUG
    if (game_singleStep) {
      game_deltaT = 1;
      game_singleStep = 0;
    }
#endif

    game_lastScrollFrame = frame;
    for (uint32_t i = 0; i < game_deltaT; i++) {
      if (game_targetCameraX != game_cameraX) {
	  game_scrollBackground();
	  conductor_process();
      }
    }

    conductor_process();

    game_render(game_deltaT);

  skip:;
  }

#if TRACKLOADER==0
 done:;
#ifdef GAME_KEYBOARD_ENABLED
  keyboard_dtor();
#endif
#endif
}


void* memcpy(void* destination, const void* source, size_t num)
{
  size_t i;
  char* d = destination;
  const char* s = source;
  for (i = 0; i < num; i++) {
    d[i] = s[i];
  }
  return destination;
}


int
memcmp(const void *s1, const void *s2, size_t n)
{
  const char* p1 = s1;
  const char* p2 = s2;

  for ( ; n-- ;) {
    if (*p1 != *p2) {
      return (*p1-*p2);
    }
    p1++;
    p2++;
  }
  return 0;
}
