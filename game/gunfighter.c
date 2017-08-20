#include "game.h"

static void
gunfighter_bulletHitEnemyCallback(__UNUSED object_t* me, __UNUSED object_t* attacker)
{
  sound_queueSound(SOUND_ENEMY_PUNCH01);
}


static void
gunfighter_bulletKillEnemyCallback(__UNUSED object_t* me, __UNUSED object_t* attacker)
{
  sound_queueSound(SOUND_DIE03);
}

fighter_data_t bullet_fighterData = {
#ifdef DEBUG
  .magicNumber = FIGHTER_DATA_MAGIC_NUMBER,
#endif
  .hitEnemyCallback = gunfighter_bulletHitEnemyCallback,
  .killEnemyCallback = gunfighter_bulletKillEnemyCallback,
  .postAttackCount = 0,
};
static gunfighter_config_t* gunfighter_config;
static uint16_t gunfighter_attackQueued;
static object_t* gunfighter_bullet;
static object_t* gunfighter;
static uint16_t gunfighter_activated;
static int16_t gunfighter_walkabout;
static void
gunfighter_bulletUpdate(uint16_t deltaT, object_t* ptr)
{
  object_updatePositionNoChecks(deltaT, ptr);

  object_set_z(ptr, object_y(ptr)+gunfighter_config->bulletHeight);

  if (object_screenx(ptr) < -32 || object_screenx(ptr) > SCREEN_WIDTH) {
    object_set_state(ptr, OBJECT_STATE_REMOVED);
  }

  object_t* collision;
  if ((collision = fighter_attackCollision(ptr, -4, FIGHTER_ENEMY_Y_ATTACK_RANGE))) {
    if (object_get_state(collision) == OBJECT_STATE_ALIVE) {
      collision->hit.attacker = ptr;
      collision->hit.dammage = gunfighter_config->bulletDammage;
      collision->hit.dx = ptr->anim->facing == FACING_RIGHT ? 8 : -8;
      object_set_state(collision, OBJECT_STATE_ABOUT_TO_BE_HIT);
      object_set_state(ptr, OBJECT_STATE_REMOVED);
    }
  }
}


static void
gunfighter_freeBullet(__UNUSED void* data)
{
  gunfighter_bullet = 0;
}


static void
gunfighter_addBullet(void* _ptr)
{
  object_t* ptr = _ptr;
  int16_t x = object_x(ptr);
  uint16_t animId = gunfighter_config->bulletAnimId;

  if (ptr->anim->facing == FACING_LEFT) {
    animId++;
    x += gunfighter_config->bulletXOffsetLeft;
  } else {
    x += gunfighter_config->bulletXOffsetRight;
  }

  gunfighter_bullet = object_add(OBJECT_ID_BULLET, 0, x, object_y(ptr)-gunfighter_config->bulletHeight, 0,
				 animId,
				 gunfighter_bulletUpdate, OBJECT_DATA_TYPE_FIGHTER, &bullet_fighterData, gunfighter_freeBullet);

  gunfighter_bullet->velocity.x = ptr->anim->facing == FACING_RIGHT ? gunfighter_config->bulletSpeed : -gunfighter_config->bulletSpeed;
  gunfighter_bullet->velocity.y = 0;
  gunfighter_bullet->width = 1;
  gunfighter_bullet->widthOffset = 0;

  sound_queueSound(SOUND_SHOOT);
}


#define doRunAway 1
#define GUNFIGHTER_RUNAWAY_THRESHOLD 32
#define GUNFIGHTER_RUNWAY_SPEEDX_MULTIPLIER 4

uint16_t
gunfighter_runaway(object_t* ptr, fighter_data_t* data, object_t* player)
{
  uint16_t runaway = 0;

  if (doRunAway && !data->walkAbout && player) {
    if (abs(object_x(player) - object_x(ptr)) < GUNFIGHTER_RUNAWAY_THRESHOLD) {
      runaway = 1;
      if (object_x(player) - object_x(ptr) > 0) {
	if (object_screenx(ptr) <(ptr->width*2)) {
	  ptr->velocity.x = fighter_data(ptr)->speedX*GUNFIGHTER_RUNWAY_SPEEDX_MULTIPLIER;
	} else {
	  ptr->velocity.x = -fighter_data(ptr)->speedX*GUNFIGHTER_RUNWAY_SPEEDX_MULTIPLIER;
	}
      } else {
	if (object_screenx(ptr) > (SCREEN_WIDTH-(ptr->width*2))) {
	  ptr->velocity.x = -fighter_data(ptr)->speedX*GUNFIGHTER_RUNWAY_SPEEDX_MULTIPLIER;
	} else {
	  ptr->velocity.x = fighter_data(ptr)->speedX*GUNFIGHTER_RUNWAY_SPEEDX_MULTIPLIER;
	}
      }
      switch (random() & 0x3) {
      case 1:
	ptr->velocity.y = fighter_data(ptr)->speedY;
	break;
      case 2:
	ptr->velocity.y = -fighter_data(ptr)->speedY;
	break;
      default:
	ptr->velocity.y = 0;
	break;
      }
    }
  }

  return runaway;
}

