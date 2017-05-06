#ifndef __HW_H
#define __HW_H

#define JOYSTICK_POS_IDLE      0
#define JOYSTICK_POS_UP        1
#define JOYSTICK_POS_LEFT      7
#define JOYSTICK_POS_RIGHT     3
#define JOYSTICK_POS_DOWN      5
#define JOYSTICK_POS_UPLEFT    8
#define JOYSTICK_POS_UPRIGHT   2
#define JOYSTICK_POS_DOWNLEFT  6
#define JOYSTICK_POS_DOWNRIGHT 4


#ifdef GAME_BLITTER_WAIT_DEBUG
#define hw_waitBlitter()  _hw_debugWaitBlitter()
void 
_hw_debugWaitBlitter(void);
#else
extern void _hw_waitBlitter();
#define hw_waitBlitter()  _hw_waitBlitter();
#endif

uint32_t
hw_getsp(void);
void 
hw_waitVerticalBlank(void);
void
hw_setupPalette(void);
#if TRACKLOADER==1
void 
hw_interruptsInit(void);
#else
#define hw_interruptsInit hw_interruptsGameInit
#endif
void
hw_interruptsGameInit(void);
void 
hw_interruptsGameInit(void);
void 
hw_waitRaster(__REG("d0", uint32_t));
void 
hw_waitScanLines(__REG("d2", uint32_t));
void 
hw_readJoystick(void);
void 
hw_readJoystick2(void);
void 
hw_waitForJoystick(void);
uint32_t
hw_getRasterLine(void);

extern volatile uint8_t hw_lastJoystickButton;
extern volatile uint8_t hw_joystickButton;
extern volatile uint8_t hw_lastJoystick2Button;
extern volatile uint8_t hw_joystick2Button;
extern volatile uint8_t hw_joystickPos;
extern volatile uint8_t hw_joystick2Pos;
extern volatile uint32_t hw_verticalBlankCount;

#define JOYSTICK_BUTTON_DOWN (hw_joystickButton&0x1)

#define JOYSTICK_IDLE() (hw_joystickPos == 0)
#define JOYSTICK_LEFT() (hw_joystickPos == 7)
#define JOYSTICK_RIGHT() (hw_joystickPos == 3)
#define JOYSTICK_UP() (hw_joystickPos == 1)
#define JOYSTICK_DOWN() (hw_joystickPos == 5)

#endif
