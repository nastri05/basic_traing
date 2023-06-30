	.file	"source.c"
	.globl	spd
	.data
	.align 4
	.type	spd, @object
	.size	spd, 4
spd:
	.long	5
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
.L2:
	movl	spd(%rip), %eax
	addl	$1, %eax
	movl	$10, %esi
	movl	%eax, %edi
	call	min
	movl	%eax, spd(%rip)
	jmp	.L2
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.12) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
