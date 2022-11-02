#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

int main() {
	int guess = 50;
	int increment = 50;
	char answer;

	while(true) {
		printf("How about %d (<,=,>)?\n", guess);
		scanf(" %c", &answer);

		if(answer == '=') {
			printf("It was a good game!\n");
			return;
		}
		else if(answer == '>'){
			if(increment < 1) {
				guess++;
			}
			guess += increment/2;
			increment /= 2;
		}
		else if(answer == '<') {
			if(increment < 1) {
				guess--;
			}
			guess -= increment/2;
			increment /= 2;
		}

	}

}
