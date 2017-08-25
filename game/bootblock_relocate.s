	section .text
	align	1
	xdef	_relocateSection
_relocateSection:
	move.l a2,-(sp)
	move.l d2,-(sp)
	move.l (a1)+,d2
	moveq #0,d1
.L2:
	cmp.l d1,d2
	bne.w .L3
	move.l (sp)+,d2
	move.l (sp)+,a2
	rts
.L3:
	move.l a0,a2
	add.l (a1)+,a2
	add.l d0,(a2)
	addq.l #1,d1
	bra.w .L2
	align	1
	xdef	_relocate
_relocate:
	movem.l d0-a6,-(sp)
	move.l a0,a2
	move.l a1,d3
	move.l #_startFastRam,d2
	move.l #_endFastRam+512,d1
	sub.l d2,d1
	moveq #9,d0
	asr.l d0,d1
	neg.w d1
	ext.l d1
	move.l d2,d0
	moveq #9,d4
	lsr.l d4,d0
	bsr.w _LoadMFMB
	move.l #_startDisk,(a2)
	move.l #_endChipRam,d1
	sub.l #_startChipRam-512,d1
	asr.l d4,d1
	ext.l d1
	move.l #_startChipRam,d0
	lsr.l d4,d0
	move.l d3,a0
	bsr.w _LoadMFMB
	lea _relocateFast,a1
	sub.l d2,a1
	move.l a2,d0
	lea (a2,a1.l),a1
	move.l a2,a0
	bsr.w _relocateSection
	lea _relocateChip,a1
	sub.l d2,a1
	move.l d3,d0
	lea (a2,a1.l),a1
	move.l a2,a0
	bsr.w _relocateSection
	movem.l (sp)+,d0-a6
	rts
