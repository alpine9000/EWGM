	if TRACKLOADER==1
	if PHOTON_TRACKLOADER=0
*
* Trackdisk read/write routines. Read and decrunch files.
* Define TDWRITE to include support for writing blocks (reading them first).
* Define TDFORMAT to include support for writing/formatting whole tracks.
*
* Written by Frank Wille in 2013.
*
* I, the copyright holder of this work, hereby release it into the
* public domain. This applies worldwide.
*
* td_init()
* d0=err = td_selectdisk(d0=diskID)
* d0=err = td_read(d0.w=firstBlock, d1.w=numBlocks, a0=destination)
* d0=err = td_write(d0.w=firstBlock, d1.w=numBlocks, a0=source) (TDWRITE)
* d0=err = td_format(d0.w=trackNum, a0=source, a4=checkSize) (TDFORMAT)
* td_motoroff()
*

	include "../include/registers.i"
	include	"../include/cia.i"

	if MUSIC_PARALLEL_LOAD==1
TrackBuffer     equ 	_music_spare_ptr
Directory	equ	_music_spare_ptr
	endif
;; TDWRITE         equ     1
TDFORMAT        equ     1		

; Port A bits (input)
DSKCHANGE	equ	2
DSKPROT		equ	3
DSKTRACK0	equ	4
DSKRDY		equ	5

; Port B bits (output)
DSKSTEP		equ	0
DSKDIREC	equ	1
DSKSIDE		equ	2
DSKSEL0		equ	3
DSKMOTOR	equ	7

; constants
NUM_TRACKS	equ	160
SECT_PER_TRK	equ	11
STEPDELAY	equ	64		; (* 64us) ca. 4ms  (minimum: 3ms)
SETTLEDELAY	equ	400		; (* 64us) ca. 25ms (minimum: 18ms)

MFM_BUFSIZE	equ	$1a00*2
MFM_READLEN	equ	$19f0		; in words
MFM_WRITELEN	equ	$1900		; in words
MFM_GAPSIZE	equ	$f0		; gap in words at track-start and end

NUM_RETRIES	equ	4		; retries on read error
DMA_TIMEOUT	equ	2000		; timeout after 2 seconds

; Do we need to write tracks?
WRITESUPPORT	set	0
		ifd	TDWRITE
WRITESUPPORT	set	1
		endif
		ifd	TDFORMAT
WRITESUPPORT	set	1
		endif

; Wait for DSKRDY
	macro	WAIT_DSKRDY
.\@:	btst	#DSKRDY,(a2)
	bne	.\@
	endm


	code


;---------------------------------------------------------------------------
	xdef	_td_init
_td_init:
	movem.l	d1-a6,-(sp)
	lea	CUSTOM,a6
	bsr 	td_init
	movem.l (sp)+,d1-a6
	rts
	
td_init:
; Initialize the trackloader and check for connected drives.

	movem.l	d2-d3/a2-a3,-(sp)
	lea	CIAA+CIAPRA,a2
	lea	CIAB+CIAPRB,a3

	; disable current disk DMA transfer, but enable disk DMA in general
	move.w	#$4000,DSKLEN(a6)
	move.w	#$8210,DMACON(a6)

	; disable DSKSYN and DSKBLK interrupts
	move.w	#$1002,INTENA(a6)

	; Motor off for all drives.
	or.b	#$f8,(a3)
	nop
	and.b	#$87,(a3)
	nop
	or.b	#$78,(a3)		; deselect all
	nop

	;--------------------------------------
	; Now check which drives are connected.
	;--------------------------------------

	moveq	#DSKSEL0+1,d2
	moveq	#1<<DSKSEL0,d3		; drive 0 is always present, no check

.1:	bclr	#DSKMOTOR,(a3)
	nop
	bclr	d2,(a3)
	nop
	bset	d2,(a3)
	nop
	bset	#DSKMOTOR,(a3)
	nop
	bclr	d2,(a3)
	nop
	bset	d2,(a3)

	; read drive's serial ID code
	moveq	#15,d1
	moveq	#0,d0
