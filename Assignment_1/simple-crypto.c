#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "simple-crypto.h"

static char otpKey[250];

char * encryptOTP(char* input) {
    char * enc = (char *) malloc(sizeof(input));

    // Generating Random Key and storing it in a static 
    for (int i=0;i<strlen(input);i++) {
        otpKey[i] = arc4random_uniform(126-33)+33;

        if (otpKey[i] > 126 || otpKey[i] < 33)
            printf("\nThis is not acceptable! ASCII Code: (%d)", otpKey[i]);
    }

    // XORing and preventing non printable chars
    for (int i=0;i<strlen(input);i++) {
        enc[i] = (char)(otpKey[i] ^ input[i]);
        enc[i] = (enc[i] % 126) + 33;
    }

    free(enc);
    return enc;
}

char * decryptOTP(char* enc) {
    char * dec = (char *) malloc(sizeof(enc));

    for (int i=0;i<strlen(enc);i++) {
        dec[i] = (char)(enc[i] - 33 ^ otpKey[i]);
    }
    free(dec);
    return dec;
}

char * encryptCaesars(char* input, int n) {
    char * enc = (char *) malloc(sizeof(input));
    for (int i=0;i<strlen(input);i++) {
        int target = input[i] + n;
        if (target > 122)
            enc[i] = 47 + target - 122;
        else if (target > 90 && target < 97)
            enc[i] = 96 + target - 90;
        else if (target > 57 && target < 65)
            enc[i] = 64 + target - 57;
        else
            enc[i] = target;
    }
    free(enc);
    return enc;
}

char * decryptCaesars(char* enc, int n) { 
    char * dec = (char *) malloc(sizeof(enc));
    for (int i=0;i<strlen(enc);i++) {
        int target = enc[i] - n;
        if (target < 48)
            dec[i] = 123 - abs(target - 48);
        else if (target < 97 && target > 90)
            dec[i] = 91 - abs(target - 97);
        else if (target < 65 && target > 57)
            dec[i] = 58 - abs(target - 65);
        else
            dec[i] = target;
    }
    free(dec);
    return dec;
}

char * encryptVigenere(char* input, char* key) {
    char * enc = (char *) malloc(sizeof(input));
    char * new_key = (char *) malloc(sizeof(input));

    // Reforming the key to match the size of the input
    int counter = 0;
    for (int i=0; i <strlen(input);i++) {
        new_key[i] = key[counter];
        counter ++;
        if (!key[counter])
            counter = 0; 
    }
    // Encrypting 
    for (int i=0;i<strlen(input);i++) 
        enc[i] = (new_key[i] + input[i]) % 26 + 65;
    free(new_key);
    free(enc);
    return enc;
}

char * decryptVigenere(char* enc, char* key) {
    char * dec = (char *) malloc(sizeof(enc));
    char * new_key = (char *) malloc(sizeof(enc));

    // Reforming the key to match the size of the input
    int counter = 0;
    for (int i=0; i <strlen(enc);i++) {
        new_key[i] = key[counter];
        counter ++;
        if (!key[counter])
            counter = 0; 
    }


    // Decrypting
    for (int i=0;i<strlen(enc);i++) 
        dec[i] = (enc[i] - new_key[i] + 26) % 26 + 65 ;

    free(new_key);
    free(dec);
    return dec;
}