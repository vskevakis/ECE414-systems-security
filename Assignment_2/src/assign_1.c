#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>
#include <openssl/aes.h>
#include <openssl/kdf.h>

#define BLOCK_SIZE 16
/* 32 byte key (256 bit key) */
#define AES_256_KEY_SIZE 32/* 16 byte block size (128 bits) */
#define AES_BLOCK_SIZE 16
#define CMAC_SIZE 16


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
	const EVP_CIPHER *cipher;
	EVP_CIPHER_CTX *ctx;
    const EVP_MD *dgst = NULL;
	unsigned char *salt = NULL;

	cipher = EVP_aes_128_ecb();
	// if (bit_mode == 256){
	// 	cipher = EVP_aes_256_ecb();
	// }

	dgst = EVP_sha1();

	if(!(ctx = EVP_MD_CTX_new()))
		return;

	if (1 != EVP_DigestInit_ex(ctx, dgst, NULL))
		return;

	if (password)
	{
		if (1 != EVP_DigestUpdate(ctx, password , strlen(password)))
			return;
	}

	if (1 != EVP_DigestFinal_ex(ctx, key, strlen(key)))
		return;


    EVP_BytesToKey(cipher, dgst, (unsigned char*)salt, (unsigned char*)password,
		strlen(password), 1, key, NULL);

	EVP_MD_CTX_free(ctx);
	return;

}


/*
 * Encrypts the data
 */
void my_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext, int bit_mode)
{
	
	int len=0, ciphertext_len=0;
	EVP_CIPHER_CTX *ctx;
	EVP_CIPHER *cipher;

	cipher = EVP_aes_128_ecb();
	if (bit_mode == 256)
		cipher = EVP_aes_256_ecb();

	// Create and initialize the context and return a pointer for success or NULL for failure
	if (!(ctx = EVP_CIPHER_CTX_new()))
		return;

	// /* Initialize the encryption operation. */
	if (1 != EVP_EncryptInit_ex(ctx, cipher, NULL, key, NULL))
		return;

	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		return;
	ciphertext_len = len;

	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);

	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);
	
	return;
}


/*
 * Decrypts the data and returns the plaintext size
 */
