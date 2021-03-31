TASK A - KDF
Key derived from password will always be 160 bit's (length) because we user SHA1 for hashing

TASK B - Data Encryption
Implemented using EVP API and encrypts the data with a given key that is generated with a given password and a bit_mode

TASK C - Data Decryption
Its basicly the reverse proccess of the data encryption.

TASK D - Data Signing
Using the CMAC API to produce a CMAC and add the CMAC to the end of the text.

TASK E - Data Verification
The CMAC is always 16 bits of length so we split the read ciphertext and keep the last 16 bits as a cmac. 
We derive another cmac using the gen_cmac function after decrypting our ciphertext and compare the two cmac's

TASK-F - Using the Tool
1. Encryption is successful
2. Decryption is successful
3. Signed the file with success
4. Can't verify either files