#include "game.h" 

typedef struct {
  hiscore_t scores[HISCORE_NUM_SCORES];
  uint32_t checksum;
#if TRACKLOADER==1
  char pad[512-(sizeof(hiscore_t)*HISCORE_NUM_SCORES)-sizeof(uint32_t)];
#endif
} hiscore_storage_t DISK_SECTOR_ALIGN;

typedef struct {
  hiscore_t scores[HISCORE_NUM_SCORES];
  uint32_t checksum;
#if TRACKLOADER==1
  char pad[(512*11)-(sizeof(hiscore_t)*HISCORE_NUM_SCORES)-sizeof(uint32_t)];
#endif
} hiscore_buffer_t;

#if TRACKLOADER==1
typedef struct {
  hiscore_t scores[HISCORE_NUM_SCORES];
  uint32_t checksum;
} hiscore_small_buffer_t;
#endif

__EXTERNAL __section(section lastTrack) 
hiscore_storage_t hiscore_disk = {
  .scores = {
    {1000, "E.G", 0, ""},
    {500, "L.E", 0, ""},
    {250, "S.T", 0, ""},
    {100, "E.M", 0, ""},
    {50, "W.A", 0, ""},
    {25, "E.D", 0, ""}
  },
  .checksum = 1925
};


static hiscore_buffer_t hiscore;
#if TRACKLOADER==1
static hiscore_small_buffer_t hiscore2;
#endif
static char hiscore_promptBuffer[4];

#ifdef GAME_HISCORE_SAVE_ENABLE
static
#endif
uint32_t
hiscore_checksum(void)
{
  uint32_t checksum = 0;
  
  for (int16_t i = 0; i < HISCORE_NUM_SCORES; i++) {
    checksum += hiscore.scores[i].score;
  }
  return checksum;
}


#if TRACKLOADER==1
uint16_t
hiscore_load(uint16_t ignoreErrors)
{
  uint16_t error = 0;
#ifdef DEBUG
  if (sizeof(hiscore_disk) != 512) {
    PANIC("(sizeof(hiscore_disk) != 512");
  }
#endif

 retry:
   error = disk_loadData(&hiscore, &hiscore_disk, sizeof(hiscore_disk));

   if (error) {
     if (ignoreErrors) {
       message_screenOn(I18N_HISCORE_LOAD_ERR);
       hw_waitScanLines(200);
     } else {
       if (message_ask(I18N_HISCORE_LOAD_RTRY)) {
	 goto retry;
       }
     }
  } else {
    uint32_t checksum = hiscore_checksum();
    
    if (checksum != hiscore.checksum) {
      if (ignoreErrors) {
	message_screenOn(I18N_HISCORE_CHSM_BAD);
	hw_waitScanLines(200);
      } else {
	if (message_ask(I18N_HISCORE_CHSM_RTRY)) {
	  goto retry;
	}
      }
    }
  }

  return error;
}
#else
__EXTERNAL uint16_t
hiscore_load(uint16_t ignoreErrors)
{
  USE(ignoreErrors);
  int16_t loaded = 0;  
#ifdef GAME_HISCORE_SAVE_ENABLE
  dos_init();

  BPTR file = Open("PROGDIR:hiscore.bin", MODE_OLDFILE);

  if (file) {
    if (Read(file, (void*)&hiscore, sizeof(hiscore)) == sizeof(hiscore)) {
      uint32_t checksum = hiscore_checksum();
      if (checksum == hiscore.checksum) {
	loaded = 1;
      }
    }
    Close(file);
  }
#endif
  if (!loaded) {
    disk_loadData(&hiscore, &hiscore_disk, sizeof(hiscore_disk));
  }
  
  return 0;
}

#endif

void
hiscore_ctor(void)
{
#if TRACKLOADER==1
  hiscore_load(0);
#endif
}


hiscore_t*
hiscore_render(void)
{
  for (int16_t i = 0; i < HISCORE_NUM_SCORES; i++) {
    hiscore.scores[i].name[sizeof(hiscore.scores[i].name)-1] = 0;
    strcpy(hiscore.scores[i].text, itoan(hiscore.scores[i].score, 6));
    strcpy(&hiscore.scores[i].text[6], " -    ");
    int16_t nameLen = strlen(hiscore.scores[i].name);
    strcpy(&hiscore.scores[i].text[9+(HISCORE_NAME_LENGTH-nameLen)], hiscore.scores[i].name);
  }
  
  return hiscore.scores;
}


#if TRACKLOADER==1
void
hiscore_saveData(uint16_t ignoreErrors)
{
  hiscore.checksum = hiscore_checksum();

  memcpy(&hiscore2, &hiscore, sizeof(hiscore2));

 retry:
  message_loading(I18N_SAVING_HISCORE);
  if (disk_write(&hiscore_disk, &hiscore, 1) != 0) {
    if (ignoreErrors) {
      message_screenOn(I18N_HISCORE_SAVE_FAIL);
      hw_waitScanLines(200);
    } else {
      if (message_ask(I18N_HISCORE_SAVE_RTRY)) {
	goto retry;
      }
    }
  }

  hiscore_load(1);
  if (memcmp(&hiscore2, &hiscore, sizeof(hiscore2)) != 0) {
    if (message_ask(I18N_HISCORE_SAVE_RTRY)) {
      memcpy(&hiscore, &hiscore2, sizeof(hiscore2));
      goto retry;
    }
  }
  message_screenOff();
}
#endif


