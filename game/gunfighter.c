#include "game.h"

static void
gunfighter_bulletHitEnemyCallback(object_t* me, object_t* attacker)
{
  __USE(me);
  __USE(attacker);
  sound_queueSound(SOUND_ENEMY_PUNCH01);        
}


static void
gunfighter_bulletKillEnemyCallback(object_t* me, object_t* attacker)
{
  __USE(me);
  __USE(attacker);
  sound_queueSound(SOUND_DIE03);
}

fighter_data_t bullet_fighterData = {
  .hitEnemyCallback = gunfighter_bulletHitEnemyCallback,
  .killEnemyCallback = gunfighter_bulletKillEnemyCallback,
  .postAttackCount = 0,
};
static gunfighter_config_t* gunfighter_config;
static uint16_t gunfighter_attackQueued;
static object_t* gunfighter_bullet;
static object_t* gunfighter;

static void
gunfighter_bulletUpdate(uint16_t deltaT, object_t* ptr)
{
  object_updatePositionNoChecks(deltaT, ptr);

  object_set_z(ptr, object_y(ptr)+gunfighter_config->bulletHeight);
  
  if (object_screenx(ptr) < -32 || object_screenx(ptr) > SCREEN_WIDTH) {
    object_set_state(ptr, OBJECT_STATE_REMOVED);
  }


  object_collision_t collision;
  if (fighter_attackCollision(ptr, &collision, -4, FIGHTER_ENEMY_Y_ATTACK_RANGE)) {  
    if (ptr->anim->facing == FACING_RIGHT && collision.right && collision.right != gunfighter) {
      if (object_get_state(collision.right) == OBJECT_STATE_ALIVE) {
	collision.right->hit.attacker = ptr;
	collision.right->hit.dammage = gunfighter_config->bulletDammage;
	collision.right->hit.dx = 1;
	object_set_state(collision.right, OBJECT_STATE_ABOUT_TO_BE_HIT);
	object_set_state(ptr, OBJECT_STATE_REMOVED);    	
      }
    } else if (ptr->anim->facing == FACING_LEFT && collision.left && collision.left != gunfighter) {
      if (object_get_state(collision.left) == OBJECT_STATE_ALIVE) {
	collision.left->hit.attacker = ptr;
	collision.left->hit.dammage = gunfighter_config->bulletDammage;
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
  uint16_t animId = gunfighter_config->bulletAnimId;

  if (ptr->anim->facing == FACING_LEFT) {
    animId++;
    x += gunfighter_config->bulletXOffsetLeft;    
  } else {
    x += gunfighter_config->bulletXOffsetRight;
  }

  gunfighter_bullet = object_add(OBJECT_ID_BULLET, 0/*OBJECT_ATTRIBUTE_COLLIDABLE*/, x, object_y(ptr)-gunfighter_config->bulletHeight, 0,
				 animId,
				 gunfighter_bulletUpdate, &bullet_fighterData, gunfighter_freeBullet);

  gunfighter_bullet->velocity.x = ptr->anim->facing == FACING_RIGHT ? gunfighter_config->bulletSpeed : -gunfighter_config->bulletSpeed;
  gunfighter_bullet->velocity.y = 0;
  gunfighter_bullet->width = 1;
  gunfighter_bullet->widthOffset = 0;
  
  sound_queueSound(SOUND_SHOOT);
}


uint16_t
gunfighter_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{ 
  uint16_t attack = enemy_intelligence(deltaT, ptr,  data);

  if (attack) {
    gunfighter_attackQueued = 1;
  }

  uint16_t hitTic = ENEMY_LEVEL2_BOSS_ATTACK_TICS_PER_FRAME*2;//data->attackConfig[ptr->actionId].hitAnimTic;
  
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

void
gunfighter_add(gunfighter_config_t* gunfighterConfig, enemy_config_t* config, int16_t x, int16_t y)
{
  gunfighter_config = gunfighterConfig;
  gunfighter_attackQueued = 0;
  gunfighter_bullet = 0;
  gunfighter = enemy_add(gunfighterConfig->animId, x, y, config); 
}
