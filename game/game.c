#include "game.h"

#define GAME_SCORE_RASTERLINE_CUTOFF 330

#define GAME_LEVEL_BONUS_TRANSFER_RATE 32
#ifdef DEBUG
#define GAME_RASTERAVERAGE_LENGTH 16
#endif

static void
game_newGame(menu_command_t command);
static void
game_loadLevel(menu_command_t command);
static void
game_render(uint16_t deltaT);
static void
game_scrollBackground(void);

frame_buffer_t game_offScreenBuffer;
frame_buffer_t game_onScreenBuffer;
uint16_t game_bplcon1;
int16_t game_cameraX;
int16_t game_screenScrollX;
int16_t game_collisions;
uint32_t game_score;
uint32_t game_lives;
uint16_t game_level;
uint16_t game_over;
uint16_t game_levelComplete;
uint32_t game_paused;
uint16_t game_numPlayers;
uint16_t game_wave;
uint16_t game_nextWave;
object_t* game_player1;
object_t* game_player2;

static volatile __section(random_c) struct framebuffeData {
#ifdef DEBUG
  uint32_t canary1;
#endif
  uint32_t overdraw1;
  uint8_t frameBuffer1[FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH*(FRAME_BUFFER_NUM_LINES+1+32)];
  uint32_t overdraw2;
  uint8_t frameBuffer2[FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH*(FRAME_BUFFER_NUM_LINES+1+32)];
#ifdef DEBUG
  uint32_t canary2;
#endif
} game_frameBufferData;

static int16_t game_scroll;
static uint16_t game_gotoMenu;				 
static uint16_t game_singleStep;
static int16_t game_targetCameraX;
static uint32_t game_lastScore;
static uint32_t game_lastScrollFrame;
static uint16_t game_lastTileX;
static uint16_t game_deltaT;
#ifdef DEBUG
static int16_t game_turtle;
static uint16_t game_rasterLines[GAME_RASTERAVERAGE_LENGTH];
static uint16_t game_rasterLinesIndex;
static uint16_t game_maxRasterLine;
static uint16_t game_collectTotal;				 
static uint32_t game_total;
static uint32_t game_frame;
static uint16_t game_average;
static int16_t game_missedFrameCount;
static uint16_t game_lastAverage;
static uint16_t game_lastMaxRasterLine;
static int16_t game_lastEnemyCount;
static int16_t game_lastItemCount;
static int16_t game_lastMissedFrameCount;
static uint32_t game_lastVerticalBlankCount;
static int16_t game_scoreBoardMode = 0;
static int16_t game_debugRenderFrame;
#endif


 __section(data_c)  copper_t copper  = {
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
  .end = {0xFFFF, 0xFFFE}
};


void
game_ctor(void)
{
  game_numPlayers = 1;
  game_onScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer2;
  game_offScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer1;
}


__EXTERNAL void
game_init(menu_command_t command)
{
  hw_waitVerticalBlank();
  palette_black();

  screen_setup((uint16_t*)&copper);
  screen_pokeCopperList(game_scoreBoardFrameBuffer, copper.sb_bpl, SCREEN_WIDTH_BYTES);

#ifdef DEBUG
  game_frameBufferData.canary1 = 0x55555555;
  game_frameBufferData.canary2 = 0xaaaaaaaa;
#endif

  game_newGame(command);
}




#ifdef DEBUG
static void __attribute__ ((noinline))
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


void
game_overCallback(void)
{
  game_gotoMenu = 1;  
  hiscore_addScore(game_score);
}


void
game_finish(void)
{
  game_collisions = 0;
  game_over = 1;
  //popup_off();
  //popup("GAME OVER!", game_overCallback);

#ifdef DEBUG
  game_checkCanary();
#endif
}


void
game_loseLife(void)
{ 
  if (game_lives > 1) {
    game_lives--;
  } else {
    game_finish();
  }
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
}
#endif


static void
game_newGame(menu_command_t command)
{
  game_score = 0;
  game_lives = 3;
  game_level = 0;
  game_wave = 0;
  game_nextWave = 1;
  game_deltaT = 0;

  if (command >= MENU_COMMAND_LEVEL) {
    game_level = command - MENU_COMMAND_LEVEL;
    command = MENU_COMMAND_PLAY;
  }

  game_loadLevel(command);
}


