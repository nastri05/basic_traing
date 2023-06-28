	.global _start
	.text
_start:
	# write(1, message, 26)
	mov $1, %rax # system call ID. 1 is write
	mov $1, %rdi # file handle 1 is stdout
	mov $message, %rsi # address of string to output
	mov $26, %rdx # string length
	syscall # system call invocation!
	# exit(0)
	mov $60, %rax # system call ID. 60 is exit
	xor %rdi, %rdi # we want return code 0
	syscall # system call invocation!
message:
	.ascii "ASM SYSCALL: Hello World!\n"