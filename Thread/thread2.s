	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_routine                        ## -- Begin function routine
	.p2align	4, 0x90
_routine:                               ## @routine
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$0, -4(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$1000000, -4(%rbp)              ## imm = 0xF4240
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	_mails(%rip), %eax
	addl	$1, %eax
	movl	%eax, _mails(%rip)
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	LBB0_1
LBB0_4:
	xorl	%eax, %eax
                                        ## kill: def $rax killed $eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	leaq	-24(%rbp), %rdi
	xorl	%eax, %eax
	movl	%eax, %ecx
	leaq	_routine(%rip), %rdx
	movq	%rcx, %rsi
	callq	_pthread_create
	cmpl	$0, %eax
	je	LBB1_2
## %bb.1:
	leaq	L_.str(%rip), %rdi
	callq	_perror
	movl	$1, %edi
	callq	_exit
LBB1_2:
	leaq	-32(%rbp), %rdi
	xorl	%eax, %eax
	movl	%eax, %ecx
	leaq	_routine(%rip), %rdx
	movq	%rcx, %rsi
	callq	_pthread_create
	cmpl	$0, %eax
	je	LBB1_4
## %bb.3:
	leaq	L_.str(%rip), %rdi
	callq	_perror
	movl	$1, %edi
	callq	_exit
LBB1_4:
	movq	-24(%rbp), %rdi
	xorl	%eax, %eax
	movl	%eax, %esi
	callq	_pthread_join
	cmpl	$0, %eax
	je	LBB1_6
## %bb.5:
	leaq	L_.str.1(%rip), %rdi
	callq	_perror
	movl	$1, %edi
	callq	_exit
LBB1_6:
	movq	-32(%rbp), %rdi
	xorl	%eax, %eax
	movl	%eax, %esi
	callq	_pthread_join
	cmpl	$0, %eax
	je	LBB1_8
## %bb.7:
	leaq	L_.str.1(%rip), %rdi
	callq	_perror
	movl	$1, %edi
	callq	_exit
LBB1_8:
	movl	_mails(%rip), %esi
	leaq	L_.str.2(%rip), %rdi
	movb	$0, %al
	callq	_printf
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_mails                          ## @mails
.zerofill __DATA,__common,_mails,4,2
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"PTHREAD_CREATE"

L_.str.1:                               ## @.str.1
	.asciz	"PTHREAD_JOIN"

L_.str.2:                               ## @.str.2
	.asciz	"Numero di mail: %d\n"

.subsections_via_symbols
