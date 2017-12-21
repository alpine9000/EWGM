#ifndef __GAME_H
#define __GAME_H

#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>

#define CONFIG_OCS_512     1
#define CONFIG_OCS_FAST    2
#define CONFIG_WORKBENCH   3

//#define DEBUG
//#define GAME_AUTOSTART_REPLAY
//#define GAME_DEBUG_OBJECT_RENDERING

#ifdef DEBUG
#define SCRIPTING
#define GAME_TURTLE                   1
#else
#define RELEASE
#endif

//#define GAME_OBJECTS_BELOW_PLAYAREA_BOTTOM
#define GAME_DONT_CLEAR_STATIONARY_OBJECTS
#define GAME_DONT_REDRAW_CLEAN_OBJECTS

#if GAME_CONFIG == CONFIG_OCS_512
#include "configs/ocs512.h"
#elif GAME_CONFIG == CONFIG_OCS_FAST
#include "configs/ocsfast.h"
#elif GAME_CONFIG == CONFIG_WORKBENCH
#include "configs/workbench.h"
#else
#error "No config specified"
#endif


#define abs(a) ((a) >= 0 ? (a) : -(a))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))


#define MAP_LEVEL1_TILE_WIDTH    100
#define MAP_LEVEL2_TILE_WIDTH    100
#define MAP_LEVEL3_TILE_WIDTH    100
#define MAP_LEVEL4_TILE_WIDTH    140

#define MAP_MAX_TILE_WIDTH       140
#define MAP_TILE_HEIGHT          13
#define TILE_WIDTH               16
#define TILE_HEIGHT              16

#define CUSTOM ((struct Custom*)0xdff000)

#define SCREEN_WIDTH             320
#define SCREEN_HEIGHT            256
#define PLAYAREA_HEIGHT          MAP_TILE_HEIGHT*TILE_HEIGHT

#define SCREEN_WIDTH_BYTES       (SCREEN_WIDTH/8)
#define SCREEN_WIDTH_WORDS       (SCREEN_WIDTH/16)
#define SCREEN_BIT_DEPTH         5
#define SCREEN_NUM_COLORS        (1<<SCREEN_BIT_DEPTH)
#define SCREEN_RES	         8 // 8=lo resolution, 4=hi resolution

#define FRAME_BUFFER_WIDTH       (SCREEN_WIDTH+48)
#define FRAME_BUFFER_WIDTH_BYTES (FRAME_BUFFER_WIDTH/8)
#define FRAME_BUFFER_OFFSCREEN_HEIGHT 0
#define FRAME_BUFFER_HORIZONTAL_SCROLL_LINES (((FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH)/((MAP_MAX_TILE_WIDTH*2)))+2)
#define FRAME_BUFFER_NUM_LINES   ((PLAYAREA_HEIGHT+FRAME_BUFFER_OFFSCREEN_HEIGHT+FRAME_BUFFER_HORIZONTAL_SCROLL_LINES))
#define FRAME_BUFFER_MAX_HEIGHT  (FRAME_BUFFER_NUM_LINES-1)

#define SCOREBOARD_BUFFER_SIZE_BYTES (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH*SCOREBOARD_HEIGHT)

#define SPRITE_SHEET_WIDTH       272
#define SPRITE_SHEET_HEIGHT      1038
#define SPRITE_SHEET_WIDTH_BYTES (SPRITE_SHEET_WIDTH/8)

#define TILE_SHEET_WIDTH         272
#define TILE_SHEET_HEIGHT        112
#define TILE_SHEET_WIDTH_BYTES   (TILE_SHEET_WIDTH/8)