.2:	add.l	d0,d0
	bclr	d2,(a3)
	nop
	btst	#DSKRDY,(a2)
	bne	.3
	addq.b	#1,d0
.3:	bset	d2,(a3)
	dbf	d1,.2

	addq.w	#1,d0
	bne	.4

	; ID is $ffff: Amiga 3.5" DD disk drive present
	bset	d2,d3

.4:	addq.b	#1,d2		; next drive
	cmp.b	#DSKSEL0+3,d2
	bls	.1

	; remember drives found
	move.b	d3,DrivesPresent

	; get Chip RAM for the MFM buffer
	move.l	#MFM_BUFSIZE,d0
	;; bsr	alloc_chipmem
	;; move.l	d0,MFMbuffer
	move.l  #MFMbuf,MFMbuffer
	

	movem.l	(sp)+,d2-d3/a2-a3
	rts


;---------------------------------------------------------------------------
	xdef	_td_selectdisk

_td_selectdisk:	
	movem.l d1-a6,-(sp)
	lea     CUSTOM,a6
        bsr     td_selectdisk
        movem.l (sp)+,d1-a6
	rts
	
td_selectdisk:
; Required for all read/write operations on a disk!
; Check if any drive contains a disk with the given disk ID. The 32-bit ID
; is stored in the first disk block, at offset 8.
; Select this drive as the current one, when found. Read the directory
; from the second disk block (track 0, sector 1).
; Motor will keep running on success and turned off otherwise.
; d0 = disk ID
; -> d0/Z = ok

	movem.l	d2-d4/a2-a3/a5,-(sp)
	move.l	d0,d4			; d4 Disk-ID
	lea	CIAA+CIAPRA,a2
	lea	CIAB+CIAPRB,a3
	move.l	Directory,a5

	; Loop over all known drives, start with drive 0.
	moveq	#DSKSEL0,d2
	move.b	DrivesPresent,d3

.loop:
	btst	d2,d3
	beq	.next			; drive not present, skip it

	bclr	d2,(a3)			; select drive
	nop
	bclr	#DSKDIREC,(a3)
	bsr	td_step
	move.w	#SETTLEDELAY,d0
	bsr	td_delay
	bset	#DSKDIREC,(a3)
	bsr	td_step

	btst	#DSKCHANGE,(a2)
	bne	.checkdisk		; found a disk in this drive
	bset	d2,(a3)			; deselect

.next:
	addq.b	#1,d2			; next drive
	cmp.b	#DSKSEL0+3,d2
	bls	.loop

	; disk not found in any drive!
	clr.b	DriveSel
	moveq	#-1,d0			; return with error
	bra	.exit

.checkdisk:
	; deselect and remember the used drive bit
	bset	d2,(a3)
	move.b	d2,DriveSel

	; start motor for DriveSel, seek track 0 and get track into the buffer
	bsr	td_track0
	beq	.readblk0

	; read error, try the next drive
.readerr:
	bsr	td_motoroff
	bra	.next

	; read block 0 and check the ID field (track 0, sector 0, offset 8)
.readblk0:
	moveq	#0,d0
	moveq	#1,d1
	move.l	a5,a0
	bsr	td_read
	bne	.readerr

	cmp.l	8(a5),d4
	bne	.readerr

	; found the right disk, read its directory block (track 0, sector 1)
	moveq	#1,d0
	moveq	#1,d1
	move.l	a5,a0
	bsr	td_read
	bne	.readerr

.exit:
	movem.l	(sp)+,d2-d4/a2-a3/a5
	rts

;---------------------------------------------------------------------------
	xdef	_td_read
_td_read:	
        movem.l d1-a6,-(sp)
	lea     CUSTOM,a6
	bsr     td_read
	movem.l (sp)+,d1-a6
	rts
	
