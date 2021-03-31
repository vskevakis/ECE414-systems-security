#include "rsa.h"
#include "utils.h"

/*
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  Returns a random prime from prime pool and the length of the pool
 */
size_t *
sieve_of_eratosthenes(int limit, int *primes_sz)
{
	// size_t *primes, *temp;
	// size_t *primes = (size_t*) malloc(RSA_SIEVE_LIMIT * sizeof(size_t));
	size_t primes[RSA_SIEVE_LIMIT];
	size_t *temp = (size_t*) malloc(RSA_SIEVE_LIMIT * sizeof(size_t));
	/* TODO */	
	// int primes[*primes_sz]; 
	size_t number, i, j;
    for (i=2;i<=limit;i++)
		temp[i]=i;
	
	i=2;
	while ((i*i)<=limit)
	{
		if (temp[i] !=0)
		{
			for (j=2;j<limit;j++)
			{
				if (temp[i]*j>limit)
					break;
				else
					temp[temp[i]*j]=0;
				
			}
		}
		i++;
	}
	j=0;
	for(i = 2; i<=limit; i++)
    {
        //If number is not 0 then it is prime
        if (temp[i]!=0) {
            primes[j] = temp[i];
			j++;
		}
    }
	primes_sz = j;

	int ran = arc4random_uniform(j)+1;
	return primes[ran];
}


/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int
gcd(int a, int b)
{
	/* TODO */
	int i,gcd;

	for (i=1 ; i<=a && i<=b ; i++)
	{
		if (a%i==0 && b%i==0)
			gcd = i;
	}
	return gcd;
}


/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t
choose_e(size_t fi_n)
{
	size_t e;

	/* TODO */
	for (e = 5; e <= 100; e++) {
		if (e % fi_n != 0 && gcd(fi_n, e) == 1)
			break;
	}

	return e;
}


/*
 * Calculates the modular inverse
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: modular inverse
 */
size_t
mod_inverse(size_t a, size_t b)
{

	/* TODO */
	size_t d;

	for (d = a + 1 ; d <= 100 ; d++) {
		if ( ( (d*a) % b) == 1 )
			break;
	}

	return d;
}

/*
 * Reads a whole file as char
 *
 * arg0: input file name
 * arg1: file buffer
 * 
 * ret: 'buffer len' 
 */
int 
read_file(char * filename, unsigned char * buffer)
{
    FILE *file = fopen(filename, "rb"); 

    /* Return in case of error */
    if (file == NULL){
      return 0; 
  	}

	fseek(file, 0L, SEEK_END); // Find the length of the file
	int fsize = ftell(file); // Save Length
	fseek(file, 0L, SEEK_SET); // Return to the start of the file

	fread(buffer, MAXSIZE, fsize, file); // Read Buffer

	fclose(file); 
	
	return fsize;

}

/*
 * Reads a whole file as size_t
 *
 * arg0: input file name
 * arg1: file buffer
 * 
 * ret: 'buffer len' 
 */
int 
read_sizet_file(char * filename, size_t * buffer)
{
    FILE *file = fopen(filename, "rb"); 

    /* Return in case of error */
    if (file == NULL){
      return 0; 
  	}

	fseek(file, 0L, SEEK_END); // Find the length of the file
	int fsize = ftell(file); // Save Length
	fseek(file, 0L, SEEK_SET); // Return to the start of the file

	fread(buffer, MAXSIZE, fsize, file); // Read Buffer

	fclose(file); 
	
	return fsize;

}

/*
 * Writes char to output file
 *
 * arg0: output file name
 * arg1: buffer to write
 * arg2: buffer length
 */
void 
write_char_file(char *filename, unsigned char * buffer, int buffer_len)
{

    FILE *file = fopen(filename,"w");

	/* Return in case of error */
    if (file == NULL){
      return; 
  	}
	/* write buffer char by char */
    for (int i =0; i< buffer_len; i++){
       fwrite(&buffer[i], sizeof(unsigned char), 1,file);
   	}

   fclose(file);
   return;

}

/*
 * Writes size_t to output file
 *
 * arg0: output file name
 * arg1: buffer to write
 * arg2: buffer length
 */
