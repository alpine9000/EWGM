#include "game.h"

#ifdef PLAYER_HARDWARE_SPRITE

#include "out/sprite_playerLeftRun0_0-aprite.h"

#include "out/sprite_playerLeftRun1_0-aprite.h"

#include "out/sprite_playerLeftRun0_1-aprite.h"

#include "out/sprite_playerLeftRun1_1-aprite.h"

#include "out/sprite_playerLeftRun0_2-aprite.h"

#include "out/sprite_playerLeftRun1_2-aprite.h"

#include "out/sprite_playerLeftRun0_3-aprite.h"

#include "out/sprite_playerLeftRun1_3-aprite.h"

#include "out/sprite_playerRightRun0_0-aprite.h"

#include "out/sprite_playerRightRun1_0-aprite.h"

#include "out/sprite_playerRightRun0_1-aprite.h"

#include "out/sprite_playerRightRun1_1-aprite.h"

#include "out/sprite_playerRightRun0_2-aprite.h"

#include "out/sprite_playerRightRun1_2-aprite.h"

#include "out/sprite_playerRightRun0_3-aprite.h"

#include "out/sprite_playerRightRun1_3-aprite.h"

#include "out/sprite_playerLeftJump0_0-aprite.h"

#include "out/sprite_playerLeftJump1_0-aprite.h"

#include "out/sprite_playerRightJump0_0-aprite.h"

#include "out/sprite_playerRightJump1_0-aprite.h"

#include "out/sprite_playerLeftStand0_0-aprite.h"

#include "out/sprite_playerLeftStand1_0-aprite.h"

#include "out/sprite_playerRightStand0_0-aprite.h"

#include "out/sprite_playerRightStand1_0-aprite.h"


#ifdef GAME_JETPACK
#include "out/sprite_playerJetpackLeftRun0_0-aprite.h"
#include "out/sprite_playerJetpackLeftRun1_0-aprite.h"
#include "out/sprite_playerJetpackLeftRun0_1-aprite.h"
#include "out/sprite_playerJetpackLeftRun1_1-aprite.h"
#include "out/sprite_playerJetpackLeftRun0_2-aprite.h"
#include "out/sprite_playerJetpackLeftRun1_2-aprite.h"
#include "out/sprite_playerJetpackLeftRun0_3-aprite.h"
#include "out/sprite_playerJetpackLeftRun1_3-aprite.h"
#include "out/sprite_playerJetpackRightRun0_0-aprite.h"
#include "out/sprite_playerJetpackRightRun1_0-aprite.h"
#include "out/sprite_playerJetpackRightRun0_1-aprite.h"
#include "out/sprite_playerJetpackRightRun1_1-aprite.h"
#include "out/sprite_playerJetpackRightRun0_2-aprite.h"
#include "out/sprite_playerJetpackRightRun1_2-aprite.h"
#include "out/sprite_playerJetpackRightRun0_3-aprite.h"
#include "out/sprite_playerJetpackRightRun1_3-aprite.h"
#include "out/sprite_playerJetpackLeftJump0_0-aprite.h"
#include "out/sprite_playerJetpackLeftJump1_0-aprite.h"
#include "out/sprite_playerJetpackRightJump0_0-aprite.h"
#include "out/sprite_playerJetpackRightJump1_0-aprite.h"
#include "out/sprite_playerJetpackLeftStand0_0-aprite.h"
#include "out/sprite_playerJetpackLeftStand1_0-aprite.h"
#include "out/sprite_playerJetpackRightStand0_0-aprite.h"
#include "out/sprite_playerJetpackRightStand1_0-aprite.h"
#include "out/sprite_playerJetpackRightThrust0_0-aprite.h"
#include "out/sprite_playerJetpackRightThrust1_0-aprite.h"
#include "out/sprite_playerJetpackLeftThrust0_0-aprite.h"
#include "out/sprite_playerJetpackLeftThrust1_0-aprite.h"
#include "out/sprite_playerJetpackRightThrust0_1-aprite.h"
#include "out/sprite_playerJetpackRightThrust1_1-aprite.h"
#include "out/sprite_playerJetpackLeftThrust0_1-aprite.h"
#include "out/sprite_playerJetpackLeftThrust1_1-aprite.h"
#endif

