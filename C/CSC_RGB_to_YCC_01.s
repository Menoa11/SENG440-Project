	.arch armv8-a
	.file	"CSC_RGB_to_YCC_01.c"
	.text
	.align	2
	.type	CSC_RGB_to_YCC_brute_force_int, %function
CSC_RGB_to_YCC_brute_force_int:
.LFB3919:
	.cfi_startproc
	sub	sp, sp, #992
	.cfi_def_cfa_offset 992
	stp	x29, x30, [sp]
	.cfi_offset 29, -992
	.cfi_offset 30, -984
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	str	d15, [sp, 32]
	.cfi_offset 19, -976
	.cfi_offset 20, -968
	.cfi_offset 79, -960
	str	w0, [sp, 60]
	str	w1, [sp, 56]
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	ldr	w2, [sp, 56]
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 112]
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	uxtw	x2, w0
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 114]
	ldr	w0, [sp, 60]
	add	w0, w0, 1
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	ldr	w2, [sp, 56]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 116]
	ldr	w0, [sp, 60]
	add	w1, w0, 1
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:R;ldr	x3, [x3, :got_lo12:R]
	uxtw	x2, w0
	uxtw	x1, w1
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 118]
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	ldr	w2, [sp, 56]
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 104]
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	uxtw	x2, w0
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 106]
	ldr	w0, [sp, 60]
	add	w0, w0, 1
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	ldr	w2, [sp, 56]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 108]
	ldr	w0, [sp, 60]
	add	w1, w0, 1
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:G;ldr	x3, [x3, :got_lo12:G]
	uxtw	x2, w0
	uxtw	x1, w1
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 110]
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	ldr	w2, [sp, 56]
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 96]
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	uxtw	x2, w0
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 98]
	ldr	w0, [sp, 60]
	add	w0, w0, 1
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	ldr	w2, [sp, 56]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 100]
	ldr	w0, [sp, 60]
	add	w1, w0, 1
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	adrp	x3, :got:B;ldr	x3, [x3, :got_lo12:B]
	uxtw	x2, w0
	uxtw	x1, w1
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	ldrb	w0, [x0]
	sxth	w0, w0
	strh	w0, [sp, 102]
	add	x0, sp, 112
	str	x0, [sp, 120]
	ldr	x0, [sp, 120]
	ldr	d31, [x0]
	str	d31, [sp, 984]
	add	x0, sp, 104
	str	x0, [sp, 128]
	ldr	x0, [sp, 128]
	ldr	d31, [x0]
	str	d31, [sp, 976]
	add	x0, sp, 96
	str	x0, [sp, 136]
	ldr	x0, [sp, 136]
	ldr	d31, [x0]
	str	d31, [sp, 968]
	mov	w0, 4096
	str	w0, [sp, 144]
	ldr	s31, [sp, 144]
	dup	v31.4s, v31.s[0]
	str	q31, [sp, 944]
	ldr	q31, [sp, 944]
	str	q31, [sp, 192]
	ldr	d31, [sp, 984]
	str	d31, [sp, 184]
	mov	w0, 66
	strh	w0, [sp, 182]
	ldr	q31, [sp, 192]
	str	q31, [sp, 160]
	ldr	d31, [sp, 184]
	str	d31, [sp, 152]
	ldrh	w0, [sp, 182]
	strh	w0, [sp, 150]
	ldr	q31, [sp, 160]
	ldr	d30, [sp, 152]
	ldr	h15, [sp, 150]
	smlal	v31.4s, v30.4h, v15.h[0]
	nop
	str	q31, [sp, 944]
	ldr	q31, [sp, 944]
	str	q31, [sp, 256]
	ldr	d31, [sp, 976]
	str	d31, [sp, 248]
	mov	w0, 129
	strh	w0, [sp, 246]
	ldr	q31, [sp, 256]
	str	q31, [sp, 224]
	ldr	d31, [sp, 248]
	str	d31, [sp, 216]
	ldrh	w0, [sp, 246]
	strh	w0, [sp, 214]
	ldr	q31, [sp, 224]
	ldr	d30, [sp, 216]
	ldr	h15, [sp, 214]
	smlal	v31.4s, v30.4h, v15.h[0]
	nop
	str	q31, [sp, 944]
	ldr	q31, [sp, 944]
	str	q31, [sp, 320]
	ldr	d31, [sp, 968]
	str	d31, [sp, 312]
	mov	w0, 25
	strh	w0, [sp, 310]
	ldr	q31, [sp, 320]
	str	q31, [sp, 288]
	ldr	d31, [sp, 312]
	str	d31, [sp, 280]
	ldrh	w0, [sp, 310]
	strh	w0, [sp, 278]
	ldr	q31, [sp, 288]
	ldr	d30, [sp, 280]
	ldr	h15, [sp, 278]
	smlal	v31.4s, v30.4h, v15.h[0]
	nop
	str	q31, [sp, 944]
	ldr	q31, [sp, 944]
	str	q31, [sp, 352]
	ldr	q31, [sp, 352]
	str	q31, [sp, 336]
	ldr	q31, [sp, 336]
	rshrn	v31.4h, v31.4s, 8
	nop
	str	d31, [sp, 936]
	mov	w0, 32768
	str	w0, [sp, 368]
	ldr	s31, [sp, 368]
	dup	v31.4s, v31.s[0]
	str	q31, [sp, 912]
	ldr	q31, [sp, 912]
	str	q31, [sp, 416]
	ldr	d31, [sp, 984]
	str	d31, [sp, 408]
	mov	w0, 38
	strh	w0, [sp, 406]
	ldr	q31, [sp, 416]
	str	q31, [sp, 384]
	ldr	d31, [sp, 408]
	str	d31, [sp, 376]
	ldrh	w0, [sp, 406]
	strh	w0, [sp, 374]
	ldr	q30, [sp, 384]
	ldr	d31, [sp, 376]
	ldr	h15, [sp, 374]
	smlsl	v30.4s, v31.4h, v15.h[0]
	nop
	str	q30, [sp, 912]
	ldr	q31, [sp, 912]
	str	q31, [sp, 480]
	ldr	d31, [sp, 976]
	str	d31, [sp, 472]
	mov	w0, 74
	strh	w0, [sp, 470]
	ldr	q31, [sp, 480]
	str	q31, [sp, 448]
	ldr	d31, [sp, 472]
	str	d31, [sp, 440]
	ldrh	w0, [sp, 470]
	strh	w0, [sp, 438]
	ldr	q30, [sp, 448]
	ldr	d31, [sp, 440]
	ldr	h15, [sp, 438]
	smlsl	v30.4s, v31.4h, v15.h[0]
	nop
	str	q30, [sp, 912]
	ldr	q31, [sp, 912]
	str	q31, [sp, 544]
	ldr	d31, [sp, 968]
	str	d31, [sp, 536]
	mov	w0, 112
	strh	w0, [sp, 534]
	ldr	q31, [sp, 544]
	str	q31, [sp, 512]
	ldr	d31, [sp, 536]
	str	d31, [sp, 504]
	ldrh	w0, [sp, 534]
	strh	w0, [sp, 502]
	ldr	q31, [sp, 512]
	ldr	d30, [sp, 504]
	ldr	h15, [sp, 502]
	smlal	v31.4s, v30.4h, v15.h[0]
	nop
	str	q31, [sp, 912]
	ldr	q31, [sp, 912]
	str	q31, [sp, 576]
	ldr	q31, [sp, 576]
	str	q31, [sp, 560]
	ldr	q31, [sp, 560]
	rshrn	v31.4h, v31.4s, 8
	nop
	str	d31, [sp, 904]
	mov	w0, 32768
	str	w0, [sp, 592]
	ldr	s31, [sp, 592]
	dup	v31.4s, v31.s[0]
	str	q31, [sp, 880]
	ldr	q31, [sp, 880]
	str	q31, [sp, 640]
	ldr	d31, [sp, 984]
	str	d31, [sp, 632]
	mov	w0, 112
	strh	w0, [sp, 630]
	ldr	q31, [sp, 640]
	str	q31, [sp, 608]
	ldr	d31, [sp, 632]
	str	d31, [sp, 600]
	ldrh	w0, [sp, 630]
	strh	w0, [sp, 598]
	ldr	q31, [sp, 608]
	ldr	d30, [sp, 600]
	ldr	h15, [sp, 598]
	smlal	v31.4s, v30.4h, v15.h[0]
	nop
	str	q31, [sp, 880]
	ldr	q31, [sp, 880]
	str	q31, [sp, 704]
	ldr	d31, [sp, 976]
	str	d31, [sp, 696]
	mov	w0, 94
	strh	w0, [sp, 694]
	ldr	q31, [sp, 704]
	str	q31, [sp, 672]
	ldr	d31, [sp, 696]
	str	d31, [sp, 664]
	ldrh	w0, [sp, 694]
	strh	w0, [sp, 662]
	ldr	q30, [sp, 672]
	ldr	d31, [sp, 664]
	ldr	h15, [sp, 662]
	smlsl	v30.4s, v31.4h, v15.h[0]
	nop
	str	q30, [sp, 880]
	ldr	q31, [sp, 880]
	str	q31, [sp, 768]
	ldr	d31, [sp, 968]
	str	d31, [sp, 760]
	mov	w0, 18
	strh	w0, [sp, 758]
	ldr	q31, [sp, 768]
	str	q31, [sp, 736]
	ldr	d31, [sp, 760]
	str	d31, [sp, 728]
	ldrh	w0, [sp, 758]
	strh	w0, [sp, 726]
	ldr	q30, [sp, 736]
	ldr	d31, [sp, 728]
	ldr	h15, [sp, 726]
	smlsl	v30.4s, v31.4h, v15.h[0]
	nop
	str	q30, [sp, 880]
	ldr	q31, [sp, 880]
	str	q31, [sp, 800]
	ldr	q31, [sp, 800]
	str	q31, [sp, 784]
	ldr	q31, [sp, 784]
	rshrn	v31.4h, v31.4s, 8
	nop
	str	d31, [sp, 872]
	add	x0, sp, 88
	str	x0, [sp, 832]
	ldr	d31, [sp, 936]
	str	d31, [sp, 824]
	ldr	d31, [sp, 824]
	ldr	x0, [sp, 832]
	str	d31, [x0]
	nop
	add	x0, sp, 80
	str	x0, [sp, 848]
	ldr	d31, [sp, 904]
	str	d31, [sp, 840]
	ldr	d31, [sp, 840]
	ldr	x0, [sp, 848]
	str	d31, [x0]
	nop
	add	x0, sp, 72
	str	x0, [sp, 864]
	ldr	d31, [sp, 872]
	str	d31, [sp, 856]
	ldr	d31, [sp, 856]
	ldr	x0, [sp, 864]
	str	d31, [x0]
	nop
	ldrsh	w0, [sp, 88]
	and	w4, w0, 255
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	ldr	w2, [sp, 56]
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrsh	w1, [sp, 90]
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	and	w4, w1, 255
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	uxtw	x2, w0
	ldr	w1, [sp, 60]
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrsh	w1, [sp, 92]
	ldr	w0, [sp, 60]
	add	w0, w0, 1
	and	w4, w1, 255
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	ldr	w2, [sp, 56]
	uxtw	x1, w0
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrsh	w2, [sp, 94]
	ldr	w0, [sp, 60]
	add	w1, w0, 1
	ldr	w0, [sp, 56]
	add	w0, w0, 1
	and	w4, w2, 255
	adrp	x3, :got:Y;ldr	x3, [x3, :got_lo12:Y]
	uxtw	x2, w0
	uxtw	x1, w1
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 7
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrsh	w0, [sp, 80]
	and	w4, w0, 255
	ldrsh	w0, [sp, 82]
	and	w1, w0, 255
	ldrsh	w0, [sp, 84]
	and	w2, w0, 255
	ldrsh	w0, [sp, 86]
	and	w3, w0, 255
	ldr	w0, [sp, 60]
	lsr	w19, w0, 1
	ldr	w0, [sp, 56]
	lsr	w20, w0, 1
	mov	w0, w4
	bl	chrominance_downsample
	and	w4, w0, 255
	adrp	x3, :got:Cb;ldr	x3, [x3, :got_lo12:Cb]
	uxtw	x2, w20
	uxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 6
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	ldrsh	w0, [sp, 72]
	and	w4, w0, 255
	ldrsh	w0, [sp, 74]
	and	w1, w0, 255
	ldrsh	w0, [sp, 76]
	and	w2, w0, 255
	ldrsh	w0, [sp, 78]
	and	w3, w0, 255
	ldr	w0, [sp, 60]
	lsr	w19, w0, 1
	ldr	w0, [sp, 56]
	lsr	w20, w0, 1
	mov	w0, w4
	bl	chrominance_downsample
	and	w4, w0, 255
	adrp	x3, :got:Cr;ldr	x3, [x3, :got_lo12:Cr]
	uxtw	x2, w20
	uxtw	x1, w19
	mov	x0, x1
	lsl	x0, x0, 2
	add	x0, x0, x1
	lsl	x0, x0, 6
	add	x0, x3, x0
	add	x0, x0, x2
	mov	w1, w4
	strb	w1, [x0]
	nop
	ldr	d15, [sp, 32]
	ldp	x29, x30, [sp]
	ldp	x19, x20, [sp, 16]
	add	sp, sp, 992
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 29
	.cfi_restore 30
	.cfi_restore 79
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3919:
	.size	CSC_RGB_to_YCC_brute_force_int, .-CSC_RGB_to_YCC_brute_force_int
	.align	2
	.type	chrominance_downsample, %function