static void
game_loadLevel(menu_command_t command)
{
  custom->bltafwm = 0xffff;
  USE(command);
  
#ifdef DEBUG
  if (game_scoreBoardMode != 0) {
    disk_loadData((void*)game_scoreBoardFrameBuffer, (void*)game_scoreBoardFrameBuffer, FRAME_BUFFER_WIDTH_BYTES*SCOREBOARD_HEIGHT*SCREEN_BIT_DEPTH);
  }
#endif 

  game_bplcon1 = 0xff;
  game_cameraX = 0;
  game_lastTileX = 0;  
  game_targetCameraX = 0;
  game_scroll = 0;
  game_singleStep = 0;
  game_paused = 0;
  game_over = 0;
  game_gotoMenu = 0;
  game_screenScrollX = 0xf;
  game_requestCameraX(0);
  game_levelComplete = 0;
  game_lastScore = 1;
  game_lastScrollFrame = 0;
#ifdef DEBUG
  game_collectTotal = 1;
  game_turtle = 0;
  game_total = 0;
  game_frame  = 0;
  game_average = 0;
  game_maxRasterLine = 0;
  game_rasterLinesIndex = 0;
  game_lastMissedFrameCount = -1;
  game_missedFrameCount = 0;
  game_lastAverage = -1;
  game_lastMaxRasterLine = -1;
  game_lastEnemyCount = -1;
  game_lastItemCount = -1;
  game_scoreBoardMode = 0;
  game_debugRenderFrame = 0;
#endif

  game_collisions = 1;

  //popup_off();

  game_switchFrameBuffers();

  sound_init();

  level_load(game_level);

  if (level.initFunctor) {
    level.initFunctor();
  }
  
  tile_init();
  tile_renderScreen(game_offScreenBuffer, game_onScreenBuffer);


#ifdef GAME_RECORDING
  switch (command) {
  case MENU_COMMAND_REPLAY:
    record_setState(RECORD_PLAYBACK);
    break;
  case MENU_COMMAND_RECORD:
    record_setState(RECORD_RECORD);
    break;
  case MENU_COMMAND_PLAY:
  default:
    record_setState(RECORD_IDLE);
    break;
  }
#endif

  object_init();
  fighter_init();
  hand_init();
  
  game_player1 = player_init(OBJECT_ID_PLAYER1, OBJECT_ANIM_PLAYER2_STAND_RIGHT, 80);
  game_player2 = 0;
  if (game_numPlayers == 2) {
    game_player2 = player_init(OBJECT_ID_PLAYER2, OBJECT_ANIM_PLAYER3_STAND_RIGHT, SCREEN_WIDTH-80);
  }

  hw_waitBlitter();

  game_render(0);

  game_switchFrameBuffers();

  game_render(0);

  hw_waitVerticalBlank();

  palette_fadeTo(level.palette, 32, 0);

  hw_waitVerticalBlank();
  hw_verticalBlankCount = 0;
#ifdef DEBUG
  game_lastVerticalBlankCount = 0;
#endif
}


void
game_switchFrameBuffers(void)
{
  copper.bplcon1[1] = game_bplcon1;
  screen_pokeCopperList(game_offScreenBuffer, copper.bpl, FRAME_BUFFER_WIDTH_BYTES);

  frame_buffer_t save = game_onScreenBuffer;
  game_onScreenBuffer = game_offScreenBuffer;
  game_offScreenBuffer = save;
}


static void
game_scrollBackground(void)
{
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
  }
    
  uint16_t screenX = FRAME_BUFFER_WIDTH-TILE_WIDTH*2;

  uint16_t count = game_cameraX-lastCameraX;
  for (int16_t i = 0; i < count; i++) {
    uint16_t x = (uint32_t)game_cameraX+(uint32_t)screenX;
    uint16_t c = i+scroll;
    if (c < MAP_TILE_HEIGHT) {
      uint16_t offset = level.tileAddresses[x>>4][c];
      gfx_renderTile(game_offScreenBuffer, screenX, c << 4, level.tileBitplanes+offset);
      gfx_renderTile(game_onScreenBuffer, screenX, c << 4, level.tileBitplanes+offset);
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
  case 3:
    text_drawScoreBoard(itoan(game_missedFrameCount, 3),5*8, 9);
    break;
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
  default:
    break;
  } 
  game_debugRenderFrame++;
  if (game_debugRenderFrame > 7) {
    game_debugRenderFrame = 0;
  }
}


static void
game_refreshScoreboard(void)
{
  game_lastScore = 1;  

#ifdef DEBUG
  if (game_scoreBoardMode == 0) {
#endif
    /*
#ifdef GAME_RECORDING    
    switch (record_getState()) {
    case RECORD_IDLE:
      text_drawScoreBoard(" SCORE  " , SCREEN_WIDTH-(14*8));  
      break;
    case RECORD_RECORD:
      text_drawScoreBoard("RECORD  " , SCREEN_WIDTH-(14*8));        
      break;
    case RECORD_PLAYBACK:
      text_drawScoreBoard("REPLAY  " , SCREEN_WIDTH-(14*8));        
      break;
    }
#else
    text_drawScoreBoard(" SCORE  " , SCREEN_WIDTH-(14*8));
#endif

    text_drawScoreBoard("BONUS 0" , 8);  
    game_renderScore(1);
    uint32_t i, x;
    for (i = 0, x = (SCREEN_WIDTH/2)-17; i < game_lives; i++, x+=10) {
      gfx_renderSprite(game_scoreBoardFrameBuffer, 208, 176, x, 5, 16, 8);
    }
    
    for (; i < 3; i++, x+= 10) {
      gfx_renderSprite(game_scoreBoardFrameBuffer, 208, 184, x, 5, 16, 8);
      }*/
#ifdef DEBUG
  }
#endif
}

