#include "game.h"

object_animation_t object_animations[] = {
  [OBJECT_ANIM_PLAYER1_RUN_RIGHT] = {
    .start =SPRITE_PLAYER1_RUN_RIGHT_1,
    .stop =SPRITE_PLAYER1_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_STAND_RIGHT] = {
    .start =SPRITE_PLAYER1_STAND_RIGHT,
    .stop =SPRITE_PLAYER1_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_RUN_LEFT] = {
    .start =SPRITE_PLAYER1_RUN_LEFT_1,
    .stop =SPRITE_PLAYER1_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_STAND_LEFT] = {
    .start =SPRITE_PLAYER1_STAND_LEFT,
    .stop =SPRITE_PLAYER1_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  // =================================
  [OBJECT_ANIM_PLAYER2_RUN_RIGHT] = {
    .start =SPRITE_PLAYER2_RUN_RIGHT_1,
    .stop =SPRITE_PLAYER2_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_STAND_RIGHT] = {
    .start =SPRITE_PLAYER2_STAND_RIGHT,
    .stop =SPRITE_PLAYER2_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_RUN_LEFT] = {
    .start =SPRITE_PLAYER2_RUN_LEFT_1,
    .stop =SPRITE_PLAYER2_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_STAND_LEFT] = {
    .start =SPRITE_PLAYER2_STAND_LEFT,
    .stop =SPRITE_PLAYER2_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  // =================================
  [OBJECT_ANIM_PLAYER3_RUN_RIGHT] = {
    .start =SPRITE_PLAYER3_RUN_RIGHT_1,
    .stop =SPRITE_PLAYER3_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_STAND_RIGHT] = {
    .start =SPRITE_PLAYER3_STAND_RIGHT,
    .stop =SPRITE_PLAYER3_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_RUN_LEFT] = {
    .start =SPRITE_PLAYER3_RUN_LEFT_1,
    .stop =SPRITE_PLAYER3_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_STAND_LEFT] = {
    .start =SPRITE_PLAYER3_STAND_LEFT,
    .stop =SPRITE_PLAYER3_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  }      
};


object_image_t object_imageAtlas[] = {
  [SPRITE_PLAYER1_STAND_RIGHT] = {
    .x = 0,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_1] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_2] = {
    .x = 64,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_3] = {
    .x = 96,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_4] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER1_STAND_LEFT] = {
    .x = 192,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_LEFT_1] = {
    .x = 160,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_LEFT_2] = {
    .x = 128,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,   
  },
  [SPRITE_PLAYER1_RUN_LEFT_3] = {
    .x = 96,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_LEFT_4] = {
    .x = 128,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
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
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_RIGHT_2] = {
    .x = 64,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 0,
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
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_STAND_LEFT] = {
    .x = 192,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_LEFT_1] = {
    .x = 160,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_LEFT_2] = {
    .x = 128,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,   
  },
  [SPRITE_PLAYER2_RUN_LEFT_3] = {
    .x = 96,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
    [SPRITE_PLAYER2_RUN_LEFT_4] = {
    .x = 128,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
    },

  //==============================
  [SPRITE_PLAYER3_STAND_RIGHT] = {
    .x = 0,
    .y = 214,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_1] = {
    .x = 32,
    .y = 214,
    .w = 48,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_2] = {
    .x = 80,
    .y = 214,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_3] = {
    .x = 112,
    .y = 214,
    .w = 48,
    .h = 60,
    .dx = -5,
    .dy = 0,    
  },
    [SPRITE_PLAYER3_RUN_RIGHT_4] = {
    .x = 80,
    .y = 214,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_STAND_LEFT] = {
    .x = 240,
    .y = 274,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_LEFT_1] = {
    .x = 192,
    .y = 274,
    .w = 48,
    .h = 60,
    .dx = -6,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_LEFT_2] = {
    .x = 160,
    .y = 274,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,   
  },
  [SPRITE_PLAYER3_RUN_LEFT_3] = {
    .x = 112,
    .y = 274,
    .w = 48,
    .h = 60,
    .dx = -3,
    .dy = 0,    
  },
    [SPRITE_PLAYER3_RUN_LEFT_4] = {
    .x = 160,
    .y = 274,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  }  
};
