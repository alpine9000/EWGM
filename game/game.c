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
uint16_t game_numPlayers;
uint16_t game_bplcon1;
int16_t game_cameraX;
int16_t game_screenScrollX;
uint16_t game_level;
uint16_t game_over;
uint32_t game_paused;
object_t* game_player1;
object_t* game_player2;
uint32_t game_player1Score;
uint32_t game_player2Score;

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


typedef union {
  struct {
    uint16_t min;
    uint8_t sec10;
    uint8_t sec;
  };
  uint32_t value;
} time_t;

static uint16_t game_lastPlayer1Health;
static uint16_t game_lastPlayer2Health;
static uint32_t game_lastPlayer1Score;
static uint32_t game_lastPlayer2Score;
static int16_t game_scroll;
static uint16_t game_gotoMenu;				 
static uint16_t game_singleStep;
static int16_t game_targetCameraX;
static uint32_t game_lastScrollFrame;
static uint16_t game_lastTileX;
static uint16_t game_deltaT;
static time_t game_levelTime;
static time_t game_lastLevelTime;
static uint16_t game_levelTicCounter;
static uint32_t game_lastVerticalBlankCount;

#ifdef GAME_TURTLE
static int16_t game_turtle;
__EXTERNAL int16_t game_missedFrameCount;
#endif

#ifdef DEBUG
uint16_t game_collisions;
static uint16_t game_rasterLines[GAME_RASTERAVERAGE_LENGTH];
static uint16_t game_rasterLinesIndex;
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
   .end = {0xFFFF, 0xFFFE}
};


void
game_ctor(void)
{
  game_numPlayers = 1;
  game_onScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer2;
  game_offScreenBuffer = (frame_buffer_t)&game_frameBufferData.frameBuffer1;
}


void
game_init(menu_command_t command)
{
  hw_waitVerticalBlank();
  palette_black();

  screen_pokeCopperList(game_scoreBoardFrameBuffer, copper.sb_bpl, SCREEN_WIDTH_BYTES);  
  screen_setup((uint16_t*)&copper);

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
}


static void
game_complete(void)
{
  game_gotoMenu = 1;

  if (game_player1Score > 0) {
    hiscore_addScore(1, game_player1Score);
  }
  if (game_player2Score > 0) {
    hiscore_addScore(2, game_player2Score);
  }
    
#ifdef DEBUG
  game_checkCanary();
#endif
}


void
game_setGameOver(void)
{
  game_over = 1;
  game_scoreBoardPlayerText(OBJECT_ID_PLAYER1, I18N_GAME_OVER);
  game_scoreBoardPlayerText(OBJECT_ID_PLAYER2, I18N_GAME_OVER);
  object_set_z(object_add(OBJECT_ID_JOYSTICK, OBJECT_CLASS_DECORATION, game_cameraX+(SCREEN_WIDTH/2-16), (PLAYAREA_HEIGHT/2)+32, 0, OBJECT_ANIM_JOYSTICK, 0, 0, 0), 4096);   
  object_set_z(object_add(OBJECT_ID_GAMEOVER, OBJECT_CLASS_DECORATION, game_cameraX+(SCREEN_WIDTH/2-35), (PLAYAREA_HEIGHT/2)-30, 0, OBJECT_ANIM_GAMEOVER, 0, 0, 0), 4096);  
}


