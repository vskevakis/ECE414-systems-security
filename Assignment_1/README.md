# Simple ciphers using C 

Assignment #1 of Systems Security Class (ACE414).
[Git Repo](https://github.com/vskevakis/ECE414-systems-security/tree/first-branch/simple-crypto)

## Usage

Make your own demos or run the demos that are available using the Makefile.

```sh
$ cd simple-crypto
$ make
```
or
```sh
$ cd simple-crypto
$ gcc simple-crypto.c -c
$ gcc demo.c -c
$ gcc simple-crypto.o demo.o -o demo
$ ./demo
```

## Description

Learning to implement some simple cryptographic algorithms using C. This assignment contains implementation of One Time Pad, Caesar's Cipher and Vigenère's Cipher algorithms using ASCII Code.

### One Time Pad Algorithm

One Time Pad is a simple but strong algorithm that uses a secret key encrypts the user's input by XOR-ing each digit of input and key.

#### Random Key Generator

The secret key is randomly generated with one condition, which is to use only printable characters of the ASCII Code (ASCII[33] - ASCII[126]), because we wanted to be able to print the key to the user or to store it without any complications. After we generate the key, we store it at a static array of characters, in order to be able to use it across our files.

#### One Time Encryption

We run through the user input and key buffers and XOR each character. An important step is to run each character through a modulo with 126 (Maximum ASCII printable character value) and add 33 (Minimum ASCII printable character value) to make sure that the encrypted buffer is printable.

#### One Time Decryption

We reverse the encryption process using as input the encrypted buffer minus the previously added 33 to ensure that we don't exceed the printable characters margin.

### Caesar's Cipher

Caesar's Cipher is one of the simplest and most widely known encryption techniques. It is a type
of substitution cipher in which each byte/character of the plaintext is replaced by a
byte/character found at some fixed number of positions down the ASCII set. 

#### Caesar's Encryption

We run through the users input buffer and we set as target, the users input plus the key number. Afterwards, we check if target value, exceeds the allowed ASCII Values ('0'-'9', 'a'-'z', 'A'-'Z') and if it exceeds we the target to the correct value by adding the difference between the next allowed value pair to the target.

#### Caesar's Decryption

We reverse the encryption process by subtracting the key value from the input buffer. An important step is to use absolute values to offset the target.

### Vigenère's Cipher

The Vigenère’s cipher encrypts an alphabetic plaintext using a series of interwoven Caesar’s
ciphers. 

#### Vigenère Encryption

First of all we need to repeat the key buffer until its the same length as the user input.Then we run through the user's buffer and the key's buffer and we add them. After the addition we use modulo 26 (The number of letters on the english alphabet) to contain our result inside the alphabet. Lastly we add 'A' or ASCII[65] to move our result inside the A-Z area of the ASCII Code. 

#### Vigenère Decryption

Again we check the key and repeat it till its the same length as the user's input and we reverse the encryption process. We subtract the key from the encrypted buffer and we reverse the modulo process we explained before.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)

