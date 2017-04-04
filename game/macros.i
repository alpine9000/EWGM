IntsOff:	macro
	move.l	a6,-(sp)
	lea	CUSTOM,a6
	move	#$7fff,INTENA(a6) 	; disable all interrupts
	move.w	#$7fff,INTREQ(a6)	; clear all requests 
	move.w	#$7fff,INTREQ(a6)	; clear all requests 
	move.l	(sp)+,a6
	endm
IntsOn:		macro
	move.l	a6,-(sp)
	lea	CUSTOM,A6
	move.w	#(INTF_SETCLR|INTF_VERTB|INTF_INTEN),INTENA(a6)			
	move.l	(sp)+,a6
	endm

P61Module: macro
	cnop	0,512	
diskmodule\1:
	incbin	\2
	cnop	0,512
enddiskmodule\1:
	endm