sprite_hsprite_t sprite_hspriteAtlas[] = {
  [SPRITE_CLIMBER_RUN_LEFT_1] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_0_sprite1,
  },
  [SPRITE_CLIMBER_RUN_LEFT_2] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_1_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_1_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_1_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_1_sprite1,
  },
  [SPRITE_CLIMBER_RUN_LEFT_3] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_2_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_2_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_2_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_2_sprite1,
  },
  [SPRITE_CLIMBER_RUN_LEFT_4] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_3_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerLeftRun0_3_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_3_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerLeftRun1_3_sprite1,
  },

  [SPRITE_CLIMBER_RUN_RIGHT_1] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_0_sprite1,
  },
  [SPRITE_CLIMBER_RUN_RIGHT_2] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_1_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_1_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_1_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_1_sprite1,
  },
  [SPRITE_CLIMBER_RUN_RIGHT_3] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_2_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_2_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_2_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_2_sprite1,
  },
  [SPRITE_CLIMBER_RUN_RIGHT_4] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_3_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerRightRun0_3_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_3_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerRightRun1_3_sprite1,
  },

  [SPRITE_CLIMBER_JUMP_LEFT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerLeftJump0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerLeftJump0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerLeftJump1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerLeftJump1_0_sprite1,
  },
  [SPRITE_CLIMBER_JUMP_RIGHT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerRightJump0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerRightJump0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerRightJump1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerRightJump1_0_sprite1,
  },

  [SPRITE_CLIMBER_STAND_LEFT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerLeftStand0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerLeftStand0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerLeftStand1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerLeftStand1_0_sprite1,
  },
  [SPRITE_CLIMBER_STAND_RIGHT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerRightStand0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerRightStand0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerRightStand1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerRightStand1_0_sprite1,
  },
#ifdef GAME_JETPACK
  [SPRITE_CLIMBER_JETPACK_RUN_LEFT_1] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_0_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_RUN_LEFT_2] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_1_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_1_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_1_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_1_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_RUN_LEFT_3] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_2_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_2_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_2_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_2_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_RUN_LEFT_4] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_3_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun0_3_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_3_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftRun1_3_sprite1,
  },

  [SPRITE_CLIMBER_JETPACK_RUN_RIGHT_1] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_0_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_RUN_RIGHT_2] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_1_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_1_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_1_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_1_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_RUN_RIGHT_3] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_2_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_2_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_2_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_2_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_RUN_RIGHT_4] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_3_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun0_3_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_3_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightRun1_3_sprite1,
  },

  [SPRITE_CLIMBER_JETPACK_JUMP_LEFT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftJump0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftJump0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftJump1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftJump1_0_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_JUMP_RIGHT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightJump0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightJump0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightJump1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightJump1_0_sprite1,
  },

  [SPRITE_CLIMBER_JETPACK_STAND_LEFT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftStand0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftStand0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftStand1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftStand1_0_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_STAND_RIGHT] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightStand0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightStand0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightStand1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightStand1_0_sprite1,
  },

  [SPRITE_CLIMBER_JETPACK_THRUST_LEFT_1] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust1_0_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_THRUST_LEFT_2] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust0_1_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust0_1_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust1_1_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackLeftThrust1_1_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_THRUST_RIGHT_1] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust0_0_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust0_0_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust1_0_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust1_0_sprite1,
  },
  [SPRITE_CLIMBER_JETPACK_THRUST_RIGHT_2] = {
    .hsprite00 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust0_1_sprite0,
    .hsprite01 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust0_1_sprite1,
    .hsprite10 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust1_1_sprite0,
    .hsprite11 = (sprite_hsprite_control_t*)&sprite_playerJetpackRightThrust1_1_sprite1,
  },
#endif
};
#endif

