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

__EXTERNAL __section(section lastTrack) 
hiscore_storage_t hiscore_disk = {
  .scores = {
    {1000, "C.S", 0, ""},
    {500, "L.K", 0, ""},
    {250, "I.I", 0, ""},
    {100, "M.E", 0, ""},
    {50, "B.S", 0, ""},
    {25, "Y.!", 0, ""}
  },
  .checksum = 1925
};


static hiscore_buffer_t hiscore;
static char hiscore_promptBuffer[4];

static uint32_t
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
       message_loading("error loading hiscore...");
       hw_waitScanLines(200);
     } else {
       if (message_ask("hiscore load fail, retry? y/n")) {
	 goto retry;
       }
     }
  } else {
    uint32_t checksum = hiscore_checksum();
    
    if (checksum != hiscore.checksum) {
      if (ignoreErrors) {
	message_loading("hiscore checksum mismatch...");
	hw_waitScanLines(200);
      } else {
	if (message_ask("hiscore chsm fail, retry? y/n")) {
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
    strcpy(&hiscore.scores[i].text[9+(3-nameLen)], hiscore.scores[i].name);
  }
  
  return hiscore.scores;
}


#if TRACKLOADER==1
void
hiscore_saveData(uint16_t ignoreErrors)
{
  return;
  
  hiscore.checksum = hiscore_checksum();

 retry:
  message_loading("Saving hiscore...");
  if (disk_write(&hiscore_disk, &hiscore, 1) != 0) {
    if (ignoreErrors) {
      message_loading("hiscore save failed...");
      hw_waitScanLines(200);
    } else {
      if (message_ask("hiscore save fail, retry? y/n")) {
	goto retry;
      }
    }
  }
  message_screenOff();
}
#endif


static char*
hiscore_prompt(char* message)
{
  uint16_t bufferIndex = 0;
  char* congrats = "CONGRATULATIONS!!";

  USE(congrats);
  USE(message);
  hiscore_promptBuffer[0] = 0;

  message_screenOn("PLEASE ENTER YOUR NAME");
  
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
hiscore_addScore(char* prompt, uint32_t score)
{
  int16_t i, dirty = 0;
  char* name;

  for (i = HISCORE_NUM_SCORES-1; i >= 0; i--) {
    if (score >= hiscore.scores[i].score) {
      if (i > 0) {
	hiscore.scores[i].score = hiscore.scores[i-1].score;
	strcpy(hiscore.scores[i].name, hiscore.scores[i-1].name);
      } else if (i == 0) {
	hiscore.scores[i].score = score;
	name = hiscore_prompt(prompt);
	strcpy(hiscore.scores[i].name, name);
	dirty = 1;
      }
    } else {
      if (i < HISCORE_NUM_SCORES-1) {
	hiscore.scores[i+1].score = score;
	name = hiscore_prompt("YOU ARE ON THE SCORE BOARD!");
	strcpy(hiscore.scores[i+1].name, name);
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

  dos_init();

  hiscore.checksum = hiscore_checksum();

  BPTR file = Open("PROGDIR:hiscore.bin", MODE_NEWFILE);
  if (file) {
    Write(file, &hiscore, sizeof(hiscore));
    Close(file);
  }
}

#endif