void 
write_sizet_file(char *filename, size_t * buffer, int buffer_len)
{

    FILE *file = fopen(filename,"w");

	/* Return in case of error */
    if (file == NULL){
      return; 
  	}
	/* write buffer char by char */
    for (int i =0; i< buffer_len; i++){
       fwrite(&buffer[i], sizeof(size_t), 1,file);
   	}

   fclose(file);
   return;

}


/*
 *
 * arg0: text
 * arg1: e or d
 * arg2: n
 */
size_t 
pmod(size_t data, size_t ed, size_t n){
	size_t x=1, y=data;

	while (ed>0) {
		if (ed%2==1)
			x = (x*y) %n;
		y = (y * y) % n;
		ed /=2;
	}

	return x % n;
	
}


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void
rsa_keygen(void)
{
	size_t p;
	size_t q;
	size_t n;
	size_t fi_n;
	size_t e;
	size_t d;

	/* TODO */
	size_t pool_sz;
	size_t *prime_pool = (size_t*) malloc(RSA_SIEVE_LIMIT * sizeof(size_t));
	prime_pool = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, pool_sz);

	/* Sieve returns a random number from the pool */
	p = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, pool_sz);
	q = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, pool_sz);

	/* Regen q until its different from p */
	while (p==q) {
		q = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, pool_sz);
	}

	n = p * q;
	fi_n = (p - 1) * (q - 1);

	e = choose_e(fi_n);
	d = mod_inverse(e, fi_n);

	/* Write Key to file */
	FILE *public_k, *private_k;

	public_k = fopen("my_public.key", "w+");
	private_k = fopen("my_private.key", "w+");

	fwrite (&n , sizeof(size_t), 1, public_k);
	fwrite (&d , sizeof(size_t), 1, public_k);
	fwrite (&n , sizeof(size_t), 1, private_k);
	fwrite (&e , sizeof(size_t), 1, private_k);

	fclose(public_k);
	fclose(private_k);
}


/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_encrypt(char *input_file, char *output_file, char *key_file)
{

	/* TODO */
	size_t n,e;
	unsigned char *key_buf = (unsigned char*) malloc(2 * sizeof(size_t));
	unsigned char *input_buf = (unsigned char*) malloc(255 * sizeof(char));

	/* Read input file and key file  */
	int keylen = read_file(key_file, key_buf);
	int inlen = read_file(input_file, input_buf);

	/* Split the key to n and e */
	int len = keylen;
	int len1 = len/2;
	int len2 = len - len1; 
	memcpy(&n, key_buf, len1);
	memcpy(&e, key_buf+len1, len2);

	/* Setup output buffer */
	/* Output buff is going to be 8 (sizeof(size_t)) times the input buff */
	size_t *output_buf = (size_t*) malloc(inlen * sizeof(size_t));
	int outlen = sizeof(size_t)*inlen;

	/* Encrypt */
	for (int i = 0; i < outlen; i++) {
		output_buf[i] = pmod(input_buf[i], e, n);
	}

	/* Write size_t buffer to file */
	write_sizet_file(output_file, output_buf, outlen);

	free(input_buf);
	free(output_buf);
	free(key_buf);
}


/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_decrypt(char *input_file, char *output_file, char *key_file)
{

	/* TODO */
	size_t n,d;
	unsigned char *key_buf = (unsigned char*) malloc(2 * sizeof(size_t));

	/* Read input file and key file  */
	int keylen = read_file(key_file, key_buf);

	/* Transform input to size_t */
	size_t *input_buf = (size_t*)malloc(255 * sizeof(char)/sizeof(size_t));

	int inlen = read_sizet_file(input_file, input_buf);

	/* Split the key to n and e */
	int len = keylen;
	int len1 = len/2;
	int len2 = len - len1; 
	memcpy(&n, key_buf, len1);
	memcpy(&d, key_buf+len1, len2);

	/* Setup output buffer */
	/* Output buff is going to be 8 (sizeof(size_t)) times the input buff */
	unsigned char *output_buf = (unsigned char*) malloc(inlen/sizeof(size_t));
	int outlen = inlen/sizeof(size_t);

	/* Decrypt */
	for (int i = 0; i < outlen; i++) {
		output_buf[i] = pmod(input_buf[i], d, n);
	}

	/* Write char buffer to file */
	write_char_file(output_file, output_buf, outlen);

	free(input_buf);
	free(output_buf);
	free(key_buf);

}
