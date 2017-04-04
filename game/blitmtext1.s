	include "includes.i"
	xdef _text_drawMaskedText8Blitter
	xref font

	
BLIT_LF_MINTERM		equ $ca		; cookie cut
BLIT_WIDTH_WORDS	equ 2		; blit 2 words to allow shifting
BLIT_WIDTH_BYTES	equ 4
FONT_HEIGHT		equ 8
FONT_WIDTH		equ 8
FONTMAP_WIDTH_BYTES	equ 32
_SCREEN_BIT_DEPTH	equ 5
_BITPLANE_WIDTH_BYTES	equ 320/8


	section .text
_text_drawMaskedText8Blitter:
	;; a0 - bitplane
	;; a1 - text
	;; d0 - xpos
	;; d1 - ypos	
	movem.l	d0-a6,-(sp)

	lea     CUSTOM,a6
	jsr     __hw_waitBlitter

	;; blitter config that is shared for every character
	move.w  #BC0F_SRCA|BC0F_SRCB|BC0F_SRCC|BC0F_DEST|BLIT_LF_MINTERM,d6 ; BLTCON0 value (masked version)
	move.w 	#FONTMAP_WIDTH_BYTES-BLIT_WIDTH_BYTES,BLTAMOD(a6)	; A modulo (only used for masked version)
	move.w 	#FONTMAP_WIDTH_BYTES-BLIT_WIDTH_BYTES,BLTBMOD(a6)	; B modulo
	move.w 	#_BITPLANE_WIDTH_BYTES-BLIT_WIDTH_BYTES,BLTCMOD(a6)	; C modulo
	move.w 	#_BITPLANE_WIDTH_BYTES-BLIT_WIDTH_BYTES,BLTDMOD(a6)	; D modulo
        mulu.w	#_BITPLANE_WIDTH_BYTES*_SCREEN_BIT_DEPTH,d1		; ypos bytes
	move.w	#$0000,BLTALWM(a6) 					; mask out extra word used for shifting
	move.w	#$ffff,BLTADAT(a6) 					; preload source mask so only BLTA?WM mask is used
	move.l	a1,a3							; character pointer
	move.l	#font,a5						; font pointer
	move.l	#fontMask,d7						; font mask pointer
	move.w	#FONTMAP_WIDTH_BYTES*_SCREEN_BIT_DEPTH*FONT_HEIGHT,d3 	; bytes per font line
.loop:
	clr.l	d2
	move.b	(a3)+,d2	; get next character
	cmp.b	#0,d2		; 0 terminates the string
	beq	.done
	move.l	a0,a4		; bitplane pointer
	bsr	DrawChar8	; draw it
	add.l	#FONT_WIDTH,d0	; increment the x position
	bra	.loop
.done:
	movem.l	(sp)+,d0-a6
	rts


	
DrawChar8:
	;; kills d2,d4,d5,a1,a2,a4
	;; d0  - xpos
	;; d1  - ypos bytes
	;; d2* - char
	;; d3  - bytes per font line
	;; d6  - bltcon0 value
	;; a4* - bitplane
	;; a5  - #font
	;; d7  - #fontMask

	sub.w	#' ',d2		; index = char - ' '
	move.w	d2,d5	
	lsr.w	#5,d5		; char / 32 = fontmap line
	andi.w	#$1f,d2		; char index in line (char index - start of line index)	
	add.l	#1,d5		; while we have a weird font image, ' ' starts on second line
	move.l	a5,a1		; #font
	mulu.w	d3,d5 		; d5 *= #FONTMAP_WIDTH_BYTES*_SCREEN_BIT_DEPTH*FONT_HEIGHT
	move.l	d7,a2		; #fontMask
	add.w	d5,a2		; add y offset in lines to fontMask address
	add.w	d5,a1		; add y offset in lines to font address
	add.w	d2,a1		; add offset into font
	add.l	d2,a2		; add offset into mask

.blitChar8:
	;; kills a4,d2,d4,d5
	;; d0 - xpos
	;; d1 - ypos bytes
	;; d2.0 - odd character = 1, even character = 0
	;; d3 - bytes per font line
	;; d6 - bltcon0 value
	;; a4 - display
	;; a1 - object
	;; a2 - mask	

 	move.l	d0,d4					; xpos
 	move.l	d0,d5					; xpos
	lsr.w	#3,d4					; d4 = xpos bytes
	
	jsr     __hw_waitBlitter
	
	btst	#0,d2					; check if odd or even char
	beq	.evenChar				;
.oddChar:
	subq	#8,d5					; offset the x position for the odd character
	move.w	#$00FF,BLTAFWM(a6)			; select the second (odd) character in the word
	subq	#1,a4					; move the destination pointer left by one byte
	bra	.continue
.evenChar:
	move.w	#$FF00,BLTAFWM(a6)			; select the first character in the word
.continue:
	;; this shift will give us the bits to shift (bits 0-3) in bits (12-15) of d5
	swap	d5					; d5 << 12
	lsr.l	#4,d5					; 	
	move.w	d5,BLTCON1(A6)				; set the shift bits 12-15, bits 00-11 cleared
	move.l 	a2,BLTAPTH(a6)				; mask bitplane
	move.l 	a1,BLTBPTH(a6)				; source bitplane		
	or.w	d6,d5					; d5 = BLTCON0 value
	move.w	d5,BLTCON0(a6)				; set minterm, dma channel and shift
	add.l 	d4,a4					; dest += XPOS_BYTES
	add.l	d1,a4					; dest += YPOS_BYTES
	move.l 	a4,BLTCPTH(a6) 				; background top left corner
	move.l 	a4,BLTDPTH(a6) 				; destination top left corner

	move.w 	#(FONT_HEIGHT*_SCREEN_BIT_DEPTH)<<6|(BLIT_WIDTH_WORDS),BLTSIZE(a6)	;rectangle size, starts blit
	rts

fontMask:
	incbin	"out/font8x8-mask.bin"	