void
game_setGameComplete(void)
{
  game_over = 1;
  music_play(4);  
  game_scoreBoardPlayerText(OBJECT_ID_PLAYER1, I18N_GAME_OVER);
  game_scoreBoardPlayerText(OBJECT_ID_PLAYER2, I18N_GAME_OVER);
  object_set_z(object_add(OBJECT_ID_JOYSTICK, OBJECT_CLASS_DECORATION, game_cameraX+(SCREEN_WIDTH/2-16), (PLAYAREA_HEIGHT/2)+32, 0, OBJECT_ANIM_JOYSTICK, 0, 0, 0), 4096);   
  object_set_z(object_add(OBJECT_ID_GAMECOMPLETE, OBJECT_CLASS_DECORATION, game_cameraX+(SCREEN_WIDTH/2-55), (PLAYAREA_HEIGHT/2)-30, 0, OBJECT_ANIM_GAMECOMPLETE, 0, 0, 0), 4096);    
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


void
game_scoreBoardPlayerText(uint16_t playerId, char* text)
{
  if (playerId == OBJECT_ID_PLAYER1) {
    text_drawText8(game_scoreBoardFrameBuffer, text, 224-3*8, 17);
  } else {
    text_drawText8(game_scoreBoardFrameBuffer, text, 48, 17);
  }
}

static void
game_scoreBoardPlayer2Score(char* text)
{
  text_drawText8(game_scoreBoardFrameBuffer, text, 48, 8);
}

static void
game_refreshScoreboard(void)
{
#ifdef DEBUG
  if (game_scoreBoardMode == 0) {
#endif
    if (game_numPlayers == 1) {
      game_scoreBoardPlayer2Score(I18N_PRESS_2);
    }
    game_scoreBoardPlayerText(OBJECT_ID_PLAYER1, I18N_BLANK_GAME_OVER);
    game_scoreBoardPlayerText(OBJECT_ID_PLAYER2, I18N_BLANK_GAME_OVER);
#ifdef DEBUG
  }
#endif
}


static void
game_newGame(menu_command_t command)
{
  game_level = 0;
  game_deltaT = 0;
  game_lastPlayer1Health = 0;
  game_lastPlayer2Health = 0;
  game_player1Score = 0;
  game_player2Score = 0;
  game_lastPlayer1Score = 0xffffffff;
  game_lastPlayer2Score = 0xffffffff;
  game_levelTime.min = 5;
  game_levelTime.sec10 = 0;
  game_levelTime.sec= 0;
  game_lastLevelTime.value = 0;
  game_levelTicCounter = 0;  

  if (command >= MENU_COMMAND_LEVEL) {
    game_level = command - MENU_COMMAND_LEVEL;
    command = MENU_COMMAND_PLAY;
  }

  game_refreshScoreboard();
  game_loadLevel(command);
}


static void
game_updatePlayerHealth(uint16_t x, uint16_t health)
{
  uint16_t score;

  for (score = 0; score < health; score+=10, x+= 5) {
    gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 288, x, 30, 16, 8);
  }

  for (; score < 100; score+= 10, x+= 5) {
    gfx_screenWidthBitBlit(game_scoreBoardFrameBuffer, 0, 296, x, 30, 16, 8);
  }
  
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
  game_rasterLinesIndex = 0;
  game_lastAverage = -1;
  game_lastMaxRasterLine = -1;
  game_lastEnemyCount = -1;
  game_lastItemCount = -1;
  game_scoreBoardMode = 0;
  game_debugRenderFrame = 0;
#endif

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

  alarm_init();
  object_init();
  fighter_init();
  hand_init();
  thing_init();
#ifdef GAME_STARS
  star_init();
#endif
    
  game_player1 = player_init(OBJECT_ID_PLAYER1, OBJECT_ANIM_PLAYER2_STAND_RIGHT, 80);
  game_player2 = 0;
  if (game_numPlayers == 2) {
    game_player2 = player_init(OBJECT_ID_PLAYER2, OBJECT_ANIM_PLAYER3_STAND_RIGHT, SCREEN_WIDTH-80);
  } else {
    game_updatePlayerHealth(50, 0);
    game_scoreBoardPlayerText(OBJECT_ID_PLAYER2, I18N_TO_PLAY);
  }

  enemy_init();

  conductor_init(level.instructions);  

#ifdef OBJECT_Z_BUFFER_COLLISION
  object_initZbuffer();
#endif
  
  hw_waitBlitter();

  game_render(0);

  game_switchFrameBuffers();

  game_render(0);

  hw_waitVerticalBlank();

  palette_fadeTo(level.palette, 32, 0);

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
#ifdef GAME_TURTLE
    text_drawScoreBoard(itoan(game_missedFrameCount, 3),5*8, 9);
#endif
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
    line += 312;
  }
  
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
#ifdef SCRIPTING
    if (game_frame == script_breakpoint) {
      game_paused = 1;
      script_breakpoint = 0xffffffff;
    }