td_read:
; Read a sequence of blocks from disk. Turns on the motor automatically.
; Reread a whole track up to NUM_RETRIES times on data checksum errors.
; d0.w = start block
; d1.w = number of blocks
; a0 = destination buffer (number of blocks * 512 bytes long)
; -> d0/Z = error code (0=ok)

	movem.l	d2-d7/a2-a3/a5,-(sp)

	move.w	d0,d6			; d6: current block
	move.w	d1,d7			; d7: remaining blocks to read
	move.l	a0,a5			; a5: buffer
	move.l	#$55555555,d3		; d3: MFM mask

	lea	CIAA+CIAPRA,a2
	lea	CIAB+CIAPRB,a3

	moveq	#0,d0
	subq.w	#1,d7
	bmi	.exit			; no blocks to read

	bsr	td_motoron

.blk_loop:
	moveq	#NUM_RETRIES+1,d4	; d4: retries left
.seek_track:
	; seek track and read it into our MFM decoding buffer
	moveq	#0,d5
	move.w	d6,d5
	divu	#SECT_PER_TRK,d5
	cmp.w	CurrentTrk,d5
	beq	.decode_blk

	move.w	d5,d0
	bsr	td_seek

	bsr	td_trackread
	beq	.decode_blk		; track ok
	subq.w	#1,d4
	beq	.exit

.retry_track:
	; seek track 0, then reposition on target track and try again
	bsr	td_track0
	beq	.seek_track
	subq.w	#1,d4
	bne	.retry_track
	bra	.exit

.decode_blk:
	lea	SectorTab,a1
	swap	d5			; sector to decode
	add.w	d5,d5
	add.w	d5,d5
	move.l	(a1,d5.w),a0		; sector data start in MFM buffer

	; decode the block while calculating the checksum
	moveq	#0,d5
	lea	512(a0),a1
	moveq	#127,d2
.decode_loop:
	move.l	(a0)+,d0
	eor.l	d0,d5
	and.l	d3,d0
	move.l	(a1)+,d1
	eor.l	d1,d5
	add.l	d0,d0
	and.l	d3,d1
	or.l	d1,d0
	move.l	d0,(a5)+
	dbf	d2,.decode_loop

	; decode data block checksum and compare with our calculated one
	movem.l	-520(a0),d0/d1
	and.l	d3,d0
	and.l	d3,d1
	add.l	d0,d0
	or.l	d1,d0
	and.l	d3,d5
	cmp.l	d0,d5
	beq	.next_blk

	; Checksum error. Reread the track NUM_RETRIES times until
	; we bail out with a fatal error.
	lea	-512(a5),a5
	subq.w	#1,d4
	bne	.retry_track
	moveq	#8,d0			; data checksum error
	bra	.exit

.next_blk:
	addq.w	#1,d6
	dbf	d7,.blk_loop

	moveq	#0,d0
.exit:
	tst.b	d0

	ifd	DEBUG
	beq	.ok
	trap	#2			; invoke the debugger on read errors
.ok:
	endif

	movem.l	(sp)+,d2-d7/a2-a3/a5
	rts


;---------------------------------------------------------------------------
td_trackread:
; Transfer a track into the MFM buffer using DMA.
; Find all sectors in the buffer and remember their address in a table.
; a2 = CIAAPRA
; a3 = CIABPRB
; -> d0/Z = error code (0=ok, 1=unformatted, 2=missingSectors, 3=badHeader)

	movem.l	d2-d3/a2,-(sp)
	move.l	MFMbuffer,a0
	addq.l	#2,a0			; make room to restore missed SYNC

	;-----------------------------------------------------
	; Disk read DMA fetches a whole track into the buffer
	;-----------------------------------------------------

	WAIT_DSKRDY
	move.w	#$4000,DSKLEN(a6)

	; select MFM encoding and enable synchronization word (DSKSYNC)
	move.w	#$7200,ADKCON(a6)
	move.w	#$8500,ADKCON(a6)
	move.w	#$4489,DSKSYNC(a6)

	; start disk-read DMA
	move.l	a0,DSKPT(a6)
	move.w	#$8000|MFM_READLEN+1,DSKLEN(a6)
	move.w	#$8000|MFM_READLEN+1,DSKLEN(a6)

	; clear	old sector pointer table in the mean time
	lea	SectorTab,a1
	moveq	#0,d0
	moveq	#SECT_PER_TRK-1,d1