__EXTERNAL int16_t gunfighter_velx = 0;

uint16_t
gunfighter_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{

  if (object_get_state(ptr) != OBJECT_STATE_ALIVE) {
    return 0;
  }

#ifdef DEBUG
  if (enemy_pause) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 0;
    return 0;
  }
#endif

  uint16_t attack = 0;

  uint32_t rand = random();

  if (!gunfighter_walkabout) {
    attack = enemy_intelligence(deltaT, ptr,  data);
    if ((rand & 0x7) == 0) {
      attack = 0;
      gunfighter_walkabout = 5;
    }
  }

  if (!gunfighter_activated && object_screenx(ptr) > (SCREEN_WIDTH-ptr->width)) {
    attack = 0;
    ptr->velocity.x = -fighter_data(ptr)->speedX;
    ptr->velocity.y = 0;
  } else {
    gunfighter_activated = 1;
  }

  uint16_t p1_runaway = gunfighter_runaway(ptr, data, game_player1);
  uint16_t p2_runaway = gunfighter_runaway(ptr, data, game_player2);


  gunfighter_velx = ptr->velocity.x;

  if (p1_runaway || p2_runaway) {
    gunfighter_walkabout = 20;
    attack = 0;
    gunfighter_attackQueued = 0;
  } else {
    object_collision_t collision;
    if ((object_collision(deltaT, ptr, &collision, ENEMY_INTERCEPT_X_RANGE, ENEMY_INTERCEPT_Y_THRESHOLD*2))) {
      if (collision.up) {
	ptr->velocity.y = data->speedY;
      } else if (collision.down) {
	ptr->velocity.y = -data->speedY;
      }

      if (collision.left) {
	ptr->velocity.x = data->speedX;
      } else if (collision.right) {
	ptr->velocity.x = -data->speedX;
      }

      /*

      switch (rand & 0x7) {
	case 0:
	  attack = 1;
	  break;
	case 1:
	  ptr->velocity.x = 0;
	  ptr->velocity.y = 0;
	  break;
	case 2:
	  ptr->velocity.x = -data->speedX;
	  ptr->velocity.y = 0;
	  break;
	case 3:
	  ptr->velocity.x = data->speedX;
	  ptr->velocity.y = 0;
	  break;
	case 4:
	  ptr->velocity.x = -data->speedX;
	  ptr->velocity.y = -data->speedY;
	  break;
	case 5:
	  ptr->velocity.x = -data->speedX;
	  ptr->velocity.y = data->speedY;
	  break;
	case 6:
	  ptr->velocity.x = data->speedX;
	  ptr->velocity.y = data->speedY;
	  break;
	case 7:
	  ptr->velocity.x = data->speedX;
	  ptr->velocity.y = -data->speedY;
	  break;
	  }*/
	data->walkAbout = rand & 0x7f;//ENEMY_WALKABOUT_TICS;
    }
 }

  if (attack) {
    gunfighter_attackQueued = 1;
  }

  data->walkAbout = gunfighter_walkabout;
  gunfighter_walkabout--;

  if (gunfighter_walkabout < 0) {
    gunfighter_walkabout = 0;
  }


  uint16_t hitTic = LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*3;//data->attackConfig[ptr->actionId].hitAnimTic;

  if (gunfighter_attackQueued && !gunfighter_bullet && data->attackCount < hitTic && data->attackCount > 0) {
    alarm_add(0, gunfighter_addBullet, ptr);
    data->attackQueued = 0;
    gunfighter_attackQueued = 0;
    return 0;
  }

  if (gunfighter_bullet) {
    data->attackQueued = 0;
    return 0;
  }

  return attack;
}

object_t*
gunfighter_add(gunfighter_config_t* gunfighterConfig, enemy_config_t* config, int16_t x, int16_t y)
{
  gunfighter_config = gunfighterConfig;
  gunfighter_activated = 0;
  gunfighter_attackQueued = 0;
  gunfighter_bullet = 0;
  gunfighter_walkabout = 0;
  gunfighter = enemy_add(gunfighterConfig->animId, OBJECT_ATTRIBUTE_PROJECTILE_LAUNCHING_ENEMY, x, y, config);
  gunfighter->id = OBJECT_ID_LEVEL2_BOSS;
  return gunfighter;
}
