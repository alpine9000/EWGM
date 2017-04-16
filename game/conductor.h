#ifndef __CONDUCTOR_H
#define __CONDUCTOR_H

extern uint16_t conductor_scrollTarget;
extern uint16_t conductor_instructionIndex;

typedef enum {
  CONDUCTOR_INSTRUCTION_END = 0,
  CONDUCTOR_INSTRUCTION_CAMERAX = 1
} conductor_instruction_type;

typedef struct {
  uint16_t type;
  uint16_t cameraX;
  uint16_t argument;
  int16_t (*callback)(uint16_t arg);
} conductor_instruction_t;

void
conductor_init(conductor_instruction_t* instructions);

void
conductor_process();

uint16_t
conductor_complete(void);

void
conductor_setScrollTarget(uint16_t target);
#endif
