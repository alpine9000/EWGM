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
  SPRITE_PLAYER1_PUNCH_LEFT_1,
  SPRITE_PLAYER1_PUNCH_LEFT_2,
  SPRITE_PLAYER1_PUNCH_RIGHT_1,
  SPRITE_PLAYER1_PUNCH_RIGHT_2,
  SPRITE_PLAYER1_HIT_LEFT,
  SPRITE_PLAYER1_HIT_RIGHT,  

  SPRITE_PLAYER2_STAND_RIGHT,
  SPRITE_PLAYER2_RUN_RIGHT_1,
  SPRITE_PLAYER2_RUN_RIGHT_2,
  SPRITE_PLAYER2_RUN_RIGHT_3,
  SPRITE_PLAYER2_RUN_RIGHT_4,
  SPRITE_PLAYER2_STAND_LEFT,
  SPRITE_PLAYER2_RUN_LEFT_1,
  SPRITE_PLAYER2_RUN_LEFT_2,
  SPRITE_PLAYER2_RUN_LEFT_3,
  SPRITE_PLAYER2_RUN_LEFT_4,   
  SPRITE_PLAYER2_PUNCH_LEFT_1,
  SPRITE_PLAYER2_PUNCH_LEFT_2,
  SPRITE_PLAYER2_PUNCH_RIGHT_1,
  SPRITE_PLAYER2_PUNCH_RIGHT_2,
  SPRITE_PLAYER2_HIT_LEFT,
  SPRITE_PLAYER2_HIT_RIGHT,    
  
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
  SPRITE_PLAYER3_PUNCH_LEFT_1,
  SPRITE_PLAYER3_PUNCH_LEFT_2,
  SPRITE_PLAYER3_PUNCH_RIGHT_1,
  SPRITE_PLAYER3_PUNCH_RIGHT_2,
  SPRITE_PLAYER3_HIT_LEFT,
  SPRITE_PLAYER3_HIT_RIGHT,    
} sprite_id_t;

typedef enum {
  OBJECT_STAND_RIGHT = 0,
  OBJECT_RUN_RIGHT = 1,
  OBJECT_RUN_LEFT = 2,
  OBJECT_STAND_LEFT = 3,
  OBJECT_PUNCH_LEFT1 = 4,
  OBJECT_PUNCH_LEFT2 = 5,
  OBJECT_PUNCH_RIGHT1 = 6,
  OBJECT_PUNCH_RIGHT2 = 7,
  OBJECT_HIT_LEFT = 8,
  OBJECT_HIT_RIGHT = 9
} object_action_t;

typedef enum {
  OBJECT_ANIM_PLAYER1_STAND_RIGHT = 0,
  OBJECT_ANIM_PLAYER1_RUN_RIGHT = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_RUN_RIGHT,
  OBJECT_ANIM_PLAYER1_STAND_LEFT = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_STAND_LEFT,
  OBJECT_ANIM_PLAYER1_RUN_LEFT  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_RUN_LEFT,
  OBJECT_ANIM_PLAYER1_PUNCH_LEFT1  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_PUNCH_LEFT1,
  OBJECT_ANIM_PLAYER1_PUNCH_LEFT2  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_PUNCH_LEFT2,
  OBJECT_ANIM_PLAYER1_PUNCH_RIGHT1  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_PUNCH_RIGHT1,
  OBJECT_ANIM_PLAYER1_PUNCH_RIGHT2  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_PUNCH_RIGHT2,
  OBJECT_ANIM_PLAYER1_HIT_LEFT  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_HIT_LEFT,
  OBJECT_ANIM_PLAYER1_HIT_RIGHT  = OBJECT_ANIM_PLAYER1_STAND_RIGHT + OBJECT_HIT_RIGHT,            

  OBJECT_ANIM_PLAYER2_STAND_RIGHT = OBJECT_ANIM_PLAYER1_HIT_RIGHT + 1,
  OBJECT_ANIM_PLAYER2_RUN_RIGHT = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_RUN_RIGHT,
  OBJECT_ANIM_PLAYER2_STAND_LEFT = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_STAND_LEFT,
  OBJECT_ANIM_PLAYER2_RUN_LEFT  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_RUN_LEFT,
  OBJECT_ANIM_PLAYER2_PUNCH_LEFT1  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_PUNCH_LEFT1,
  OBJECT_ANIM_PLAYER2_PUNCH_LEFT2  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_PUNCH_LEFT2,
  OBJECT_ANIM_PLAYER2_PUNCH_RIGHT1  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_PUNCH_RIGHT1,
  OBJECT_ANIM_PLAYER2_PUNCH_RIGHT2  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_PUNCH_RIGHT2,
  OBJECT_ANIM_PLAYER2_HIT_LEFT  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_HIT_LEFT,
  OBJECT_ANIM_PLAYER2_HIT_RIGHT  = OBJECT_ANIM_PLAYER2_STAND_RIGHT + OBJECT_HIT_RIGHT,              

  OBJECT_ANIM_PLAYER3_STAND_RIGHT = OBJECT_ANIM_PLAYER2_HIT_RIGHT + 1,
  OBJECT_ANIM_PLAYER3_RUN_RIGHT = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_RUN_RIGHT,
  OBJECT_ANIM_PLAYER3_STAND_LEFT = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_STAND_LEFT,
  OBJECT_ANIM_PLAYER3_RUN_LEFT  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_RUN_LEFT,
  OBJECT_ANIM_PLAYER3_PUNCH_LEFT1  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_PUNCH_LEFT1,
  OBJECT_ANIM_PLAYER3_PUNCH_LEFT2  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_PUNCH_LEFT2,
  OBJECT_ANIM_PLAYER3_PUNCH_RIGHT1  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_PUNCH_RIGHT1,
  OBJECT_ANIM_PLAYER3_PUNCH_RIGHT2  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_PUNCH_RIGHT2,  
  OBJECT_ANIM_PLAYER3_HIT_LEFT  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_HIT_LEFT,
  OBJECT_ANIM_PLAYER3_HIT_RIGHT  = OBJECT_ANIM_PLAYER3_STAND_RIGHT + OBJECT_HIT_RIGHT,
  
} object_animation_id_t;

