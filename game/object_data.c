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
  [OBJECT_ANIM_PLAYER2_RUN_RIGHT] = {
    .start =SPRITE_PLAYER2_RUN_RIGHT_1,
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
    .start =SPRITE_PLAYER2_PUNCH_LEFT_1,
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
  [OBJECT_ANIM_PLAYER3_RUN_RIGHT] = {
    .start = SPRITE_PLAYER3_RUN_RIGHT_1,
    .stop = SPRITE_PLAYER3_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_STAND_RIGHT] = {
    .start = SPRITE_PLAYER3_STAND_RIGHT,
    .stop = SPRITE_PLAYER3_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_RUN_LEFT] = {
    .start = SPRITE_PLAYER3_RUN_LEFT_1,
    .stop = SPRITE_PLAYER3_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_STAND_LEFT] = {
    .start = SPRITE_PLAYER3_STAND_LEFT,
    .stop = SPRITE_PLAYER3_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_PUNCH_LEFT1] = {
    .start = SPRITE_PLAYER3_PUNCH_LEFT_1,
    .stop = SPRITE_PLAYER3_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_PUNCH_LEFT2] = {
    .start = SPRITE_PLAYER3_PUNCH_LEFT_2,
    .stop = SPRITE_PLAYER3_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },      
  [OBJECT_ANIM_PLAYER3_PUNCH_RIGHT1] = {
    .start = SPRITE_PLAYER3_PUNCH_RIGHT_1,
    .stop = SPRITE_PLAYER3_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_PUNCH_RIGHT2] = {
    .start = SPRITE_PLAYER3_PUNCH_RIGHT_2,
    .stop = SPRITE_PLAYER3_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_HIT_LEFT] = {
    .start = SPRITE_PLAYER3_HIT_LEFT,
    .stop = SPRITE_PLAYER3_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_HIT_RIGHT] = {
    .start = SPRITE_PLAYER3_HIT_RIGHT,
    .stop = SPRITE_PLAYER3_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_KICK_LEFT] = {
    .start = SPRITE_PLAYER3_KICK_LEFT_1,
    .stop = SPRITE_PLAYER3_KICK_LEFT_3,
    .speed = 8,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_KICK_RIGHT] = {
    .start = SPRITE_PLAYER3_KICK_RIGHT_1,
    .stop = SPRITE_PLAYER3_KICK_RIGHT_3,
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
  [OBJECT_ANIM_BOSS_RUN_RIGHT] = {
    .start = SPRITE_BOSS_RUN_RIGHT_1,
    .stop = SPRITE_BOSS_RUN_RIGHT_4,
    .speed = ENEMY_BOSS_RUN_ANIM_SPEED,
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
    .speed = ENEMY_BOSS_RUN_ANIM_SPEED,
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
    .speed = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_BOSS_ATTACK_LEFT2] = {
    .start = SPRITE_BOSS_ATTACK_LEFT_1,
    .stop = SPRITE_BOSS_ATTACK_LEFT_3,
    .speed = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_LEFT
  },      
  [OBJECT_ANIM_BOSS_ATTACK_RIGHT1] = {
    .start = SPRITE_BOSS_ATTACK_RIGHT_1,
    .stop = SPRITE_BOSS_ATTACK_RIGHT_3,
    .speed = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_BOSS_ATTACK_RIGHT2] = {
    .start = SPRITE_BOSS_ATTACK_RIGHT_1,
    .stop = SPRITE_BOSS_ATTACK_RIGHT_3,
    .speed = ENEMY_BOSS_ATTACK_TICS_PER_FRAME,
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

  [OBJECT_ANIM_MOTORBIKE_RUN_RIGHT] = {
    .start = SPRITE_MOTORBIKE_RIDE_RIGHT_1,
    .stop = SPRITE_MOTORBIKE_RIDE_RIGHT_2,
    .speed = ENEMY_MOTORBIKE_RIDE_ANIM_SPEED,
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
    .speed = ENEMY_MOTORBIKE_RIDE_ANIM_SPEED,
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
    .stop = SPRITE_BONUS_BURGER,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  // =================================
  [OBJECT_ANIM_BONUS_COLA] = {
    .start = SPRITE_BONUS_COLA,
    .stop = SPRITE_BONUS_COLA,
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
    .x = 208,
    .y = 731,
    .w = 48,
    .h = 52,
    .dx = -2,
    .dy = 0,    
    },
  //==============================
  [SPRITE_PLAYER2_STAND_RIGHT] = {
    .x = 0,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_RIGHT_1] = {
    .x = 32,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_RIGHT_2] = {
    .x = 64,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_RIGHT_3] = {
    .x = 96,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,    
  },
    [SPRITE_PLAYER2_RUN_RIGHT_4] = {
    .x = 32,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_STAND_LEFT] = {
    .x = 192,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_LEFT_1] = {
    .x = 160,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_LEFT_2] = {
    .x = 128,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,   
  },
  [SPRITE_PLAYER2_RUN_LEFT_3] = {
    .x = 96,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 2,
    .dy = 0,    
  },

  [SPRITE_PLAYER2_RUN_LEFT_4] = {
    .x = 128,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,    
    },

  [SPRITE_PLAYER2_PUNCH_LEFT_1] = {
    .x = 224,
    .y = 162,
    .w = 48,
    .h = 53,
    .dx = -4,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = -3,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_PUNCH_RIGHT_1] = {
    .x = 160,
    .y = 108,
    .w = 48,
    .h = 53,
    .dx = 3,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_PUNCH_RIGHT_2] = {
    .x = 128,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 3,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_HIT_LEFT] = {
    .x = 0,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_HIT_RIGHT] = {
    .x = 32,
    .y = 162,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
    },

  [SPRITE_PLAYER2_KICK_LEFT_1] = {
    .x = 80,
    .y = 731,
    .w = 32,
    .h = 56,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_KICK_LEFT_2] = {
    .x = 192,
    .y = 619,
    .w = 48,
    .h = 56,
    .dx = -6,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_KICK_LEFT_3] = {
    .x = 80,
    .y = 731,
    .w = 32,
    .h = 56,
    .dx = 0,
    .dy = 0,    
    },

  [SPRITE_PLAYER2_KICK_RIGHT_1] = {
    .x = 0,
    .y = 731,
    .w = 32,
    .h = 56,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_KICK_RIGHT_2] = {
    .x = 32,
    .y = 731,
    .w = 48,
    .h = 56,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_KICK_RIGHT_3] = {
    .x = 0,
    .y = 731,
    .w = 32,
    .h = 56,
    .dx = 0,
    .dy = 0,    
    },            

  //==============================
  [SPRITE_PLAYER3_STAND_RIGHT] = {
    .x = 0,
    .y = 215,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_1] = {
    .x = 32,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_2] = {
    .x = 80,
    .y = 215,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_3] = {
    .x = 112,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = -5,
    .dy = 0,    
  },
    [SPRITE_PLAYER3_RUN_RIGHT_4] = {
    .x = 80,
    .y = 215,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_STAND_LEFT] = {
    .x = 240,
    .y = 275,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_LEFT_1] = {
    .x = 192,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -6,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_LEFT_2] = {
    .x = 160,
    .y = 275,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,   
  },
  [SPRITE_PLAYER3_RUN_LEFT_3] = {
    .x = 112,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -3,
    .dy = 0,    
  },
    [SPRITE_PLAYER3_RUN_LEFT_4] = {
    .x = 160,
    .y = 275,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },

  [SPRITE_PLAYER3_PUNCH_LEFT_1] = {
    .x = 16,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -19,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 275,
    .w = 48,
    .h = 60,
    .dx = -19,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_PUNCH_RIGHT_1] = {
    .x = 208,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = 1,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 215,
    .w = 48,
    .h = 60,
    .dx = 1,
    .dy = 0,    
    },  
  [SPRITE_PLAYER3_HIT_LEFT] = {
    .x = 32,
    .y = 335,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_HIT_RIGHT] = {
    .x = 0,
    .y = 335,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },

  [SPRITE_PLAYER3_KICK_LEFT_1] = {
    .x = 240,
    .y = 560,
    .w = 32,
    .h = 60,
    .dx = -2,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_KICK_LEFT_2] = {
    .x = 208,
    .y = 927,
    .w = 48,
    .h = 60,
    .dx = -8,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_KICK_LEFT_3] = {
    .x = 240,
    .y = 560,
    .w = 32,
    .h = 60,
    .dx = -2,
    .dy = 0,    
    },

  [SPRITE_PLAYER3_KICK_RIGHT_1] = {
    .x = 240,
    .y = 621,
    .w = 32,
    .h = 60,
    .dx = 1,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_KICK_RIGHT_2] = {
    .x = 160,
    .y = 927,
    .w = 48,
    .h = 60,
    .dx = -3,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_KICK_RIGHT_3] = {
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

  //===========================

  [SPRITE_MOTORBIKE_RIDE_RIGHT_1] = {
    .x = 0,
    .y = 803,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_MOTORBIKE_RIDE_RIGHT_2] = {
    .x = 80,
    .y = 803,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_MOTORBIKE_HIT_RIGHT] = {
    .x = 0,
    .y = 865,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_MOTORBIKE_BROKEN_RIGHT] = {
    .x = 160,
    .y = 803,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,    
  },  
  [SPRITE_MOTORBIKE_RIDE_LEFT_1] = {
    .x = 80,
    .y = 865,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_MOTORBIKE_RIDE_LEFT_2] = {
    .x = 160,
    .y = 865,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_MOTORBIKE_HIT_LEFT] = {
    .x = 80,
    .y = 927,
    .w = 80,
    .h = 62,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_MOTORBIKE_BROKEN_LEFT] = {
    .x = 0,
    .y = 927,
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
    .x = 144,
    .y = 544,
    .w = 80,
    .h = 8,
    .dx = 0,
    .dy = 0,    
    },

    [SPRITE_GAMECOMPLETE] = {
    .x = 144,
    .y = 552,
    .w = 110,
    .h = 8,
    .dx = 0,
    .dy = 0,    
    },

  [SPRITE_DEATHMATCH] = {
    .x = 96,
    .y = 787,
    .w = 96,
    .h = 8,
    .dx = 0,
    .dy = 0,    
    },

  [SPRITE_LEVELCOMPLETE] = {
    .x = 96,
    .y = 795,
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

  //========================== 
  [SPRITE_STAR] = {
    .x = 112,
    .y = 547,
    .w = 16,
    .h = 3,
    .dx = 0,
    .dy = 0,    
    },   
  
};