int
mydecrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext, int bit_mode)
{
	int plaintext_len;

	plaintext_len = 0;
	/*TODO Task C */
	int ret, len;
	EVP_CIPHER_CTX *ctx = NULL;
	EVP_CIPHER *cipher;

	cipher = EVP_aes_128_ecb();
	if (bit_mode == 256)
		cipher = EVP_aes_256_ecb();

	// Create and initialize the context and return a pointer for success or NULL for failure
	if (!(ctx = EVP_CIPHER_CTX_new()))
		return NULL;

	/* Initialise the decryption operation. */
	if (1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, NULL))
		return NULL;

	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		return NULL;

	plaintext_len = len;

	
	EVP_DecryptFinal_ex(ctx, plaintext + len, &len);

    plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);


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
	size_t cmaclen;
	int keylen;
	CMAC_CTX *ctx;
	EVP_CIPHER *cipher;

	keylen = 16;
	cipher = EVP_aes_128_ecb();

	if (bit_mode==256) {
		keylen = 32;
		cipher = EVP_aes_256_ecb();
	}

	if(!(ctx = CMAC_CTX_new()))
		return;

	if (1 != CMAC_Init(ctx, key, keylen, cipher, NULL))
		return;

	if (data)
	{
		if (1 != CMAC_Update(ctx, data , data_len))
			return;
	}

	if (1 != CMAC_Final(ctx, cmac, &cmaclen))
		return;

	CMAC_CTX_free(ctx);
	return;
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

	unsigned char cmac_help[16];
	int j=0;
	for (int i=strlen(cmac2)-strlen(cmac1);i<strlen(cmac2);i++){
		cmac_help[j]=cmac2[i];
		j++;
	}
	verify = CRYPTO_memcmp(cmac_help, cmac1, strlen(cmac1));

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
	
	/* Read the INPUT and OUTPUT File */
	FILE *infptr, *outfptr;
	long fsize=0;

	if ((outfptr = fopen(output_file, "wb")) == NULL) {
		printf("Error opening output file");
		exit(1);
	}
	else if (((infptr = fopen(input_file, "rb")) == NULL)) {
		printf("Error opening input file");
		exit(1);
	}

	/* Initialize the library */
	OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();  
	unsigned char *key = malloc(160);	/* the user defined password */
	unsigned char *plaintext;
	unsigned char *ciphertext;
	unsigned char cmac[16] = {0};
	unsigned char cmac1[16] = {0};
	unsigned char cmac2[16] = {0};

	/* Keygen from password */
	keygen(password, key, NULL, bit_mode);

	/* Operate on the data according to the mode */
	/* encrypt */
	if (op_mode==0) {
		// Read Plaintext from input file
		fseek(infptr, 0L, SEEK_END); // Find the length of the text
		fsize = ftell(infptr); 
		fseek(infptr, 0L, SEEK_SET); // Return to the begging of the file
		plaintext = calloc( 1, fsize+1 );
		ciphertext = calloc( 1, fsize+1 );
		fread(plaintext, 1, fsize, infptr); // Actually Read from the file's start to the file's end
		// Encrypt plaintext to ciphertext
		my_encrypt(plaintext, strlen((char *)plaintext), key, NULL, ciphertext, bit_mode);
		// Write cipher text to output file
		fwrite(ciphertext, sizeof(unsigned char), strlen((char *)ciphertext), outfptr);
	}

	/* decrypt */
	if (op_mode==1) {
		// Read Ciphertext from input file
		fseek(infptr, 0L, SEEK_END); // Find the length of the text
		fsize = ftell(infptr); 
		fseek(infptr, 0L, SEEK_SET); // Return to the begging of the file
		plaintext = calloc( 1, fsize+1 );
		ciphertext = calloc( 1, fsize+1 );
		fread(ciphertext, 1, fsize, infptr); // Actually Read from the file's start to the file's end
		// Decrypt ciphertext to plaintext
		int dec_len = mydecrypt(ciphertext, strlen((char *)ciphertext), key, NULL, plaintext, bit_mode);
		plaintext[dec_len]='\0'; // Prevent txt file from corrupting
		// Write plaintext to output file
		fwrite(plaintext, sizeof(unsigned char), strlen(plaintext), outfptr);
	}


	/* sign */
	if (op_mode==2) {
		// Read Plaintext from input file
		fseek(infptr, 0L, SEEK_END); // Find the length of the text
		fsize = ftell(infptr); 
		fseek(infptr, 0L, SEEK_SET); // Return to the begging of the file
		plaintext = calloc( 1, fsize+1 );
		ciphertext = calloc( 1, fsize+1 );
		fread(plaintext, 1, fsize, infptr); // Actually Read from the file's start to the file's end
		// Encrypt plaintext to ciphertext
		my_encrypt(plaintext, strlen((char *)plaintext), key, NULL, ciphertext, bit_mode);
		// Write cipher text to output file
		fwrite(ciphertext, sizeof(unsigned char), strlen((char *)ciphertext), outfptr);
		// Generate CMAC for plaintext
		gen_cmac(plaintext, strlen((unsigned char *)plaintext), key, cmac, bit_mode);
		// Write CMAC to output file (Sign the file)
		fwrite(cmac, sizeof(unsigned char), strlen((char *)cmac), outfptr);	
	}


	// /* verify */

	if (op_mode==3) {
		// Read Plaintext from input file
		fseek(infptr, 0L, SEEK_END); // Find the length of the text
		fsize = ftell(infptr); 
		fseek(infptr, 0L, SEEK_SET); // Return to the begging of the file
		plaintext = calloc( 1, fsize+1 );
		ciphertext = calloc( 1, fsize+1 );
		// Read Ciphertext and CMAC Sign
		fread(ciphertext, fsize-17, 1, infptr);
		fread(cmac2, fsize, 1, infptr);
		// Decrypt ciphertext to plaintext
		int dec_len = mydecrypt(ciphertext, strlen((unsigned char *)ciphertext), key, NULL, plaintext, bit_mode);
		// Generate CMAC From plaintext
		gen_cmac(plaintext, strlen((unsigned char *)plaintext), key, cmac1, bit_mode);
		// Verify Generated and Read CMAC
		if (verify_cmac(cmac1,cmac2)==0) {
			plaintext[dec_len]='\0'; // Prevent txt file from corrupting
			fwrite(plaintext, sizeof(unsigned char), strlen(plaintext), outfptr);
		}
		
	}

	
	/* Clean up */
	fclose(infptr);
	fclose(outfptr);
	free(input_file);
	free(output_file);
	free(password);


	/* END */
	return 0;
}
