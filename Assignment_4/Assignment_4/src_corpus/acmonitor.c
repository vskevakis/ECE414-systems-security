#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Some helper functions and stuff */
/* Linked list that contains potential malicious users */
struct node {
   int data;
   int key;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList() {
   	struct node *ptr = head;
	printf("Malicious Users:\n");
	//start from the beginning
	while(ptr != NULL && ptr->data>7) {
		printf("%d\n",ptr->key);
		ptr = ptr->next;
	}
}

//insert link at the first location
void insertFirst(int key, int data) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->key = key;
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

//find a link with given key
struct node* find(int key) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//delete a link with given key
struct node* delete(int key) {

   //start from the first link
   struct node* current = head;
   struct node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

struct entry {

	int uid; /* user id (positive integer) */
	int access_type; /* access type values [0-2] */
	int action_denied; /* is action denied values [0-1] */

	time_t date; /* file access date */
	time_t time; /* file access time */

	char *file; /* filename (string) */
	char *fingerprint; /* file fingerprint */

	/* add here other fields if necessary */
	/* ... */
	/* ... */

};


void
usage(void)
{
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
		   "Options:\n"
		   "-m, Prints malicious users\n"
		   "-i <filename>, Prints table of users that modified "
		   "the file <filename> and the number of modifications\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}


void 
list_unauthorized_accesses(FILE *log)
{

	/* add your code here */
	/* Get number of lines on the file */
	// int nlines;
	// char c; 

	// for (c = getc(log); c != EOF; c = getc(log)) 
    //     if (c == '\n') // Increment count if this character is newline 
    //         nlines++; 

	/* Read file line by line */
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
	int uid, is_action_denied, denied_times;

    if (log == NULL)
        exit(EXIT_FAILURE);
	
    while ((read = getline(&line, &len, log)) != -1) {
        // printf("%s", line);
		/* Split line and get uid and action-denied-flag */
		uid = atoi(strtok(line, " ")); 
		for (int i=1; i<8; i++)
			strtok(NULL, " "); // We don't care for these elements
		is_action_denied = atoi(strtok(NULL, " "));
		/* Check if uid is already on the list */
		if (is_action_denied==1) {
			struct node *foundLink = find(uid);
			if (foundLink == NULL) {
				denied_times = 1;
				insertFirst(uid, denied_times); // Insert uid with 1 denied time
			}
			else {
				denied_times = foundLink -> data; // Save current denied times
				delete(uid); // Delete current node
				insertFirst(uid, denied_times+1); // Insert Element with updated denied times
			}
		}
    }

	printList();

    if (line)
        free(line);

	return;

}


void
list_file_modifications(FILE *log, char *file_to_scan)
{

	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */

	return;

}




int 
main(int argc, char *argv[])
{

	int ch;
	FILE *log;

	if (argc < 2)
		usage();

	log = fopen("./file_logging.log", "r");
	if (log == NULL) {
		printf("Error opening log file \"%s\"\n", "./log");
		return 1;
	}

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			list_file_modifications(log, optarg);
			break;
		case 'm':
			list_unauthorized_accesses(log);
			break;
		default:
			usage();
		}

	}


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}


