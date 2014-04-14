#include <iostream>

unsigned long long *rsp()
{
	unsigned long long *p;
	asm("mov %%esp, %0":"=a"(p));//Movemos rsp a p
	return p;
}

int main()
{
	unsigned long long *p = rsp();
	std::cout << "esp = " << std::hex << p
	          << "  -->  (esp) = " << std::hex << *p << std::endl;
}

/*
(gdb) file rsp

(gdb) disassemble main

0000000000400a30 <main>:
  400a30:	55                   	push   %rbp
  400a31:	ba 09 00 00 00       	mov    $0x9,%edx
  400a36:	be 70 0d 40 00       	mov    $0x400d70,%esi
  400a3b:	bf a0 20 60 00       	mov    $0x6020a0,%edi
  400a40:	53                   	push   %rbx
  400a41:	48 89 e0             	mov    %rsp,%rax
  400a44:	48 83 ec 08          	sub    $0x8,%rsp
  400a48:	48 89 c3             	mov    %rax,%rbx
  400a4b:	48 8b 28             	mov    (%rax),%rbp
  400a4e:	e8 6d ff ff ff       	callq  4009c0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400a53:	48 8b 05 46 16 20 00 	mov    0x201646(%rip),%rax        # 6020a0 <std::cout@@GLIBCXX_3.4>
  400a5a:	48 89 de             	mov    %rbx,%rsi
  400a5d:	bf a0 20 60 00       	mov    $0x6020a0,%edi
  400a62:	48 8b 40 e8          	mov    -0x18(%rax),%rax
  400a66:	8b 90 b8 20 60 00    	mov    0x6020b8(%rax),%edx
  400a6c:	83 e2 b5             	and    $0xffffffb5,%edx
  400a6f:	83 ca 08             	or     $0x8,%edx
  400a72:	89 90 b8 20 60 00    	mov    %edx,0x6020b8(%rax)
  400a78:	e8 e3 fe ff ff       	callq  400960 <std::ostream& std::ostream::_M_insert<void const*>(void const*)@plt>
  400a7d:	ba 0e 00 00 00       	mov    $0xe,%edx
  400a82:	48 89 c3             	mov    %rax,%rbx
  400a85:	be 7a 0d 40 00       	mov    $0x400d7a,%esi
  400a8a:	48 89 c7             	mov    %rax,%rdi
  400a8d:	e8 2e ff ff ff       	callq  4009c0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400a92:	48 8b 03             	mov    (%rbx),%rax
  400a95:	48 89 d9             	mov    %rbx,%rcx
  400a98:	48 89 ee             	mov    %rbp,%rsi
  400a9b:	48 89 df             	mov    %rbx,%rdi
  400a9e:	48 03 48 e8          	add    -0x18(%rax),%rcx
  400aa2:	8b 51 18             	mov    0x18(%rcx),%edx
  400aa5:	83 e2 b5             	and    $0xffffffb5,%edx
  400aa8:	83 ca 08             	or     $0x8,%edx
  400aab:	89 51 18             	mov    %edx,0x18(%rcx)
  400aae:	e8 fd fe ff ff       	callq  4009b0 <std::ostream& std::ostream::_M_insert<unsigned long long>(unsigned long long)@plt>
  400ab3:	48 89 c7             	mov    %rax,%rdi
  400ab6:	e8 35 ff ff ff       	callq  4009f0 <std::basic_ostream<char, std::char_traits<char> >& std::endl<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&)@plt>
  400abb:	48 83 c4 08          	add    $0x8,%rsp
  400abf:	31 c0                	xor    %eax,%eax
  400ac1:	5b                   	pop    %rbx
  400ac2:	5d                   	pop    %rbp
  400ac3:	c3                   	retq   

(gdb) r

(gdb) info r
rax            0x400800	4196352
rbx            0x0	0
rcx            0x60	96
rdx            0x7fffffffdf38	140737488346936
rsi            0x7fffffffdf28	140737488346920
rdi            0x1	1
rbp            0x0	0x0
rsp            0x7fffffffde48	0x7fffffffde48

(gdb) x/8gx $rsp
0x7fffffffde48:	0x00000039df221d65	0x0000000000000000
0x7fffffffde58:	0x00007fffffffdf28	0x0000000100000000
0x7fffffffde68:	0x0000000000400860	0x0000000000000000
0x7fffffffde78:	0x3422790743325712	0x00000000004008f0
*/
