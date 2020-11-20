#include <stdio.h>
#include "simple-crypto.h"

#define N 50

int main()
{
    int key;
    char input[N], charKey[N];

    // Using One Time Pad
    // Key is being generated inside otpEncrypt()
    printf("[OTP] input: ");
    scanf("%s", input);
    printf("[OTP] encrypted: %s", encryptOTP(input));
    printf("\n[OTP] decrypted %s", decryptOTP(encryptOTP(input)));
    
    // Using Caesars
    printf("\n[Caesars] input: ");
    scanf("%s", input);
    printf("[Caesars] key: ");
    scanf("%d", &key);
    printf("[Caesars] encrypted: %s", encryptCaesars(input, key));
    printf("\n[Caesars] decrypted: %s", decryptCaesars(encryptCaesars(input, key), key));

    // // Using Vigenere's - Only Caps Allowed
    printf("\n[Vigenere] input: ");
    scanf("%s", input);
    printf("[Vigenere] key: ");
    scanf("%s", charKey);
    printf("[Vigenere] encrypted: %s", encryptVigenere(input, charKey));
    printf("\n[Vigenere] decrypted: %s", decryptVigenere(encryptVigenere(input, charKey), charKey));

    return 0;
}
