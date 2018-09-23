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

    @code body
	ldr	r0, =string0
	bl printf
	ldr v1, =main	@���omain����}
	mov v6, v1		@�ƥ�main����},�H�K�p��۹�PC
	ldr v2, =exit	@���oexit����}

loop:
	mov r1, v1					@�ƻs��}��r1
	ldr r0, =string1
	cmp r1, v2					@�ˬd�O�_����
	bgt exit
	sub r1, r1, v6				@�p��۹�PC
	bl printf
	
	@�ˬd�O�_��DPI
	ldr v3, [v1]																@Ū�J���O
	and v3, v3, #0x0c000000		@[27:26]
	mov v3, v3, lsr #26
	cmp v3, #0					@DPI[27:26]��00
	bne n_DPI					@���ODPI
	
	@condition code
	ldr v3, [v1]
	mov v3, v3, lsr #28			@[31:28]
	ldr r0, =string2
	add r0, r0, v3, lsl #3		@�C�Ӹ�Ʀ�8�Ӧr��
	bl printf
	
	@opcode
	ldr v3, [v1]
	and v3, v3, #0x01e00000		@[24:21]
	mov v3, v3, lsr #21
	ldr r0, =string4
	add r0, r0, v3, lsl #3		@�C�Ӹ�Ʀ�8�Ӧr��
	bl printf
	
	@destination register
	mov v4, v3					@v4�x�sopcode
	mov v5, #8
	cmp v5, v4
	cmple v4, #11
	ldrle r0, =string8			@test, compare�S��Rd
	ldrgt r0, =string5
	ldr v3, [v1]
	and v3, v3, #0x0000f000		@[15:12]
	mov r1, v3, lsr #12
	bl printf
	
	@first operand register
	ldr r0, =string5
	cmp v4, #13					@MOV�S��Op1
	ldreq r0, =string8
	cmp v4, #15					@MVN�S��Op1
	ldreq r0, =string8
	ldr v3, [v1]
	and v3, v3, #0x000f0000		@[19:16]
	mov r1, v3, lsr #16
	bl printf
	
	@# - �ˬd�Oimmediate(1)��shift(0)
	ldr v3, [v1]
	and v3, v3, #0x02000000		@[25]
	mov v3, v3, lsr #25
	cmp v3, #0
	
	@immediate(1)
	ldrgt v4, [v1]
	andgt v4, v4, #0x000000ff	@8-bit immediate
	ldrgt v5, [v1]
	andgt v5, v5, #0x00000f00	@#rot
	movgt v5, v5, lsr #7		@�ֲ��@��N���H2	
	rorgt r1, v4, v5			@immediate ���� (2*(#rot))��
	ldrgt r0, =string6
	
	@shift(0)
	ldreq v4, [v1]
	andeq r1, v4, #0x0000000f	@Rm - second operand register
	ldreq r0, =string7
	bl printf
	
	add v1, v1, #4				@����U�ӫ��O
	b loop						@�^��loop�~��B�z

@���ODPI
n_DPI:
	ldr r0, =string3			@"xxx"
	bl printf
	add v1, v1, #4				@����U�ӫ��O
	b loop

exit:	
	@epilogue
	ldmfd	sp!, {fp, lr}
	bx	lr

@data section

string0:
	.ascii "PC\tcondition\tinstruction\tdstreg\t1st-op\t2nd-op\n\0"
string1:
	@for PC
	.ascii "%d\t\0"
string2:
	@for condition
	@�['\0'���C�Ӹ�Ƴ��O8�Ӧr��
	.ascii "EQ\t\0\0\0\0\0"
    .ascii "NE\t\0\0\0\0\0"
    .ascii "CS/HS\t\0\0"
    .ascii "CC/LO\t\0\0"
    .ascii "MI\t\0\0\0\0\0"
    .ascii "PL\t\0\0\0\0\0"
    .ascii "VS\t\0\0\0\0\0"
    .ascii "VC\t\0\0\0\0\0"
    .ascii "HI\t\0\0\0\0\0"
    .ascii "LS\t\0\0\0\0\0"
    .ascii "GE\t\0\0\0\0\0"
    .ascii "LT\t\0\0\0\0\0"
    .ascii "GT\t\0\0\0\0\0"
    .ascii "LE\t\0\0\0\0\0"
    .ascii "AL\t\0\0\0\0\0"
    .ascii "NV\t\0\0\0\0\0"
string3:
	@for not a DPI
	.ascii "\t\txxx\n\0"
string4:
	@for DPI
	@�['\0'���C�Ӹ�Ƴ��O8�Ӧr��
	.ascii "\tAND\t\t\0\0"
    .ascii "\tEOR\t\t\0\0"
    .ascii "\tSUB\t\t\0\0"
    .ascii "\tRSB\t\t\0\0"
    .ascii "\tADD\t\t\0\0"
    .ascii "\tADC\t\t\0\0"
    .ascii "\tSBC\t\t\0\0"
    .ascii "\tRSC\t\t\0\0"
    .ascii "\tTST\t\t\0\0"
    .ascii "\tTEQ\t\t\0\0"
    .ascii "\tCMP\t\t\0\0"
    .ascii "\tCMN\t\t\0\0"
    .ascii "\tORR\t\t\0\0"
    .ascii "\tMOV\t\t\0\0"
    .ascii "\tBIC\t\t\0\0"
    .ascii "\tMVN\t\t\0\0"
string5:
	@for dstreg, 1st-op
	.ascii "r%d\t\0"
string6:
	@for immediate
	.ascii "%u\n\0"
string7:
	@for second operand register
	.ascii "r%d\n\0"
string8:
	@for test, compare no Rd
    .ascii "  \t\0"
    .end
