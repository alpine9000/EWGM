#include "game.h"

object_animation_t object_animations[] = {
  [OBJECT_ANIM_ENEMY_LEVEL1_1_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL1_1_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL1_1_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL1_1_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL1_1_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL1_1_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL1_1_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL1_1_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL1_1_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_PUNCH_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL1_1_PUNCH_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL1_1_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_PUNCH_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL1_1_PUNCH_LEFT_2,
    .stop = SPRITE_ENEMY_LEVEL1_1_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_PUNCH_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL1_1_PUNCH_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL1_1_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_PUNCH_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL1_1_PUNCH_RIGHT_2,
    .stop = SPRITE_ENEMY_LEVEL1_1_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL1_1_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL1_1_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_1_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL1_1_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL1_1_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================

  [OBJECT_ANIM_ENEMY_LEVEL2_1_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_1_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL2_1_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_1_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL2_1_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_1_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL2_1_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_1_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL2_1_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_PUNCH_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL2_1_PUNCH_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL2_1_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_PUNCH_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL2_1_PUNCH_LEFT_2,
    .stop = SPRITE_ENEMY_LEVEL2_1_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_PUNCH_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL2_1_PUNCH_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL2_1_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_PUNCH_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL2_1_PUNCH_RIGHT_2,
    .stop = SPRITE_ENEMY_LEVEL2_1_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_1_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL2_1_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_1_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_1_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL2_1_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_PLAYER1_RUN_RIGHT] = {
    .start =SPRITE_PLAYER1_RUN_RIGHT_1,
    .stop = SPRITE_PLAYER1_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_STAND_RIGHT] = {
    .start = SPRITE_PLAYER1_STAND_RIGHT,
    .stop = SPRITE_PLAYER1_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_RUN_LEFT] = {
    .start = SPRITE_PLAYER1_RUN_LEFT_1,
    .stop = SPRITE_PLAYER1_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_STAND_LEFT] = {
    .start = SPRITE_PLAYER1_STAND_LEFT,
    .stop = SPRITE_PLAYER1_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_LEFT1] = {
    .start =SPRITE_PLAYER1_PUNCH_LEFT_1,
    .stop = SPRITE_PLAYER1_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_LEFT2] = {
    .start = SPRITE_PLAYER1_PUNCH_LEFT_2,
    .stop = SPRITE_PLAYER1_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_RIGHT1] = {
    .start = SPRITE_PLAYER1_PUNCH_RIGHT_1,
    .stop = SPRITE_PLAYER1_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_RIGHT2] = {
    .start = SPRITE_PLAYER1_PUNCH_RIGHT_2,
    .stop = SPRITE_PLAYER1_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_HIT_LEFT] = {
    .start = SPRITE_PLAYER1_HIT_LEFT,
    .stop = SPRITE_PLAYER1_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_HIT_RIGHT] = {
    .start = SPRITE_PLAYER1_HIT_RIGHT,
    .stop = SPRITE_PLAYER1_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_KICK_LEFT] = {
    .start = SPRITE_PLAYER1_KICK_LEFT_1,
    .stop = SPRITE_PLAYER1_KICK_LEFT_3,
    .speed = 8,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_KICK_RIGHT] = {
    .start = SPRITE_PLAYER1_KICK_RIGHT_1,
    .stop = SPRITE_PLAYER1_KICK_RIGHT_3,
    .speed = 8,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_PLAYER2_RUN_RIGHT] = {
    .start = SPRITE_PLAYER2_RUN_RIGHT_1,
    .stop = SPRITE_PLAYER2_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_STAND_RIGHT] = {
    .start = SPRITE_PLAYER2_STAND_RIGHT,
    .stop = SPRITE_PLAYER2_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_RUN_LEFT] = {
    .start = SPRITE_PLAYER2_RUN_LEFT_1,
    .stop = SPRITE_PLAYER2_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_STAND_LEFT] = {
    .start = SPRITE_PLAYER2_STAND_LEFT,
    .stop = SPRITE_PLAYER2_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_PUNCH_LEFT1] = {
    .start = SPRITE_PLAYER2_PUNCH_LEFT_1,
    .stop = SPRITE_PLAYER2_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_PUNCH_LEFT2] = {
    .start = SPRITE_PLAYER2_PUNCH_LEFT_2,
    .stop = SPRITE_PLAYER2_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_PUNCH_RIGHT1] = {
    .start = SPRITE_PLAYER2_PUNCH_RIGHT_1,
    .stop = SPRITE_PLAYER2_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_PUNCH_RIGHT2] = {
    .start = SPRITE_PLAYER2_PUNCH_RIGHT_2,
    .stop = SPRITE_PLAYER2_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_HIT_LEFT] = {
    .start = SPRITE_PLAYER2_HIT_LEFT,
    .stop = SPRITE_PLAYER2_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_HIT_RIGHT] = {
    .start = SPRITE_PLAYER2_HIT_RIGHT,
    .stop = SPRITE_PLAYER2_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_KICK_LEFT] = {
    .start = SPRITE_PLAYER2_KICK_LEFT_1,
    .stop = SPRITE_PLAYER2_KICK_LEFT_3,
    .speed = 8,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_KICK_RIGHT] = {
    .start = SPRITE_PLAYER2_KICK_RIGHT_1,
    .stop = SPRITE_PLAYER2_KICK_RIGHT_3,
    .speed = 8,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_ENEMY_LEVEL1_2_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL1_2_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL1_2_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL1_2_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL1_2_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL1_2_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL1_2_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL1_2_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL1_2_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_PUNCH_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL1_2_PUNCH_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL1_2_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_PUNCH_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL1_2_PUNCH_LEFT_2,
    .stop = SPRITE_ENEMY_LEVEL1_2_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_PUNCH_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL1_2_PUNCH_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL1_2_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_PUNCH_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL1_2_PUNCH_RIGHT_2,
    .stop = SPRITE_ENEMY_LEVEL1_2_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL1_2_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL1_2_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL1_2_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL1_2_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL1_2_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_ENEMY_LEVEL2_2_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_2_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL2_2_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_2_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL2_2_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_2_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL2_2_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_2_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL2_2_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_PUNCH_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL2_2_PUNCH_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL2_2_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_PUNCH_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL2_2_PUNCH_LEFT_2,
    .stop = SPRITE_ENEMY_LEVEL2_2_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_PUNCH_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL2_2_PUNCH_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL2_2_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_PUNCH_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL2_2_PUNCH_RIGHT_2,
    .stop = SPRITE_ENEMY_LEVEL2_2_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_2_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL2_2_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_2_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_2_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL2_2_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },


  // =================================
  [OBJECT_ANIM_BOSS_RUN_RIGHT] = {
    .start = SPRITE_BOSS_RUN_RIGHT_1,
    .stop = SPRITE_BOSS_RUN_RIGHT_4,
    .speed = LEVEL1_BOSS_RUN_ANIM_SPEED,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_BOSS_STAND_RIGHT] = {
    .start = SPRITE_BOSS_STAND_RIGHT,
    .stop = SPRITE_BOSS_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_BOSS_RUN_LEFT] = {
    .start = SPRITE_BOSS_RUN_LEFT_1,
    .stop = SPRITE_BOSS_RUN_LEFT_4,
    .speed = LEVEL1_BOSS_RUN_ANIM_SPEED,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_BOSS_STAND_LEFT] = {
    .start = SPRITE_BOSS_STAND_LEFT,
    .stop = SPRITE_BOSS_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_BOSS_ATTACK_LEFT1] = {
    .start = SPRITE_BOSS_ATTACK_LEFT_1,
    .stop = SPRITE_BOSS_ATTACK_LEFT_3,
    .speed = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_BOSS_ATTACK_LEFT2] = {
    .start = SPRITE_BOSS_ATTACK_LEFT_1,
    .stop = SPRITE_BOSS_ATTACK_LEFT_3,
    .speed = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_BOSS_ATTACK_RIGHT1] = {
    .start = SPRITE_BOSS_ATTACK_RIGHT_1,
    .stop = SPRITE_BOSS_ATTACK_RIGHT_3,
    .speed = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_BOSS_ATTACK_RIGHT2] = {
    .start = SPRITE_BOSS_ATTACK_RIGHT_1,
    .stop = SPRITE_BOSS_ATTACK_RIGHT_3,
    .speed = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_BOSS_HIT_LEFT] = {
    .start = SPRITE_BOSS_HIT_LEFT,
    .stop = SPRITE_BOSS_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_BOSS_HIT_RIGHT] = {
    .start = SPRITE_BOSS_HIT_RIGHT,
    .stop = SPRITE_BOSS_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },


  // =================================
  [OBJECT_ANIM_LEVEL2_BOSS_RUN_RIGHT] = {
    .start = SPRITE_LEVEL2_BOSS_RUN_RIGHT_1,
    .stop = SPRITE_LEVEL2_BOSS_RUN_RIGHT_4,
    .speed = LEVEL2_BOSS_RUN_ANIM_SPEED,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_STAND_RIGHT] = {
    .start = SPRITE_LEVEL2_BOSS_STAND_RIGHT,
    .stop = SPRITE_LEVEL2_BOSS_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_RUN_LEFT] = {
    .start = SPRITE_LEVEL2_BOSS_RUN_LEFT_1,
    .stop = SPRITE_LEVEL2_BOSS_RUN_LEFT_4,
    .speed = LEVEL2_BOSS_RUN_ANIM_SPEED,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_STAND_LEFT] = {
    .start = SPRITE_LEVEL2_BOSS_STAND_LEFT,
    .stop = SPRITE_LEVEL2_BOSS_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_ATTACK_LEFT1] = {
    .start = SPRITE_LEVEL2_BOSS_ATTACK_LEFT_1,
    .stop = SPRITE_LEVEL2_BOSS_ATTACK_LEFT_4,
    .speed = LEVEL2_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_ATTACK_LEFT2] = {
    .start = SPRITE_LEVEL2_BOSS_ATTACK_LEFT_1,
    .stop = SPRITE_LEVEL2_BOSS_ATTACK_LEFT_4,
    .speed = LEVEL2_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_ATTACK_RIGHT1] = {
    .start = SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_1,
    .stop = SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_4,
    .speed = LEVEL2_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_ATTACK_RIGHT2] = {
    .start = SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_1,
    .stop = SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_4,
    .speed = LEVEL2_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_HIT_LEFT] = {
    .start = SPRITE_LEVEL2_BOSS_HIT_LEFT,
    .stop = SPRITE_LEVEL2_BOSS_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL2_BOSS_HIT_RIGHT] = {
    .start = SPRITE_LEVEL2_BOSS_HIT_RIGHT,
    .stop = SPRITE_LEVEL2_BOSS_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================


  [OBJECT_ANIM_ENEMY_LEVEL2_3_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_3_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL2_3_RUN_RIGHT_4,
    .speed = LEVEL2_3_ENEMY_RUN_ANIM_SPEED,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_3_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL2_3_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_3_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL2_3_RUN_LEFT_4,
    .speed = LEVEL2_3_ENEMY_RUN_ANIM_SPEED,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_3_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL2_3_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_ATTACK_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL2_3_ATTACK_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL2_3_ATTACK_LEFT_3,
    .speed = LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_ATTACK_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL2_3_ATTACK_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL2_3_ATTACK_LEFT_3,
    .speed = LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_ATTACK_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL2_3_ATTACK_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL2_3_ATTACK_RIGHT_3,
    .speed = LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_ATTACK_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL2_3_ATTACK_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL2_3_ATTACK_RIGHT_3,
    .speed = LEVEL2_3_ENEMY_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL2_3_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL2_3_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL2_3_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL2_3_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL2_3_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================

  [OBJECT_ANIM_LEVEL3_BOSS_RUN_RIGHT] = {
    .start = SPRITE_LEVEL3_BOSS_RUN_RIGHT_1,
    .stop = SPRITE_LEVEL3_BOSS_RUN_RIGHT_4,
    .speed = LEVEL3_BOSS_RUN_ANIM_SPEED,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_STAND_RIGHT] = {
    .start = SPRITE_LEVEL3_BOSS_STAND_RIGHT,
    .stop = SPRITE_LEVEL3_BOSS_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_RUN_LEFT] = {
    .start = SPRITE_LEVEL3_BOSS_RUN_LEFT_1,
    .stop = SPRITE_LEVEL3_BOSS_RUN_LEFT_4,
    .speed = LEVEL2_BOSS_RUN_ANIM_SPEED,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_STAND_LEFT] = {
    .start = SPRITE_LEVEL3_BOSS_STAND_LEFT,
    .stop = SPRITE_LEVEL3_BOSS_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_PUNCH_LEFT1] = {
    .start = SPRITE_LEVEL3_BOSS_ATTACK_LEFT_1,
    .stop = SPRITE_LEVEL3_BOSS_ATTACK_LEFT_1,
    .speed = LEVEL3_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_PUNCH_LEFT2] = {
    .start = SPRITE_LEVEL3_BOSS_ATTACK_LEFT_2,
    .stop = SPRITE_LEVEL3_BOSS_ATTACK_LEFT_2,
    .speed = LEVEL3_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_PUNCH_RIGHT1] = {
    .start = SPRITE_LEVEL3_BOSS_ATTACK_RIGHT_1,
    .stop = SPRITE_LEVEL3_BOSS_ATTACK_RIGHT_1,
    .speed = LEVEL3_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_PUNCH_RIGHT2] = {
    .start = SPRITE_LEVEL3_BOSS_ATTACK_RIGHT_2,
    .stop = SPRITE_LEVEL3_BOSS_ATTACK_RIGHT_2,
    .speed = LEVEL3_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_HIT_LEFT] = {
    .start = SPRITE_LEVEL3_BOSS_HIT_LEFT,
    .stop = SPRITE_LEVEL3_BOSS_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_LEVEL3_BOSS_HIT_RIGHT] = {
    .start = SPRITE_LEVEL3_BOSS_HIT_RIGHT,
    .stop = SPRITE_LEVEL3_BOSS_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================


  [OBJECT_ANIM_ENEMY_LEVEL3_1_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_1_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL3_1_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_1_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL3_1_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_1_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL3_1_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_1_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL3_1_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_PUNCH_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL3_1_PUNCH_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL3_1_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_PUNCH_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL3_1_PUNCH_LEFT_2,
    .stop = SPRITE_ENEMY_LEVEL3_1_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_PUNCH_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL3_1_PUNCH_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL3_1_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_PUNCH_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL3_1_PUNCH_RIGHT_2,
    .stop = SPRITE_ENEMY_LEVEL3_1_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_1_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL3_1_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_1_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_1_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL3_1_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================


  [OBJECT_ANIM_ENEMY_LEVEL3_2_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_2_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL3_2_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_2_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL3_2_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_2_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL3_2_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_2_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL3_2_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_PUNCH_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL3_2_PUNCH_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL3_2_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_PUNCH_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL3_2_PUNCH_LEFT_2,
    .stop = SPRITE_ENEMY_LEVEL3_2_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_PUNCH_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL3_2_PUNCH_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL3_2_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_PUNCH_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL3_2_PUNCH_RIGHT_2,
    .stop = SPRITE_ENEMY_LEVEL3_2_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_2_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL3_2_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_2_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_2_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL3_2_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================


  [OBJECT_ANIM_ENEMY_LEVEL3_3_RUN_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_3_RUN_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL3_3_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_3_STAND_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL3_3_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_RUN_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_3_RUN_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL3_3_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_STAND_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_3_STAND_LEFT,
    .stop = SPRITE_ENEMY_LEVEL3_3_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_PUNCH_LEFT1] = {
    .start = SPRITE_ENEMY_LEVEL3_3_PUNCH_LEFT_1,
    .stop = SPRITE_ENEMY_LEVEL3_3_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_PUNCH_LEFT2] = {
    .start = SPRITE_ENEMY_LEVEL3_3_PUNCH_LEFT_2,
    .stop = SPRITE_ENEMY_LEVEL3_3_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_PUNCH_RIGHT1] = {
    .start = SPRITE_ENEMY_LEVEL3_3_PUNCH_RIGHT_1,
    .stop = SPRITE_ENEMY_LEVEL3_3_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_PUNCH_RIGHT2] = {
    .start = SPRITE_ENEMY_LEVEL3_3_PUNCH_RIGHT_2,
    .stop = SPRITE_ENEMY_LEVEL3_3_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_HIT_LEFT] = {
    .start = SPRITE_ENEMY_LEVEL3_3_HIT_LEFT,
    .stop = SPRITE_ENEMY_LEVEL3_3_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_ENEMY_LEVEL3_3_HIT_RIGHT] = {
    .start = SPRITE_ENEMY_LEVEL3_3_HIT_RIGHT,
    .stop = SPRITE_ENEMY_LEVEL3_3_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },


  // =================================

  [OBJECT_ANIM_MOTORBIKE_RUN_RIGHT] = {
    .start = SPRITE_MOTORBIKE_RIDE_RIGHT_1,
    .stop = SPRITE_MOTORBIKE_RIDE_RIGHT_2,
    .speed = LEVEL1_MOTORBIKE_RIDE_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_MOTORBIKE_STAND_RIGHT] = {
    .start = SPRITE_MOTORBIKE_RIDE_RIGHT_1,
    .stop = SPRITE_MOTORBIKE_RIDE_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_MOTORBIKE_RUN_LEFT] = {
    .start = SPRITE_MOTORBIKE_RIDE_LEFT_1,
    .stop = SPRITE_MOTORBIKE_RIDE_LEFT_2,
    .speed = LEVEL1_MOTORBIKE_RIDE_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_MOTORBIKE_STAND_LEFT] = {
    .start = SPRITE_MOTORBIKE_RIDE_LEFT_1,
    .stop = SPRITE_MOTORBIKE_RIDE_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_MOTORBIKE_HIT_LEFT] = {
    .start = SPRITE_MOTORBIKE_HIT_LEFT,
    .stop = SPRITE_MOTORBIKE_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_MOTORBIKE_HIT_RIGHT] = {
    .start = SPRITE_MOTORBIKE_HIT_RIGHT,
    .stop = SPRITE_MOTORBIKE_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_HAND] = {
    .start = SPRITE_HAND,
    .stop = SPRITE_HAND,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_DOOR] = {
    .start = SPRITE_DOOR,
    .stop = SPRITE_DOOR,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_JOYSTICK] = {
    .start = SPRITE_JOYSTICK1,
    .stop = SPRITE_JOYSTICK2,
    .speed = 25,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_GAMEOVER] = {
    .start = SPRITE_GAMEOVER,
    .stop = SPRITE_GAMEOVER,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_GAMECOMPLETE] = {
    .start = SPRITE_GAMECOMPLETE,
    .stop = SPRITE_GAMECOMPLETE,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_DEATHMATCH] = {
    .start = SPRITE_DEATHMATCH,
    .stop = SPRITE_DEATHMATCH,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_LEVELCOMPLETE] = {
    .start = SPRITE_LEVELCOMPLETE,
    .stop = SPRITE_LEVELCOMPLETE,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_PHONEBOOTH] = {
    .start = SPRITE_PHONEBOOTH,
    .stop = SPRITE_PHONEBOOTH,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PHONEBOOTH_BROKEN] = {
    .start = SPRITE_PHONEBOOTH_BROKEN,
    .stop = SPRITE_PHONEBOOTH_BROKEN,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_PHONEBOOTH_JUNK1] = {
    .start = SPRITE_PHONEBOOTH_JUNK1,
    .stop = SPRITE_PHONEBOOTH_JUNK1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PHONEBOOTH_JUNK2] = {
    .start = SPRITE_PHONEBOOTH_JUNK2,
    .stop = SPRITE_PHONEBOOTH_JUNK2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PHONEBOOTH_JUNK3] = {
    .start = SPRITE_PHONEBOOTH_JUNK3,
    .stop = SPRITE_PHONEBOOTH_JUNK3,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_POSTBOX] = {
    .start = SPRITE_POSTBOX,
    .stop = SPRITE_POSTBOX,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_POSTBOX_BROKEN] = {
    .start = SPRITE_POSTBOX_BROKEN,
    .stop = SPRITE_POSTBOX_BROKEN,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_LOCKER1] = {
    .start = SPRITE_LOCKER1,
    .stop = SPRITE_LOCKER1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LOCKER1_BROKEN] = {
    .start = SPRITE_LOCKER1_BROKEN,
    .stop = SPRITE_LOCKER1_BROKEN,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LOCKER2] = {
    .start = SPRITE_LOCKER2,
    .stop = SPRITE_LOCKER2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LOCKER2_BROKEN] = {
    .start = SPRITE_LOCKER2_BROKEN,
    .stop = SPRITE_LOCKER2_BROKEN,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LOCKER3] = {
    .start = SPRITE_LOCKER3,
    .stop = SPRITE_LOCKER3,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_LOCKER3_BROKEN] = {
    .start = SPRITE_LOCKER3_BROKEN,
    .stop = SPRITE_LOCKER3_BROKEN,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_SIXPACK1] = {
    .start = SPRITE_SIXPACK1,
    .stop = SPRITE_SIXPACK1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_SIXPACK2] = {
    .start = SPRITE_SIXPACK2,
    .stop = SPRITE_SIXPACK2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_SIXPACK3] = {
    .start = SPRITE_SIXPACK3,
    .stop = SPRITE_SIXPACK3,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_TABLE] = {
    .start = SPRITE_TABLE,
    .stop = SPRITE_TABLE,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_CHAIR1] = {
    .start = SPRITE_CHAIR1,
    .stop = SPRITE_CHAIR1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_CHAIR2] = {
    .start = SPRITE_CHAIR2,
    .stop = SPRITE_CHAIR2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_POSTBOX_JUNK1] = {
    .start = SPRITE_POSTBOX_JUNK1,
    .stop = SPRITE_POSTBOX_JUNK1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_POSTBOX_JUNK2] = {
    .start = SPRITE_POSTBOX_JUNK2,
    .stop = SPRITE_POSTBOX_JUNK2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_POSTBOX_JUNK3] = {
    .start = SPRITE_POSTBOX_JUNK3,
    .stop = SPRITE_POSTBOX_JUNK3,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_BONUS_BURGER] = {
    .start = SPRITE_BONUS_BURGER,
    .stop = SPRITE_HEALTH_BURGER,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_BONUS_COLA] = {
    .start = SPRITE_BONUS_COLA,
    .stop = SPRITE_HEALTH_COLA,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_BONUS_WALLET] = {
    .start = SPRITE_BONUS_WALLET,
    .stop = SPRITE_POINTS_WALLET,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================

  [OBJECT_ANIM_BONUS_BEER] = {
    .start = SPRITE_BONUS_BEER,
    .stop = SPRITE_POINTS_BEER,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_STAR] = {
    .start = SPRITE_STAR,
    .stop = SPRITE_STAR,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================

  [OBJECT_ANIM_BULLET_RIGHT] = {
    .start = SPRITE_BULLET_RIGHT1,
    .stop = SPRITE_BULLET_RIGHT2,
    .speed = 8,
    .facing = FACING_RIGHT
  },

  [OBJECT_ANIM_BULLET_LEFT] = {
    .start = SPRITE_BULLET_LEFT1,
    .stop = SPRITE_BULLET_LEFT2,
    .speed = 8,
    .facing = FACING_LEFT
  },

  // =================================
  [OBJECT_ANIM_MOTORBIKE_BROKEN_LEFT] = {
    .start = SPRITE_MOTORBIKE_BROKEN_LEFT,
    .stop = SPRITE_MOTORBIKE_BROKEN_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },

  [OBJECT_ANIM_MOTORBIKE_BROKEN_RIGHT] = {
    .start = SPRITE_MOTORBIKE_BROKEN_RIGHT,
    .stop = SPRITE_MOTORBIKE_BROKEN_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_PUNCHING_BAG_STAND_RIGHT] = {
    .start = SPRITE_PUNCHING_BAG_MIDDLE,
    .stop = SPRITE_PUNCHING_BAG_MIDDLE,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PUNCHING_BAG_STAND_LEFT] = {
    .start = SPRITE_PUNCHING_BAG_MIDDLE,
    .stop = SPRITE_PUNCHING_BAG_MIDDLE,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PUNCHING_BAG_HIT_LEFT] = {
    .start = SPRITE_PUNCHING_BAG_LEFT,
    .stop = SPRITE_PUNCHING_BAG_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PUNCHING_BAG_HIT_RIGHT] = {
    .start = SPRITE_PUNCHING_BAG_RIGHT,
    .stop = SPRITE_PUNCHING_BAG_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PUNCHING_BAG_PUNCH_LEFT] = {
    .start = SPRITE_PUNCHING_BAG_RIGHT,
    .stop = SPRITE_PUNCHING_BAG_RIGHT2,
    .speed = 10,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PUNCHING_BAG_PUNCH_RIGHT] = {
    .start = SPRITE_PUNCHING_BAG_LEFT,
    .stop = SPRITE_PUNCHING_BAG_LEFT2,
    .speed = 10,
    .facing = FACING_RIGHT
  },

  // =================================
  [OBJECT_ANIM_POMMEL_HORSE] = {
    .start = SPRITE_POMMEL_HORSE,
    .stop = SPRITE_POMMEL_HORSE,
    .speed = 0,
    .facing = FACING_RIGHT
  },

};


object_image_t object_imageAtlas[] = {
  [SPRITE_ENEMY_LEVEL1_1_STAND_RIGHT] = {
    .x = 0,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_RIGHT_1] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_RIGHT_2] = {
    .x = 64,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_RIGHT_3] = {
    .x = 96,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = -2,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_RIGHT_4] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_1_STAND_LEFT] = {
    .x = 160,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_LEFT_1] = {
    .x = 128,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_LEFT_2] = {
    .x = 96,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_LEFT_3] = {
    .x = 64,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 2,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_RUN_LEFT_4] = {
    .x = 96,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_1_PUNCH_LEFT_1] = {
    .x = 192,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = -3,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_1_PUNCH_LEFT_2] = {
    .x = 192,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = -2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_1_PUNCH_RIGHT_1] = {
    .x = 128,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_1_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 3,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL1_1_HIT_RIGHT] = {
    .x = 0,
    .y = 52,
    .w = 48,
    .h = 52,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_1_HIT_LEFT] = {
    .x = 224,
    .y = 731,
    .w = 48,
    .h = 52,
    .dx = -2,
    .dy = 0,
    },
  //==============================
  [SPRITE_ENEMY_LEVEL2_1_STAND_RIGHT] = {
    .x = 0,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_RIGHT_1] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_RIGHT_2] = {
    .x = 64,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_RIGHT_3] = {
    .x = 96,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = -5,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_RIGHT_4] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_1_STAND_LEFT] = {
    .x = 160,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_LEFT_1] = {
    .x = 128,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_LEFT_2] = {
    .x = 96,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_LEFT_3] = {
    .x = 64,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 5,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_RUN_LEFT_4] = {
    .x = 96,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_1_PUNCH_LEFT_1] = {
    .x = 192,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = -1,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_1_PUNCH_LEFT_2] = {
    .x = 192,
    .y = 52,
    .w = 32,
    .h = 52,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_1_PUNCH_RIGHT_1] = {
    .x = 128,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_1_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 0,
    .w = 32,
    .h = 52,
    .dx = 1,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL2_1_HIT_RIGHT] = {
    .x = 0,
    .y = 52,
    .w = 48,
    .h = 52,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_1_HIT_LEFT] = {
    .x = 224,
    .y = 731,
    .w = 48,
    .h = 52,
    .dx = 0,
    .dy = 0,
    },
  //==============================
  [SPRITE_PLAYER1_STAND_RIGHT] = {
    .x = 0,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_PLAYER1_RUN_RIGHT_1] = {
    .x = 32,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,
  },
  [SPRITE_PLAYER1_RUN_RIGHT_2] = {
    .x = 64,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,
  },
  [SPRITE_PLAYER1_RUN_RIGHT_3] = {
    .x = 96,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,
  },
    [SPRITE_PLAYER1_RUN_RIGHT_4] = {
    .x = 64,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_PLAYER1_STAND_LEFT] = {
    .x = 192,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_PLAYER1_RUN_LEFT_1] = {
    .x = 160,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,
  },
  [SPRITE_PLAYER1_RUN_LEFT_2] = {
    .x = 128,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,
  },
  [SPRITE_PLAYER1_RUN_LEFT_3] = {
    .x = 96,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 2,
    .dy = 0,
  },

  [SPRITE_PLAYER1_RUN_LEFT_4] = {
    .x = 128,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,
    },

  [SPRITE_PLAYER1_PUNCH_LEFT_1] = {
    .x = 224,
    .y = 162,
    .w = 48,
    .h = 53,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_PLAYER1_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -3,
    .dy = 0,
    },
  [SPRITE_PLAYER1_PUNCH_RIGHT_1] = {
    .x = 160,
    .y = 108,
    .w = 48,
    .h = 53,
    .dx = 3,
    .dy = 0,
    },
  [SPRITE_PLAYER1_PUNCH_RIGHT_2] = {
    .x = 128,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 3,
    .dy = 0,
    },
  [SPRITE_PLAYER1_HIT_LEFT] = {
    .x = 0,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PLAYER1_HIT_RIGHT] = {
    .x = 32,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_PLAYER1_KICK_LEFT_1] = {
    .x = 80,
    .y = 731,
    .w = 32,
    .h = 55,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PLAYER1_KICK_LEFT_2] = {
    .x = 192,
    .y = 619,
    .w = 48,
    .h = 55,
    .dx = -6,
    .dy = 0,
    },
  [SPRITE_PLAYER1_KICK_LEFT_3] = {
    .x = 80,
    .y = 731,
    .w = 32,
    .h = 55,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_PLAYER1_KICK_RIGHT_1] = {
    .x = 0,
    .y = 731,
    .w = 32,
    .h = 55,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PLAYER1_KICK_RIGHT_2] = {
    .x = 32,
    .y = 731,
    .w = 48,
    .h = 55,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PLAYER1_KICK_RIGHT_3] = {
    .x = 0,
    .y = 731,
    .w = 32,
    .h = 55,
    .dx = 0,
    .dy = 0,
    },

  //==============================
  [SPRITE_PLAYER2_STAND_RIGHT] = {
    .x = 0,
    .y = 215,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_PLAYER2_RUN_RIGHT_1] = {
    .x = 32,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_PLAYER2_RUN_RIGHT_2] = {
    .x = 80,
    .y = 215,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_PLAYER2_RUN_RIGHT_3] = {
    .x = 112,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = -5,
    .dy = 0,
  },
    [SPRITE_PLAYER2_RUN_RIGHT_4] = {
    .x = 80,
    .y = 215,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PLAYER2_STAND_LEFT] = {
    .x = 240,
    .y = 275,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_PLAYER2_RUN_LEFT_1] = {
    .x = 192,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -6,
    .dy = 0,
  },
  [SPRITE_PLAYER2_RUN_LEFT_2] = {
    .x = 160,
    .y = 275,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_PLAYER2_RUN_LEFT_3] = {
    .x = 112,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -3,
    .dy = 0,
  },
    [SPRITE_PLAYER2_RUN_LEFT_4] = {
    .x = 160,
    .y = 275,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_PLAYER2_PUNCH_LEFT_1] = {
    .x = 16,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -19,
    .dy = 0,
    },
  [SPRITE_PLAYER2_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -19,
    .dy = 0,
    },
  [SPRITE_PLAYER2_PUNCH_RIGHT_1] = {
    .x = 208,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_PLAYER2_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_PLAYER2_HIT_LEFT] = {
    .x = 32,
    .y = 335,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PLAYER2_HIT_RIGHT] = {
    .x = 0,
    .y = 335,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_PLAYER2_KICK_LEFT_1] = {
    .x = 240,
    .y = 560,
    .w = 32,
    .h = 60,
    .dx = -2,
    .dy = 0,
    },
  [SPRITE_PLAYER2_KICK_LEFT_2] = {
    .x = 208,
    .y = 911,
    .w = 48,
    .h = 60,
    .dx = -8,
    .dy = 0,
    },
  [SPRITE_PLAYER2_KICK_LEFT_3] = {
    .x = 240,
    .y = 560,
    .w = 32,
    .h = 60,
    .dx = -2,
    .dy = 0,
    },

  [SPRITE_PLAYER2_KICK_RIGHT_1] = {
    .x = 240,
    .y = 621,
    .w = 32,
    .h = 60,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_PLAYER2_KICK_RIGHT_2] = {
    .x = 160,
    .y = 911,
    .w = 48,
    .h = 60,
    .dx = -3,
    .dy = 0,
    },
  [SPRITE_PLAYER2_KICK_RIGHT_3] = {
    .x = 240,
    .y = 621,
    .w = 32,
    .h = 60,
    .dx = 1,
    .dy = 0,
    },

  //==============================
  [SPRITE_ENEMY_LEVEL1_2_STAND_RIGHT] = {
    .x = 0,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_2_RUN_RIGHT_1] = {
    .x = 48,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_2_RUN_RIGHT_2] = {
    .x = 96,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_2_RUN_RIGHT_3] = {
    .x = 144,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL1_2_RUN_RIGHT_4] = {
    .x = 96,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_2_STAND_LEFT] = {
    .x = 192,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_2_RUN_LEFT_1] = {
    .x = 144,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_2_RUN_LEFT_2] = {
    .x = 96,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL1_2_RUN_LEFT_3] = {
    .x = 48,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL1_2_RUN_LEFT_4] = {
    .x = 96,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL1_2_PUNCH_LEFT_1] = {
    .x = 144,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = -3,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_2_PUNCH_LEFT_2] = {
    .x = 0,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = -2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_2_PUNCH_RIGHT_1] = {
    .x = 192,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_2_PUNCH_RIGHT_2] = {
    .x = 0,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_2_HIT_LEFT] = {
    .x = 96,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL1_2_HIT_RIGHT] = {
    .x = 48,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },


  //==============================
  [SPRITE_ENEMY_LEVEL2_2_STAND_RIGHT] = {
    .x = 0,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_2_RUN_RIGHT_1] = {
    .x = 48,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_2_RUN_RIGHT_2] = {
    .x = 96,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_2_RUN_RIGHT_3] = {
    .x = 144,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL2_2_RUN_RIGHT_4] = {
    .x = 96,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_2_STAND_LEFT] = {
    .x = 192,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_2_RUN_LEFT_1] = {
    .x = 144,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_2_RUN_LEFT_2] = {
    .x = 96,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_2_RUN_LEFT_3] = {
    .x = 48,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL2_2_RUN_LEFT_4] = {
    .x = 96,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL2_2_PUNCH_LEFT_1] = {
    .x = 144,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = -3,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_2_PUNCH_LEFT_2] = {
    .x = 0,
    .y = 675,
    .w = 48,
    .h = 56,
    .dx = -2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_2_PUNCH_RIGHT_1] = {
    .x = 192,
    .y = 563,
    .w = 48,
    .h = 56,
    .dx = 2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_2_PUNCH_RIGHT_2] = {
    .x = 0,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_2_HIT_LEFT] = {
    .x = 96,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_2_HIT_RIGHT] = {
    .x = 48,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },


  //==============================
  [SPRITE_BOSS_STAND_RIGHT] = {
    .x = 225,
    .y = 0,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_BOSS_RUN_RIGHT_1] = {
    .x = 64,
    .y = 336,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_BOSS_RUN_RIGHT_2] = {
    .x = 160,
    .y = 336,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_BOSS_RUN_RIGHT_3] = {
    .x = 192,
    .y = 395,
    .w = 48,
    .h = 56,
    .dx = -1,
    .dy = 0,
  },
    [SPRITE_BOSS_RUN_RIGHT_4] = {
    .x = 160,
    .y = 336,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_BOSS_STAND_LEFT] = {
    .x = 224,
    .y = 56,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_BOSS_RUN_LEFT_1] = {
    .x = 112,
    .y = 336,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_BOSS_RUN_LEFT_2] = {
    .x = 48,
    .y = 507,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_BOSS_RUN_LEFT_3] = {
    .x = 0,
    .y = 451,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_BOSS_RUN_LEFT_4] = {
    .x = 48,
    .y = 507,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_BOSS_ATTACK_LEFT_1] = {
    .x = 192,
    .y = 451,
    .w = 48,
    .h = 56,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_BOSS_ATTACK_LEFT_2] = {
    .x = 144,
    .y = 451,
    .w = 48,
    .h = 56,
    .dx = -11,
    .dy = 0,
    },
  [SPRITE_BOSS_ATTACK_LEFT_3] = {
    .x = 48,
    .y = 451,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_BOSS_ATTACK_RIGHT_1] = {
    .x = 0,
    .y = 395,
    .w = 48,
    .h = 56,
    .dx = -12,
    .dy = 0,
    },
  [SPRITE_BOSS_ATTACK_RIGHT_2] = {
    .x = 48,
    .y = 395,
    .w = 48,
    .h = 56,
    .dx = -1,
    .dy = 0,
    },
  [SPRITE_BOSS_ATTACK_RIGHT_3] = {
    .x = 144,
    .y = 395,
    .w = 48,
    .h = 56,
    .dx = -1,
    .dy = 0,
    },
  [SPRITE_BOSS_HIT_LEFT] = {
    .x = 96,
    .y = 451,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_BOSS_HIT_RIGHT] = {
    .x = 96,
    .y = 395,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  //==============================
  [SPRITE_LEVEL2_BOSS_STAND_RIGHT] = {
    .x = 0,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL2_BOSS_RUN_RIGHT_1] = {
    .x = 48,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL2_BOSS_RUN_RIGHT_2] = {
    .x = 96,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL2_BOSS_RUN_RIGHT_3] = {
    .x = 144,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = -1,
    .dy = 0,
  },
    [SPRITE_LEVEL2_BOSS_RUN_RIGHT_4] = {
    .x = 96,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LEVEL2_BOSS_STAND_LEFT] = {
    .x = 192,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL2_BOSS_RUN_LEFT_1] = {
    .x = 144,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL2_BOSS_RUN_LEFT_2] = {
    .x = 96,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL2_BOSS_RUN_LEFT_3] = {
    .x = 48,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_LEVEL2_BOSS_RUN_LEFT_4] = {
    .x = 96,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_LEVEL2_BOSS_ATTACK_LEFT_1] = {
    .x = 0,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LEVEL2_BOSS_ATTACK_LEFT_2] = {
    .x = 48,
    .y = 917,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LEVEL2_BOSS_ATTACK_LEFT_3] = {
    .x = 0,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_LEVEL2_BOSS_ATTACK_LEFT_4] = {
    .x = 0,
    .y = 842,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_1] = {
    .x = 192,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_2] = {
    .x = 0,
    .y = 917,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_3] = {
    .x = 192,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_LEVEL2_BOSS_ATTACK_RIGHT_4] = {
    .x = 192,
    .y = 786,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_LEVEL2_BOSS_HIT_LEFT] = {
    .x = 48,
    .y = 507,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LEVEL2_BOSS_HIT_RIGHT] = {
    .x = 192,
    .y = 336,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,
    },

  //==============================
  [SPRITE_ENEMY_LEVEL2_3_STAND_RIGHT] = {
    .x = 225,
    .y = 6,
    .w = 32,
    .h = 50,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_3_RUN_RIGHT_1] = {
    .x = 64,
    .y = 342,
    .w = 48,
    .h = 50,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_3_RUN_RIGHT_2] = {
    .x = 160,
    .y = 342,
    .w = 32,
    .h = 50,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_3_RUN_RIGHT_3] = {
    .x = 192,
    .y = 401,
    .w = 48,
    .h = 50,
    .dx = -1,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL2_3_RUN_RIGHT_4] = {
    .x = 160,
    .y = 342,
    .w = 32,
    .h = 50,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_STAND_LEFT] = {
    .x = 224,
    .y = 62,
    .w = 48,
    .h = 50,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_3_RUN_LEFT_1] = {
    .x = 112,
    .y = 342,
    .w = 32,
    .h = 50,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_3_RUN_LEFT_2] = {
    .x = 240,
    .y = 342,
    .w = 32,
    .h = 50,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL2_3_RUN_LEFT_3] = {
    .x = 0,
    .y = 457,
    .w = 48,
    .h = 50,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL2_3_RUN_LEFT_4] = {
    .x = 240,
    .y = 342,
    .w = 32,
    .h = 50,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL2_3_ATTACK_LEFT_1] = {
    .x = 192,
    .y = 457,
    .w = 48,
    .h = 50,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_ATTACK_LEFT_2] = {
    .x = 144,
    .y = 457,
    .w = 48,
    .h = 50,
    .dx = -11,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_ATTACK_LEFT_3] = {
    .x = 48,
    .y = 457,
    .w = 48,
    .h = 50,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_ATTACK_RIGHT_1] = {
    .x = 0,
    .y = 401,
    .w = 48,
    .h = 50,
    .dx = -12,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_ATTACK_RIGHT_2] = {
    .x = 48,
    .y = 401,
    .w = 48,
    .h = 50,
    .dx = -1,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_ATTACK_RIGHT_3] = {
    .x = 144,
    .y = 401,
    .w = 48,
    .h = 50,
    .dx = -1,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_HIT_LEFT] = {
    .x = 96,
    .y = 457,
    .w = 48,
    .h = 50,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL2_3_HIT_RIGHT] = {
    .x = 96,
    .y = 401,
    .w = 48,
    .h = 50,
    .dx = 0,
    .dy = 0,
    },


  //===========================

  [SPRITE_MOTORBIKE_RIDE_RIGHT_1] = {
    .x = 0,
    .y = 787,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_MOTORBIKE_RIDE_RIGHT_2] = {
    .x = 80,
    .y = 787,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_MOTORBIKE_HIT_RIGHT] = {
    .x = 0,
    .y = 849,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_MOTORBIKE_BROKEN_RIGHT] = {
    .x = 160,
    .y = 787,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_MOTORBIKE_RIDE_LEFT_1] = {
    .x = 80,
    .y = 849,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_MOTORBIKE_RIDE_LEFT_2] = {
    .x = 160,
    .y = 849,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_MOTORBIKE_HIT_LEFT] = {
    .x = 80,
    .y = 911,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_MOTORBIKE_BROKEN_LEFT] = {
    .x = 0,
    .y = 911,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,
  },


  //==========================
  [SPRITE_HAND] = {
    .x = 208,
    .y = 112,
    .w = 64,
    .h = 50,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_DOOR] = {
    .x = 0,
    .y = 0,
    .w = 64,
    .h = 64,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_JOYSTICK1] = {
    .x = 0,
    .y = 507,
    .w = 32,
    .h = 32,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_JOYSTICK2] = {
    .x = 144,
    .y = 507,
    .w = 32,
    .h = 32,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_GAMEOVER] = {
    .x = 112,
    .y = 736,
    .w = 80,
    .h = 8,
    .dx = 0,
    .dy = 0,
    },

    [SPRITE_GAMECOMPLETE] = {
    .x = 112,
    .y = 744,
    .w = 110,
    .h = 8,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_DEATHMATCH] = {
    .x = 112,
    .y = 752,
    .w = 96,
    .h = 8,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_LEVELCOMPLETE] = {
    .x = 144,
    .y = 545,
    .w = 119,
    .h = 8,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_PHONEBOOTH] = {
    .x = 240,
    .y = 335,
    .w = 32,
    .h = 80,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PHONEBOOTH_BROKEN] = {
    .x = 240,
    .y = 416,
    .w = 32,
    .h = 65,
    .dx = 0,
    .dy = 0,
    },
  //==========================
  [SPRITE_PHONEBOOTH_JUNK1] = {
    .x = 128,
    .y = 514,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PHONEBOOTH_JUNK2] = {
    .x = 128,
    .y = 530,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PHONEBOOTH_JUNK3] = {
    .x = 128,
    .y = 546,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_POSTBOX] = {
    .x = 240,
    .y = 481,
    .w = 32,
    .h = 64,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_POSTBOX_BROKEN] = {
    .x = 208,
    .y = 335,
    .w = 32,
    .h = 37,
    .dx = 0,
    .dy = 0,
    },
  //==========================
  [SPRITE_LOCKER1] = {
    .x = 0,
    .y = 848,
    .w = 16,
    .h = 48,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LOCKER1_BROKEN] = {
    .x = 48,
    .y = 848,
    .w = 16,
    .h = 48,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LOCKER2] = {
    .x = 16,
    .y = 848,
    .w = 16,
    .h = 48,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LOCKER2_BROKEN] = {
    .x = 64,
    .y = 848,
    .w = 16,
    .h = 48,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LOCKER3] = {
    .x = 32,
    .y = 848,
    .w = 16,
    .h = 48,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LOCKER3_BROKEN] = {
    .x = 80,
    .y = 848,
    .w = 16,
    .h = 48,
    .dx = 0,
    .dy = 0,
    },
  //==========================
    [SPRITE_SIXPACK1] = {
    .x = 240,
    .y = 400,
    .w = 27,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
    [SPRITE_SIXPACK2] = {
    .x = 240,
    .y = 416,
    .w = 27,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
    [SPRITE_SIXPACK3] = {
    .x = 240,
    .y = 432,
    .w = 27,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
  //==========================
  [SPRITE_POSTBOX_JUNK1] = {
    .x = 96,
    .y = 528,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_POSTBOX_JUNK2] = {
    .x = 96,
    .y = 544,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_POSTBOX_JUNK3] = {
    .x = 112,
    .y = 528,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  //==========================
    [SPRITE_TABLE] = {
    .x = 96,
    .y = 944,
    .w = 55,
    .h = 29,
    .dx = 0,
    .dy = 0,
    },
    [SPRITE_CHAIR1] = {
    .x = 240,
    .y = 481,
    .w = 23,
    .h = 32,
    .dx = 0,
    .dy = 0,
    },
    [SPRITE_CHAIR2] = {
    .x = 240,
    .y = 513,
    .w = 23,
    .h = 32,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_BONUS_BURGER] = {
    .x = 96,
    .y = 512,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_BONUS_COLA] = {
    .x = 112,
    .y = 512,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_HEALTH_BURGER] = {
    .x = 192,
    .y = 528,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_HEALTH_COLA] = {
    .x = 192,
    .y = 528,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_BONUS_WALLET] = {
    .x = 224,
    .y = 528,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_POINTS_WALLET] = {
    .x = 208,
    .y = 528,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_BONUS_BEER] = {
    .x = 208,
    .y = 512,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_POINTS_BEER] = {
    .x = 208,
    .y = 528,
    .w = 16,
    .h = 16,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_STAR] = {
    .x = 112,
    .y = 547,
    .w = 16,
    .h = 3,
    .dx = 0,
    .dy = 0,
    },

  //==========================
  [SPRITE_BULLET_RIGHT1] = {
    .x = 192,
    .y = 512,
    .w = 16,
    .h = 4,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_BULLET_RIGHT2] = {
    .x = 192,
    .y = 516,
    .w = 16,
    .h = 4,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_BULLET_LEFT1] = {
    .x = 192,
    .y = 520,
    .w = 16,
    .h = 4,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_BULLET_LEFT2] = {
    .x = 192,
    .y = 524,
    .w = 16,
    .h = 4,
    .dx = 0,
    .dy = 0,
    },

  //==========================

  [SPRITE_LEVEL3_BOSS_STAND_RIGHT] = {
    .x = 64,
    .y = 336,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL3_BOSS_RUN_RIGHT_1] = {
    .x = 96,
    .y = 336,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL3_BOSS_RUN_RIGHT_2] = {
    .x = 128,
    .y = 336,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL3_BOSS_RUN_RIGHT_3] = {
    .x = 160,
    .y = 336,
    .w = 32,
    .h = 53,
    .dx = -5,
    .dy = 0,
  },
    [SPRITE_LEVEL3_BOSS_RUN_RIGHT_4] = {
    .x = 128,
    .y = 336,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_LEVEL3_BOSS_STAND_LEFT] = {
    .x = 192,
    .y = 395,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL3_BOSS_RUN_LEFT_1] = {
    .x = 192,
    .y = 448,
    .w = 32,
    .h = 53,
    .dx = -5,
    .dy = 0,
  },
  [SPRITE_LEVEL3_BOSS_RUN_LEFT_2] = {
    .x = 160,
    .y = 395,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_LEVEL3_BOSS_RUN_LEFT_3] = {
    .x = 160,
    .y = 448,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,
  },
    [SPRITE_LEVEL3_BOSS_RUN_LEFT_4] = {
    .x = 160,
    .y = 395,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_LEVEL3_BOSS_ATTACK_LEFT_1] = {
    .x = 96,
    .y = 448,
    .w = 32,
    .h = 53,
    .dx = -8,
    .dy = 0,
    },
  [SPRITE_LEVEL3_BOSS_ATTACK_LEFT_2] = {
    .x = 128,
    .y = 448,
    .w = 32,
    .h = 53,
    .dx = -7,
    .dy = 0,
    },
  [SPRITE_LEVEL3_BOSS_MISSILE_LEFT_1] = {
    .x = 0,
    .y = 448,
    .w = 48,
    .h = 53,
    .dx = -10,
    .dy = 0,
    },

  [SPRITE_LEVEL3_BOSS_ATTACK_RIGHT_1] = {
    .x = 32,
    .y = 395,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_LEVEL3_BOSS_ATTACK_RIGHT_2] = {
    .x = 0,
    .y = 395,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,
    },
  [SPRITE_LEVEL3_BOSS_MISSILE_RIGHT_1] = {
    .x = 112,
    .y = 395,
    .w = 48,
    .h = 56,
    .dx = 1,
    .dy = 0,
    },

  [SPRITE_LEVEL3_BOSS_HIT_RIGHT] = {
    .x = 64,
    .y = 395,
    .w = 48,
    .h = 53,
    .dx = -7,
    .dy = 0,
    },
  [SPRITE_LEVEL3_BOSS_HIT_LEFT] = {
    .x = 48,
    .y = 448,
    .w = 48,
    .h = 53,
    .dx = -6,
    .dy = 0,
    },

  //=================================

  [SPRITE_ENEMY_LEVEL3_1_STAND_RIGHT] = {
    .x = 0,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_RIGHT_1] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_RIGHT_2] = {
    .x = 64,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_RIGHT_3] = {
    .x = 96,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_RIGHT_4] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_1_STAND_LEFT] = {
    .x = 192,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_LEFT_1] = {
    .x = 160,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_LEFT_2] = {
    .x = 128,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_LEFT_3] = {
    .x = 96,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_RUN_LEFT_4] = {
    .x = 128,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_1_PUNCH_LEFT_1] = {
    .x = 32,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_1_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -3,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_1_PUNCH_RIGHT_1] = {
    .x = 128,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 2,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_1_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 3,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL3_1_HIT_RIGHT] = {
    .x = 0,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_1_HIT_LEFT] = {
    .x = 192,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },

  //==============================
  [SPRITE_ENEMY_LEVEL3_2_STAND_RIGHT] = {
    .x = 0,
    .y = 563,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_2_RUN_RIGHT_1] = {
    .x = 64,
    .y = 563,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_2_RUN_RIGHT_2] = {
    .x = 32,
    .y = 563,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_2_RUN_RIGHT_3] = {
    .x = 96,
    .y = 563,
    .w = 32,
    .h = 53,
    .dx = -4,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL3_2_RUN_RIGHT_4] = {
    .x = 32,
    .y = 563,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_2_STAND_LEFT] = {
    .x = 160,
    .y = 616,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_2_RUN_LEFT_1] = {
    .x = 96,
    .y = 616,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_2_RUN_LEFT_2] = {
    .x = 128,
    .y = 616,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_2_RUN_LEFT_3] = {
    .x = 64,
    .y = 616,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL3_2_RUN_LEFT_4] = {
    .x = 128,
    .y = 616,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL3_2_PUNCH_LEFT_1] = {
    .x = 0,
    .y = 616,
    .w = 32,
    .h = 53,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_2_PUNCH_LEFT_2] = {
    .x = 48,
    .y = 502,
    .w = 48,
    .h = 53,
    .dx = -5,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_2_PUNCH_RIGHT_1] = {
    .x = 128,
    .y = 563,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_2_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 563,
    .w = 48,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_2_HIT_RIGHT] = {
    .x = 208,
    .y = 563,
    .w = 32,
    .h = 54,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_2_HIT_LEFT] = {
    .x = 208,
    .y = 676,
    .w = 32,
    .h = 54,
    .dx = 0,
    .dy = 0,
    },

  //==============================
  [SPRITE_ENEMY_LEVEL3_3_STAND_RIGHT] = {
    .x = 0,
    .y = 669,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_3_RUN_RIGHT_1] = {
    .x = 64,
    .y = 669,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_3_RUN_RIGHT_2] = {
    .x = 32,
    .y = 669,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_3_RUN_RIGHT_3] = {
    .x = 96,
    .y = 669,
    .w = 32,
    .h = 53,
    .dx = -4,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL3_3_RUN_RIGHT_4] = {
    .x = 32,
    .y = 669,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_3_STAND_LEFT] = {
    .x = 176,
    .y = 787,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_3_RUN_LEFT_1] = {
    .x = 112,
    .y = 787,
    .w = 32,
    .h = 53,
    .dx = -4,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_3_RUN_LEFT_2] = {
    .x = 144,
    .y = 787,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
  },
  [SPRITE_ENEMY_LEVEL3_3_RUN_LEFT_3] = {
    .x = 80,
    .y = 787,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,
  },
    [SPRITE_ENEMY_LEVEL3_3_RUN_LEFT_4] = {
    .x = 144,
    .y = 787,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },

  [SPRITE_ENEMY_LEVEL3_3_PUNCH_LEFT_1] = {
    .x = 48,
    .y = 787,
    .w = 32,
    .h = 53,
    .dx = -6,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_3_PUNCH_LEFT_2] = {
    .x = 0,
    .y = 787,
    .w = 32,
    .h = 53,
    .dx = -7,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_3_PUNCH_RIGHT_1] = {
    .x = 128,
    .y = 669,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_3_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 670,
    .w = 48,
    .h = 53,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_3_HIT_LEFT] = {
    .x = 208,
    .y = 787,
    .w = 48,
    .h = 54,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_ENEMY_LEVEL3_3_HIT_RIGHT] = {
    .x = 224,
    .y = 731,
    .w = 48,
    .h = 54,
    .dx = -3,
    .dy = 0,
    },

  //==============================
  [SPRITE_PUNCHING_BAG_MIDDLE] = {
    .x = 144,
    .y = 849,
    .w = 16,
    .h = 32,
    .dx = 0,
    .dy = 0,
    },
  [SPRITE_PUNCHING_BAG_LEFT] = {
    .x = 160,
    .y = 849,
    .w = 32,
    .h = 32,
    .dx = 3,
    .dy = 0,
    },
  [SPRITE_PUNCHING_BAG_RIGHT] = {
    .x = 192,
    .y = 849,
    .w = 32,
    .h = 32,
    .dx = -4,
    .dy = 0,
    },
  [SPRITE_PUNCHING_BAG_LEFT2] = {
    .x = 160,
    .y = 849,
    .w = 32,
    .h = 32,
    .dx = 3,
    .dy = 0,
    },
  [SPRITE_PUNCHING_BAG_RIGHT2] = {
    .x = 192,
    .y = 849,
    .w = 32,
    .h = 32,
    .dx = -4,
    .dy = 0,
    },

  //==============================
  [SPRITE_POMMEL_HORSE] = {
    .x = 96,
    .y = 848,
    .w = 32,
    .h = 26,
    .dx = 0,
    .dy = 0,
    },
};
