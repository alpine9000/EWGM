	include 'includes.i'
        xdef _palette_menuInstall:

	section .text

_palette_menuInstall:
	move.l	a6,-(sp)
	lea	CUSTOM,a6
	include	"out/menu-palette.s"
	move.l	(sp)+,a6
	rts