#endif
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
#define _object_min(x,y)(x<=y?x:y)
	game_requestCameraX(min(game_cameraX+(SCREEN_WIDTH/3), conductor_scrollTarget));
	hand_hide();
      }
    }
  }
}

static void
game_render(uint16_t deltaT)
{
  object_render(game_offScreenBuffer, deltaT);
#ifdef GAME_STARS
  star_update(deltaT);
#endif
  game_updateWave();
#if 0  
  if (level.effectFunctor) {
    level.effectFunctor(game_offScreenBuffer);
  }
#endif
}


uint16_t
game_requestCameraX(int16_t targetCameraX)
{
  game_targetCameraX = targetCameraX;

  if (game_targetCameraX < 0) {
    game_targetCameraX = 0;
  } else if (game_targetCameraX >= GAME_WORLD_WIDTH-SCREEN_WIDTH) {
    game_targetCameraX = GAME_WORLD_WIDTH-SCREEN_WIDTH-1;
  }

  return targetCameraX;
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


void
game_startPlayback(void)
{

  palette_black();
  game_loadLevel(MENU_COMMAND_REPLAY);
  music_restart();
  hw_waitVerticalBlank();
  hw_verticalBlankCount = 0;
  game_lastVerticalBlankCount = 0;
}


void
game_startRecord(void)
{
  palette_black();
  game_loadLevel(MENU_COMMAND_REPLAY);
  record_setState(RECORD_RECORD);
  music_restart();
  hw_waitVerticalBlank();
  hw_verticalBlankCount = 0;
  game_lastVerticalBlankCount = 0;
}


int16_t
game_processKeyboard()
{
  switch (keyboard_key) {
#ifdef DEBUG
  case 'C':
    game_collisions = !game_collisions;
    break;
  case 'E':
    enemy_pause = !enemy_pause;
    break;
  case 'O':
    game_setGameOver();
    break;
  case 'D':
    game_scoreBoardMode = !game_scoreBoardMode;
    if (game_scoreBoardMode) {
      game_refreshDebugScoreboard();
    } else {
      //gfx_fillRect(game_scoreBoardFrameBuffer, 0, 0, FRAME_BUFFER_WIDTH, SCOREBOARD_HEIGHT, 0);
      game_refreshScoreboard();
    }
    break;    
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
  case 'S':
    record_setState(RECORD_IDLE);
    break;
  case 'L':
    game_startPlayback();
    break;
#endif
  case 'P':
    game_paused = !game_paused;
    game_lastVerticalBlankCount = hw_verticalBlankCount;
  case 'T':
    game_singleStep = 1;
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
  case 'Q':
    return 1;
    break;
  case '1':
    game_numPlayers = 1;
    game_init(MENU_COMMAND_PLAY);
    break;
  case '2':
    if (!game_over && game_player2 == 0 && game_numPlayers == 1) {
      game_numPlayers = 2;
      game_scoreBoardPlayer2Score(I18N_BLANK_GAME_OVER);
      game_scoreBoardPlayerText(OBJECT_ID_PLAYER2, I18N_BLANK_GAME_OVER);
      game_player2 = player_init(OBJECT_ID_PLAYER2, OBJECT_ANIM_PLAYER3_STAND_RIGHT, game_cameraX+SCREEN_WIDTH-80);
    }
    break;    
  }

  return 0;
}


static void
game_updateScoreboard(void)
{
#ifdef DEBUG
    if (!game_scoreBoardMode) {
#endif
    if (game_player1 && game_lastPlayer1Health != ((fighter_data_t*)game_player1->data)->health) {
	game_updatePlayerHealth(225, ((fighter_data_t*)game_player1->data)->health);
	game_lastPlayer1Health = ((fighter_data_t*)game_player1->data)->health;
    } else if (game_player2 && game_lastPlayer2Health != ((fighter_data_t*)game_player2->data)->health) {
      game_updatePlayerHealth(50, ((fighter_data_t*)game_player2->data)->health);
      game_lastPlayer2Health = ((fighter_data_t*)game_player2->data)->health;
    } else if (game_levelTime.value != game_lastLevelTime.value) {
      uint16_t x = 146;
      uint16_t y = 19;      
      text_clrBlit(game_scoreBoardFrameBuffer, x, y, 9*3+5, 11);      
      text_drawBigNumeral(game_scoreBoardFrameBuffer, game_levelTime.sec, x+5+9+9, y, 11);
      text_drawBigNumeral(game_scoreBoardFrameBuffer, game_levelTime.sec10, x+5+9, y, 11);
      text_drawBigNumeral(game_scoreBoardFrameBuffer, game_levelTime.min, x, y, 11);
      text_drawBigNumeral(game_scoreBoardFrameBuffer, 10, x+7, y, 11);
      game_lastLevelTime = game_levelTime;
    } else if (game_lastPlayer1Score != game_player1Score) {
      frame_buffer_t fb = game_scoreBoardFrameBuffer;
      text_drawText8(fb, itoan(game_player1Score, 6), 224, 8);
      game_lastPlayer1Score = game_player1Score;
      custom->bltafwm = 0xffff;
    } else if (game_lastPlayer2Score != game_player2Score) {
      if (game_player2) {
	frame_buffer_t fb = game_scoreBoardFrameBuffer;
	text_drawText8(fb, itoan(game_player2Score, 6), 48, 8);
	game_lastPlayer2Score = game_player2Score;
	custom->bltafwm = 0xffff;
      }
    }

#ifdef DEBUG
    }
#endif    
}


static void
game_decrementTime(void)
{
  if (!game_over) {
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
      game_setGameOver();      
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
  P61_Target = 0;
  P61_Master = 0;
  
  custom->color[0] = 0;
  hw_interruptsInit(); // Don't enable interrupts until music is set up
  
  game_ctor();

  logo_load();

  music_play(0);

  logo_display();

  music_toggle();
  
  hiscore_ctor();

  menu_command_t menuCommand;
 menu:
#ifdef DEBUG
  game_checkStack();
#endif
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
#ifdef DEBUG
    game_checkStack();
#endif
    keyboard_read();
    hw_readJoystick();
    if (game_numPlayers == 2) {
      hw_readJoystick2();
    }

    if (game_over && game_fire()) {
      game_complete();
    }
    
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
      
#ifdef DEBUG
    if (game_paused && game_singleStep != 1) {
      goto skip;
    }
#endif
    

#ifdef GAME_TURTLE    
    if (game_turtle > 1) {
      custom->color[21] = 0xf00;
      game_turtle--;
    } else if (game_turtle == 1) {
      custom->color[21] = level.palette[21];
      game_turtle--;
    }
#endif

    game_updateScoreboard();
      
#ifdef DEBUG
    debug_showRasterLine();
#endif
    
    sound_schedule();

    if (game_gotoMenu) {
      goto menu;
    }

    hw_waitVerticalBlank();    
    
    sound_vbl();

    if (
#ifdef DEBUG
	game_collectTotal &&
#endif
	hw_verticalBlankCount-game_lastVerticalBlankCount > 2) {
#ifdef GAME_TURTLE
      game_turtle = 5;
      game_missedFrameCount++;      
#endif
    }
#ifdef GAME_25_FPS
    else if (hw_verticalBlankCount-game_lastVerticalBlankCount == 1) {
      hw_waitVerticalBlank();
    }
#endif
      
    game_lastVerticalBlankCount = hw_verticalBlankCount;

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
     
    alarm_process(game_deltaT);
    game_render(game_deltaT);    

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


int
memcmp(void *s1, void *s2, size_t n)
{
  char* p1 = s1;
  char* p2 = s2;
  
  for ( ; n-- ;) {
    if (*p1 != *p2) {
      return (*p1-*p2);
    }
    p1++;
    p2++;
  }
  return 0;
}

