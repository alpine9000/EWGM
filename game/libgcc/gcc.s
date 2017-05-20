; libgcc routines for 68000 w/o floating-point hardware.
; Copyright (C) 1994-2016 Free Software Foundation, Inc. 
;
; This file is part of GCC.
;
; GCC is free software; you can redistribute it and/or modify it
; under the terms of the GNU General Public License as published by the
; Free Software Foundation; either version 3, or (at your option) any
; later version.
; 
; This file is distributed in the hope that it will be useful, but
; WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
; General Public License for more details.
; 
; Under Section 7 of GPL version 3, you are granted additional
; permissions described in the GCC Runtime Library Exception, version
; 3.1, as published by the Free Software Foundation.
; 
; You should have received a copy of the GNU General Public License and
; a copy of the GCC Runtime Library Exception along with this program;
; see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
; <http://www.gnu.org/licenses/>. 
	
	section .text
	xdef ___modsi3
	align 	1
___modsi3:
	move.l	8(sp),d1	; d1 = divisor
	move.l	4(sp),d0	; d0 = dividend
	move.l	d1,-(sp)
	move.l	d0,-(sp)
	jsr	___divsi3
	addq.l	#8,sp
	move.l	8(sp),d1	; d1 = divisor
	move.l	d1,-(sp)
	move.l	d0,-(sp)
	jsr	___mulsi3	; d0 = (a/b)*b
	addq.l	#8,sp
	move.l	4(sp),d1	; d1 = dividend
	sub.l	d0,d1		; d1 = a - (a/b)*b
	move.l	d1,d0
	rts


	xdef ___umodsi3
___umodsi3:
	move.l	8(sp),d1	/* d1 = divisor */
	move.l	4(sp),d0	/* d0 = dividend */
	move.l	d1,-(sp)
	move.l	d0,-(sp)
	jsr	___udivsi3
	addq.l	#8,sp
	move.l	8(sp),d1	/* d1 = divisor */
	move.l	d1,-(sp)
	move.l	d0,-(sp)
	jsr	___mulsi3	/* d0 = (a/b)*b */
	addq.l	#(8),sp
	move.l	4(sp),d1	/* d1 = dividend */
	sub.l	d0,d1		/* d1 = a - (a/b)*b */
	move.l	d1,d0
	rts	

	xdef ___mulsi3
	align 	1
___mulsi3:
	move.w	4(sp),d0	; x0 -> d0
	mulu.w	10(sp),d0	; x0*y1
	move.w	6(sp),d1	; x1 -> d1
	mulu.w	8(sp),d1	; x1*y0
	add.w	d1,d0
	swap	d0
	clr.w	d0
	move.w	6(sp),d1	; x1 -> d1
	mulu.w	10(sp),d1	; x1*y1
	add.l	d1,d0
	rts	
	

	xdef	___divsi3
	align 	1
___divsi3:	
	move.l	d2,-(sp)
	moveq	#1,d2		; sign of result stored in d2 (=1 or =-1)
	move.l	12(sp),d1	; d1 = divisor
	bpl	.L1
	neg.l	d1
	neg.b	d2		; change sign because divisor <0 
.L1:	move.l	8(sp),d0	; d0 = dividend
	bpl	.L2
	neg.l	d0
	neg.b	d2
.L2:	move.l	d1,-(sp)
	move.l	d0,-(sp)
	jsr	___udivsi3	; divide abs(dividend) by abs(divisor)
	addq.l	#8,sp
	tst.b	d2
	bpl	.L3
	neg.l	d0
.L3:	move.l	(sp)+,d2
	rts


	xdef	___udivsi3
	align	1
___udivsi3:
	move.l	d2,-(sp)
	move.l	12(sp),d1	; d1 = divisor
	move.l	8(sp),d0	; d0 = dividend
	cmp.l	#$10000,d1 	; divisor >= 2 ^ 16 ?  
	bcc	.L3		; then try next algorithm
	move.l	d0,d2
	clr.w	d2
	swap	d2
	divu	d1,d2          	; high quotient in lower word
	move.w	d2,d0		; save high quotient
	swap	d0
	move.w	10(sp),d2	; get low dividend + high rest
	divu	d1,d2		; low quotient
	move.w	d2,d0
	bra	.L6
.L3:	move.l	d1,d2		; use d2 as divisor backup
.L4:	lsr.l	#1,d1		; shift divisor
	lsr.l	#1,d0		; shift dividend
	cmp.l	#$10000,d1 	; still divisor >= 2 ^ 16 ? 
	bcc	.L4
	divu	d1,d0		; now we have 16-bit divisor
	and.l	#$ffff,d0 	; mask out divisor, ignore remainder
	;; Multiply the 16-bit tentative quotient with the 32-bit divisor.  Because of
	;; the operand ranges, this might give a 33-bit product.  If this product is 
	;; greater than the dividend, the tentative quotient was too large. */
	move.l	d2,d1
	mulu	d0,d1		; low part, 32 bits
	swap	d2
	mulu	d0,d2		; high part, at most 17 bits
	swap	d2		; align high part with low part
	tst.w	d2		; high part 17 bits?
	bne	.L5		; if 17 bits, quotient was too large
	add.l	d2,d1		; add parts
	bcs	.L5		; if sum is 33 bits, quotient was too large
	cmp.l	8(sp),d1	; compare the sum with the dividend
	bls	.L6		; if sum > dividend, quotient was too large
.L5:	subq.l	#1,d0		; adjust quotient
.L6:	move.l	(sp)+,d2
	rts
	
