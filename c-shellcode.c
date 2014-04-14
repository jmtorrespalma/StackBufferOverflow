#include <unistd.h>

int main()
{
	execve("/bin/sh", NULL, NULL);
}

/*
0000000000400ac6 <main>:
  400ac6:	31 d2                	xor    %edx,%edx
  400ac8:	31 f6                	xor    %esi,%esi
  400aca:	bf 10 94 48 00       	mov    $0x489410,%edi
  400acf:	e9 cc d1 00 00       	jmpq   40dca0 <__execve>
  400ad4:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400adb:	00 00 00 
  400ade:	66 90                	xchg   %ax,%ax
*/