#define RASTER_X_START	         (0x81+((320-SCREEN_WIDTH)/2)) /* hard coded coordinates from hardware manual */
#define RASTER_Y_START	         0x25
#define RASTER_X_STOP	         RASTER_X_START+SCREEN_WIDTH
#define GAME_RASTER_Y_STOP       RASTER_Y_START+PLAYAREA_HEIGHT+SCOREBOARD_HEIGHT
#define SCREEN_RASTER_Y_STOP     RASTER_Y_START+SCREEN_HEIGHT
#define LOGO_RASTER_Y_STOP       RASTER_Y_START+SCREEN_HEIGHT-12
#define MESSAGE_RASTER_Y_STOP    RASTER_Y_START+PLAYAREA_HEIGHT
#define SCOREBOARD_HEIGHT        43
#define GAME_PAVEMENT_START      66

#define GAME_WORLD_HEIGHT        (MAP_TILE_HEIGHT*TILE_HEIGHT)
#define GAME_LEVEL1_WORLD_WIDTH  (MAP_LEVEL1_TILE_WIDTH*TILE_WIDTH)
#define GAME_LEVEL2_WORLD_WIDTH  (MAP_LEVEL2_TILE_WIDTH*TILE_WIDTH)
#define GAME_LEVEL3_WORLD_WIDTH  (MAP_LEVEL3_TILE_WIDTH*TILE_WIDTH)
#define GAME_LEVEL4_WORLD_WIDTH  (MAP_LEVEL4_TILE_WIDTH*TILE_WIDTH)


#define GAME_PLAYER1_HEALTH_SCOREBOARD_X 225
#define GAME_PLAYER2_HEALTH_SCOREBOARD_X 50

#define GAME_BIG_FONT_HEIGHT         12
#define GAME_BIG_FONT_WIDTH          12
#define GAME_BIG_FONT_WIDTH_SPACING  1
#define GAME_BIG_FONT_GAP            (GAME_BIG_FONT_WIDTH_SPACING+GAME_BIG_FONT_WIDTH)
#define GAME_BIG_FONT_COLON_GAP      7

#define GAME_COUNTDOWN_COLOR_TOP_OK      0x0b0
#define GAME_COUNTDOWN_COLOR_BOTTOM_OK   0x090
#define GAME_COUNTDOWN_COLOR_TOP_WARN    0xb00
#define GAME_COUNTDOWN_COLOR_BOTTOM_WARN 0x900

typedef UBYTE uint8_t;
typedef SHORT int16_t;
typedef USHORT uint16_t;
typedef LONG int32_t;
typedef ULONG uint32_t;
typedef ULONG size_t;

#ifdef SCAN_CHECKER

#define __INLINE
#define __NOINLINE
#define __SECTION_DATA_C
#if TRACKLOADER==1
#define __SECTION_RANDOM_C
#define __SECTION_RANDOM
#define __SECTION_DISK
#define __SECTION_LASTTRACK
#else
#define __SECTION_RANDOM_C
#define __SECTION_RANDOM
#define __SECTION_DISK
#define __SECTION_LASTTRACK
#endif
#define __EXTERNAL
#define __REG(reg, arg)     arg
//#define __USE(x) do { x = x; } while(0);
#define __UNUSED            __attribute__ ((unused))
#define __stdargs

#else

#define __INLINE            inline
#define __NOINLINE          __attribute__ ((noinline))
#define __SECTION_DATA_C    __attribute__ ((section ("data_c,data,chip")))
#if TRACKLOADER==1
#define __SECTION_RANDOM_C  __attribute__ ((section ("random_c")))
#define __SECTION_RANDOM    __attribute__ ((section("random")))
#define __SECTION_DISK      __attribute__ ((section ("disk")))
#define __SECTION_LASTTRACK __attribute__ ((section ("lastTrack")))
#else
#define __SECTION_RANDOM_C  __attribute__ ((section ("bss_c,bss,chip")))
#define __SECTION_RANDOM    __attribute__ ((section ("bss")))
#define __SECTION_DISK      __attribute__ ((section (".text")))
#define __SECTION_LASTTRACK __attribute__ ((section (".text")))
#endif
#define __EXTERNAL          __attribute__ ((externally_visible))
#define __REG(reg, arg)     register arg asm(reg)
#define __UNUSED            __attribute__ ((unused))

