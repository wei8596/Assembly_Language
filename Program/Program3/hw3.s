@the information that tells arm-none-eabi-as what arch. to assemble to 
	.cpu arm926ej-s
	.fpu softvfp

@this is code section
	.text
	.align	2   @align 4 byte
	.global	main
main:

    @prologue
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4

    @code body
    ldr v1, [a2, #4] @v1 �r��򩳦�}
    mov v2, #0 @v2 v1�첾�q
    mov v3, #0 @v3 �ഫ�᪺binary
LOOP1:
    ldrb v4, [v1, v2] @���J1�Ӧr��
    cmp v4, #0 @'\0'
    beq Convert
    sub v4, v4, #'0' @�r����Ʀr
    add v3, v3, v3, lsl #2 @5*v3
    mov v3, v3, lsl #1 @10*v3
    add v3, v3, v4 @v3 = 10*v3 + v4
    
    add v2, v2, #1
    cmp v2, #10 @v3�̦h�s2^31-1 (�Q�i��10bits)
    bne LOOP1
    
Convert:
    ldr a1, =string0
    ldr a2, [a2, #4] @a2 ��J����
    mov a3, #0 @a3 binary
    @��J��0
    cmp v3, #0
    beq Sign
    
    clz v2, v3 @v2 �Ĥ@��bit=1������
    rsb v2, v2, #32 @��ƪ���
    sub v2, v2, #1 @���W�ƫ�p�ƫᦳ�Ī���
    @shift point number
    rsb v1, v2, #23
    
    @M
    mov a3, v3, lsl v1 @��J���
    @�M��1.xxx*2^n��1 ([23])
    mov a3, a3, lsl #9
    mov a3, a3, lsr #9
    
    @EXP
    add v4, v2, #127 @excess-127
    mov v4, v4, lsl #23 @[30:23]
    orr a3, a3, v4 @��J
    
Sign:
    bic a3, a3, #0x80000000 @[31]��J0 (����)
    
Output:
	stmfd sp!, {a1-a4, lr}
	bl printf
	ldmfd sp!, {a1-a4, lr}
    @binary
    ldr a1, =string1
    mov v1, #32 @32bits (single-precision)
    
LOOP2:
	mov a2, a3, lsr #31 @MSB
	cmp a2, #0
	moveq a2, #'0'
	movne a2, #'1'
	stmfd sp!, {a1-a4, lr}
	bl printf
	ldmfd sp!, {a1-a4, lr}
	
	mov a3, a3, lsl #1 @����MSB
	subs v1, v1, #1
	bne LOOP2
	
	mov a2, #'.'
	bl printf
	
	@epilogue
	sub	sp, fp, #4
	ldmfd	sp!, {fp, lr}
	bx	lr

@data section

string0:
	.ascii	"%s is coded by \0"
string1:
	.ascii "%c\0"
    
    .end
