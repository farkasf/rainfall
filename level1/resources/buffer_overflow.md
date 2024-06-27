# buffer overflow

In most systems, the stack grows downwards, from higher memory addresses to lower memory addresses. When a function is called, a stack frame is created that contains:

- <u>return address</u>: the address to return to after the function finishes.
- <u>saved Frame Pointer (SFP)</u>: the previous frame's base pointer, used to restore the stack after the function call.
- <u>local variables</u>: space for the function's local variables.
- <u>buffer</u>: an array or a buffer that the function uses.

## vulnerable function
Let's consider a simple example in C:
``` C
void vulnerable_function() {
	char buffer[10];
	// Some operations
}

int main() {
	vulnerable_function();
	return 0;
}
```
When <code>vulnerable_function</code> is called, the stack frame might look like this:
``` python
|-------------------------| <- High Memory Address
|       Return Address    |
|    Saved Frame Pointer  |
|       Local Variables   |
| buffer[9] (1 byte)      |
| buffer[8] (1 byte)      |
| buffer[7] (1 byte)      |
| buffer[6] (1 byte)      |
| buffer[5] (1 byte)      |
| buffer[4] (1 byte)      |
| buffer[3] (1 byte)      |
| buffer[2] (1 byte)      |
| buffer[1] (1 byte)      |
| buffer[0] (1 byte)      | <- Low Memory Address
|-------------------------|
```

## buffer overflow attack
A buffer overflow occurs when data written to the buffer exceeds its allocated size and overwrites adjacent memory. Here's what happens if more than 10 bytes are written to <code>buffer</code>:
```python
|-------------------------|
|       Return Address    | <- Potentially overwritten
|    Saved Frame Pointer  | <- Potentially overwritten
|       Local Variables   | <- Potentially overwritten
| buffer[19] (overflow)   | <- Overflowing into other stack areas
| buffer[18] (overflow)   |
| buffer[17] (overflow)   |
| buffer[16] (overflow)   |
| buffer[15] (overflow)   |
| buffer[14] (overflow)   |
| buffer[13] (overflow)   |
| buffer[12] (overflow)   |
| buffer[11] (overflow)   |
| buffer[10] (overflow)   |
| buffer[9]  (1 byte)     |
| buffer[8]  (1 byte)     |
| buffer[7]  (1 byte)     |
| buffer[6]  (1 byte)     |
| buffer[5]  (1 byte)     |
| buffer[4]  (1 byte)     |
| buffer[3]  (1 byte)     |
| buffer[2]  (1 byte)     |
| buffer[1]  (1 byte)     |
| buffer[0]  (1 byte)     |
|-------------------------|
```

## exploiting the overflow
If an attacker can control the data written to the buffer, they can overwrite the return address with an address of their choice. This could redirect the program's execution to malicious code. For example:
1. <b>Overwriting the Return Address</b>: By providing more than 10 bytes of input, the attacker can overwrite the return address stored on the stack.
2. <b>Redirecting Execution</b>: By setting the return address to the location of their malicious code (often injected elsewhere in the buffer), the attacker can gain control over the execution flow.

Here is a simple schematic of how the overflow works:

``` python
Before Overflow:

|-------------------------|
|       Return Address    |
|    Saved Frame Pointer  |
|       Local Variables   |
| buffer[9]               |
| buffer[8]               |
| buffer[7]               |
| buffer[6]               |
| buffer[5]               |
| buffer[4]               |
| buffer[3]               |
| buffer[2]               |
| buffer[1]               |
| buffer[0]               |
|-------------------------|

After Overflow (assuming attacker input overflows buffer):

|-------------------------|
| New Return Address      | <- Overwritten by attacker
| New SFP (optional)      | <- Possibly overwritten
| Malicious Code / Data   | <- Overflow data
| buffer[9]               |
| buffer[8]               |
| buffer[7]               |
| buffer[6]               |
| buffer[5]               |
| buffer[4]               |
| buffer[3]               |
| buffer[2]               |
| buffer[1]               |
| buffer[0]               |
|-------------------------|
```

## calculating the offset
The 'offset' in the context of a buffer overflow attack refers to the number of bytes between the start of the buffer and the location of the return address on the stack. By calculating this offset, an attacker knows exactly how much data to write in order to overwrite the return address. The return address is typically located immediately after the buffer and other local variables. If there are no other local variables, the return address will be at an offset of <code>[buffersize]</code> bytes plus the size of any padding or saved frame pointer.

```python
|-------------------------| <- High Memory Address
|       Return Address    | <- Offset = 14 (if 2 bytes padding)
|    Saved Frame Pointer  | <- Offset = 12 (if 2 bytes padding)
|       Padding (if any)  | <- Offset = 10 (if 2 bytes padding)
| buffer[9] (1 byte)      | <- Offset = 9
| buffer[8] (1 byte)      | <- Offset = 8
| buffer[7] (1 byte)      | <- Offset = 7
| buffer[6] (1 byte)      | <- Offset = 6
| buffer[5] (1 byte)      | <- Offset = 5
| buffer[4] (1 byte)      | <- Offset = 4
| buffer[3] (1 byte)      | <- Offset = 3
| buffer[2] (1 byte)      | <- Offset = 2
| buffer[1] (1 byte)      | <- Offset = 1
| buffer[0] (1 byte)      | <- Offset = 0
|-------------------------|
```
