	include "includes.i"
	xdef font
	xdef _fontPtr

FONT_HEIGHT		equ 8
FONT_WIDTH		equ 8
FONTMAP_WIDTH_BYTES	equ 32
_SCREEN_BIT_DEPTH	equ 5
FONT_BIT_DEPTH          equ 1
_BITPLANE_WIDTH_BYTES	equ 320/8	
	
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

	section data_c
	xdef _text_fontData
_text_fontData:	
font:
	incbin	"out/font8x8.bin"
_fontPtr:
	dc.l	font
