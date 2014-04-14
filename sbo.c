#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char buffer[64];         // minimal 64 bit shellcode is 37 bytes 8+64 bytes
	//printf("Esto %p\n", buffer);	
	strcpy(buffer, argv[1]); // insecure function, better strncpy

	//void *p;
	//asm("mov %%ebp, %0":"=a"(p));//Movemos ebp a p
	//printf("Esto %p\n", p);	
	return 0;

}

/*
0000000000400440 <main>:
  400440:	48 83 ec 48          	sub    $0x48,%rsp
  400444:	48 8b 76 08          	mov    0x8(%rsi),%rsi Paso el argv a rsi que es un parametro
  400448:	48 89 e7             	mov    %rsp,%rdi	buffer aquí, son los parametros de strcpy
  40044b:	e8 c0 ff ff ff       	callq  400410 <strcpy@plt>
  400450:	31 c0                	xor    %eax,%eax
  400452:	48 83 c4 48          	add    $0x48,%rsp Liberar pila
  400456:	c3                   	retq

main recibe argc en rdi = 2 y argv en rsi = 0x7fffffffde58 (argv[0], argv[1],...)

inicialmente (en ddd)...
	(gdb) info r
	rax            0x400440
	rbx            0x0
	rcx            0x400550
	rdx            0x7fffffffde70
	rsi            0x7fffffffde58
	rdi            0x2
	rbp            0x0
	rsp            0x7fffffffdd78

rsp se decrementa en 0x48 = 72 = 8 + 64 (variable buffer + espacio para buffer)

after:
  400440:	48 83 ec 48          	sub    $0x48,%rsp --> rsp = 0x7fffffffdd30

after:
  400444:	48 8b 76 08          	mov    0x8(%rsi),%rsi --> rsi = 0x7fffffffe1f8

after:
  400448:	48 89 e7             	mov    %rsp,%rdi --> rdi = 0x7fffffffdd30

after:
  40044b:	e8 c0 ff ff ff       	callq  400410 <strcpy@plt> --> rsp = 0x7fffffffdd28, rip = 0x400410 in strcpy@plt; saved rip 0x400450

0000000000400410 <strcpy@plt>:
  400410:	ff 25 02 0c 20 00    	jmpq   *0x200c02(%rip)        # 601018 <_GLOBAL_OFFSET_TABLE_+0x18>
  400416:	68 00 00 00 00       	pushq  $0x0
  40041b:	e9 e0 ff ff ff       	jmpq   400400 <_init+0x20>



char *strcpy(char *dest, const char *orig);
             rdi = rsp =  0x7fffffffddb0 | rsi = 0x7fffffffde18 (argv + 8)

*/
