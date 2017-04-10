	include "includes.i"
	xdef _text_drawMaskedText8Blitter
	xdef _text_clearMaskedText8Blitter:
	xref font
	xdef _fontPtr
	
BLIT_LF_MINTERM		equ $ca		; cookie cut
BLIT_WIDTH_WORDS	equ 2		; blit 2 words to allow shifting
BLIT_WIDTH_BYTES	equ 4
FONT_HEIGHT		equ 8
FONT_WIDTH		equ 8
FONTMAP_WIDTH_BYTES	equ 32
_SCREEN_BIT_DEPTH	equ 5
FONT_BIT_DEPTH          equ 1
_BITPLANE_WIDTH_BYTES	equ 320/8
MODULO	                equ (_BITPLANE_WIDTH_BYTES-BLIT_WIDTH_BYTES)+(_BITPLANE_WIDTH_BYTES*(_SCREEN_BIT_DEPTH-1))
FONTMAP_MODULO		equ (FONTMAP_WIDTH_BYTES-BLIT_WIDTH_BYTES)+(FONTMAP_WIDTH_BYTES*(FONT_BIT_DEPTH-1))	

	section .text
_text_drawMaskedText8Blitter:
	;; a0 - bitplane
	;; a1 - text
	;; d0 - xpos
	;; d1 - ypos	
	movem.l	d0-d7/a0-a6,-(sp)

	lea	CUSTOM,a6
	jsr	__hw_waitBlitter

	;; blitter config that is shared for every character
	move.w  #BC0F_SRCA|BC0F_SRCB|BC0F_SRCC|BC0F_DEST|BLIT_LF_MINTERM,d6 ; BLTCON0 value (masked version)
	move.w 	#FONTMAP_MODULO,BLTAMOD(a6)	; A modulo (only used for masked version)
	move.w 	#FONTMAP_MODULO,BLTBMOD(a6)	; B modulo
	move.w 	#MODULO,BLTCMOD(a6)		; C modulo
	move.w 	#MODULO,BLTDMOD(a6)		; D modulo
        mulu.w	#_BITPLANE_WIDTH_BYTES*_SCREEN_BIT_DEPTH,d1		; ypos bytes
	move.w	#$0000,BLTALWM(a6) 					; mask out extra word used for shifting
	move.w	#$ffff,BLTADAT(a6) 					; preload source mask so only BLTA?WM mask is used
	move.l	a1,a3							; character pointer
	move.l	#font,a5						; font pointer
	move.l	#font,d7						; font mask pointer
	move.w	#FONTMAP_WIDTH_BYTES*FONT_HEIGHT,d3 	; bytes per font line
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
	movem.l	(sp)+,d0-d7/a0-a6
	rts


_text_clearMaskedText8Blitter:
	;; a0 - bitplane
	;; a1 - text
	;; d0 - xpos
	;; d1 - ypos	
	movem.l	d0-d7/a0-a6,-(sp)

	lea	CUSTOM,a6
	jsr	__hw_waitBlitter

	;; blitter config that is shared for every character
	move.w  #BC0F_SRCA|BC0F_SRCC|BC0F_DEST|BLIT_LF_MINTERM,d6 ; BLTCON0 value (masked version)
	move.w 	#FONTMAP_MODULO,BLTAMOD(a6)	; A modulo (only used for masked version)
	move.w 	#FONTMAP_MODULO,BLTBMOD(a6)	; B modulo
	move.w 	#MODULO,BLTCMOD(a6)		; C modulo
	move.w 	#MODULO,BLTDMOD(a6)		; D modulo
	move.w  #0,BLTBDAT(a6)
        mulu.w	#_BITPLANE_WIDTH_BYTES*_SCREEN_BIT_DEPTH,d1		; ypos bytes
	move.w	#$0000,BLTALWM(a6) 					; mask out extra word used for shifting
	move.w	#$ffff,BLTADAT(a6) 					; preload source mask so only BLTA?WM mask is used
	move.l	a1,a3							; character pointer
	move.l	#font,a5						; font pointer
	move.l	#font,d7						; font mask pointer
	move.w	#FONTMAP_WIDTH_BYTES*FONT_HEIGHT,d3 	; bytes per font line
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
	movem.l	(sp)+,d0-d7/a0-a6
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

	jsr	__hw_waitBlitter
	
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

	move.w 	#(FONT_HEIGHT)<<6|(BLIT_WIDTH_WORDS),BLTSIZE(a6)	;rectangle size, starts blit
	rts


