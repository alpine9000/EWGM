#include "game.h"

#define GUNFIGHTER_ATTACK_DURATION_TICS (ENEMY_GUNFIGHTER_ATTACK_TICS_PER_FRAME*3)
#define GUNFIGHTER_BULLET_SPEED 8
#define GUNFIGHTER_BULLET_HEIGHT 24
#define GUNFIGHTER_ATTACK_RANGE         (SCREEN_WIDTH/2)
#define GUNFIGHER_SHOT_DAMMAGE 50
#define GUNFIGHTER_SPEED 4

static uint16_t
gunfighter_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);

static fighter_attack_config_t gunfighter_attackConfig[] = {
  [OBJECT_PUNCH_LEFT1] = {
    .rangeX = GUNFIGHTER_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = GUNFIGHTER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0
  },
  [OBJECT_PUNCH_RIGHT1] =  {
    .rangeX = GUNFIGHTER_ATTACK_RANGE,
    .dammage = 0,
    .durationTics = GUNFIGHTER_ATTACK_DURATION_TICS,
    .hitAnimTic = 0,
    .vx = 0,
    .vy = 0,
    .jump = 0    
  }
};


static enemy_config_t gunfighter_config = {
  .attackRangeY = FIGHTER_ENEMY_Y_ATTACK_RANGE,
  .attackConfig = gunfighter_attackConfig,
  .attackWait = ENEMY_ATTACK_WAIT_TICS,
  .postAttackInvincibleTics = 0,
  .numAttacks = 1,
  .speed = 8,
  .intelligence = gunfighter_intelligence
};

static uint16_t gunfighter_attackQueued;
static object_t* gunfighter_bullet;
static object_t* gunfighter;

static void
gunfighter_bulletUpdate(uint16_t deltaT, object_t* ptr)
{
  object_updatePositionNoChecks(deltaT, ptr);

  object_set_z(ptr, object_y(ptr)+GUNFIGHTER_BULLET_HEIGHT);
  
  if (object_screenx(ptr) < -32 || object_screenx(ptr) > SCREEN_WIDTH) {
    object_set_state(ptr, OBJECT_STATE_REMOVED);
  }


  object_collision_t collision;
  if (fighter_attackCollision(ptr, &collision, 2, FIGHTER_ENEMY_Y_ATTACK_RANGE)) {  
    if (ptr->anim->facing == FACING_RIGHT && collision.right && collision.right != gunfighter) {
      if (object_get_state(collision.right) == OBJECT_STATE_ALIVE) {
	collision.right->hit.attacker = ptr;
	collision.right->hit.dammage = GUNFIGHER_SHOT_DAMMAGE;
	collision.right->hit.dx = 1;
	object_set_state(collision.right, OBJECT_STATE_ABOUT_TO_BE_HIT);
	object_set_state(ptr, OBJECT_STATE_REMOVED);    	
      }
    } else if (ptr->anim->facing == FACING_LEFT && collision.left && collision.left != gunfighter) {
      if (object_get_state(collision.left) == OBJECT_STATE_ALIVE) {
	collision.left->hit.attacker = ptr;
	collision.left->hit.dammage = GUNFIGHER_SHOT_DAMMAGE;
	collision.left->hit.dx = -1;
	object_set_state(collision.left, OBJECT_STATE_ABOUT_TO_BE_HIT);
	object_set_state(ptr, OBJECT_STATE_REMOVED);    	
      }
    }

  }
}

static void
gunfighter_freeBullet(void* data)
{
  __USE(data);
  gunfighter_bullet = 0;
}

static void
gunfighter_addBullet(void* _ptr)
{
  object_t* ptr = _ptr;
  int16_t x = object_x(ptr);
  uint16_t animId = OBJECT_ANIM_BULLET_LEFT;

  if (ptr->anim->facing == FACING_RIGHT) {
    x += OBJECT_WIDTH;
    animId = OBJECT_ANIM_BULLET_RIGHT;
  } 

  gunfighter_bullet = object_add(OBJECT_ID_BULLET, OBJECT_ATTRIBUTE_COLLIDABLE, x, object_y(ptr)-GUNFIGHTER_BULLET_HEIGHT, 0,
				 animId,
				 gunfighter_bulletUpdate, 0, gunfighter_freeBullet);

  gunfighter_bullet->velocity.x = ptr->anim->facing == FACING_RIGHT ? GUNFIGHTER_BULLET_SPEED : -GUNFIGHTER_BULLET_SPEED;
  gunfighter_bullet->velocity.y = 0;

  sound_queueSound(SOUND_SHOOT);
}


static uint16_t
gunfighter_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{ 
  uint16_t attack = enemy_intelligence(deltaT, ptr,  data);

  if (attack) {
    gunfighter_attackQueued = 1;
  }
  
  if (gunfighter_attackQueued && !gunfighter_bullet && data->attackCount < (ENEMY_GUNFIGHTER_ATTACK_TICS_PER_FRAME*2) && data->attackCount > 0) {
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

void
gunfighter_add(int16_t x, int16_t y)
{
  gunfighter_attackQueued = 0;
  gunfighter_bullet = 0;
  gunfighter = enemy_add(OBJECT_ANIM_GUNFIGHTER_STAND_RIGHT, x, y, &gunfighter_config);  
}