#ifdef DEBUG
static void
debug_showRasterLine(void)
{
  
  if (((game_scoreBoardMode && ((hw_getRasterLine() < GAME_SCORE_RASTERLINE_CUTOFF))) || game_levelComplete)) {
    debug_mode();
  }

  int16_t line = hw_getRasterLine();  

  if (hw_verticalBlankCount-game_lastVerticalBlankCount > 1) {
    line += 312;
  }
  
  //  if (line < 0) {
  //    line = 0;
  //  }

  game_rasterLines[game_rasterLinesIndex++] = line;
  if (line > game_maxRasterLine) {
    game_maxRasterLine = line;
  }
  if (game_rasterLinesIndex >= GAME_RASTERAVERAGE_LENGTH) {
    game_rasterLinesIndex = 0;
  }

  #if 0
  game_average = 0;
  for (int16_t i = 0; i < GAME_RASTERAVERAGE_LENGTH; i++) {
    game_average += game_rasterLines[i];
  }
  game_average = game_average >> 4 /* / GAME_RASTERAVERAGE_LENGTH */;
#endif
  game_average = line;
  
  if (game_collectTotal) {
    game_total += line;
    game_frame++;
    if (game_frame == script_breakpoint) {
      game_paused = 1;
      script_breakpoint = 0xffffffff;
    }
  }

  return;  
}
#endif


static void
game_render(uint16_t deltaT)
{
  //tile_renderInvalidTiles(game_offScreenBuffer);

  //popup_saveBackground(game_offScreenBuffer);

  //popup_render(game_offScreenBuffer);
  //object_saveBackground(game_offScreenBuffer);
  
  object_render(game_offScreenBuffer, deltaT);
  
  if (level.effectFunctor) {
    level.effectFunctor(game_offScreenBuffer);
  }  
}


void
game_requestCameraX(int16_t targetCameraX)
{
  game_targetCameraX = targetCameraX;

  if (game_targetCameraX < 0) {
    game_targetCameraX = 0;
  } else if (game_targetCameraX >= WORLD_WIDTH-SCREEN_WIDTH) {
    game_targetCameraX = WORLD_WIDTH-SCREEN_WIDTH-1;
  }
}


static void
game_playLevel(uint16_t levelIndex)
{
  game_level = levelIndex;
  if (game_level >= LEVEL_NUM_LEVELS) {
    game_level = 0;
  }
  palette_black();
  game_loadLevel(MENU_COMMAND_PLAY);
#ifdef GAME_RECORDING
  record_setState(RECORD_IDLE);
#endif
}



//static
void
game_finishLevel(void)
{
  game_playLevel(game_level + 1);
}


void
game_setLevelComplete(void)
{
  if (!game_levelComplete) {
    //popup("LEVEL COMPLETE!", game_finishLevel);
    game_levelComplete = 1;
    game_collisions = 0;
  }
}


void
game_startPlayback(void)
{
  game_score = 0;
  game_lives = 3;

  palette_black();
  music_restart();
  game_loadLevel(MENU_COMMAND_REPLAY);
}


void
game_startRecord(void)
{
  palette_black();
  game_loadLevel(MENU_COMMAND_REPLAY);
  record_setState(RECORD_RECORD);
}


