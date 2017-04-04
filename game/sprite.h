#ifndef __SPRITE_H
#define __SPRITE_H


typedef struct {
  gfx_blit_t blit[2];
} sprite_save_t;  


typedef struct {
  int16_t x;
  int16_t y;
} velocity_t;


typedef struct {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
} image_t;


typedef struct {
  int16_t x;
  int16_t y;
  int16_t imageIndex;
  sprite_save_t* save;
  frame_buffer_t saveBuffer;
  uint16_t saveBufferHeightOffset;
  image_t* image;
} sprite_t;


extern image_t sprite_imageAtlas[];
extern uint16_t sprite_6byteWideLUT[64];

void
_sprite_render(frame_buffer_t fb, sprite_t* actor, void (*render)(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h));
void 
sprite_save(frame_buffer_t fb, sprite_t* a);
void
sprite_restore(sprite_save_t* save);

void
sprite_ctor(void);

#define sprite_render(fb, a) _sprite_render(fb, &a, gfx_renderSprite)
#define sprite_render16NoShift(fb, a) _sprite_render(fb, &a, gfx_renderSprite16NoShift)

typedef enum  {
  SPRITE_CLIMBER_RUN_LEFT_1 = 0,
  SPRITE_CLIMBER_RUN_LEFT_2 = 1,
  SPRITE_CLIMBER_RUN_LEFT_3 = 2,
  SPRITE_CLIMBER_RUN_LEFT_4 = 3,
  SPRITE_CLIMBER_STAND_LEFT = 4,
  SPRITE_CLIMBER_JUMP_LEFT = 5,
  SPRITE_CLIMBER_RUN_RIGHT_1 = 6,
  SPRITE_CLIMBER_RUN_RIGHT_2 = 7,
  SPRITE_CLIMBER_RUN_RIGHT_3 = 8,
  SPRITE_CLIMBER_RUN_RIGHT_4 = 9,
  SPRITE_CLIMBER_STAND_RIGHT = 10,
  SPRITE_CLIMBER_JUMP_RIGHT = 11,
  SPRITE_CLOUD_1 = 12,
  SPRITE_CLOUD_2 = 13,
  SPRITE_ENEMY_RUN_LEFT_1 = 14,
  SPRITE_ENEMY_RUN_LEFT_2 = 15,
  SPRITE_ENEMY_RUN_LEFT_3 = 16,
  SPRITE_ENEMY_RUN_LEFT_4 = 17,
  SPRITE_ENEMY_RUN_RIGHT_1 = 18,
  SPRITE_ENEMY_RUN_RIGHT_2 = 19,
  SPRITE_ENEMY_RUN_RIGHT_3 = 20,
  SPRITE_ENEMY_RUN_RIGHT_4 = 21,
  SPRITE_ENEMY_SKATE_RIGHT_1 = 22,
  SPRITE_ENEMY_SKATE_RIGHT_2 = 23,
  SPRITE_ENEMY_SKATE_LEFT_1 = 24,
  SPRITE_ENEMY_SKATE_LEFT_2 = 25,
  SPRITE_ENEMY_DRAGON_RIGHT_1 = 26,
  SPRITE_ENEMY_DRAGON_RIGHT_2 = 27,
  SPRITE_ENEMY_DRAGON_RIGHT_3 = 28,
  SPRITE_ENEMY_DRAGON_RIGHT_4 = 29,
  SPRITE_COIN_1 = 30,
  SPRITE_COIN_2 = 31,
  SPRITE_COIN_3 = 32,
s  SPRITE_COIN_4 = 33,
  SPRITE_COIN_5 = 34,
  SPRITE_COIN_6 = 35,
  SPRITE_COIN_7 = 36,
  SPRITE_COIN_8 = 37,
  SPRITE_COIN_9 = 38,
  SPRITE_COIN_10 = 39,
  SPRITE_COIN_11 = 40,
  SPRITE_COIN_12 = 41,
  SPRITE_COIN_13 = 42,
  SPRITE_COIN_14 = 43,
#ifdef GAME_JETPACK
  SPRITE_JETPACK = 44,
  SPRITE_CLIMBER_JETPACK_RUN_LEFT_1,
  SPRITE_CLIMBER_JETPACK_RUN_LEFT_2,
  SPRITE_CLIMBER_JETPACK_RUN_LEFT_3,
  SPRITE_CLIMBER_JETPACK_RUN_LEFT_4,
  SPRITE_CLIMBER_JETPACK_STAND_LEFT,
  SPRITE_CLIMBER_JETPACK_JUMP_LEFT,
  SPRITE_CLIMBER_JETPACK_RUN_RIGHT_1,
  SPRITE_CLIMBER_JETPACK_RUN_RIGHT_2,
  SPRITE_CLIMBER_JETPACK_RUN_RIGHT_3,
  SPRITE_CLIMBER_JETPACK_RUN_RIGHT_4,
  SPRITE_CLIMBER_JETPACK_STAND_RIGHT,
  SPRITE_CLIMBER_JETPACK_JUMP_RIGHT,
  SPRITE_CLIMBER_JETPACK_THRUST_LEFT_1,
  SPRITE_CLIMBER_JETPACK_THRUST_LEFT_2,
  SPRITE_CLIMBER_JETPACK_THRUST_RIGHT_1,
  SPRITE_CLIMBER_JETPACK_THRUST_RIGHT_2,
#endif
  SPRITE_JOYSTICK_1,
  SPRITE_JOYSTICK_2
} sprite_id_t;


#endif