typedef enum {
  FACING_LEFT,
  FACING_RIGHT
} object_direction_t;

typedef enum {
  OBJECT_STATE_ALIVE,
  OBJECT_STATE_REMOVED,
  OBJECT_STATE_DEAD,
  OBJECT_STATE_HIT,
  OBJECT_STATE_FLASHING
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
  int16_t _x;
  int16_t _y;
  int16_t _z;
  int16_t _px;
  int16_t _py;
  object_velocity_t velocity;
  int16_t imageIndex;
  object_image_t* image;
  uint16_t baseId;
  uint16_t animId;
  uint16_t actionId;
  object_animation_t* anim;  
  object_save_t save;
  uint16_t frameCounter;
  uint16_t deadRenderCount;
  uint16_t state;
  void (*update)(uint16_t deltaT, struct object* ptr);
  void* data;
  uint32_t lastUpdatedFrame;
  uint16_t visible;
} object_t;


typedef struct {
  object_t* up;
  object_t* down;
  object_t* left;
  object_t* right;
} object_collision_t;


extern object_image_t object_imageAtlas[];
extern object_animation_t object_animations[];
object_t* object_activeList;

void
object_init(void);
object_t*
object_add(int16_t x, int16_t y, int16_t dx, int16_t anim, void (*update)(uint16_t deltaT, object_t* ptr), void* data);
void
object_render(frame_buffer_t fb);
void
object_saveBackground(frame_buffer_t fb);
void
object_restoreBackground(frame_buffer_t fb);
void
object_updatePosition(uint16_t deltaT, object_t* ptr);
void
object_setAction(object_t* ptr, object_action_t action);

inline static int16_t
object_x(object_t* ptr) {
  return ptr->_x;
}

inline static int16_t
object_y(object_t* ptr) {
  return ptr->_y;
}

inline static int16_t
object_z(object_t* ptr) {
  return ptr->_z;
}

inline static int16_t
object_px(object_t* ptr) {
  return ptr->_px;
}

inline static int16_t
object_py(object_t* ptr) {
  return ptr->_py;
}

inline static void
object_set_px(object_t* ptr, int16_t px)
{
  ptr->_px = px;  
  ptr->_x = ptr->_px / OBJECT_PHYSICS_FACTOR;

  /* TODO: player width, not image width */
  if (ptr->_x + OBJECT_WIDTH >= SCREEN_WIDTH) {
    ptr->_x = SCREEN_WIDTH - OBJECT_WIDTH - 1;
    ptr->_px = ptr->_x * OBJECT_PHYSICS_FACTOR;
  }
}

inline static void
object_set_py(object_t* ptr, int16_t py)
{
  ptr->_py = py;  
  ptr->_y = ptr->_py / OBJECT_PHYSICS_FACTOR;

  if (ptr->_y >= PLAYAREA_HEIGHT) {
    ptr->_y = PLAYAREA_HEIGHT-2;
    ptr->_py = ptr->_y * OBJECT_PHYSICS_FACTOR;
  }

  if ((ptr->_y-ptr->image->h) <= 0) {
    ptr->_y = ptr->image->h;
    ptr->_py = ptr->_y * OBJECT_PHYSICS_FACTOR;
  }
}

inline static void
object_set_z(object_t* ptr, int16_t z)
{
  ptr->_z = z;
}

#endif