.1:	move.l	d0,(a1)+
	dbf	d1,.1

	bsr	wait_disk_dma
	bmi	.err_empty		; DMA timeout

	;-----------------------------------------------------------------
	; Decode sector headers and enter sector data pointers into table.
	;-----------------------------------------------------------------

	lea	SectorTab,a1
	lea	MFM_READLEN*2(a0),a2
	move.l	#$55555555,d2
	moveq	#SECT_PER_TRK-1,d3

	; restore the first SYNC, which is not written by the DMA
	move.w	#$4489,-(a0)

	; find one or two SYNCs in front of a header
.3:	cmp.l	a2,a0
	bhs	.err_no_sync		; no sync found, missing sector
	cmp.w	#$4489,(a0)+
	bne	.3
	cmp.w	#$4489,(a0)+
	beq	.4
	subq.l	#2,a0

	; decode the header's info field
.4:	movem.l	(a0),d0-d1
	and.l	d2,d0
	and.l	d2,d1
	add.l	d0,d0
	or.l	d1,d0

	ifd	DEBUG
	; verify the track number
	swap	d0
	cmp.b	CurrentTrk+1,d0
	bne	.err_wrongtrk
	swap	d0
	endif

	; use sector number in bits 8-15 as table index
	clr.b	d0
	lsr.w	#6,d0
	cmp.b	#SECT_PER_TRK<<2,d0
	bhs	.err_header

	tst.l	(a1,d0.w)
	beq	.5

	; sector already found in buffer, skip to the next one
	lea	56+1024(a0),a0
	bra	.3

	; enter sector data pointer into the table
.5:	lea	56(a0),a0
	move.l	a0,(a1,d0.w)

	; skip data, scan for next sector
	lea	1024(a0),a0
	dbf	d3,.3

	moveq	#0,d0			; ok, no error
	bra	.done
.err_empty:
	moveq	#1,d0			; timeout while waiting for disk-DMA
	bra	.done
.err_no_sync:
	moveq	#2,d0			; sector not found in MFM buffer
	bra	.done
.err_header:
	moveq	#3,d0			; illegal sector number in the header
	ifd	DEBUG
	bra	.done
.err_wrongtrk:
	moveq	#4,d0			; wrong track number in the header
	endif
.done:
	movem.l	(sp)+,d2-d3/a2
	rts


	ifd	TDFORMAT
;---------------------------------------------------------------------------
	xdef	_td_format

_td_format:	
	movem.l d1-a6,-(sp)
	lea     CUSTOM,a6
	bsr     td_format
	movem.l (sp)+,d1-a6
	rts
	
td_format:
; Write a whole track with new contents, without reading it first.
; Turns on the motor automatically.
; d0.w = track number
; a0 = source buffer (SECT_PER_TRACK * 512 bytes long)
; a4 = check size
; -> d0/Z = error code (0=ok)

	movem.l	d6-d7/a2-a3/a5,-(sp)

	lea	CIAA+CIAPRA,a2
	lea	CIAB+CIAPRB,a3
	move.l	a0,a5			; a5 buffer
	moveq	#NUM_RETRIES,d7
	move.w	d0,d6			; d6 track number

	; start the motor and seek the target track
	bsr	td_motoron
.seek:
	move.w	d6,d0
	bsr	td_seek

	; format this track with our buffer contents
	move.l	a5,a0
	bsr	td_trackwrite
	bne	.error

	;; VEGETA!!!!!  - verify caused issues, so we skip it for now....
	bra 	.exit
	if 0
	; verify the track
	bsr	td_trackread		; force rereading it to MFMBuffer
	bne	.error
	move.l	TrackBuffer,a0
	moveq	#SECT_PER_TRK,d0
	mulu	d6,d0
	moveq	#SECT_PER_TRK,d1
	bsr	td_read			; decode sectors
	bne	.error

	; compare with the original buffer
	move.l	TrackBuffer,a0
	move.l	a5,a1
	moveq	#0,d0
	;; move.w	#(SECT_PER_TRK*512/4)-1,d1
	move.w	a4,d1
.1:	cmpm.l	(a0)+,(a1)+
	dbne	d1,.1
	beq	.exit			; verified ok
	moveq	#6,d0			; error code 6: verify error
	bra	.error
	endif
