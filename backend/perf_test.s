.data
PRINT_INT:
	.string	"%d\n"
PRINT_FLOAT:
	.string	"%f\n"
PRINT_FLOAT_VECT:
	.string	"%f "
PRINT_INT_VECT:
	.string	"%d "
PRINT_ENDL:
	.string	"\n"
.align 16
SSE_CONST_ONE:
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
.text
.globl main
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	pushl	$400000016
	pushl	$16
	subl	$4, %esp
	movl	%esp, (%esp)
	call	posix_memalign
	movl	(%esp), %ecx
	movl	%ecx, -4(%ebp)
	addl	$12, %esp
	pushl	$400000016
	pushl	$16
	subl	$4, %esp
	movl	%esp, (%esp)
	call	posix_memalign
	movl	(%esp), %ecx
	movl	%ecx, -8(%ebp)
	addl	$12, %esp
	movl	$0, %ecx
	movl	%ecx, -16(%ebp)
	movl	$0, %ecx
	movl	%ecx, -12(%ebp)
	movl	-4(%ebp), %ebx
	movl	$100000000, %eax
L_ECC_0:
	addl	$1, (%ebx)
	addl	$4, %ebx
	subl	$1, %eax
	cmp	$0, %eax
	jg	L_ECC_0
	movl	-8(%ebp), %ebx
	movl	$100000000, %eax
L_ECC_1:
	addl	$1, (%ebx)
	addl	$4, %ebx
	subl	$1, %eax
	cmp	$0, %eax
	jg	L_ECC_1
	movl	-4(%ebp), %ecx
	movl	-8(%ebp), %ebx
	movl	$100000000, %eax
L_ECC_2:
	movl	(%ebx), %edx
	addl	%edx, (%ecx)
	addl	$4, %ebx
	addl	$4, %ecx
	subl	$1, %eax
	cmp	$0, %eax
	jg	L_ECC_2
loop:
	movl	-4(%ebp), %ebx
	movl	-12(%ebp), %edx
	imull	$4, %edx
	addl	%edx, %ebx
	movl	(%ebx), %ecx
	addl	%ecx, -16(%ebp)
	add	$1, -12(%ebp)
	movl	-12(%ebp), %ecx
	movl	$10000000, %edx
	cmp	%edx, %ecx
	jge	L_ECC_3
	jmp	loop
L_ECC_3:
	pushl	-4(%ebp)
	call	free
	addl	$4, %esp
	pushl	-8(%ebp)
	call	free
	addl	$4, %esp
	movl	$0, %eax
	movl	%ebp, %esp
	popl	%ebp
	ret
	pushl	-4(%ebp)
	call	free
	addl	$4, %esp
	pushl	-8(%ebp)
	call	free
	addl	$4, %esp
	movl	%ebp, %esp
	popl	%ebp
	ret
