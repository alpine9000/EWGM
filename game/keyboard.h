#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEYBOARD_CODE_BACKSPACE 65
#define KEYBOARD_CODE_RETURN    68
#define KEYBOARD_CODE_ESC       27
extern uint16_t keyboard_code;
extern uint16_t keyboard_lastCode;
extern uint16_t keyboard_key;
extern uint16_t keyboard_lastKey;
void
keyboard_ctor(void);

void
keyboard_dtor(void);

void
keyboard_read(void);

#endif
