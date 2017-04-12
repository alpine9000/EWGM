#ifndef __GAME_H
#define __GAME_H

#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>

//#define DEBUG_SPEED                   1
#define DEBUG                         1
#define GAME_KEYBOARD_ENABLED         1
//#define GAME_TIME_USE_COUNTER         1
//#define GAME_RECORDING                1
//#define GAME_BLITTER_WAIT_DEBUG       1

#define INLINE inline
#define STATIC_INLINE static inline

#define abs(a) ((a) >= 0 ? (a) : -(a))  
#define max(a, b) ((a) > (b) ? (a) : (b))

#define MENU_NUM_ITEMS      6

#define MAP_TILE_WIDTH      60
#define MAP_TILE_HEIGHT     13
#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define CUSTOM ((struct Custom*)0xdff000)

#define SCREEN_WIDTH          320
#define SCREEN_HEIGHT          256
#define PLAYAREA_HEIGHT        MAP_TILE_HEIGHT*TILE_HEIGHT


#define FRAME_BUFFER_OFFSCREEN_HEIGHT 16
#define FRAME_BUFFER_NUM_LINES ((PLAYAREA_HEIGHT+FRAME_BUFFER_OFFSCREEN_HEIGHT))
#define FRAME_BUFFER_MAX_HEIGHT (FRAME_BUFFER_NUM_LINES-1)
#define FRAME_BUFFER_WIDTH  (SCREEN_WIDTH+64)

#define SPRITE_SHEET_WIDTH  272
#define SPRITE_SHEET_HEIGHT 507
#define SPRITE_SHEET_WIDTH_BYTES (SPRITE_SHEET_WIDTH/8)

#define TILE_SHEET_WIDTH  272
#define TILE_SHEET_HEIGHT 112
#define TILE_SHEET_WIDTH_BYTES (TILE_SHEET_WIDTH/8)

#define SCREEN_WIDTH_BYTES  (SCREEN_WIDTH/8)

#define FRAME_BUFFER_WIDTH_BYTES  (FRAME_BUFFER_WIDTH/8)
#define SCREEN_WIDTH_WORDS  (SCREEN_WIDTH/16)
#define SCREEN_BIT_DEPTH    5
#define SCREEN_NUM_COLORS   (1<<SCREEN_BIT_DEPTH)
#define SCREEN_RES	    8 // 8=lo resolution, 4=hi resolution
#define RASTER_X_START	    (0x81+((320-SCREEN_WIDTH)/2)) /* hard coded coordinates from hardware manual */
#define RASTER_Y_START	    0x25
#define RASTER_X_STOP	    RASTER_X_START+SCREEN_WIDTH
#define GAME_RASTER_Y_STOP	    RASTER_Y_START+PLAYAREA_HEIGHT+SCOREBOARD_HEIGHT
#define SCREEN_RASTER_Y_STOP  RASTER_Y_START+SCREEN_HEIGHT
#define SCOREBOARD_HEIGHT   43
#define GAME_PAVEMENT_START 66

#define WORLD_HEIGHT        (MAP_TILE_HEIGHT*TILE_HEIGHT)
#define WORLD_WIDTH         (MAP_TILE_WIDTH*TILE_WIDTH)
#define SCROLL_PIXELS       4

#define PHYSICS_VELOCITY_G        1
#define PHYSICS_TERMINAL_VELOCITY (SCROLL_PIXELS*2)
#define PHYSICS_VELOCITY_RUN      2
#define PHYSICS_VELOCITY_JUMP     -16
#define PHYSICS_VELOCITY_KILL     -10
#ifdef GAME_JETPACK
#define PHYSICS_VELOCITY_JETPACK  -4
#define PHYSICS_TERMINAL_JETPACK_V (SCROLL_PIXELS)
#endif

typedef UBYTE uint8_t;
typedef SHORT int16_t;
typedef USHORT uint16_t;
typedef LONG int32_t;
typedef ULONG uint32_t;
typedef ULONG size_t;

#define __NOLOAD __section(section noload)

#define __EXTERNAL __attribute__((externally_visible))

extern void* memcpy(void* destination, void* source, size_t num);
extern void* memset(void *dst, int c, size_t n);

#undef __chip
#define __section(x) __attribute__ ((section (#x))) 
#define __REG(reg, arg) register arg asm(reg)
#define USE(x) do { x = x; } while(0);

typedef volatile uint8_t* frame_buffer_t;
typedef volatile struct Custom* custom_t;

#define custom ((custom_t)0xdff000)

extern int16_t game_cameraX;
extern int16_t game_screenScrollX;
extern int16_t game_collisions;
extern frame_buffer_t game_onScreenBuffer;
extern frame_buffer_t game_offScreenBuffer;
extern frame_buffer_t game_scoreBoardFrameBuffer;
extern uint16_t game_over;
extern uint16_t game_levelComplete;
extern uint16_t game_numPlayers;
extern uint16_t game_wave;
extern uint16_t game_nextWave;
extern uint32_t game_player1Score;
extern uint32_t game_player2Score;

#include "string.h"
#include "registers.h"
#include "hw.h"
#include "panic.h"
#include "disk.h"
#include "init.h"
#include "gfx.h"
#include "object.h"

extern object_t* game_player1;
extern object_t* game_player2;

#include "fighter.h"
#include "player.h"
#include "enemy.h"
#include "screen.h"
#include "hiscore.h"
#include "menu.h"
#include "palette.h"
#include "record.h"
#include "level.h"
#include "mouse.h"
#include "tile.h"
#include "copper.h"
#include "music.h"
#include "text.h"
#include "sound.h"
#include "keyboard.h"
#include "message.h"
#include "dos.h"
#include "script.h"
#include "sort.h"
#include "logo.h"
#include "hand.h"
#include "wave.h"

#define game_fire() ((!(hw_lastJoystickButton&0x1) && (hw_joystickButton&0x1)) || \
			(keyboard_key && keyboard_code == KEYBOARD_CODE_RETURN))
void 
game_loop(void);
uint16_t
game_requestCameraX(int16_t targetCameraX);
void
game_loseLife(void);
void
game_setLevelComplete(void);
void
game_switchFrameBuffers(void);
#endif /* __GAME_H */