static char*
hiscore_prompt(char* message)
{
  uint16_t bufferIndex = 0;
  char* congrats = I18N_CONGRATULATIONS;

  USE(congrats);
  USE(message);
  hiscore_promptBuffer[0] = 0;

  message_screenOn(I18N_PLEASE_ENTER_NAME);
  
  text_drawMaskedText8Blitter(game_offScreenBuffer, congrats, (SCREEN_WIDTH/2)-(strlen(congrats)*4), (SCREEN_HEIGHT/2)-22-16);
  text_drawMaskedText8Blitter(game_offScreenBuffer, message, (SCREEN_WIDTH/2)-(strlen(message)*4), (SCREEN_HEIGHT/2)-22);

  int x = (SCREEN_WIDTH/2)-8;
  int y = (SCREEN_HEIGHT/2)+22;

  text_drawMaskedText8Blitter(game_offScreenBuffer, "___", x, y);

  for (;;) {
    char str[2] = {0,0};
    keyboard_read();

#ifdef DEBUG
#ifdef SCRIPTING
    script_process();
#endif
#endif
    
    if (keyboard_key) {            
      //gfx_fillRectSmallScreen(game_offScreenBuffer, 0, 0, 16*8, 8, 0);
      //text_drawMaskedText8Blitter(game_offScreenBuffer, itoa(keyboard_code), 0, 0);
      
      if (keyboard_code == KEYBOARD_CODE_BACKSPACE) {
	if (bufferIndex > 0) {
	  x-=8;
	  hiscore_promptBuffer[bufferIndex] = 0;
	  bufferIndex--;
	  gfx_fillRectSmallScreen(game_offScreenBuffer, x, y, 8, 8, 0);
	  text_drawMaskedText8Blitter(game_offScreenBuffer, "_", x, y);	
	}
      } else if (keyboard_code != KEYBOARD_CODE_RETURN) {
	if (bufferIndex < 3) {
	  hiscore_promptBuffer[bufferIndex] = keyboard_key;
	  bufferIndex++;
	  hiscore_promptBuffer[bufferIndex] = 0;	
	  str[0] = keyboard_key;
	  gfx_fillRectSmallScreen(game_offScreenBuffer, x, y, 8, 8, 0);
	  text_drawMaskedText8Blitter(game_offScreenBuffer, str, x, y);
	  x+=8;
	}
      }
    }

    hw_readJoystick();
    if (game_fire()) {
      break;
    }

    hw_waitVerticalBlank();
  }
  
  hw_waitBlitter();
  custom->bltafwm = 0xffff;

  message_screenOff();

  return hiscore_promptBuffer;
}


void
hiscore_addScore(uint16_t playerNumber, uint32_t score)
{
  int16_t i, dirty = 0;
  char* name;

  for (i = HISCORE_NUM_SCORES-1; i >= 0; i--) {
    if (score >= hiscore.scores[i].score) {
      if (i > 0) {
	hiscore.scores[i].score = hiscore.scores[i-1].score;
	strncpy(hiscore.scores[i].name, hiscore.scores[i-1].name, HISCORE_NAME_LENGTH);
	hiscore.scores[i].name[HISCORE_NAME_LENGTH] = 0;
      } else if (i == 0) {
	hiscore.scores[i].score = score;
	if (playerNumber == 1) {
	  name = hiscore_prompt(I18N_PLAYER1_HISCORE);
	} else {
	  name = hiscore_prompt(I18N_PLAYER2_HISCORE);
	}
	strncpy(hiscore.scores[i].name, name, HISCORE_NAME_LENGTH);
	hiscore.scores[i].name[HISCORE_NAME_LENGTH] = 0;
	dirty = 1;
      }
    } else {
      if (i < HISCORE_NUM_SCORES-1) {
	hiscore.scores[i+1].score = score;
	if (playerNumber == 1) {	
	  name = hiscore_prompt(I18N_PLAYER1_ONBOARD);
	} else {
	  name = hiscore_prompt(I18N_PLAYER2_ONBOARD);
	}
	strncpy(hiscore.scores[i+1].name, name, HISCORE_NAME_LENGTH);
	hiscore.scores[i+1].name[HISCORE_NAME_LENGTH] = 0;
	dirty = 1;
      }
      break;
    }
  }

#if TRACKLOADER==1
  if (dirty) {
    hiscore_saveData(0);
  }
#else
  USE(dirty);
#endif
}


#if TRACKLOADER==0

__EXTERNAL void
hiscore_save(void)
{ 
  //extern BPTR startupDirLock;

#ifdef GAME_HISCORE_SAVE_ENABLE  
  dos_init();

  hiscore.checksum = hiscore_checksum();

  BPTR file = Open("PROGDIR:hiscore.bin", MODE_NEWFILE);
  if (file) {
    Write(file, &hiscore, sizeof(hiscore));
    Close(file);
  }
#endif
}

#endif