.retry:
	; seek track 0, then reposition and retry to format the target track
	bsr	td_track0
	beq	.seek

	; write/read/verify error: retry the whole procedure a few times
.error:
	subq.w	#1,d7			; decrement retry-counter
	bne	.retry

.exit:
	movem.l	(sp)+,d6-d7/a2-a3/a5
	tst.b	d0
	rts

	endif	; TDFORMAT


	ifd	TDWRITE
;---------------------------------------------------------------------------
	xdef	_td_write

_td_write:	
	movem.l d1-a6,-(sp)
	lea     CUSTOM,a6
	bsr     td_write
	movem.l (sp)+,d1-a6
	rts
	
td_write:
; Write a sequence of blocks to disk. Turns on the motor automatically.
; d0.w = start block
; d1.w = number of blocks
; a0 = source buffer (number of blocks * 512 bytes long)
; -> d0/Z = error code (0=ok)

	movem.l	d4-d7/a2/a5,-(sp)
	move.w	d0,d6			; d6: current block
	move.w	d1,d7			; d7: remaining blocks to write
	move.l	a0,a5			; a5: buffer
	move.l	TrackBuffer,a2

	moveq	#0,d0
	subq.w	#1,d7
	bmi	.exit			; no blocks to write

	moveq	#-1,d4			; d4: first block in track buffer

.blk_loop:
	moveq	#0,d5
	move.w	d6,d5
	divu	#SECT_PER_TRK,d5
	swap	d5
	move.w	d6,d0
	sub.w	d5,d0			; first block number in track
	cmp.w	d4,d0
	beq	.write_blk		; track is already in write-buffer

	; write the last modified track back to disk
	tst.w	d4
	bmi	.get_track
	move.l	a2,a0
	bsr	td_trackwrite
	bne	.exit

	; get all blocks of the target track into our write-buffer
.get_track:
	move.w	d0,d4
	moveq	#SECT_PER_TRK,d1
	move.l	a2,a0
	bsr	td_read
	bne	.exit

	; now copy a new modified block to the write-buffer
.write_blk:
	lsl.w	#8,d5
	add.w	d5,d5
	lea	(a2,d5.w),a0
	moveq	#127,d0
.1:	move.l	(a5)+,(a0)+
	dbf	d0,.1

	; next block
	addq.w	#1,d6
	dbf	d7,.blk_loop

	; make sure the last modified track gets written as well
	move.l	a2,a0
	bsr	td_trackwrite
	bne	.exit

	; reread it to update the SectorTab
	bsr	td_trackread

.exit:
	tst.b	d0
	movem.l	(sp)+,d4-d7/a2/a5
	rts

	endif	; TDWRITE


	ifne	WRITESUPPORT
;---------------------------------------------------------------------------
td_trackwrite:
; Construct a valid AmigaDOS track in the MFM buffer from the sectors
; in the given track buffer. Add a gap at the start and at the end of
; the buffer, which will overlap when writing the track to disk using DMA.
; The head is expected to be positioned at the correct track to write
; (usually by a previous read operation).
; a0 = track buffer (SECT_PER_TRACK * 512 bytes)
; -> d0/Z = error code (0=ok, 1=timeout, 5=writeProtect)

	movem.l	d2-d7/a2-a3,-(sp)

	move.l	a0,a2			; a2 track buffer
	move.l	MFMbuffer,a0
	move.l	#$55555555,d3		; MFM mask

	;----------------------------------------
	; construct an AmigaDOS format MFM track
	;----------------------------------------

	; write gap at the beginning of the track
	bsr	write_gap

	; constant part of the info field: $ff, track.b, 0, 0
	moveq	#0,d6
	subq.w	#1,d6
	move.b	CurrentTrk+1,d6
	swap	d6

	moveq	#SECT_PER_TRK-1,d5

