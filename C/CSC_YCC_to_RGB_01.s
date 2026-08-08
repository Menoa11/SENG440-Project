	.arch armv8-a
	.file	"CSC_YCC_to_RGB_01.c"
	.text
	.align	2
	.type	saturation_int, %function
saturation_int:
.LFB3916:
	.cfi_startproc
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, 12]
	ldr	w2, [sp, 12]
	ldr	w1, [sp, 12]
	mov	w0, 255
	cmp	w2, 255
	csel	w0, w1, w0, le
	str	w0, [sp, 12]
	ldr	w0, [sp, 12]
	bic	w0, w0, w0, asr #31
	str	w0, [sp, 12]
	ldr	w0, [sp, 12]
	and	w0, w0, 255
	add	sp, sp, 16
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3916:
	.size	saturation_int, .-saturation_int
	.align	2
	.type	CSC_YCC_to_RGB_brute_force_int, %function
CSC_YCC_to_RGB_brute_force_int:
.LFB3920:
	.cfi_startproc
	sub	sp, sp, #640
	.cfi_def_cfa_offset 640
	stp	x29, x30, [sp]
	.cfi_offset 29, -640
	.cfi_offset 30, -632
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	str	d15, [sp, 32]
	.cfi_offset 19, -624
	.cfi_offset 20, -616
	.cfi_offset 79, -608
	str	w0, [sp, 60]
	str	w1, [sp, 56]
	strb	w2, [sp, 55]
	strb	w3, [sp, 54]
	strb	w4, [sp, 53]
	strb	w5, [sp, 52]
	strb	w6, [sp, 51]
	strb	w7, [sp, 50]
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	ldrsw	x2, [sp, 56]
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sub	w0, w0, #16
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 112]
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	sxtw	x2, w0
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sub	w0, w0, #16
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 114]
	ldr	w0, [sp, 60]
	add	w0, w0, 1
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	ldrsw	x2, [sp, 56]
	sxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sub	w0, w0, #16
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 116]
	ldr	w0, [sp, 60]
	add	w1, w0, 1
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	sxtw	x2, w0
	sxtw	x1, w1
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sub	w0, w0, #16
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 118]
	ldrb	w0, [sp, 55]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 104]
	ldrb	w0, [sp, 54]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 106]
	ldrb	w0, [sp, 53]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 108]
	ldrb	w0, [sp, 52]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 110]
	ldrb	w0, [sp, 51]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 96]
	ldrb	w0, [sp, 50]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 98]
	ldrb	w0, [sp, 640]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 100]
	ldrb	w0, [sp, 648]
	and	w0, w0, 65535
	sub	w0, w0, #128
	and	w0, w0, 65535
	sxth	w0, w0
	strh	w0, [sp, 102]
	add	x0, sp, 112
	str	x0, [sp, 120]
	ldr	x0, [sp, 120]
	ldr	d31, [x0]
	str	d31, [sp, 632]
	add	x0, sp, 104
	str	x0, [sp, 128]
	ldr	x0, [sp, 128]
	ldr	d31, [x0]
	str	d31, [sp, 624]
	add	x0, sp, 96
	str	x0, [sp, 136]
	ldr	x0, [sp, 136]
	ldr	d31, [x0]
	str	d31, [sp, 616]
	ldr	d31, [sp, 632]
	str	d31, [sp, 152]
	mov	w0, 74
	strh	w0, [sp, 150]
	ldr	d31, [sp, 152]
	ldr	h15, [sp, 150]
	smull	v31.4s, v31.4h, v15.h[0]
	str	q31, [sp, 592]
	ldr	q31, [sp, 592]
	str	q31, [sp, 208]
	ldr	d31, [sp, 616]
	str	d31, [sp, 200]
	mov	w0, 102
	strh	w0, [sp, 198]
	ldr	q31, [sp, 208]
	str	q31, [sp, 176]
	ldr	d31, [sp, 200]
	str	d31, [sp, 168]
	ldrh	w0, [sp, 198]
	strh	w0, [sp, 166]
	ldr	q31, [sp, 176]
	ldr	d30, [sp, 168]
	ldr	h15, [sp, 166]
	smlal	v31.4s, v30.4h, v15.h[0]
	nop
	str	q31, [sp, 240]
	ldr	q31, [sp, 240]
	str	q31, [sp, 224]
	ldr	q31, [sp, 224]
	sqrshrun	v31.4h, v31.4s, 6
	nop
	str	d31, [sp, 584]
	ldr	q31, [sp, 592]
	str	q31, [sp, 304]
	ldr	d31, [sp, 616]
	str	d31, [sp, 296]
	mov	w0, 52
	strh	w0, [sp, 294]
	ldr	q31, [sp, 304]
	str	q31, [sp, 272]
	ldr	d31, [sp, 296]
	str	d31, [sp, 264]
	ldrh	w0, [sp, 294]
	strh	w0, [sp, 262]
	ldr	q30, [sp, 272]
	ldr	d31, [sp, 264]
	ldr	h15, [sp, 262]
	smlsl	v30.4s, v31.4h, v15.h[0]
	nop
	str	q30, [sp, 368]
	ldr	d31, [sp, 624]
	str	d31, [sp, 360]
	mov	w0, 25
	strh	w0, [sp, 358]
	ldr	q31, [sp, 368]
	str	q31, [sp, 336]
	ldr	d31, [sp, 360]
	str	d31, [sp, 328]
	ldrh	w0, [sp, 358]
	strh	w0, [sp, 326]
	ldr	q30, [sp, 336]
	ldr	d31, [sp, 328]
	ldr	h15, [sp, 326]
	smlsl	v30.4s, v31.4h, v15.h[0]
	nop
	str	q30, [sp, 400]
	ldr	q31, [sp, 400]
	str	q31, [sp, 384]
	ldr	q31, [sp, 384]
	sqrshrun	v31.4h, v31.4s, 6
	nop
	str	d31, [sp, 576]
	ldr	q31, [sp, 592]
	str	q31, [sp, 464]
	ldr	d31, [sp, 624]
	str	d31, [sp, 456]
	mov	w0, 129
	strh	w0, [sp, 454]
	ldr	q31, [sp, 464]
	str	q31, [sp, 432]
	ldr	d31, [sp, 456]
	str	d31, [sp, 424]
	ldrh	w0, [sp, 454]
	strh	w0, [sp, 422]
	ldr	q31, [sp, 432]
	ldr	d30, [sp, 424]
	ldr	h15, [sp, 422]
	smlal	v31.4s, v30.4h, v15.h[0]
	nop
	str	q31, [sp, 496]
	ldr	q31, [sp, 496]
	str	q31, [sp, 480]
	ldr	q31, [sp, 480]
	sqrshrun	v31.4h, v31.4s, 6
	nop
	str	d31, [sp, 568]
	add	x0, sp, 88
	str	x0, [sp, 528]
	ldr	d31, [sp, 584]
	str	d31, [sp, 520]
	ldr	d31, [sp, 520]
	ldr	x0, [sp, 528]
	str	d31, [x0]
	nop
	add	x0, sp, 80
	str	x0, [sp, 544]
	ldr	d31, [sp, 576]
	str	d31, [sp, 536]
	ldr	d31, [sp, 536]
	ldr	x0, [sp, 544]
	str	d31, [x0]
	nop
	add	x0, sp, 72
	str	x0, [sp, 560]
	ldr	d31, [sp, 568]
	str	d31, [sp, 552]
	ldr	d31, [sp, 552]
	ldr	x0, [sp, 560]
	str	d31, [x0]
	nop
	ldrh	w0, [sp, 88]
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	ldrsw	x2, [sp, 56]
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 90]
	mov	w1, w0
	ldr	w0, [sp, 56]
	add	w19, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	sxtw	x2, w19
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 92]
	mov	w1, w0
	ldr	w0, [sp, 60]
	add	w19, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	ldrsw	x2, [sp, 56]
	sxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 94]
	mov	w1, w0
	ldr	w0, [sp, 60]
	add	w19, w0, 1
	ldr	w0, [sp, 56]
	add	w20, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	sxtw	x2, w20
	sxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 80]
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	ldrsw	x2, [sp, 56]
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 82]
	mov	w1, w0
	ldr	w0, [sp, 56]
	add	w19, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	sxtw	x2, w19
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 84]
	mov	w1, w0
	ldr	w0, [sp, 60]
	add	w19, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	ldrsw	x2, [sp, 56]
	sxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 86]
	mov	w1, w0
	ldr	w0, [sp, 60]
	add	w19, w0, 1
	ldr	w0, [sp, 56]
	add	w20, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	sxtw	x2, w20
	sxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 72]
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	ldrsw	x2, [sp, 56]
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 74]
	mov	w1, w0
	ldr	w0, [sp, 56]
	add	w19, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	sxtw	x2, w19
	ldrsw	x1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 76]
	mov	w1, w0
	ldr	w0, [sp, 60]
	add	w19, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	ldrsw	x2, [sp, 56]
	sxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrh	w0, [sp, 78]
	mov	w1, w0
	ldr	w0, [sp, 60]
	add	w19, w0, 1
	ldr	w0, [sp, 56]
	add	w20, w0, 1
	mov	w0, w1
	bl	saturation_int
	and	w4, w0, 255
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	sxtw	x2, w20
	sxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	nop
	ldr	d15, [sp, 32]
	ldp	x29, x30, [sp]
	ldp	x19, x20, [sp, 16]
	add	sp, sp, 640
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 29
	.cfi_restore 30
	.cfi_restore 79
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3920:
	.size	CSC_YCC_to_RGB_brute_force_int, .-CSC_YCC_to_RGB_brute_force_int
	.align	2
	.type	chrominance_upsample, %function