int16_t
game_processKeyboard()
{
  switch (keyboard_key) {
#ifdef DEBUG
  case 'L':
    // game_requestCameraX(WORLD_WIDTH-SCREEN_WIDTH);
    break;
  case 'O':
    {
      game_finish();
      break;
      
    }
    break;
  case 'D':
    game_scoreBoardMode = !game_scoreBoardMode;
    if (game_scoreBoardMode) {
      game_collisions = 0;
      game_refreshDebugScoreboard();
    } else {
      game_collisions = 1;
      //gfx_fillRect(game_scoreBoardFrameBuffer, 0, 0, FRAME_BUFFER_WIDTH, SCOREBOARD_HEIGHT, 0);
      game_refreshScoreboard();
    }
    break;    
#endif    
  case 'X':
    game_collectTotal = !game_collectTotal;
    break;
    
#endif
#ifdef GAME_RECORDING
  case 'A':
    record_showAddress();
    break;
  case 'R':
    game_startRecord();
    break;
  case 'P':
    game_paused = !game_paused;
#ifdef DEBUG
    game_lastVerticalBlankCount = hw_verticalBlankCount;
#endif
    //game_startPlayback();
    break;
  case 'S':
    record_setState(RECORD_IDLE);
    break;
#endif
  case 'C':
    game_setLevelComplete();
    break;
  case 'T':
    game_singleStep = 1;
    break;
  case ' ':

    break;
  case 'Z':
    music_next();
    break;
  case 'N':
    game_playLevel(game_level+1);
    break;
  case 'M':
    music_toggle();
    break;
#ifdef GAME_JETPACK
  case 'J':
    player.jetpackFuel += 100000;
    break;
#endif
  case 'Q':
    return 1;
    break;
  case '1':
    game_playLevel(0);
    break;
  case '2':
    game_playLevel(1);
    break;
  case '3':
    game_playLevel(2);
    break;
  }

  return 0;
}


__EXTERNAL void
game_loop()
{
  P61_Target = 0;
  P61_Master = 0;
  
  custom->color[0] = 0;
  hw_interruptsInit(); // Don't enable interrupts until music is set up
  
  game_ctor();

  logo_display();

  music_play(0);

  music_toggle();
  
  hiscore_ctor();

  menu_command_t menuCommand;
 menu:
  game_level = 0;
  if ((menuCommand = menu_loop(game_over == 1 ? MENU_MODE_HISCORES : MENU_MODE_MENU)) == MENU_COMMAND_EXIT) {
#if TRACKLOADER==0
    goto done;
#endif
  }

  if (menuCommand == MENU_COMMAND_MENU) {
    goto menu;
  }

  game_init(menuCommand);

  for (;;) {

    if (game_nextWave) {
      enemy_init(game_player1, game_player2);
      game_nextWave = 0;
    }    
    
    keyboard_read();
    hw_readJoystick();
    if (game_numPlayers == 2) {
      hw_readJoystick2();
    }

    if (!game_paused) {
      record_process();
    }

#ifdef DEBUG
    script_process();
#endif

    if (game_processKeyboard()) {
#ifdef DEBUG
      game_checkCanary();
#endif
      goto menu;
    }
      
#ifdef DEBUG
    if (game_paused && game_singleStep != 1) {
      goto skip;
    }
    game_singleStep = 0;
#endif
    

#ifdef DEBUG
    if (game_turtle > 1) {
      custom->color[16] = 0xf00;
      game_turtle--;
    } else if (game_turtle == 1) {
      custom->color[16] = level.palette[16];
      game_turtle--;
    }
#endif

      
#ifdef DEBUG
    debug_showRasterLine();
#endif
    
    sound_schedule();

    if (game_gotoMenu) {
      goto menu;
    }

    hw_waitVerticalBlank();    
#ifdef DEBUG_SPEED
    custom->color[0] = 0xf00;
#endif
    
    sound_vbl();
    
#ifdef DEBUG
    if (game_collectTotal && hw_verticalBlankCount-game_lastVerticalBlankCount > 2) {
      game_missedFrameCount++;
      game_turtle = 5;
    }
      
    game_lastVerticalBlankCount = hw_verticalBlankCount;
#endif

    game_switchFrameBuffers();

    uint32_t frame = hw_verticalBlankCount;
    game_deltaT = hw_verticalBlankCount-game_lastScrollFrame;
    game_lastScrollFrame = frame;
    for (uint32_t i = 0; i < game_deltaT; i++) {
      if (game_targetCameraX != game_cameraX) {
	game_scrollBackground();      
      }
    }
     
    //object_restoreBackground(game_offScreenBuffer);

    if (game_deltaT == 0 || game_deltaT > 2) {
      PANIC("deltat");
    }
    
    game_render(game_deltaT);    

#ifdef DEBUG_SPEED
    custom->color[0] = 0;
#endif
#ifdef DEBUG
  skip:;
#endif
  }

#if TRACKLOADER==0
 done:;
#ifdef GAME_KEYBOARD_ENABLED
  keyboard_dtor();
#endif
#endif
}


#if 0
void *__memset(__REG("a0", void *dst), __REG("d0", int32_t c), __REG("d1", uint32_t n))
{
  if (n) {
    char *d = dst;
    
    do {
      *d++ = c;
    } while (--n);
  }
  return dst;
}
#endif


void* memcpy(void* destination, void* source, size_t num)
{
  size_t i;
  char* d = destination;
  char* s = source;
  for (i = 0; i < num; i++) {
    d[i] = s[i];
  }
  return destination;
}
