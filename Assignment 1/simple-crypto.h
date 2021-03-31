#ifndef SIMPLE_CRYPTO_H
#define SIMPLE_CRYPTO_H

char * encryptOTP(char* input);
char * decryptOTP(char* enc);
char * encryptCaesars(char* input, int n);
char * decryptCaesars(char* enc, int n);
char * encryptVigenere(char* input, char* key);
char * decryptVigenere(char* enc, char* key);

#endif
