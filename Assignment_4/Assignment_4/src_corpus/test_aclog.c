#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

int main() 
{
	int i;
	size_t bytes;
	FILE *file;
	char filenames[10][7] = {"file_0", "file_1", 
			"file_2", "file_3", "file_4",
			"file_5", "file_6", "file_7", 		
			"file_8", "file_9"};


	/* example source code */
	setreuid(001,001);
	system("./test_aclog");

	for (i = 0; i < 10; i++) {

		file = fopen(filenames[i], "w+");
		if (file == NULL) 
			printf("fopen error\n");
		else {
			bytes = fwrite(filenames[i], strlen(filenames[i]), 1, file);
			fclose(file);
		}

	}


	/* add your code here */
	/* Setting UID to 1 */
	// int real = getuid();
	// int euid = geteuid();
	// printf("The REAL UID =: %d\n", real);
	// printf("The EFFECTIVE UID =: %d\n", euid);
	// file = fopen("test_file", "w+");
	// if (file != NULL) {
	// 	printf("Opened File");
	// }
	// fclose(file);

	/* ... */
	/* ... */
	/* ... */
	/* ... */


}
