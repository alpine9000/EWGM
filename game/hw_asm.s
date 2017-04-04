	include "includes.i"

	xdef __hw_waitBlitter
	xdef _hw_waitVerticalBlank
	xdef _hw_waitRaster
	xdef _hw_readJoystick
	xdef _hw_readJoystick2
	xdef _hw_joystickButton
	xref _hw_lastJoystickButton
	xdef _hw_joystick2Button
	xref _hw_lastJoystick2Button	
	xdef _hw_joystickPos
	xdef _hw_joystick2Pos		
	xdef _hw_interruptsInit
	xdef _hw_waitScanLines
	xdef _hw_getRasterLine
	xdef _hw_verticalBlankCount

	section .text
ciaa_pra  = $bfe001
_joy0dat   = $dff00a
_joy1dat   = $dff00c	
potgor    = $dff016
bit_joyb1 = 7
bit_joyb2 = 14

__hw_waitBlitter:
	move.l	a6,-(sp)
	lea 	CUSTOM,a6
	tst 	DMACONR(a6)		;for compatibility
.waitblit:
	btst 	#6,DMACONR(a6)
	bne.s 	.waitblit
	move.l	(sp)+,a6
	rts

_hw_waitVerticalBlank:	
	movem.l	d0,-(sp)
.loop:
	move.l	$dff004,d0
	and.l	#$1ff00,d0
	cmp.l	#(285)<<8,d0	; wait for the scan line
	bne.b	.loop
.loop2:
	move.l	$dff004,d0
	and.l	#$1ff00,d0
	cmp.l	#(285)<<8,d0	; wait for the scan line to pass (A4000 is fast!)
	beq.b	.loop2
	movem.l (sp)+,d0
	rts

_hw_getRasterLine:
	move.l	$dff004,d0
	and.l	#$1ff00,d0
	lsr.l	#8,d0
	rts

_hw_waitRaster:		;wait for rasterline d0.w. Modifies d0-d2/a0.
	movem.l d0-a6,-(sp)
	move.l #$1ff00,d2
	lsl.l #8,d0
	and.l d2,d0
	lea $dff004,a0
.wr:
	move.l (a0),d1
	and.l d2,d1
	cmp.l d1,d0
	bne.s .wr
	movem.l (sp)+,d0-a6
	rts

WaitScanLines:	 macro
	        if \1 != 0
	        lea     $dff006,a0
	        move.w  #\1-1,d2
	.\@nTimes:
	        move.w  (a0),d0
	        lsr.w   #8,d0
	.\@loop:
	        move.w  (a0),d1
	        lsr.w   #8,d1
	        cmp.w   d0,d1
	        beq     .\@loop
	        dbra    d2,.\@nTimes
	.\@done:
	        endif
	        endm
	
	
_hw_waitScanLines:
	movem.l	d0-d2/a0,-(sp)
	lea     $dff006,a0
.nTimes:
        move.w  (a0),d0
        lsr.w   #8,d0
.loop:
        move.w  (a0),d1
        lsr.w   #8,d1
        cmp.w   d0,d1
        beq     .loop
        dbra    d2,.nTimes
.done:
	movem.l	(sp)+,d0-d2/a0
	rts	
	
_hw_readJoystick:
	;; updates the joystick variables to contina the state of the buttons in bits 8 and 9,
        ;; and the lower byte holds the direction of the stick: 
	movem.l	d0/d1,-(sp)	
	btst    #bit_joyb2&7,potgor
        seq     d0
	add.w   d0,d0
	btst    #bit_joyb1,ciaa_pra
        seq     d0
	add.w   d0,d0
	move.w  _joy1dat,d1
	ror.b   #2,d1
	lsr.w   #6,d1
	and.w   #%1111,d1
	move.b	(.conv,pc,d1.w),d0
	move.w  _hw_joystickButton,_hw_lastJoystickButton
	move.w	d0,_hw_joystickButton
	movem.l (sp)+,d0/d1
        rts
.conv:
        dc.b      0,5,4,3,1,0,3,2,8,7,0,1,7,6,5,0

_hw_readJoystick2:
	;; updates the joystick variables to contina the state of the buttons in bits 8 and 9,
        ;; and the lower byte holds the direction of the stick: 
	movem.l	d0/d1,-(sp)	
	btst    #bit_joyb2&7,potgor
        seq     d0
	add.w   d0,d0
	btst    #bit_joyb1,ciaa_pra
        seq     d0
	add.w   d0,d0
	move.w  _joy0dat,d1
	ror.b   #2,d1
	lsr.w   #6,d1
	and.w   #%1111,d1
	move.b	(.conv,pc,d1.w),d0
	move.w  _hw_joystick2Button,_hw_lastJoystick2Button
	move.w	d0,_hw_joystick2Button
	movem.l (sp)+,d0/d1
        rts
.conv:
        dc.b      0,5,4,3,1,0,3,2,8,7,0,1,7,6,5,0

	
_hw_interruptsInit:
	movem.l	a0/a3/a6,-(sp)
	lea	CUSTOM,a6	
	lea	Level3InterruptHandler,a3
	move.l	vectorBase,a0
 	move.l	a3,LVL3_INT_VECTOR(a0)
	move.w	#(INTF_SETCLR|INTF_VERTB|INTF_INTEN),INTENA(a6)
	movem.l	(sp)+,a0/a3/a6
	rts

Level3InterruptHandler:
	movem.l	d0-a6,-(sp)
	lea	CUSTOM,a6
.checkVerticalBlank:
	move.w	INTREQR(a6),d0
	and.w	#INTF_VERTB,d0	
	beq	.checkCopper

.verticalBlank:
	move.w	#INTF_VERTB,INTREQ(a6)	; clear interrupt bit	
	add.l	#1,_hw_verticalBlankCount
	move.w	_P61_Target,d0
	cmp.w   P61_Master,d0
	beq	.ok
	blt	.lowerVolume
	add.w	#1,P61_Master
	bra	.ok
.lowerVolume:
	sub.w	#1,P61_Master
	bra	.ok
.ok:
	cmp.w	#0,P61_Master
	bne	.playMusic
	move.w  #0,AUD0VOL(a6)
	move.w  #0,AUD1VOL(a6)
	move.w  #0,AUD2VOL(a6)
	bra	.checkCopper
.playMusic:
	jsr	P61_Music
.checkCopper:
	move.w	INTREQR(a6),d0
	and.w	#INTF_COPER,d0	
	beq.s	.interruptComplete
.copperInterrupt:
	move.w	#INTF_COPER,INTREQ(a6)	; clear interrupt bit	
	
.interruptComplete:
	movem.l	(sp)+,d0-a6
	rte

	align 4

_hw_lastJoystickButton
	dc.w	0
_hw_lastJoystick2Button
	dc.w	0	
_hw_joystickButton:
	dc.b	0
_hw_joystickPos:
	dc.b	0
_hw_joystick2Button:
	dc.b	0	
_hw_joystick2Pos:
	dc.b	0	
_hw_verticalBlankCount:
	ds.l	0


	
