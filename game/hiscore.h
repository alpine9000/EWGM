#ifndef __HISCORE_H
#define __HISCORE_H


#define HISCORE_NUM_SCORES 6
#define HISCORE_NAME_LENGTH 3
#define HISCORE_TEXT_LENGTH 15

typedef struct {
  uint32_t score;
  char name[HISCORE_NAME_LENGTH+1];
  uint32_t checksum;
  char text[HISCORE_TEXT_LENGTH];
} hiscore_t;

void
hiscore_ctor(void);

uint16_t
hiscore_load(uint16_t ignoreErrors);

hiscore_t*
hiscore_render(void);

void
hiscore_addScore(uint16_t playerNumber, uint32_t score);

void
hiscore_default(void);

void
hiscore_saveData(uint16_t ingoreErrors);
#endif
