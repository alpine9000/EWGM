	include "includes.i"

	if TRACKLOADER=1
	if FASTRAM=0
byteMap:
	dc.l	Entry
	dc.l	endCode-byteMap
	endif
	endif

	if TRACKLOADER=0
	section .text
	include "wbstartup.i"		; does nothing if TRACKLOADER=1
	endif

	if FASTRAM=1	
	section .entry
	xdef _disk_dataStart
_disk_dataStart:
	dc.l	0
	xdef _hw_fastRamStart
_hw_fastRamStart:
	dc.l	0
	endif
Entry:
	if TRACKLOADER=0
	jmp 	StartupFromOS
	else
	lea	userstack,a7	
	endif

	jmp 	Main
Main:
	jsr	_init_amiga
	jsr	_game_loop

	if TRACKLOADER=0
QuitGame:
	IntsOff
	jsr	_hw_waitVerticalBlank		
	movem.l	d0-a6,-(sp)
	jsr	P61_End
	movem.l	(sp)+,d0-a6
	jmp	LongJump
	endif

	if 0
	cnop    0,4	
__memset:	
	movem.l .l387,-(a7)
	move.l  (8+.l389,a7),d1
	move.l  (12+.l389,a7),d0
	move.l  (4+.l389,a7),a1
	tst.l   d0
	beq     .l371
	move.l  a1,a0
.l370
	move.b  d1,(a0)+
	subq.l  #1,d0
	bne     .l370
.l371
	move.l  a1,d0
.l387   reg
.l389   equ     0
        rts
	else
	xdef _memset	
	cnop    0,4
_memset:
	movem.l	a1,-(sp)
	movem.l	l383,-(a7)
	move.l	a0,a1
	tst.l	d1
	beq	l367
l366
	move.b	d0,(a0)+
	subq.l	#1,d1
	bne	l366
l367
	move.l	a1,d0
l383	reg
l385	equ	0
	movem.l	(sp)+,a1
	rts
	endif
	
	section .text
	align 4
	include "os.i"

	if TRACKLOADER=1
	section disk
	cnop 0,512	
	else
	section data
	endif	
	
	xdef _scoreBoardBitplanes
_scoreBoardBitplanes:
	incbin  "out/scoreboard.bin"	

	if TRACKLOADER=1
	section disk
	cnop 0,512	
	else
	section data
	endif
	
	xdef _menu_menuBitplanes
	xdef _menu_menuBitplanesEnd
_menu_menuBitplanes:	
	ifd GAME_COMPRESS_DATA
	incbin	"out/menu.bin.lz"
	else
	incbin	"out/menu.bin"	
	endif
_menu_menuBitplanesEnd:

	if TRACKLOADER=1
	section disk
	cnop 0,512	
	else
	section data
	endif	
	
	xdef _player_selectBitplanes
	xdef _player_selectBitplanesEnd
_player_selectBitplanes:	
	ifd GAME_COMPRESS_DATA
	incbin	"out/player_select.bin.lz"
	else
	incbin	"out/player_select.bin"	
	endif
_player_selectBitplanesEnd:
	
	if TRACKLOADER=1
	cnop 0,512
	endif
	xdef _logo_logoBitplanes
	xdef _logo_logoBitplanesEnd
_logo_logoBitplanes:
	ifd GAME_COMPRESS_DATA
	incbin   "out/logo.bin.lz"
	else
	incbin   "out/logo.bin"	
	endif
_logo_logoBitplanesEnd:	
	if TRACKLOADER=1
	cnop 0, 512
	endif
	
	data_c
	align 4
	if SFX==1
	xdef _sound_Bud_Punch01
_sound_Bud_Punch01:	
	incbin "out/Bud_Punch01.raw"	
	xdef _sound_enemy01
_sound_enemy01:	
	incbin "out/enemy01.raw"
	
	xdef _sound_Terence_punch01
_sound_Terence_punch01:	
	incbin "out/Terence_punch01.raw"
	xdef _sound_enemy02
_sound_enemy02:	
	incbin "out/enemy02.raw"	

	xdef _sound_Terence_punch02
_sound_Terence_punch02:	
	incbin "out/Terence_punch02.raw"
	xdef _sound_enemy03
_sound_enemy03:	
	incbin "out/enemy03.raw"

	xdef _sound_coin
_sound_coin:	
	incbin "out/coin1.raw"
	
	xdef _sound_pop
_sound_pop:	
	incbin "out/pop.raw"
	endif
	

	if TRACKLOADER=1
	align 4
	if FASTRAM=1
	section stack
	endif
	xdef _startUserstack
_startUserstack:
startUserstack:
	ds.b	1000
	xdef _userstack
_userstack:
userstack:
	endif
	end
