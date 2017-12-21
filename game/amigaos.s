	xdef _amigaos_OpenLibrary
_amigaos_OpenLibrary:
	movem.l d1-a6,-(sp)
	jsr -552(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_CloseLibrary
_amigaos_CloseLibrary:
	movem.l d1-a6,-(sp)
	jsr -414(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_Open
_amigaos_Open:
	movem.l d1-a6,-(sp)
	jsr -30(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_Close
_amigaos_Close:
	movem.l d1-a6,-(sp)
	jsr -36(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_Write
_amigaos_Write:
	movem.l d1-a6,-(sp)
	jsr -48(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_Read
_amigaos_Read:
	movem.l d1-a6,-(sp)
	jsr -42(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_Delay
_amigaos_Delay:
	movem.l d1-a6,-(sp)
	jsr -198(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_CurrentDir
_amigaos_CurrentDir:
	movem.l d1-a6,-(sp)
	jsr -126(a6)
	movem.l (sp)+,d1-a6
	rts