#endif

#define custom ((custom_t)0xdff000)


extern void* memcpy(void* destination, const void* source, size_t num);
extern void* memset(void *dst, int c, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);


typedef volatile uint8_t* frame_buffer_t;
typedef volatile struct Custom* custom_t;


extern int16_t game_cameraX;
extern int16_t game_screenScrollX;
extern frame_buffer_t game_menuBuffer;
extern frame_buffer_t game_messageBuffer;
extern frame_buffer_t game_onScreenBuffer;
extern frame_buffer_t game_offScreenBuffer;
#ifdef GAME_TRIPLE_BUFFER
extern frame_buffer_t game_backScreenBuffer;
#endif
extern frame_buffer_t game_scoreBoardFrameBuffer;
extern uint16_t game_demo;
extern uint16_t game_over;
extern uint16_t game_loopControl;
extern uint16_t game_numPlayers;
extern uint32_t game_player1Score;
extern uint32_t game_player2Score;
extern uint16_t game_difficulty;
extern uint16_t game_killScore;
extern uint16_t game_scoreboardLoaded;
#ifdef DEBUG
extern uint16_t game_collisions;
#endif
#ifndef RELEASE
extern uint16_t game_startReplay;
extern uint16_t game_startLevelIndex;
#endif
extern uint16_t player1_character;
extern uint16_t game_maxScrollX;

enum {
  GAME_DIFFICULTY_EASY,
  GAME_DIFFICULTY_HARD
};

enum {
  GAME_LOOP_CONTROL_RUN = 0,
  GAME_LOOP_CONTROL_GOTO_MENU = 1,
  GAME_LOOP_CONTROL_DISPLAY_DEATHMATCH = 2,
  GAME_LOOP_CONTROL_DEATHMATCH = 3,
  GAME_LOOP_CONTROL_GAME_OVER = 4,
  GAME_LOOP_CONTROL_GAME_COMPLETE = 5
};

#include "i18n.h"
#include "string.h"
#include "random.h"
#include "amigaos.h"
#include "registers.h"
#include "hw.h"
#include "panic.h"
#include "disk.h"
#include "akiko.h"
#include "init.h"
#include "gfx.h"
#include "object.h"
#include "fighter.h"
#include "player.h"
#include "enemy.h"
#include "screen.h"
#include "menu.h"
#include "hiscore.h"
#include "palette.h"
#include "record.h"
#include "conductor.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level.h"
#include "mouse.h"
#include "tile.h"
#include "copper.h"
#include "music.h"
#include "text.h"
#include "sound.h"
#include "keyboard.h"
#include "message.h"
#include "script.h"
#include "sort.h"
#include "logo.h"
#include "hand.h"
#include "alarm.h"
#include "thing.h"
#include "baloon.h"
#include "star.h"
#include "motorbike.h"
#include "gunfighter.h"

extern object_t* game_player1;
extern object_t* game_player2;

#define game_fire() ((!(hw_lastJoystickButton&0x1) && (hw_joystickButton&0x1)) || \
                     (!(hw_lastJoystick2Button&0x1) && (hw_joystick2Button&0x1)) || \
			(keyboard_lastKey != keyboard_key && keyboard_key && keyboard_code == KEYBOARD_CODE_RETURN))
void
game_loop(void);
uint16_t
game_requestCameraX(int16_t targetCameraX);
void
game_switchFrameBuffers(void);
void
game_scoreBoardPlayer1Text(char* text);
void
game_scoreBoardPlayer2Text(char* text);
void
game_setGameOver(void);
void
game_setGameComplete(void);
void
game_updatePlayerHealth(uint16_t x, int16_t health);
void
game_updatePlayer1Health(uint16_t x, int16_t health);
void
game_updatePlayer2Health(uint16_t x, int16_t health);
void
game_maxHealth(void);
#ifdef DEBUG
void
game_checkCanary(void);
#endif
#endif /* __GAME_H */
