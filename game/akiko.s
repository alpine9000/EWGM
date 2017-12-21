	include graphics/gfxbase.i
	if TRACKLOADER=0
Execbase           = 4
Openlibrary        = -552
AKIKO_BASE         = $B80000
AKIKO_INTENA       = $B80008
AKIKO_INTREQ       = $B80004  ; read only
AKIKO_TRANSFER_REQ = $B80020    ; not sure of the name of the register
AKIKO_DMA          = $B80024
AKIKO_DMA_STOP     = $13ffffff   ; clear bits 31, 30, 29, 27, 26 mask
AKIKO_ID           = $C0CACAFE

	xdef akiko_present
akiko_present:
	movem.l	d1-a6,-(sp)
	move.l	(Execbase).w,a6		;Locate exec base
	lea	akiko_gfx_lib(pc),a1		;Locate ASCII string
	moveq	#40,d0			;Specifically v40 library
	jsr	Openlibrary(a6)		;Open library
	beq.s	.lib_error		;If NULL then not v40 library
	move.l	d0,a6			;Move pointer into A6
	move.l	gb_HWEmul(a6),d0	;gb_ChunkytoPlanarPtr(a6),d0
					;Attempt to see if Akiko present
	beq.s	.no_akiko_present
	moveq	#1,d0
	bra	.done
.no_akiko_present:
.lib_error:
	moveq	#0,d0
.done:				;
	move.l	d0,_akiko_present
	movem.l	(sp)+,d1-a6
	rts

	xdef _akiko_ctor
_akiko_ctor:
	cmp.l	#1,_akiko_present
	;; 	cmp.l   #AKIKO_ID,AKIKO_BASE
        bne.b   .noakiko
	move.l  AKIKO_INTENA,akiko_saved_intena
        move.l  AKIKO_DMA,akiko_saved_dma

	and.l   #AKIKO_DMA_STOP,AKIKO_DMA
        move.l  #0,AKIKO_INTENA ; disable all Akiko CD interrupts
	move.w  #0,AKIKO_TRANSFER_REQ ; clear possible still active transfer complete interrupt request.
        move.l  #0,AKIKO_INTENA
        move.w  #0,AKIKO_TRANSFER_REQ ; clear
.noakiko:
	rts

	xdef _akiko_dtor
_akiko_dtor:
	;; 	cmp.l   #AKIKO_ID,AKIKO_BASE
	cmp.l	#1,_akiko_present
        bne.b   .noakiko
	move.l  akiko_saved_dma,AKIKO_DMA   ; enable Akiko DMA first
        move.l  akiko_saved_intena,AKIKO_INTENA ; enable all Akiko CD interrupts
        ; small empiric delay just in case...
        move.l  #100,D5
        bsr     readdelay_d5
.noakiko:
	rts

readdelay_d5:
	move.l	d5,-(sp)
.loop1:
        tst.l   d5
        beq.b   .nodelay        ; don't wait
        move.l  d5,-(a7)
	move.b      $dff006,d5
.loop2:
        cmp.b   $dff006,d5
        beq.s   .loop2
        move.l  (a7)+,d5
        subq.l  #1,d5
        bne.b   .loop1
.nodelay:
	move.l  (sp)+,d5
        rts

akiko_gfx_lib:
	dc.b	'graphics.library',0

	align 4
akiko_saved_intena:
	dc.l	1
akiko_saved_dma:
	dc.l	1

	xdef _akiko_present
_akiko_present:
	dc.l	1
	endif
