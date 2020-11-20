#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>

/* Add to Log file func */
void
log(uid_t uid, unsigned char *file_name, char *datetime, int access_type, int is_action_denied, unsigned char md5){
	/* Log File */
	FILE *ptr;
	ptr = fopen("logfile.txt" ,"a+");
	if (ptr != NULL) {
		printf("Writting to log file");
	fprintf("\n%d %c %c %d %d %c \n", uid, file_name, datetime, access_type, is_action_denied, md5);
	}

	fclose(ptr);
}

FILE *
fopen(const char *path, const char *mode) 
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);


	/* add your code here */
	printf("Hey");
	MD5_CTX *c;
	FILE *ptr;
	uid_t uid;
	time_t current_time;
	unsigned char *file_name, *datetime;
	int file_mode, is_action_denied;

	unsigned char digest[16];

	/* Initializing MD5 Hash */
	MD5_Init(c);

	/* Action is denied until proven the oposite */
	is_action_denied = 1;

	uid = getuid(); // Get user ID
	strcpy(file_name, path); // Get File Name

	/* Getting the Date and Time */
	current_time = time(NULL);

	if (current_time == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    datetime = ctime(&current_time);

	if (datetime == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }

	/* Checking fopen Mode and if user has permission for each action */
	/* Case mode == 2 || Open a file */
	if (strcmp(mode, "r") || (strcmp(mode, "rb"))) {
		file_mode = 2;
		if ((access, R_OK) == 0) { 
			/* access will return 0 for permission success */
			is_action_denied = 0;

			fopen(path, "rb");
			fseek(ptr, 0, SEEK_END);
			int flen = ftell(ptr);
			rewind(ptr);

			unsigned char buffer = (unsigned char *)malloc(flen * sizeof(char));
			fread(buffer, flen, 1, ptr);

			/* Updating MD5 */
			MD5_Update(c, buffer, flen);

			fclose(ptr);
			free(buffer);
		}
		else
		{
			/* If user doesn't have permission we write NULL to MD5 */
			unsigned char *buffer = NULL;
			MD5_Update(c, buffer, strlen(buffer));
		}
	}
	else {
		/* If original fopen with read rights is success then the file exists */
		/* Case mode == 3 || Write to file */
		if (ptr = original_fopen(path, "r")){
			file_mode = 3;
			if ((access, W_OK) == 0) {
				is_action_denied = 0;

				fseek(ptr, 0, SEEK_END);
				int flen = ftell(ptr);
				rewind(ptr);

				unsigned char buffer = (unsigned char *)malloc(flen * sizeof(char));
				fread(buffer, flen, 1, ptr);

				/* Updating MD5 */
				MD5_Update(c, buffer, flen);

				fclose(ptr);
				free(buffer);
			}
			else
			{
			/* If user doesn't have permission we write NULL to MD5 */
			unsigned char *buffer = NULL;
			MD5_Update(c, buffer, strlen(buffer));
			}	
    	}
		else {
			/* Case mode == 1 || Create a new file */
			file_mode = 1;
			if ((access, W_OK) == 0) 
				is_action_denied = 0;

			/* If user doesn't have permission or file is new we write NULL to MD5 */
			unsigned char *buffer = NULL;
			MD5_Update(c, buffer, strlen(buffer));
		}
	}
	
	/* File Fingerprint Finalize*/
	MD5_Final(digest, c);

	/* Call log */	
	log(uid, path, datetime, file_mode, is_action_denied, digest);


	/* ... */
	/* ... */
	/* ... */
	/* ... */


	return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{

	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);


	/* add your code here */
	printf("Hey");
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	return original_fwrite_ret;
}


