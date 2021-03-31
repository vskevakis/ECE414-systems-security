Vasilis Skevakis
AM: 2012030033

1) Finding the buffer and return address
By running the Greeter program using the gdm we can find the address for the buffer (p &buf) and by overfilling the buffer until our return address
is 0x414141 we can find the return address location. The total number of bytes we have to fill with our exploited buffer is 52 bytes.

2) Generate Shellcode
To generate my shellcode I used the assembly code of the file "shellcode.asm" which I built with the makefile on the genShell folder.
By running the "./shellcode" we can test our assembly code and we can use "make raw" to  generate our raw shellcode that we later use on our python exploit.

3) Input file generation
I wrote a python script that prints 52 bytes that contain some no-op characters, our generated raw shellcode and a specific return address (repeated 5 times).
The return address that we choose is the address of the Name buffer on the Greeter program, because this buffer is directly executable due to the mprotect
that the Greeter program uses. So when the buf buffer overflows, the return address will lead the program to execute our malicious shellcode and
will launch a terminal.

4) Testing the exploit
By running "(python2 ./exploit.py ; cat) | ./Greeter" we can't see it but it launches a terminal and we can use ls or whoami or any other bash command
to test it out. We may be able to launch the terminal on a different window in future work.
