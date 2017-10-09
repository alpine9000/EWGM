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
player_intelligence(__UNUSED uint16_t deltaT, object_t* ptr, fighter_data_t* data)
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
  {}
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


fighter_attack_config_t player_attackConfigEasy[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_EASY,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_EASY,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_EASY,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_EASY,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  } ,
  [OBJECT_KICK_LEFT] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_EASY*2,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = -(PLAYER_SPEED_X/2)*OBJECT_PHYSICS_FACTOR,
    .vy = -4*OBJECT_PHYSICS_FACTOR,
    .jump = 1
  } ,
  [OBJECT_KICK_RIGHT] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_EASY*2,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = (PLAYER_SPEED_X/2)*OBJECT_PHYSICS_FACTOR,
    .vy = -4*OBJECT_PHYSICS_FACTOR,
    .jump = 1
  }
};

fighter_attack_config_t player_attackConfigHard[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_HARD,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_LEFT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_HARD,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = FIGHTER_LONG_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_HARD,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT2] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_HARD,
    .durationTics = PLAYER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  } ,
  [OBJECT_KICK_LEFT] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_HARD*2,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = -(PLAYER_SPEED_X/2)*OBJECT_PHYSICS_FACTOR,
    .vy = -4*OBJECT_PHYSICS_FACTOR,
    .jump = 1
  } ,
  [OBJECT_KICK_RIGHT] =  {
    .rangeX = FIGHTER_SHORT_PUNCH_RANGE,
    .dammage = PLAYER_ATTACK_DAMMAGE_HARD*2,
    .durationTics = ENEMY_DEFAULT_ATTACK_DURATION_TICS,
    .hitAnimTic = LEVEL1_BOSS_ATTACK_TICS_PER_FRAME,
    .vx = (PLAYER_SPEED_X/2)*OBJECT_PHYSICS_FACTOR,
    .vy = -4*OBJECT_PHYSICS_FACTOR,
    .jump = 1
  }
};

static void
player_player1KillCallback(__UNUSED object_t* me, __UNUSED object_t* victim)
{
  game_player1Score += game_killScore;
  sound_queueSound(SOUND_DIE01);
}

static void
player_player1HitCallback(__UNUSED object_t* me, __UNUSED object_t* victim)
{
  sound_queueSound(SOUND_TERENCE_PUNCH01);
}

static void
player_player2KillCallback(__UNUSED object_t* me, __UNUSED object_t* victim)
{
  game_player2Score += game_killScore;
  sound_queueSound(SOUND_DIE02);
}

static void
player_player2HitCallback(__UNUSED object_t* me, __UNUSED object_t* victim)
{
  sound_queueSound(SOUND_BUD_PUNCH01);
}

static void
player_player1DieCallback(__UNUSED object_t* me)
{
  game_player1 = 0;
  game_scoreBoardPlayer1Text(I18N_GAME_OVER);
  if (!game_player2) {
    game_setGameOver();
  } else if (game_loopControl == GAME_LOOP_CONTROL_DEATHMATCH) {
    game_setGameComplete();
  }
}


static void
player_player2DieCallback(__UNUSED object_t* me)
{
  game_player2 = 0;
  if (!game_player1) {
    game_setGameOver();
  } else if (game_loopControl == GAME_LOOP_CONTROL_DEATHMATCH) {
    game_setGameComplete();
  }
  game_scoreBoardPlayer2Text(I18N_GAME_OVER);
}

object_t*
player_init(uint16_t id, uint16_t animId, int16_t x, int16_t health)
{
  if (health == 0) {
    health = PLAYER_INITIAL_HEALTH;
  }

  fighter_attack_config_t* attackConfig;
  //  attackConfig = game_difficulty == GAME_DIFFICULTY_HARD ? player_attackConfigHard : player_attackConfigEasy;
  extern  fighter_attack_config_t level4_boss_attackConfig[];
  attackConfig = level4_boss_attackConfig;
  object_t* ptr = fighter_add(id, OBJECT_ATTRIBUTE_PLAYER, animId, x, 100, health, attackConfig, level_playerIntelligence());
  fighter_data_t* data = fighter_data(ptr);
  data->numAttacks = 2;
  data->flashCount = FIGHTER_SPAWN_FLASH_COUNT_TICS;
  data->flashDurationTics = FIGHTER_SPAWN_FLASH_DURATION_TICS;
  uint16_t width;
  if (id == OBJECT_ID_PLAYER1) {
    width = PLAYER_PLAYER1_WIDTH;
    data->hitEnemyCallback = player_player1HitCallback;
    data->killEnemyCallback = player_player1KillCallback;
    data->dieCallback = player_player1DieCallback;
  } else {
    width = PLAYER_PLAYER2_WIDTH;
    data->hitEnemyCallback = player_player2HitCallback;
    data->killEnemyCallback = player_player2KillCallback;
    data->dieCallback = player_player2DieCallback;
  }
  ptr->widthOffset = (OBJECT_WIDTH-width)/2;
  ptr->width = OBJECT_WIDTH;
  object_set_state(ptr, OBJECT_STATE_FLASHING);
  return ptr;
}