image_t sprite_imageAtlas[] = {
  [SPRITE_CLIMBER_RUN_LEFT_1] = { // 0. climber run left 1
    .x = 0,
    .y = 75,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_RUN_LEFT_2] = { // 1. climber run left 2
    .x = 32,
    .y = 75,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_RUN_LEFT_3] = { // 2. climber run left 3
    .x = 64,
    .y = 75,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_RUN_LEFT_4] = { // 3. climber run left 4
    .x = 96,
    .y = 75,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_STAND_LEFT] =  { // 4. climber stand left 
    .x = 128,
    .y = 75,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_JUMP_LEFT] =  { // 5. climber jump left
    .x = 160,
    .y = 75,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_RUN_RIGHT_1] = { // 6. climber run right 1
    .x = 0,
    .y = 123,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_RUN_RIGHT_2] = { // 7. climber run right 2
    .x = 32,
    .y = 123,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_RUN_RIGHT_3] = { // 8. climber run right 3
    .x = 64,
    .y = 123,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_RUN_RIGHT_4] = { // 9. climber run right 4
    .x = 96,
    .y = 123,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_STAND_RIGHT] = { // 10. climber stand right 
    .x = 128,
    .y = 123,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLIMBER_JUMP_RIGHT]  = { // 11. climber jump right
    .x = 160,
    .y = 123,
    .w = 32,
    .h = 37
  },
  [SPRITE_CLOUD_1]  = {
    .x = 0,
    .y = 32,
    .w = CLOUD_WIDTH,
    .h = CLOUD_HEIGHT
  },
  [SPRITE_CLOUD_2]  = {
    .x = 48,
    .y = 32,
    .w = CLOUD_WIDTH,
    .h = CLOUD_HEIGHT
  },
  [SPRITE_ENEMY_RUN_LEFT_1] = { 
    .x = 0,
    .y = 173,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_RUN_LEFT_2] = { 
    .x = 32,
    .y = 173,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_RUN_LEFT_3] = { 
    .x = 64,
    .y = 173,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_RUN_LEFT_4] = { 
    .x = 96,
    .y = 173,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_RUN_RIGHT_1] = { 
    .x = 0,
    .y = 221,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_RUN_RIGHT_2] = { 
    .x = 32,
    .y = 221,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_RUN_RIGHT_3] = { 
    .x = 64,
    .y = 221,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_RUN_RIGHT_4] = { 
    .x = 96,
    .y = 221,
    .w = 32,
    .h = ENEMY_RUN_HEIGHT
  },
  [SPRITE_ENEMY_SKATE_RIGHT_1] = { 
    .x = 128,
    .y = 171,
    .w = 32,
    .h = 37
  },
  [SPRITE_ENEMY_SKATE_RIGHT_2] = { 
    .x = 160,
    .y = 171,
    .w = 32,
    .h = 37
  },
  [SPRITE_ENEMY_SKATE_LEFT_1] = { 
    .x = 128,
    .y = 219,
    .w = 32,
    .h = 37
  },
  [SPRITE_ENEMY_SKATE_LEFT_2] = { 
    .x = 160,
    .y = 219,
    .w = 32,
    .h = 37
  },
  [SPRITE_ENEMY_DRAGON_RIGHT_1] = { 
    .x = 96,
    .y = 38,
    .w = 32,
    .h = 25
  },
  [SPRITE_ENEMY_DRAGON_RIGHT_2] = { 
    .x = 128,
    .y = 38,
    .w = 32,
    .h = 25
  },
  [SPRITE_ENEMY_DRAGON_RIGHT_3] = { 
    .x = 160,
    .y = 38,
    .w = 32,
    .h = 25
  },
  [SPRITE_ENEMY_DRAGON_RIGHT_4] = { 
    .x = 192,
    .y = 38,
    .w = 32,
    .h = 25
  },
  [SPRITE_COIN_1] = { 
    .x = 96,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_2] = { 
    .x = 112,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_3] = { 
    .x = 128,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_4] = { 
    .x = 144,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_5] = { 
    .x = 160,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_6] = { 
    .x = 176,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_7] = { 
    .x = 192,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_8] = { 
    .x = 208,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_9] = { 
    .x = 224,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_10] = { 
    .x = 240,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_11] = { 
    .x = 256,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_12] = { 
    .x = 272,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_13] = { 
    .x = 288,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
  [SPRITE_COIN_14] = { 
    .x = 304,
    .y = 2,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
#ifdef GAME_JETPACK
  [SPRITE_JETPACK] = {
    .x = 96,
    .y = 18,
    .w = ITEM_WIDTH,
    .h = ITEM_HEIGHT
  },
#endif
  [SPRITE_JOYSTICK_1] = {
    .x = 256,
    .y = 112,
    .w = 32,
    .h = 32
  },
  [SPRITE_JOYSTICK_2] = {
    .x = 288,
    .y = 112,
    .w = 32,
    .h = 32
  }
};


uint16_t sprite_6byteWideLUT[64];
#ifdef PLAYER_HARDWARE_SPRITE
__section(data_c) uint16_t sprite_nullhsprite[] = {
  0x0000, 0x0000,
  0x0000, 0x0000
};
#endif

void
sprite_ctor(void)
{
#ifdef PLAYER_HARDWARE_SPRITE

  for (size_t i = 0; i < sizeof(sprite_hspriteAtlas)/sizeof(sprite_hsprite_t); i++) {
    if (sprite_hspriteAtlas[i].hsprite01) {
      sprite_hspriteAtlas[i].hsprite01->attach = 1;
    }
    if (sprite_hspriteAtlas[i].hsprite11) {
      sprite_hspriteAtlas[i].hsprite11->attach = 1;
    }
  }

  for (int16_t i = 0; i < 64; i++) {
    sprite_6byteWideLUT[i] = i * (6*SCREEN_BIT_DEPTH);
  }

  int16_t index = 1;
  for (int16_t i = 0; i < 8; i++) {
    copper.sprpt[index] = ((uint32_t)sprite_nullhsprite & 0xffff);
    index += 2;
    copper.sprpt[index] = (uint32_t)sprite_nullhsprite >> 16;
    index += 2;
  }
#endif
}

#ifndef INLINE_EVERYTHING
#include "sprite_inlines.h"
#endif
