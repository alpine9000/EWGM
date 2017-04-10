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
    ptr->velocity.x = -PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_RIGHT:
    ptr->velocity.x = PLAYER_SPEED_X*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_UP:
    ptr->velocity.y = -PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
    break;
  case JOYSTICK_POS_DOWN:
    ptr->velocity.y = PLAYER_SPEED_Y*OBJECT_PHYSICS_FACTOR;
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
  USE(data);
  USE(deltaT);
  uint16_t attack = 0;
  uint16_t buttonDown = 0;
  
  if (ptr->id == OBJECT_ID_PLAYER1) {
    player_processJoystick(ptr, hw_joystickPos);
    buttonDown = hw_joystickButton & 0x1;
  } else if (ptr->id == OBJECT_ID_PLAYER2) {
    player_processJoystick(ptr, hw_joystick2Pos);
    buttonDown = hw_joystick2Button & 0x1;
  }

  if (buttonDown && data->buttonReleased) {
    attack = 1;
  }

  if (!buttonDown) {
    data->buttonReleased = 1;
  }

  return attack;
}


object_t*
player_init(uint16_t id, uint16_t animId, int16_t x)
{
  object_t* ptr = fighter_add(id, animId, x, 100, PLAYER_INITIAL_HEALTH, PLAYER_ATTACK_DAMMAGE, player_intelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->attackDurationFrames = PLAYER_ATTACK_DURATION_TICS;
  data->attackHitAnimTic = 0;
  data->numAttacks = 2;
  uint16_t width;
  if (id == OBJECT_ID_PLAYER1) {
    width = PLAYER_PLAYER1_WIDTH;
  } else {
    width = PLAYER_PLAYER2_WIDTH;
  }
  data->widthOffset = (OBJECT_WIDTH-width)/2;  
  return ptr;
}
