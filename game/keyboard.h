#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEYBOARD_CODE_BACKSPACE 65
#define KEYBOARD_CODE_RETURN    68

extern uint16_t keyboard_code;
extern uint16_t keyboard_lastCode;
extern uint16_t keyboard_key;

void
keyboard_ctor(void);

void
keyboard_dtor(void);

void
keyboard_read(void);

#endif
