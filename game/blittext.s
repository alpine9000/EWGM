	include "includes.i"
	xdef _text_drawText8Blitter
	xdef _text_intToAscii
	xref _fontAtlas
	;; xref _gfx_dyOffsetsLUT
	xdef font
	
BLIT_LF_MINTERM		equ $ca		; cookie cut
BLIT_WIDTH_WORDS	equ 2		; blit 2 words to allow shifting
BLIT_WIDTH_BYTES	equ 4
FONT_HEIGHT		equ 8
FONT_WIDTH		equ 8
FONTMAP_WIDTH_BYTES	equ 32
_SCREEN_BIT_DEPTH	equ 5
FONT_BIT_DEPTH	        equ 1
_BITPLANE_WIDTH_BYTES	equ 256/8
MODULO	                equ (_BITPLANE_WIDTH_BYTES-BLIT_WIDTH_BYTES)+(_BITPLANE_WIDTH_BYTES*(_SCREEN_BIT_DEPTH-1))
FONTMAP_MODULO		equ (FONTMAP_WIDTH_BYTES-BLIT_WIDTH_BYTES)+(FONTMAP_WIDTH_BYTES*(FONT_BIT_DEPTH-1))
	
_text_drawText8Blitter:
	;; a0 - bitplane
	;; a1 - text
	;; d0 - xpos
	;; d1 - ypos	
	movem.l	d0-d3/a0-a2/a5-a6,-(sp)

	lea	CUSTOM,a6
	jsr	__hw_waitBlitter

	;; blitter config that is shared for every character
	move.w	#BC0F_SRCB|BC0F_SRCC|BC0F_DEST|BLIT_LF_MINTERM,d3 	; BLTCON0 value
	move.w 	#FONTMAP_MODULO,BLTBMOD(a6)	                        ; B modulo
	move.w 	#MODULO,BLTCMOD(a6)					; C modulo
	move.w 	#MODULO,BLTDMOD(a6)					; D modulo
	move.w	#$0000,BLTALWM(a6) 					; mask out extra word used for shifting
	move.w	#$ffff,BLTADAT(a6) 					; preload source mask so only BLTA?WM mask is used

	
	move.l	#font,a5						; font pointer
	mulu.w	#_BITPLANE_WIDTH_BYTES*_SCREEN_BIT_DEPTH,d1		; d1 = ypos bytes
	;; 	mulu.w #_BITPLANE_WIDTH_BYTES,d1
	;; 	lea 	_gfx_dyOffsetsLUT,a2
	;; 	move.w  (a2,d1.l),d1

	move.l	a0,a2							; bitplane pointer	
	adda.w	d1,a2							; dest += YPOS_BYTES	
 	move.w	d0,d1							; xpos
	lsr.w	#3,d1							; d2 = xpos bytes
	adda.w 	d1,a2							; dest += XPOS_BYTES

.loop:
	moveq	#0,d1
	move.b	(a1)+,d1				; get next character
	cmp.b	#0,d1					; 0 terminates the string
	beq	.done					; finished!
	lsl.w	#2,d1					; font atlas index = char * 4		
 	move.l	d0,d2					; xpos
	
	jsr	__hw_waitBlitter
	
	btst	#2,d1					; check if odd or even char
	beq	.evenChar				;
.oddChar:
	subq	#8,d2					; offset the x position for the odd character
	move.w	#$00FF,BLTAFWM(a6)			; select the second (odd) character in the word
	subq	#1,a2					; move the destination pointer left by one byte
	move.l 	a2,BLTCPTH(a6) 				; background top left corner
	move.l 	a2,BLTDPTH(a6) 				; destination top left corner
	addq	#1,a2
	bra	.continue
.evenChar:
	move.w	#$FF00,BLTAFWM(a6)			; select the first character in the word
	move.l 	a2,BLTCPTH(a6) 				; background top left corner
	move.l 	a2,BLTDPTH(a6) 				; destination top left corner	
.continue:

	;; this shift will give us the bits to shift (bits 0-3) in bits (12-15) of d2
	swap	d2					; d2 << 12
	lsr.l	#4,d2					;

	move.w	d2,BLTCON1(A6)				; set the shift bits 12-15, bits 00-11 cleared
	move.l 	_fontAtlas(pc,d1.w),BLTBPTH(a6)		; source bitplane		
	or.w	d3,d2					; d2 = BLTCON0 value
	move.w	d2,BLTCON0(a6)				; set minterm, dma channel and shift
	;; 	move.w 	#(FONT_HEIGHT*_SCREEN_BIT_DEPTH)<<6|(BLIT_WIDTH_WORDS),BLTSIZE(a6)	;rectangle size, starts blit
	move.w 	#(FONT_HEIGHT)<<6|(BLIT_WIDTH_WORDS),BLTSIZE(a6)	;rectangle size, starts blit	

	add.l	#FONT_WIDTH,d0	; increment the x position
	addq	#1,a2		; increment the dest buffer pointer
	bra	.loop
.done:
	movem.l	(sp)+,d0-d3/a0-a2/a5-a6
	rts


CharAddress:	macro
	dc.l	font+(((\1)/FONTMAP_WIDTH_BYTES)*(FONT_HEIGHT*FONTMAP_WIDTH_BYTES*FONT_BIT_DEPTH))+((\1)-(((\1)/FONTMAP_WIDTH_BYTES)*FONTMAP_WIDTH_BYTES))
	endm

	