.sect_loop:
	; sector starts with two 0-bytes followed by two SYNCs
	bsr	write_nulls
	move.l	#$44894489,(a0)+

	; Write sector number and number of sectors until the gap into
	; the info field. Layout:
	; 0: $ff
	; 1: track number
	; 2: sector number
	; 3: number of sectors until gap
	moveq	#SECT_PER_TRK-1,d0
	sub.w	d5,d0
	lsl.w	#8,d0
	move.b	d5,d0
	addq.b	#1,d0
	or.l	d6,d0
	bsr	encodeLong

	; Fill the sector label field with zeros.
	moveq	#7,d4
.1:	bsr	write_nulls
	dbf	d4,.1

	; skip the checksum fields for now
	lea	16(a0),a0

	; encode the data block
	lea	512(a0),a1
	moveq	#127,d4
.2:	move.l	(a2)+,d7		; data longword to encode into MFM
	move.l	d7,d0
	lsr.l	#1,d0
	bsr	encodeEven		; encode odd bits
	move.l	d0,(a0)+
	exg	a0,a1
	move.l	d7,d0
	bsr	encodeEven		; encode even bits
	move.l	d0,(a0)+
	exg	a0,a1
	dbf	d4,.2
	bsr	checkBorder

	; calculate checksums
	lea	-568(a0),a0		; back to header start
	bsr	dataChksum
	move.l	d0,a1
	bsr	headerChksum

	; insert checksums
	lea	40(a0),a0		; checksum fields in the header
	bsr	encodeLong		; write header checksum
	move.l	a1,d0
	bsr	encodeLong		; write data checksum
	bsr	checkBorder

	lea	1024(a0),a0		; skip data
	dbf	d5,.sect_loop		; next sector

	; write gap at the end of the track
	bsr	write_gap

	;-----------------------------------
	; write the track to disk using DMA
	;-----------------------------------

	lea	CIAA+CIAPRA,a2
	lea	CIAB+CIAPRB,a3

	; check if disk is write-protected
	btst	#DSKPROT,(a2)
	beq	.err_wprot

	WAIT_DSKRDY
	move.w	#$4000,DSKLEN(a6)

	; set MFM precompensation, disable WORDSYNC
	move.w	#$6600,ADKCON(a6)
	move.w	#$9100,ADKCON(a6)

	; select 140ns precompensation for cylinders 40-79
	cmp.w	#NUM_TRACKS/2,CurrentTrk
	blo	.3
	move.w	#$a000,ADKCON(a6)	; set PRECOMP0 (140ns)

	; start disk-write DMA
.3:	move.l	MFMbuffer,DSKPT(a6)
	move.w	#$c000|MFM_WRITELEN,DSKLEN(a6)
	move.w	#$c000|MFM_WRITELEN,DSKLEN(a6)

	bsr	wait_disk_dma
	bmi	.err_timeout

	moveq	#0,d0			; ok, no error
	bra	.done
.err_timeout:
	moveq	#1,d0			; timeout while waiting for disk-DMA
	bra	.done
.err_wprot:
	moveq	#5,d0			; disk is write-protected

.done:
	movem.l	(sp)+,d2-d7/a2-a3
	rts


;---------------------------------------------------------------------------
write_nulls:
; Write two MFM-encoded 0-bytes.
; a0 = MFM destination pointer
; -> a0 = new MFM pointer (+4)

	btst	#0,-1(a0)
	beq	.1
	move.l	#$2aaaaaaa,(a0)+
	rts
.1:	move.l	#$aaaaaaaa,(a0)+
	rts


;---------------------------------------------------------------------------
write_gap:
; Write MFM_GAPSIZE words of $aaaa as gap.
; a0 = MFM destination pointer
; d3 = MFM data mask
; -> a0 = new MFM pointer

	not.l	d3
	moveq	#MFM_GAPSIZE/2-1,d0
.1:	move.l	d3,(a0)+
	dbf	d0,.1
	not.l	d3
	rts


;---------------------------------------------------------------------------
encodeEven:
; Encode even bits of a longword into MFM, regarding the right border
; of the previous MFM word in memory.
; d0 = even bits of this longworld will be encoded
; d3 = MFM data mask
; a0 = MFM destination pointer for this word
; -> d0 = MFM result
; Register a1 is preserved and d2 will be trashed!

	and.l	d3,d0
	move.l	d0,d2
	eor.l	d3,d2
	move.l	d2,d1
	add.l	d2,d2
	lsr.l	#1,d1
	bset	#31,d1
	and.l	d2,d1
	or.l	d1,d0
	btst	#0,-1(a0)
	beq	.1
	bclr	#31,d0
