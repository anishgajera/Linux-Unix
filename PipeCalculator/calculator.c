#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>


int fds[200][2];
#define MAXLEN 1000
char line[MAXLEN];
int numOperators = 0;

void child(char operator, int index) {
	//close and dup
	//close(0);
	close(0);
	//dup();
	dup(fds[2 * index][0]);
	//close(3);
	close(3);
	//dup();
	dup(fds[(2 * index) + 1][0]);
	//store result
	close(1);
	dup(fds[(2 * index) + 2][1]);
	//close all pipes to avoid children hanging
	//  for i 0 to 2*numOperators
	//  	 close(fds[i][0]);
	//      close(fds[i][1]);
	for(int i = 0; i < 2*numOperators; i++)
	{
		close(fds[i][0]);
		close(fds[i][1]);
	}

	//exec1: +, -, *, / --> add subtract multiply divide
	if(operator ==  '+')
	{
		execl("./add", "add", NULL);
	}
	else if (operator == '*')
	{
		execl("./multiply", "multiply", NULL);
	}
	else if (operator == '-')
	{
		execl("./subtract", "subtract", NULL);
	}
	else if (operator == '/')
	{
		execl("./divide", "divide", NULL);
	}
	exit(1);
}

int main(int argc, char *argv[])
{

	char operators[100];
	//int numOperators = 0;
	int operand;
	int result;
	FILE *finput = fopen(argv[1], "r");
	fgets(line, MAXLEN, finput);
	//line may be "a + b - c + d / e - f"
	strtok(line, " \n");
	char *cptr;
	while (cptr = strtok(NULL, " \n")) {
		operators[numOperators++] = *cptr;
		strtok(NULL, " \n");
		//printf("%c\n", operators[numOperators - 1]);
	}

	//create 2*numOperators+1 pipes!
	//for loop i
	//	child(operators[i], i);
	for(int i = 0; i < (2 * numOperators) + 1; i++)
		pipe(fds[i]);

	//create child
	for(int i = 0; i < numOperators; i++)
	{
		if(!fork())
			child(operators[i], i);
	}

	//while next line data available
	//	read and pump them into the pipes
	//	read the result from the last child
	//	print it out
	while (fscanf(finput, "%d", &operand) > 0) {
		write(fds[0][1], &operand, sizeof(int));

		for(int i = 0; i < numOperators; i++)
		{
			fscanf(finput, "%d", &operand);
			write(fds[(2 * i) + 1][1], &operand, sizeof(int));
		}

		//read result
		read(fds[2 * numOperators][0], &result, sizeof(int));
		printf("%d\n", result);
	}
}
