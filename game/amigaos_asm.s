
	if TRACKLOADER==0
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

	xdef _amigaos_OpenDevice
_amigaos_OpenDevice:
	movem.l d1-a6,-(sp)
	jsr -444(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_AllocSignal
_amigaos_AllocSignal:
	movem.l d1-a6,-(sp)
	jsr -330(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_AllocMem
_amigaos_AllocMem:
	movem.l d1-a6,-(sp)
	jsr -198(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_FreeSignal
_amigaos_FreeSignal:
	movem.l d1-a6,-(sp)
	jsr -336(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_AddPort
_amigaos_AddPort:
	movem.l d1-a6,-(sp)
	jsr -354(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_FindTask
_amigaos_FindTask:
	movem.l d1-a6,-(sp)
	jsr -294(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_RemPort
_amigaos_RemPort:
	movem.l d1-a6,-(sp)
	jsr -360(a6)
	movem.l (sp)+,d1-a6
	rts


	xdef _amigaos_FreeMem
_amigaos_FreeMem:
	movem.l d1-a6,-(sp)
	jsr -210(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_AbortIO
_amigaos_AbortIO:
	movem.l d1-a6,-(sp)
	jsr -480(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_WaitIO
_amigaos_WaitIO:
	movem.l d1-a6,-(sp)
	jsr -474(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_DoIO
_amigaos_DoIO:
	movem.l d1-a6,-(sp)
	jsr -456(a6)
	movem.l (sp)+,d1-a6
	rts

	xdef _amigaos_SendIO
_amigaos_SendIO:
	movem.l d1-a6,-(sp)
	jsr -462(a6)
	movem.l (sp)+,d1-a6
	rts
	endif
