#define _GNU_SOURCE
#define MD5_LENGTH 16

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>


FILE * 
fopen_direct(const char *path, const char *mode){

    FILE *original_fopen_ret;
    FILE *(*original_fopen)(const char*, const char*);

    /* call the original fopen function */
    original_fopen = dlsym(RTLD_NEXT, "fopen");
    original_fopen_ret = (*original_fopen)(path, mode);

    return original_fopen_ret;
}

/* Add to Log file func */
void
write_log(int uid, unsigned char *file_name, char *datetime, int access_type, int is_action_denied, char *digest){
	/* Log File */
	FILE * ptr;
	ptr = fopen_direct("file_logging.log" ,"a");
	if (ptr != NULL) {
		fprintf(ptr, "%d %s %s %d %d %s\n", uid, file_name, strtok(datetime, "\n"), access_type, is_action_denied, digest);
	}

	fclose(ptr);
}


FILE *
fopen(const char *path, const char *mode) 
{
	/* Have to check file existence before procceding */
	int exists = cfileexists(path); // Helper

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);


	/* add your code here */	
	char md5_hash[MD5_LENGTH*2+1];
	int uid = getuid();
	unsigned char *file_name, *datetime;
	int file_mode, is_action_denied;


	/* Getting the time */
	time_t now = time(&now);        
    struct tm *ptm = localtime(&now); 

	/* Action is denied until proven the oposite */
	is_action_denied = 1;
	
	file_name = realpath(path, NULL);

	/* Checking fopen Mode and if user has permission for each action */
	/* Case mode == 1 || File Creation */
    if (exists) {
		if (!strcmp(mode, "r") || (!strcmp(mode, "rb"))) {
			file_mode = 2;
			if (access(path, R_OK) == 0)
				is_action_denied = 0;
		}
		else {
			file_mode = 3;
			if (access(path, W_OK) == 0) {
				is_action_denied = 0;
			}
		}	
    }
	else
	{
		file_mode = 1;
		printf("File Mode: 1");
			if (access(path, W_OK) == 0)
				is_action_denied = 0;	
	}

	if ((is_action_denied==1) || (file_mode==1)) {
		/* MD5 is hash if user can't access the file or file is just created */
		strcpy(md5_hash, "0");
	}
	else {
		gen_md5(path, md5_hash);
	}

	/* Call log */	
	write_log(uid, file_name, asctime(ptm), file_mode, is_action_denied, md5_hash);

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
	char md5_hash[MD5_LENGTH*2+1];
	int MAXSIZE = 0xFFF;
    char proclnk[0xFFF];
    char path[0xFFF];
	int uid = getuid();
	unsigned char *file_name, *datetime;
	int fno, file_mode, is_action_denied;
	ssize_t r;

	/* Finding file name from fp */
	if (stream != NULL)
    {
        fno = fileno(stream);
        sprintf(proclnk, "/proc/self/fd/%d", fno);
        r = readlink(proclnk, path, MAXSIZE);
        if (r < 0)
        {
            printf("failed to readlink\n");
            exit(1);
        }
        path[r] = '\0';
    }
	fflush(stream);
	file_name = basename(path);
	printf("FILENAME IS %s\n", file_name);

	/* Getting the time */
	time_t now = time(&now);        
    struct tm *ptm = localtime(&now); 

	/* Action is denied until proven the oposite */
	is_action_denied = 1;

	if (access(file_name, W_OK)==0)
		is_action_denied = 0;

	gen_md5(file_name, md5_hash);
	write_log(uid, path, asctime(ptm), 3, is_action_denied, md5_hash);

	return original_fwrite_ret;
}

int cfileexists(const char* filename){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else // -1
        return 0;
}

void
gen_md5(const char *path, char md5_hash[]) {
	
	MD5_CTX c;
	FILE *file = fopen_direct(path, "r"); 
		
	unsigned char *buffer;
	unsigned char digest[MD5_LENGTH];

	fseek(file, 0, SEEK_END); // Find the length of the file
	int fsize = ftell(file); // Save Length
	fseek(file, 0L, SEEK_SET); // Return to the start of the file

	buffer = (unsigned char *)malloc(fsize);

	fread(buffer, 1, fsize, file); // Read Buffer

	print_string(buffer,fsize);

	/* Generate MD5 Hash */
	MD5_Init(&c);

	MD5_Update(&c, buffer, fsize);
	
	MD5_Final(digest, &c);

	/* Make it HEX */
	for(int i=0,j=0;i<MD5_DIGEST_LENGTH;i++,j+=2){
        sprintf(&md5_hash[j], "%02X", digest[i]);
    }

	free(buffer);
	fclose(file); 
	return;
}

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