chrominance_upsample:
.LFB3921:
	.cfi_startproc
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	strb	w0, [sp, 31]
	strb	w1, [sp, 30]
	strb	w2, [sp, 29]
	strb	w3, [sp, 28]
	str	x4, [sp, 16]
	str	x5, [sp, 8]
	str	x6, [sp]
	ldrb	w1, [sp, 31]
	ldrb	w0, [sp, 30]
	add	w0, w1, w0
	str	w0, [sp, 44]
	ldr	w0, [sp, 44]
	add	w0, w0, 1
	str	w0, [sp, 44]
	ldr	w0, [sp, 44]
	asr	w0, w0, 1
	and	w1, w0, 255
	ldr	x0, [sp, 16]
	strb	w1, [x0]
	ldrb	w1, [sp, 31]
	ldrb	w0, [sp, 29]
	add	w0, w1, w0
	str	w0, [sp, 40]
	ldr	w0, [sp, 40]
	add	w0, w0, 1
	str	w0, [sp, 40]
	ldr	w0, [sp, 40]
	asr	w0, w0, 1
	and	w1, w0, 255
	ldr	x0, [sp, 8]
	strb	w1, [x0]
	ldrb	w1, [sp, 31]
	ldrb	w0, [sp, 30]
	add	w1, w1, w0
	ldrb	w0, [sp, 29]
	add	w1, w1, w0
	ldrb	w0, [sp, 28]
	add	w0, w1, w0
	str	w0, [sp, 36]
	ldr	w0, [sp, 36]
	add	w0, w0, 2
	str	w0, [sp, 36]
	ldr	w0, [sp, 36]
	asr	w0, w0, 2
	and	w1, w0, 255
	ldr	x0, [sp]
	strb	w1, [x0]
	nop
	nop
	add	sp, sp, 48
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3921:
	.size	chrominance_upsample, .-chrominance_upsample
	.align	2
	.type	CSC_YCC_to_RGB_block, %function
