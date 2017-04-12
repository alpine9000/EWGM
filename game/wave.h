#ifndef __WAVE_H
#define __WAVE_H

typedef struct {
  uint16_t cameraX;
  void (*callback)(void);
} wave_instruction_t;

void
wave_init();


void
wave_process();

#endif