CharAddress:	macro
	dc.l	font+(((\1)/FONTMAP_WIDTH_BYTES)*(FONT_HEIGHT*FONTMAP_WIDTH_BYTES*FONT_BIT_DEPTH))+((\1)-(((\1)/FONTMAP_WIDTH_BYTES)*FONTMAP_WIDTH_BYTES))
	endm

	xdef _fontAtlas
_fontAtlas			;
fontAtlas:
	CharAddress    0
	CharAddress    1
	CharAddress    2
	CharAddress    3
	CharAddress    4
	CharAddress    5
	CharAddress    6
	CharAddress    7
	CharAddress    8
	CharAddress    9
	CharAddress    10
	CharAddress    11
	CharAddress    12
	CharAddress    13
	CharAddress    14
	CharAddress    15
	CharAddress    16
	CharAddress    17
	CharAddress    18
	CharAddress    19
	CharAddress    20
	CharAddress    21
	CharAddress    22
	CharAddress    23
	CharAddress    24
	CharAddress    25
	CharAddress    26
	CharAddress    27
	CharAddress    28
	CharAddress    29
	CharAddress    30
	CharAddress    31
	CharAddress    32
	CharAddress    33
	CharAddress    34
	CharAddress    35
	CharAddress    36
	CharAddress    37
	CharAddress    38
	CharAddress    39
	CharAddress    40
	CharAddress    41
	CharAddress    42
	CharAddress    43
	CharAddress    44
	CharAddress    45
	CharAddress    46
	CharAddress    47
	CharAddress    48
	CharAddress    49
	CharAddress    50
	CharAddress    51
	CharAddress    52
	CharAddress    53
	CharAddress    54
	CharAddress    55
	CharAddress    56
	CharAddress    57
	CharAddress    58
	CharAddress    59
	CharAddress    60
	CharAddress    61
	CharAddress    62
	CharAddress    63
	CharAddress    64
	CharAddress    65
	CharAddress    66
	CharAddress    67
	CharAddress    68
	CharAddress    69
	CharAddress    70
	CharAddress    71
	CharAddress    72
	CharAddress    73
	CharAddress    74
	CharAddress    75
	CharAddress    76
	CharAddress    77
	CharAddress    78
	CharAddress    79
	CharAddress    80
	CharAddress    81
	CharAddress    82
	CharAddress    83
	CharAddress    84
	CharAddress    85
	CharAddress    86
	CharAddress    87
	CharAddress    88
	CharAddress    89
	CharAddress    90
	CharAddress    91
	CharAddress    92
	CharAddress    93
	CharAddress    94
	CharAddress    95
	CharAddress    96
	CharAddress    97
	CharAddress    98
	CharAddress    99
	CharAddress    100
	CharAddress    101
	CharAddress    102
	CharAddress    103
	CharAddress    104
	CharAddress    105
	CharAddress    106
	CharAddress    107
	CharAddress    108
	CharAddress    109
	CharAddress    110
	CharAddress    111
	CharAddress    112
	CharAddress    113
	CharAddress    114
	CharAddress    115
	CharAddress    116
	CharAddress    117
	CharAddress    118
	CharAddress    119
	CharAddress    120
	CharAddress    121
	CharAddress    122
	CharAddress    123
	CharAddress    124
	CharAddress    125
	CharAddress    126
	CharAddress    127

	xdef _text_intToAscii
_text_intToAscii:
;;;  d0.l number
;;;  d2.l num chars
	movem.l d1-d2/a0-a1,-(a7)
	lea     staticBuffer,a0
	move.l  a0,a1
	add.l   d2,a0
	move.b  #0,(a0)
	moveq 	#10,d2
.loop:
	divu.w  d2,d0
	swap    d0
	addi.b  #"0",d0
	move.b  d0,-(a0)
	move.b  #0,d0
	swap    d0
	tst.w   d0
	bne.s   .loop
.loop2:
	cmp.l   a0,a1
	beq     .done
	move.b  #'0',-(a0)
	bra     .loop2
.done:
	move.l	#staticBuffer,d0
	movem.l (a7)+,d1-d2/a0-a1
        rts


staticBuffer:
        dc.b    "000000000"
        dc.b    0

	if TRACKLOADER=0
	section data_c
	endif
font:
	incbin	"out/font8x8.bin"
_fontPtr:
	dc.l	font