CSC_YCC_to_RGB_block:
.LFB3922:
	.cfi_startproc
	sub	sp, sp, #112
	.cfi_def_cfa_offset 112
	stp	x29, x30, [sp, 16]
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	add	x29, sp, 16
	str	w0, [sp, 76]
	str	w1, [sp, 72]
	str	x2, [sp, 64]
	str	x3, [sp, 56]
	str	x4, [sp, 48]
	str	x5, [sp, 40]
	ldr	w0, [sp, 72]
	asr	w0, w0, 1
	str	w0, [sp, 108]
	ldr	w0, [sp, 108]
	cmp	w0, 318
	bgt	.L24
	ldr	w0, [sp, 108]
	add	w0, w0, 1
	b	.L25
.L24:
	ldr	w0, [sp, 108]
.L25:
	str	w0, [sp, 104]
	ldrsw	x0, [sp, 108]
	ldr	x1, [sp, 64]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 103]
	ldrsw	x0, [sp, 104]
	ldr	x1, [sp, 64]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 102]
	ldrsw	x0, [sp, 108]
	ldr	x1, [sp, 56]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 101]
	ldrsw	x0, [sp, 104]
	ldr	x1, [sp, 56]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 100]
	ldrsw	x0, [sp, 108]
	ldr	x1, [sp, 48]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 99]
	ldrsw	x0, [sp, 104]
	ldr	x1, [sp, 48]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 98]
	ldrsw	x0, [sp, 108]
	ldr	x1, [sp, 40]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 97]
	ldrsw	x0, [sp, 104]
	ldr	x1, [sp, 40]
	add	x0, x1, x0
	ldrb	w0, [x0]
	strb	w0, [sp, 96]
	add	x2, sp, 93
	add	x1, sp, 94
	add	x0, sp, 95
	mov	x6, x2
	mov	x5, x1
	mov	x4, x0
	ldrb	w3, [sp, 100]
	ldrb	w2, [sp, 101]
	ldrb	w1, [sp, 102]
	ldrb	w0, [sp, 103]
	bl	chrominance_upsample
	add	x2, sp, 90
	add	x1, sp, 91
	add	x0, sp, 92
	mov	x6, x2
	mov	x5, x1
	mov	x4, x0
	ldrb	w3, [sp, 96]
	ldrb	w2, [sp, 97]
	ldrb	w1, [sp, 98]
	ldrb	w0, [sp, 99]
	bl	chrominance_upsample
	ldrb	w1, [sp, 95]
	ldrb	w2, [sp, 94]
	ldrb	w3, [sp, 93]
	ldrb	w4, [sp, 92]
	ldrb	w5, [sp, 91]
	ldrb	w0, [sp, 90]
	strb	w0, [sp, 8]
	mov	w0, w5
	strb	w0, [sp]
	mov	w7, w4
	ldrb	w6, [sp, 99]
	mov	w5, w3
	mov	w4, w2
	mov	w3, w1
	ldrb	w2, [sp, 103]
	ldr	w1, [sp, 72]
	ldr	w0, [sp, 76]
	bl	CSC_YCC_to_RGB_brute_force_int
	nop
	ldp	x29, x30, [sp, 16]
	add	sp, sp, 112
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3922:
	.size	CSC_YCC_to_RGB_block, .-CSC_YCC_to_RGB_block
	.align	2
	.global	CSC_YCC_to_RGB
	.type	CSC_YCC_to_RGB, %function
