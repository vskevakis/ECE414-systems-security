#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>

#include <openssl/kdf.h>

#define BLOCK_SIZE 16


/* function prototypes */
void print_hex(unsigned char *, size_t);
void print_string(unsigned char *, size_t); 
void usage(void);
void check_args(char *, char *, unsigned char *, int, int);
void keygen(unsigned char *, unsigned char *, unsigned char *, int);
void my_encrypt(unsigned char *, int, unsigned char *, unsigned char *, 
    unsigned char *, int );
int decrypt(unsigned char *, int, unsigned char *, unsigned char *, 
    unsigned char *, int);
void gen_cmac(unsigned char *, size_t, unsigned char *, unsigned char *, int);
int verify_cmac(unsigned char *, unsigned char *);



/* TODO Declare your function prototypes here... */



/*
 * Prints the hex value of the input
 * 16 values per line
 */
void
print_hex(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++) {
			if (!(i % 16) && (i != 0))
				printf("\n");
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
}


/*
 * Prints the input as string
 */
void
print_string(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++)
			printf("%c", data[i]);
		printf("\n");
	}
}


/*
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void
usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "    assign_1 -i in_file -o out_file -p passwd -b bits" 
	        " [-d | -e | -s | -v]\n"
	    "    assign_1 -h\n"
	);
	printf(
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -p    psswd   Password for key generation\n"
	    " -b    bits    Bit mode (128 or 256 only)\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -s            Encrypt+sign input and store results to output\n"
	    " -v            Decrypt+verify input and store results to output\n"
	    " -h            This help message\n"
	);
	exit(EXIT_FAILURE);
}


/*
 * Checks the validity of the arguments
 * Check the new arguments you introduce
 */
void
check_args(char *input_file, char *output_file, unsigned char *password, 
    int bit_mode, int op_mode)
{
	if (!input_file) {
		printf("Error: No input file!\n");
		usage();
	}

	if (!output_file) {
		printf("Error: No output file!\n");
		usage();
	}

	if (!password) {
		printf("Error: No user key!\n");
		usage();
	}

	if ((bit_mode != 128) && (bit_mode != 256)) {
		printf("Error: Bit Mode <%d> is invalid!\n", bit_mode);
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


/*
 * Generates a key using a password
 */
void
keygen(unsigned char *password, unsigned char *key, unsigned char *iv,
    int bit_mode)
{
	iv = 1;
	int keylen;
	unsigned char *salt = NULL;
	if ( !bit_mode && bit_mode == 256)
		keylen = 256;
	else if (bit_mode == 128)
		keylen = 128;
	else {
		printf("Unacceptable key length size");
		return;
	}

	PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), (unsigned char*)salt, strlen(salt), iv, 32+16, key);
    printf("PKCS5_PBKDF2_HMAC_SHA1(\"%s\", \"%s\", %d)=\n", password, salt, iv);
    print_hex(key, 32+16);

	iv = 1;
    EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (unsigned char*)salt, (unsigned char*)password, strlen(password), iv, key, key+32);
    printf("EVP_BytesToKey(\"%s\", \"%s\", %d)=\n", password, salt, iv);
    print_hex(key, 32+16);
	
	return;

}


/*
 * Encrypts the data
 */
void my_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext, int bit_mode)
{

	/* TODO Task B */
	EVP_CIPHER_CTX *ctx;
	EVP_CIPHER *cipher_type;
	int cipherlen = strlen(ciphertext);
	key = "0123456789abcdeF";
	iv = "1234567887654321";

	/* Don't set key or IV right away; we want to check lengths */
	// if (bit_mode = 128)
	// 	cipher_type = ENV_aes_128_cbc();
	// else if(bit_mode = 256)
	// 	cipher_type = ENV_aes_256_cbc();
	// else
	// 	return;
	cipher_type = ENV_aes_256_cbc();

	ctx = EVP_CIPHER_CTX_new();
	EVP_CipherInit_ex(ctx, cipher_type, NULL, key, iv, my_encrypt);
	// OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
	// OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);

	/* Now we can set key and IV */
	// EVP_CipherInit_ex(ctx, NULL, NULL, key, iv, encrypt);

	for (;;) {
		// inlen = fread(plaintext, 1, 1024, in);
		if (plaintext_len <= 0)
			break;
		if (!EVP_CipherUpdate(ctx, ciphertext, &cipherlen, plaintext, plaintext_len)) {
			/* Error */
			printf(ciphertext);
			EVP_CIPHER_CTX_free(ctx);
			return;
		}
		// fwrite(ciphertext, 1, cipherlen, out);
	}
	if (!EVP_CipherFinal_ex(ctx, ciphertext, &cipherlen)) {
		/* Error */
		EVP_CIPHER_CTX_free(ctx);
		return;
	}
	// fwrite(ciphertext, 1, cipherlen, out);

	EVP_CIPHER_CTX_free(ctx);

return;
}


/*
 * Decrypts the data and returns the plaintext size
 */
int
decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext, int bit_mode)
{
	int plaintext_len;

	plaintext_len = 0;

	/*TODO Task C */

	return plaintext_len;
}


/*
 * Generates a CMAC
 */
void
gen_cmac(unsigned char *data, size_t data_len, unsigned char *key, 
    unsigned char *cmac, int bit_mode)
{

	/* TODO Task D */

}


/*
 * Verifies a CMAC
 */
int
verify_cmac(unsigned char *cmac1, unsigned char *cmac2)
{
	int verify;

	verify = 0;

	/* TODO Task E */

	return verify;
}



/* TODO Develop your functions here... */



/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with 
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int
main(int argc, char **argv)
{
	int opt;			/* used for command line arguments */
	int bit_mode;			/* defines the key-size 128 or 256 */
	int op_mode;			/* operation mode */
	char *input_file;		/* path to the input file */
	char *output_file;		/* path to the output file */
	unsigned char *password;	/* the user defined password */

	/* Init arguments */
	input_file = NULL;
	output_file = NULL;
	password = NULL;
	bit_mode = -1;
	op_mode = -1;


	/*
	 * Get arguments
	 */
	while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:")) != -1) {
		switch (opt) {
		case 'b':
			bit_mode = atoi(optarg);
			break;
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'p':
			password = (unsigned char *)strdup(optarg);
			break;
		case 'd':
			/* if op_mode == 1 the tool decrypts */
			op_mode = 1;
			break;
		case 'e':
			/* if op_mode == 1 the tool encrypts */
			op_mode = 0;
			break;
		case 's':
			/* if op_mode == 1 the tool signs */
			op_mode = 2;
			break;
		case 'v':
			/* if op_mode == 1 the tool verifies */
			op_mode = 3;
			break;
		case 'h':
		default:
			usage();
		}
	}


	/* check arguments */
	check_args(input_file, output_file, password, bit_mode, op_mode);



	/* TODO Develop the logic of your tool here... */




	/* Initialize the library */


	/* Keygen from password */


	/* Operate on the data according to the mode */
	/* encrypt */

	/* decrypt */

	/* sign */

	/* verify */
		

	/* Clean up */
	free(input_file);
	free(output_file);
	free(password);


	/* END */
	return 0;
}
