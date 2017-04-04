#ifndef __OBJECT_H
#define __OBJECT_H

#define OBJECT_PHYSICS_FACTOR 2
#define OBJECT_HEIGHT 52
#define OBJECT_WIDTH 32

//#define OBJECT_BACKING_STORE 1

#define OBJECT_MAX_OBJECTS    8

#ifdef OBJECT_BACKING_STORE
#define OBJECT_MAX_BLIT_WIDTH 48+16
#define OBJECT_MAX_HEIGHT     60
#endif

typedef enum {
  SPRITE_PLAYER1_STAND_RIGHT,
  SPRITE_PLAYER1_RUN_RIGHT_1,
  SPRITE_PLAYER1_RUN_RIGHT_2,
  SPRITE_PLAYER1_RUN_RIGHT_3,
  SPRITE_PLAYER1_RUN_RIGHT_4,
  SPRITE_PLAYER1_STAND_LEFT,
  SPRITE_PLAYER1_RUN_LEFT_1,
  SPRITE_PLAYER1_RUN_LEFT_2,
  SPRITE_PLAYER1_RUN_LEFT_3,
  SPRITE_PLAYER1_RUN_LEFT_4,

  SPRITE_PLAYER2_STAND_RIGHT,
  SPRITE_PLAYER2_RUN_RIGHT_1,
  SPRITE_PLAYER2_RUN_RIGHT_2,
  SPRITE_PLAYER2_RUN_RIGHT_3,
  SPRITE_PLAYER2_RUN_RIGHT_4,
  SPRITE_PLAYER2_STAND_LEFT,
  SPRITE_PLAYER2_RUN_LEFT_1,
  SPRITE_PLAYER2_RUN_LEFT_2,
  SPRITE_PLAYER2_RUN_LEFT_3,
  SPRITE_PLAYER2_RUN_LEFT_4 ,   
  
  SPRITE_PLAYER3_STAND_RIGHT,
  SPRITE_PLAYER3_RUN_RIGHT_1,
  SPRITE_PLAYER3_RUN_RIGHT_2,
  SPRITE_PLAYER3_RUN_RIGHT_3,
  SPRITE_PLAYER3_RUN_RIGHT_4,
  SPRITE_PLAYER3_STAND_LEFT,
  SPRITE_PLAYER3_RUN_LEFT_1,
  SPRITE_PLAYER3_RUN_LEFT_2,
  SPRITE_PLAYER3_RUN_LEFT_3,
  SPRITE_PLAYER3_RUN_LEFT_4 ,   
} sprite_id_t;

typedef enum {
  OBJECT_STAND_RIGHT = 0,
  OBJECT_RUN_RIGHT = 1,
  OBJECT_RUN_LEFT = 2,
  OBJECT_STAND_LEFT = 3
} object_action_t;

typedef enum {
  OBJECT_ANIM_PLAYER1_STAND_RIGHT = 0,
  OBJECT_ANIM_PLAYER1_RUN_RIGHT = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_RUN_RIGHT,
  OBJECT_ANIM_PLAYER1_STAND_LEFT = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_STAND_LEFT,
  OBJECT_ANIM_PLAYER1_RUN_LEFT  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_RUN_LEFT,

  OBJECT_ANIM_PLAYER2_STAND_RIGHT = 4,
  OBJECT_ANIM_PLAYER2_RUN_RIGHT = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_RUN_RIGHT,
  OBJECT_ANIM_PLAYER2_STAND_LEFT = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_STAND_LEFT,
  OBJECT_ANIM_PLAYER2_RUN_LEFT  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_RUN_LEFT,

  OBJECT_ANIM_PLAYER3_STAND_RIGHT = 8,
  OBJECT_ANIM_PLAYER3_RUN_RIGHT = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_RUN_RIGHT,
  OBJECT_ANIM_PLAYER3_STAND_LEFT = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_STAND_LEFT,
  OBJECT_ANIM_PLAYER3_RUN_LEFT  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_RUN_LEFT  
} object_animation_id_t;

typedef enum {
  FACING_LEFT,
  FACING_RIGHT
} object_direction_t;

typedef enum {
  OBJECT_STATE_ALIVE,
  OBJECT_STATE_REMOVED,
  OBJECT_STATE_DEAD
} object_state_t;

typedef struct {
  int16_t start;
  int16_t stop;
  int16_t speed;
  object_direction_t facing;
} object_animation_t;

typedef struct {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  int16_t dx;
  int16_t dy;
} object_image_t;

typedef struct {
  int16_t x;
  int16_t y;
} object_velocity_t;

#ifdef OBJECT_BACKING_STORE
typedef struct {
  uint8_t fb[(OBJECT_MAX_BLIT_WIDTH/8)*SCREEN_BIT_DEPTH*OBJECT_MAX_HEIGHT];
} object_backing_store_t;
#else
typedef struct {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
} object_position_t;
#endif

typedef struct {
#ifdef OBJECT_BACKING_STORE
  object_backing_store_t buffers[2];
  frame_buffer_t buffer;
  gfx_blit_t blits[2];
  gfx_blit_t* blit;
#else
  object_position_t positions[2];
  object_position_t* position;
#endif
} object_save_t;

typedef struct object {
  struct object* next;
  struct object* prev;
  int16_t x;
  int16_t y;
  int16_t px;
  int16_t py;
  int16_t lx;
  int16_t ly;
  object_velocity_t velocity;
  int16_t imageIndex;
  object_image_t* image;
  uint16_t baseId;
  uint16_t animId;
  object_animation_t* anim;  
  object_save_t save;
  uint16_t frameCounter;
  uint16_t deadRenderCount;
  uint16_t state;
  void (*update)(struct object* ptr);
  void* data;
  int16_t walkAbout; // todo: move to enemy data
  uint32_t lastUpdatedFrame;
} object_t;

typedef struct {
  int16_t up;
  int16_t down;
  int16_t left;
  int16_t right;
} object_movement_t;


extern object_image_t object_imageAtlas[];
extern object_animation_t object_animations[];
void
object_init(void);
object_t*
object_add(int16_t x, int16_t y, int16_t dx, int16_t anim, void (*update)(object_t* ptr), void* data);
void
object_render(frame_buffer_t fb);
void
object_saveBackground(frame_buffer_t fb);
void
object_restoreBackground(frame_buffer_t fb);
object_t*
object_collision(object_t* a);
int16_t
object_collision2(object_t* a, object_movement_t* movement);
int16_t
object_strikingDistanceX(object_t* a, object_t* b);
void
object_updateObject(object_t* ptr);
#endif
