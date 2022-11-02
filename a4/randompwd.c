#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

//need parameters since we're getting arguments from the user through command line
int main(int argc, char *argv[])
{
	srand(getpid());
	char password[100];
	//want to get arguments and convert to integer (atoi) for all parameter specifications
	int minLen = atoi(argv[1]);
	int maxLen = atoi(argv[2]);
	int minLower = atoi(argv[3]);
	int minUpper = atoi(argv[4]);
	int minDigits = atoi(argv[5]);
	int minSpecial = atoi(argv[6]);

	//character arrays for each category
	char lower[] = "abcdefghijklmnopqrstuvwxyz";
	char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char digits[] = "1234567890";
	char special[] = "!@#$%^&*+:;";

	//printf("%i %i %i %i %i %i\n", minLen, maxLen, minLower, minUpper, minDigits, minSpecial);

	int length = (rand() % (maxLen - minLen + 1)) + minLen;
	int randomChar = 0;
	int i;
	for(i = 0; i < length; i++)
	{
		randomChar = rand() % 4 + 1;
		if(randomChar == 1 && minLower >= 0)
		{
			minLower--;
			password[i] = lower[rand() % strlen(lower)];
		}
		else if(randomChar == 2 && minUpper >= 0)
		{
			minUpper--;
			password[i] = upper[rand() % strlen(upper)];
		}
		else if(randomChar == 3 && minDigits >= 0)
		{
			minDigits--;
			password[i] = digits[rand() % strlen(digits)];
		}
		else if(randomChar == 4 && minSpecial >= 0)
		{
			minSpecial--;
			password[i] = special[rand() % strlen(special)];
		}
	}
	puts(password);

}
