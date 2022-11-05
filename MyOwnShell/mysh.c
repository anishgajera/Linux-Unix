#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

//read all the quotes from quotes.txt
//when client asks for a motivational quote, select one randomly and send it out.

#define MAXQUOTES 10000
#define MAXLEN 1000

char *commands[100];
int fds[100][2];
int numPipes = 1;
char *quotes[MAXQUOTES];
int numQuotes=0;
int tochild[2], toparent[2];

//runs a simple command
//cmdname arg1 arg2 arg3 ...
void runCommand(char *command) {
	//split and assemble the arguments and invoke execvp()
	//use strtok(..)
	char *arguments[MAXLEN];
	char *cmd = strtok(command, " |\n");
	arguments[0] = cmd;
	int i = 1;
	while ((arguments[i] = strtok(NULL, " |\n")) != NULL)
	{
		i++;
	}
	execvp(cmd, arguments);
	fputs("I hope you don't see me!\n", stderr);

}

int parent(char *pr) {
	close(1);
	dup(tochild[1]);

	close(0);
	dup(toparent[0]);

	close(tochild[0]); close(tochild[1]);
	close(toparent[0]); close(toparent[1]);
	runCommand(pr);
	//execl("./computer", "computer", NULL);
	fputs("I hope you do not see this one either!", stderr);

}

//cmd0 | cmd1 | cmd2 | cmd3 | cmd4
void children(int i) {
	//rewire pipes to 0 and 1
	//close unnecessary pipes
	//run ith command
	runCommand(commands[i]);
}



void child(char *ch) {

	close(0);
	dup(tochild[0]);
	close(1);
	dup(toparent[1]);

	close(tochild[0]); close(tochild[1]);
	close(toparent[0]); close(toparent[1]);
	//execl("./user", "user", NULL);

	runCommand(ch);

	fputs("I hope you do not see me!", stderr);
}

void closePipes() {
	for(int i = 0; i <= numPipes; i++)
	{
		close(fds[i][0]);
		close(fds[i][1]);
	}
}

void processLine(char *line) {
	char *pipePtr = strchr(line, '|');
	char *equalPtr = strchr(line, '=');

	if (pipePtr) { //not NULL
		//command has several sub-commands connected with pipes
		//setup commands array
		commands[0] = strtok(line, "|\n");
		//setup pipes array
		numPipes = 1;
		while (commands[numPipes] = strtok(NULL, "|\n"))
		{
			numPipes++;
		}
		numPipes--;

		for(int i = 0; i < numPipes+1; i++)
			pipe(fds[i]);

		for(int i = 0; i < numPipes; i++) {
		if (fork() == 0)
		{
			if (i > 0) {
				close(0);
				dup(fds[i][0]);
			}
			//close(0);
			//dup(fds[0][0]);
			//close(fds[0][1]);
			//runCommand(commands[1]);

			close(1);
			dup(fds[i+1][1]);
			closePipes();
			child(i);
			//runCommand(commands[0]);
		}
		//create children --> invoke child(i) in a loop
		//cmd0 | cmd1 | cmd2 | cmd3 | cmd4
		//close(fds[i+1][0]);
		//runCommand(commands[i]);
	}
		close(0);
		dup(fds[numPipes][0]);
		closePipes();
		runCommand(commands[numPipes]);
	}
	else if (equalPtr)
	{
		//command has = operator, so 2 commands --> 2 processes
    	char *ch = strtok(NULL, "=\n");
		pipe(tochild);
		char *pr = strtok(line, "=\n");
		pipe(toparent);

		if (fork() > 0)
			parent(pr);
		else
			child(ch);

	} else
		//it is a simple command, no pipe or = character
		runCommand(line);

	exit(0);
}

int main() {
	// load up all the quotes from quotes.txt
	//FILE *fp = fopen("quotes.txt", "r");
	char line[MAXLEN];
	FILE *fp = fopen("quotes.txt", "r");
	if (!fp) {
		puts("quotes.txt cannot be opened for reading.");
		exit(1);
	}
	int i = 0;

	while (fgets(line, MAXLEN, fp)) {
		quotes[i] = (char *) malloc (strlen(line)+1);
		strcpy(quotes[i], line);
		i++;
	}
	numQuotes = i;
	srand(getpid() + time(NULL) + getuid());

	// infinite loop to serve the customer
	while (1) {
		//output a random quote to stderr
		fputs(quotes[rand()%numQuotes], stderr);
		fprintf(stderr, "# ");
		//get the user input
		fgets(line, 1000, stdin);

		//spawn a child for taking care of it
		if (fork() == 0)
			processLine(line);

		//wait the child to finish the job!
		//int x=0;
		//wait(&x);
		wait(NULL);
	}
}
