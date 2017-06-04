#include "game.h"


static void
player_processJoystick(object_t * ptr, uint8_t joystickPos)
{
  switch (joystickPos) {
  case JOYSTICK_POS_IDLE:
    ptr->velocity.x = 0;
    ptr->velocity.y = 0;
    break;
  case JOYSTICK_POS_LEFT:
    ptr->velocity.y = 0;    
    ptr->velocity.x = -PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_RIGHT:
    ptr->velocity.y = 0;
    ptr->velocity.x = PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_UP:
    ptr->velocity.y = -PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = 0;    
    break;
  case JOYSTICK_POS_DOWN:
    ptr->velocity.y = PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = 0;    
    break;    
  case JOYSTICK_POS_UPRIGHT:
    ptr->velocity.y = -PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;    
    break;
  case JOYSTICK_POS_UPLEFT:
    ptr->velocity.y = -PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = -PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_DOWNRIGHT:
    ptr->velocity.y = PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;    
    break;
  case JOYSTICK_POS_DOWNLEFT:
    ptr->velocity.y = PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = -PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;
    break;
  }
}


uint16_t
player_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  if (data->attackCount > 0 && data->attackJump) {
    return 0;
  }
      
  if (object_get_state(ptr) != OBJECT_STATE_ALIVE) {
    if (object_get_state(ptr) == OBJECT_STATE_FLASHING && data->health > 0) {
      goto ok;
    }
    return 0;
  }

 ok:
  __USE(deltaT);
  uint16_t attack = 0;
  uint16_t buttonDown = 0;
  uint16_t joyUp = 0;

  player_processJoystick(ptr, *ptr->joystickPos);
  buttonDown = *ptr->joystickButton & 0x1;
  joyUp = *ptr->joystickPos  == JOYSTICK_POS_UP || *ptr->joystickPos == JOYSTICK_POS_UPRIGHT || *ptr->joystickPos == JOYSTICK_POS_UPLEFT;

  if (buttonDown && data->buttonReleased) {
    if (joyUp) {
      data->attackType = 2;
    }
      
    attack = 1;
  }

  if (!buttonDown) {
    data->buttonReleased = 1;
  }

  return attack;
}
    

fighter_attack_config_t player_attackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  } ,
  [OBJECT_KICK_LEFT] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = -(PLAYER_SPEED_X/2)*OBJECT_PHYSICS_FACTOR,
    .vy = -4*OBJECT_PHYSICS_FACTOR,
    .jump = 1    
  } ,
  [OBJECT_KICK_RIGHT] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE*2,
    .durationTics = ENEMY_ATTACK_DURATION_TICS,
    .hitAnimTic = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = (PLAYER_SPEED_X/2)*OBJECT_PHYSICS_FACTOR,
    .vy = -4*OBJECT_PHYSICS_FACTOR,
    .jump = 1    
  } 
};
    
object_t*
player_init(uint16_t id, uint16_t animId, int16_t x)
{
  object_t* ptr = fighter_add(id, animId, x, 100, PLAYER_INITIAL_HEALTH, player_attackConfig, player_intelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->numAttacks = 2;
  data->flashCount = FIGHTER_SPAWN_FLASH_COUNT_TICS;
  data->flashDurationTics = FIGHTER_SPAWN_FLASH_DURATION_TICS;
  uint16_t width;
  if (id == OBJECT_ID_PLAYER1) {
    width = PLAYER_PLAYER1_WIDTH;
  } else {
    width = PLAYER_PLAYER2_WIDTH;
  }
  ptr->widthOffset = (OBJECT_WIDTH-width)/2;
  ptr->width = OBJECT_WIDTH;
  object_set_state(ptr, OBJECT_STATE_FLASHING);  
  return ptr;
}