CSC_YCC_to_RGB:
.LFB3923:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	str	wzr, [sp, 60]
	b	.L27
.L32:
	ldr	w0, [sp, 60]
	asr	w0, w0, 1
	str	w0, [sp, 52]
	ldr	w0, [sp, 52]
	cmp	w0, 238
	bgt	.L28
	ldr	w0, [sp, 52]
	add	w0, w0, 1
	b	.L29
.L28:
	ldr	w0, [sp, 52]
.L29:
	str	w0, [sp, 48]
	ldrsw	x1, [sp, 52]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 6
	adrp	x1, :got:Cb;ldr	x1, [x1, :got_lo12:Cb]
	add	x0, x0, x1
	str	x0, [sp, 40]
	ldrsw	x1, [sp, 48]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 6
	adrp	x1, :got:Cb;ldr	x1, [x1, :got_lo12:Cb]
	add	x0, x0, x1
	str	x0, [sp, 32]
	ldrsw	x1, [sp, 52]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 6
	adrp	x1, :got:Cr;ldr	x1, [x1, :got_lo12:Cr]
	add	x0, x0, x1
	str	x0, [sp, 24]
	ldrsw	x1, [sp, 48]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 6
	adrp	x1, :got:Cr;ldr	x1, [x1, :got_lo12:Cr]
	add	x0, x0, x1
	str	x0, [sp, 16]
	str	wzr, [sp, 56]
	b	.L30
.L31:
	ldr	x5, [sp, 16]
	ldr	x4, [sp, 24]
	ldr	x3, [sp, 32]
	ldr	x2, [sp, 40]
	ldr	w1, [sp, 56]
	ldr	w0, [sp, 60]
	bl	CSC_YCC_to_RGB_block
	ldr	w0, [sp, 56]
	add	w0, w0, 2
	ldr	x5, [sp, 16]
	ldr	x4, [sp, 24]
	ldr	x3, [sp, 32]
	ldr	x2, [sp, 40]
	mov	w1, w0
	ldr	w0, [sp, 60]
	bl	CSC_YCC_to_RGB_block
	ldr	w0, [sp, 56]
	add	w0, w0, 4
	ldr	x5, [sp, 16]
	ldr	x4, [sp, 24]
	ldr	x3, [sp, 32]
	ldr	x2, [sp, 40]
	mov	w1, w0
	ldr	w0, [sp, 60]
	bl	CSC_YCC_to_RGB_block
	ldr	w0, [sp, 56]
	add	w0, w0, 6
	ldr	x5, [sp, 16]
	ldr	x4, [sp, 24]
	ldr	x3, [sp, 32]
	ldr	x2, [sp, 40]
	mov	w1, w0
	ldr	w0, [sp, 60]
	bl	CSC_YCC_to_RGB_block
	ldr	w0, [sp, 56]
	add	w0, w0, 8
	str	w0, [sp, 56]
.L30:
	ldr	w0, [sp, 56]
	cmp	w0, 639
	ble	.L31
	ldr	w0, [sp, 60]
	add	w0, w0, 2
	str	w0, [sp, 60]
.L27:
	ldr	w0, [sp, 60]
	cmp	w0, 479
	ble	.L32
	nop
	nop
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3923:
	.size	CSC_YCC_to_RGB, .-CSC_YCC_to_RGB
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
