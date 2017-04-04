	xdef _P61_Init
	section .text
_P61_Init:
	movem.l	d0-a6,-(sp)
        sub.l   a1,a1
        sub.l   a2,a2
        moveq   #0,d0
	jsr     P61_Init
	movem.l	(sp)+,d0-a6
	rts