.1:	rts


;---------------------------------------------------------------------------
encodeLong:
; Encode a longword as MFM and write it to the buffer, including border
; checks for the previous and the next word.
; d0 = longword to encode
; a0 = MFM destination pointer
; -> a0 = new MFM pointer (+8)
; Register a1 is preserved and d2 will be trashed!

	move.l	d0,-(sp)
	lsr.l	#1,d0
	bsr	encodeEven
	move.l	d0,(a0)+
	move.l	(sp)+,d0
	bsr	encodeEven
	move.l	d0,(a0)+
	rts


;---------------------------------------------------------------------------
checkBorder:
; Fix the right MFM word's border bit.
; a0 = pointer to the MFM word on the right border

	move.b	(a0),d0
	btst	#0,-1(a0)
	bne	.1
	btst	#6,d0
	bne	.2
	or.b	#$80,d0			; set the right border bit
	move.b	d0,(a0)
	rts
.1:	and.b	#$7f,d0			; clear the right border bit
	move.b	d0,(a0)
.2:	rts


;---------------------------------------------------------------------------
headerChksum:
; Calculate sector header checksum.
; a0 = Start of sector header in MFM buffer
; d3 = MFM mask
; -> d0 = checksum
; a0/a1 are preserved!

	movem.l	d2/a0,-(sp)
	moveq	#9,d2
	bra	mfmchksum


;---------------------------------------------------------------------------
dataChksum:
; Calculate data block checksum.
; a0 = Start of sector header in MFM buffer
; d3 = MFM mask
; -> d0 = checksum
; a0/a1 are preserved!

	movem.l	d2/a0,-(sp)
	lea	56(a0),a0		; start of the data block
	move.w	#255,d2

mfmchksum:
	moveq	#0,d0
.1:	move.l	(a0)+,d1
	eor.l	d1,d0
	dbf	d2,.1

	and.l	d3,d0
	movem.l	(sp)+,d2/a0
	rts

	endif	; WRITESUPPORT


;---------------------------------------------------------------------------
wait_disk_dma:
; Wait until DMA transfer is finished. Reset DSKLEN.
; -> d0/N = timeout
; a0 and a1 are preserved, d2 is trashed!

	move.w	#$0002,INTREQ(a6)
	move.w	#DMA_TIMEOUT,d2

.1:	moveq	#16,d0
	bsr	td_delay		; ~1ms
	moveq	#2,d0
	and.w	INTREQR(a6),d0
	dbne	d2,.1

	move.w	#$4000,DSKLEN(a6)
	move.w	#$0002,INTREQ(a6)

	tst.w	d2
	rts


;---------------------------------------------------------------------------
td_motoron:
; Turn the drive's motor on and wait until rotating at full speed.
; The drive stays selected.
; a2 = CIAAPRA
; a3 = CIABPRB

	tst.b	MotorOn
	bne	.1			; already running

	bclr	#DSKMOTOR,(a3)
	nop
	moveq	#0,d0
	move.b	DriveSel,d0
	bclr	d0,(a3)

	WAIT_DSKRDY
	st	MotorOn

.1:	rts


;---------------------------------------------------------------------------
	xdef	_td_motoroff

_td_motoroff:	
	movem.l d1-a6,-(sp)
	lea     CUSTOM,a6
	bsr     td_motoroff
	movem.l (sp)+,d1-a6
	rts	
	
td_motoroff:
; Turn off the drive's motor, unselect the drive.

	tst.b	MotorOn
	beq	.1			; already off

	movem.l	d2/a2-a3,-(sp)
	lea	CIAA+CIAPRA,a2
	lea	CIAB+CIAPRB,a3

	moveq	#0,d2
	move.b	DriveSel,d2
	bset	d2,(a3)
	moveq	#2,d0
	bsr	td_delay

	bset	#DSKMOTOR,(a3)
	nop
	bclr	d2,(a3)
	nop
	bset	d2,(a3)

	clr.b	MotorOn
	movem.l	(sp)+,d2/a2-a3
