#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

int main() {
	srand(getpid());
	int value = rand() % 100 + 1;
	int guess;
	int counter = 0;
	bool run = true;

	puts("Ok, I am ready!");
	//puts( "Enter your guess: ");
	//scanf("%", &guess);

	while(run) {
		counter++;
		puts( "Enter your guess: ");
		scanf("%d", &guess);
		if(guess > value)
		{
			//counter++;
			puts("Go lower!");
			//puts( "Enter your guess: ");
			//scanf("%d", &guess);
		}
		else if(guess < value)
		{
			//counter++;
			puts("Go higher!");
			//puts( "Enter your guess: ");
			//scanf("%d", &guess);
		}
		else if(guess == value)
		{
			//print "you got it in # tries" where # is the counter
			printf("You got it in %d tries\n", counter);
			run = false;
		}
	}

}
