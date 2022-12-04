//Usage: quoteclient <server-fifo-filename>
//
//create own named pipe (fifo) and set permissions
//send this fifo name to server
//open own named pipe
//read the quote and display

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLEN 100

int main(int argc, char *argv[]) {	
	if (argc !=2) {
		puts("Usage: qclient <server-fifo-name>");
		exit(1);
	}

	// argv[1] is the server fifo name

    char clientfifo[MAXLEN];
	sprintf(clientfifo, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(clientfifo, 0600);
	chmod(clientfifo, 0622);
	
	//open argv[1] for writing, send clientfifo
	FILE *fp = fopen(argv[1], "w");
	if (!fp) {
		printf("Cannot open %s for writing.\n", argv[1]);
		exit(1);
	}

	fputs(clientfifo, fp);
	fclose(fp);

	//open clientfifo for reading and read the quote & print in the screen - improve your life! :-)
	FILE *quotefp = fopen(clientfifo, "r");
	//read the quote!
	char buffer[MAXLEN] = "";
	fgets(buffer, MAXLEN, quotefp);
	printf(buffer);
	
	int counter = 0;
	fflush(stdout);

	/* Set mask array - mask[i] == true if
	 * the character s[i] has been guessed correctly. 
	 * Mask must be allocated/initialized to all false
	 */

	int N = strlen(buffer);
	int mask[N];
	for(int i = 0; i < N - 1; i++) {
		mask[i] = 0;
	}

	int endgame = 0;
	while(!endgame) 
	{
		//print word with '*' for all unguessed letters of the word
		printf("Enter a letter in word ");
		for(int j = 0; j < N - 1; j++)
		{
			if(mask[j])
			{
				printf("%c", buffer[j]);
			}
			else 
			{
				printf("*");
			}
		}
	//get next guessed letter
	char letterguess;
	printf(" > ");
	fflush(stdout);
	scanf(" %c", &letterguess);


	//
	for(int k = 0; k < N - 1; k++)
	{
		if(buffer[k] == letterguess)
		{
			mask[k] = 1;
		}
		else if(buffer[k] != letterguess)
		{
			if(k == (N - 1))
				counter++;
		}
	}
		
	//check if player won
	endgame = 1;
	for(int m = 0; m < N - 1l; ++m)
	{
		if(!mask[m]) {
			endgame = 0;
			//printf("test");
			//printf("%c", "The word is ", buffer);
			break;
		}
	}


	}



	fclose(quotefp);

	unlink(clientfifo);
}
