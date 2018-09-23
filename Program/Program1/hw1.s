	.cpu arm7tdmi-s
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"hw1.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L2
.L3:
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L2:
	ldr	r3, [fp, #-20]
	add	r3, r3, #4
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	add	r3, r2, r3
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L3
	b	.L4
.L6:
	ldr	r3, [fp, #-20]
	add	r3, r3, #4
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	cmp	r3, #64
	bls	.L5
	ldr	r3, [fp, #-20]
	add	r3, r3, #4
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	cmp	r3, #90
	bhi	.L5
	ldr	r3, [fp, #-20]
	add	r3, r3, #4
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	ldr	r2, [fp, #-20]
	add	r2, r2, #4
	ldr	r1, [r2, #0]
	ldr	r2, [fp, #-8]
	add	r2, r1, r2
	ldrb	r2, [r2, #0]	@ zero_extendqisi2
	add	r2, r2, #32
	and	r2, r2, #255
	strb	r2, [r3, #0]
.L5:
	ldr	r3, [fp, #-20]
	add	r3, r3, #4
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r0, r3
	bl	putchar
	ldr	r3, [fp, #-8]
	sub	r3, r3, #1
	str	r3, [fp, #-8]
.L4:
	ldr	r3, [fp, #-20]
	add	r3, r3, #4
	ldr	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L6
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, lr}
	bx	lr
	.size	main, .-main
	.ident	"GCC: (Sourcery G++ Lite 2011.03-42) 4.5.2"
