#include <stdio.h>
#include "simple-crypto.h"

#define N 50

int main()
{
    int key;
    char input[N], charKey[N];

    while(1) {
    
    // Using Caesars
    printf("\n[Caesars] input: ");
    scanf("%s", input);
    printf("[Caesars] key: ");
    scanf("%d", &key);
    printf("[Caesars] encrypted: %s", encryptCaesars(input, key));
    printf("\n[Caesars] decrypted: %s", decryptCaesars(encryptCaesars(input, key), key));

    
    }
    return 0;
}
