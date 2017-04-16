#ifndef __RECORD_H
#define __RECORD_H

#ifdef GAME_RECORDING

#define RECORD_MAX_RECORD 1024

typedef struct {
  uint8_t joystickPos;
  uint8_t joystickButton;
  uint8_t joystick2Pos;
  uint8_t joystick2Button;  
  uint16_t frame;
  uint16_t key;
} record_item_t;

typedef enum {
  RECORD_IDLE = 0,
  RECORD_RECORD = 1,
  RECORD_PLAYBACK = 2
} record_state_t;

typedef struct {
  uint32_t size;
  record_state_t state;
  uint32_t index;
  uint32_t lastJoystickPos;
  uint32_t lastJoystickButton;
  uint32_t lastJoystick2Pos;
  uint32_t lastJoystick2Button;  
  uint16_t lastKey;
  uint8_t joystickPos;
  uint8_t joystickButton;
  uint8_t joystick2Pos;
  uint8_t joystick2Button;  
  uint16_t frame;
  record_item_t buffer[RECORD_MAX_RECORD];
} record_t;

void
record_setState(record_state_t state);
record_state_t
record_getState(void);
void
record_process(void);
void
record_showAddress(void);

#else

#define record_setState(x)
#define record_process()

#endif

#endif
