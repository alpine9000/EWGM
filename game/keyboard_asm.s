        ;;;  take from http://eab.abime.net/showthread.php?t=71099
	include hardware/cia.i
	include hardware/intbits.i
	include ../include/registers.i
	xdef _keyboard_ctor
	xdef _keyboard_dtor
	xdef _keyboard_keymap
	xdef _keyboard_code
	
	section .text
_keyboard_ctor:
	movem.l	d0-a6,-(a7)
	move.l	vectorBase,a0
	move.l	$68(a0),oldint
	move.b	#CIAICRF_SETCLR|CIAICRF_SP,(ciaicr+$bfe001)
	;; clear all ciaa-interrupts
	tst.b	(ciaicr+$bfe001)
	;; set input mode
	and.b	#~(CIACRAF_SPMODE),(ciacra+$bfe001)
	;; clear ports interrupt
	move.w	#INTF_PORTS,(INTREQR+$dff000)
	;; allow ports interrupt
	move.l	#kbint,$68(a0)
	move.w	#INTF_SETCLR|INTF_INTEN|INTF_PORTS,(intena+$dff000)
	movem.l	(a7)+,d0-a6
	rts

_keyboard_dtor:
	movem.l	d0-a6,-(a7)
	move.l	vectorBase,a0
	move.w	#INTF_SETCLR|INTF_PORTS,(intena+$dff000)
	move.l	oldint,$68(a0)
	movem.l	(a7)+,d0-a6
	rts

kbint:
	movem.l	d0-d1/a0-a2,-(a7)
	
	lea	$dff000,a0
	move.w	intreqr(a0),d0
	btst	#INTB_PORTS,d0
	beq	.end

	lea	$bfe001,a1
	btst	#CIAICRB_SP,ciaicr(a1)
	beq	.end

	;; read key and store him
	move.b	ciasdr(a1),d0
	or.b	#CIACRAF_SPMODE,ciacra(a1)
	not.b	d0
	ror.b	#1,d0
	spl	d1
	and.w	#$7f,d0
	;; lea	keys(pc),a2
	;; l	move.b	d1,(a2,d0.w)

	cmp.w	_keyboard_code,d0
	beq	.same
	move.w  d0,_keyboard_code
	bra	.diff
.same
	move.w  #0,_keyboard_code
.diff
	;; clr.w	$100

	;; handshake
	moveq	#3-1,d1
.wait1:
	move.b	vhposr(a0),d0
.wait2:
	cmp.b	vhposr(a0),d0
	beq	.wait2
	dbf	d1,.wait1

	;; set input mode
	and.b	#~(CIACRAF_SPMODE),ciacra(a1)

.end:
	move.w	#INTF_PORTS,intreq(a0)
	tst.w	intreqr(a0)
	movem.l	(a7)+,d0-d1/a0-a2
	rte

	align 	4
_keyboard_code:
	dc.l 	0
oldint:
	dc.l	0
_keyboard_keymap:
	dc.b    0,"1234567890-=",$5c,0,"0" ; $00-$0f
	dc.b    "QWERTYUIOP[]",0,"123"	   ; $10-$1f
	dc.b    "ASDFGHJKL;'",0,0,"456"    ; $20-$2f
	dc.b    "<ZXCVBNM,./",0,".789"	   ; $30-$3f
	dc.b    ' ',8,0,10,10,27,8,0	   ; $40-$47

	align 4