.1:	rts


;---------------------------------------------------------------------------
td_seek:
; Step to the required cylinder and select the right head.
; d0.w = track to seek
; a2 = CIAAPRA
; a3 = CIABPRB

	cmp.w	#NUM_TRACKS,d0
	bhs	.exit			; illegal track

	movem.l	d2-d3,-(sp)
	move.w	CurrentTrk,d3
	move.w	d0,d2
	btst	#0,d2
	bne	.1

	; select lower head
	bset	#DSKSIDE,(a3)
	bclr	#0,d3
	bra	.2

	; select upper head
.1:	bclr	#DSKSIDE,(a3)
	bset	#0,d3

.2:	cmp.w	d3,d2
	beq	.done
	bhi	.3

	; step outwards
	bset	#DSKDIREC,(a3)
	subq.w	#2,d3
	bra	.4

.3:	; step inwards
	bclr	#DSKDIREC,(a3)
	addq.w	#2,d3

.4:	bsr	td_step
	bra	.2

.done:
	move.w	d2,CurrentTrk

	move.w	#SETTLEDELAY,d0
	bsr	td_delay

	movem.l	(sp)+,d2-d3
.exit:
	rts


;---------------------------------------------------------------------------
td_track0:
; Turn motor on. Seek track 0, reset CurrentTrk to 0.
; a2 = CIAAPRA
; a3 = CIABPRB
; -> d0/Z = error code (0=ok, 1=unformatted, 2=missingSectors, 3=badHeader)

	bsr	td_motoron		; select the drive, start motor

	bset	#DSKSIDE,(a3)		; select lower head: track 0
	bset	#DSKDIREC,(a3)		; step outwards

.1:	moveq	#STEPDELAY,d0
	bsr	td_delay		; wait until TRACK0 signal is valid

	btst	#DSKTRACK0,(a2)
	beq	.2

	bsr	td_step
	bra	.1

	; head is positioned over track 0 now, read it
.2:	clr.w	CurrentTrk
	bra	td_trackread


;---------------------------------------------------------------------------
td_step:
; Step a track into selected direction.
; a2 = CIAAPRA
; a3 = CIABPRB

	moveq	#STEPDELAY,d0
	bsr	td_delay
	bclr	#DSKSTEP,(a3)
	nop
	bset	#DSKSTEP,(a3)
	rts


;---------------------------------------------------------------------------
td_delay:
; Wait for ca. count * 64us.
; d0.w = count
; a0 and a1 are reserved!

.1:	move.b	VHPOSR(a6),d1
.2:	cmp.b	VHPOSR(a6),d1
	beq	.2
	subq.w	#1,d0
	bne	.1
	rts



	if MUSIC_PARALLEL_LOAD==0
	;; 	section	data_c
Directory:
	dc.l	DirBuffer
	
	ifne	WRITESUPPORT
	if 0
	; track buffer (when writing a track)
TrackBuffer:
	dc.l	TrkWriteBuffer
	endif
	endif
	endif


	section	bss_c


	; buffer for raw MFM track data
MFMbuffer:
	ds.l	1

	; pointers to start of each sector in MFMbuffer
SectorTab:
	ds.l	SECT_PER_TRK

	; current track in MFM buffer (-1 = none)
CurrentTrk:
	ds.w	1

	; SEL bits for present 3.5" disk drives
DrivesPresent:
	ds.b	1

	; selected disk drive
DriveSel:
	ds.b	1

	; true when drive motor is running
MotorOn:
	ds.b	1
	even



	bss_c


	; The directory from disk block 1 is cached here. It contains
	; up to 255 start blocks for files. The file length is calculated
	; by using the next start block.
	if MUSIC_PARALLEL_LOAD=0

DirBuffer:
	ds.w	256
	if 0
	ifne	WRITESUPPORT
TrkWriteBuffer:
	ds.b	SECT_PER_TRK*512
	endif
	endif
	endif

MFMbuf:
	dcb.b   $1a00*2
	endif
	endif
	