chrominance_downsample:
.LFB3920:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	strb	w0, [sp, 15]
	strb	w1, [sp, 14]
	strb	w2, [sp, 13]
	strb	w3, [sp, 12]
	ldrb	w1, [sp, 15]
	ldrb	w0, [sp, 14]
	add	w1, w1, w0
	ldrb	w0, [sp, 13]
	add	w1, w1, w0
	ldrb	w0, [sp, 12]
	add	w0, w1, w0
	str	w0, [sp, 28]
	ldr	w0, [sp, 28]
	add	w0, w0, 2
	str	w0, [sp, 28]
	ldr	w0, [sp, 28]
	asr	w0, w0, 2
	str	w0, [sp, 28]
	ldr	w0, [sp, 28]
	and	w0, w0, 255
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3920:
	.size	chrominance_downsample, .-chrominance_downsample
	.align	2
	.global	CSC_RGB_to_YCC
	.type	CSC_RGB_to_YCC, %function
CSC_RGB_to_YCC:
.LFB3921:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	str	wzr, [sp, 28]
	b	.L35
.L38:
	str	wzr, [sp, 24]
	b	.L36
.L37:
	ldr	w1, [sp, 24]
	ldr	w0, [sp, 28]
	bl	CSC_RGB_to_YCC_brute_force_int
	ldr	w0, [sp, 24]
	add	w0, w0, 2
	mov	w1, w0
	ldr	w0, [sp, 28]
	bl	CSC_RGB_to_YCC_brute_force_int
	ldr	w0, [sp, 24]
	add	w0, w0, 4
	mov	w1, w0
	ldr	w0, [sp, 28]
	bl	CSC_RGB_to_YCC_brute_force_int
	ldr	w0, [sp, 24]
	add	w0, w0, 6
	mov	w1, w0
	ldr	w0, [sp, 28]
	bl	CSC_RGB_to_YCC_brute_force_int
	ldr	w0, [sp, 24]
	add	w0, w0, 8
	str	w0, [sp, 24]
.L36:
	ldr	w0, [sp, 24]
	cmp	w0, 639
	bls	.L37
	ldr	w0, [sp, 28]
	add	w0, w0, 2
	str	w0, [sp, 28]
.L35:
	ldr	w0, [sp, 28]
	cmp	w0, 479
	bls	.L38
	nop
	nop
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3921:
	.size	CSC_RGB_to_YCC, .-CSC_RGB_to_YCC
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
