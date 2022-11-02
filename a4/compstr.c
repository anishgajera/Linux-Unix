#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>


int main()
{
	//char arrays (strings) which hold input
	char str1[100];
	char str2[100];
	//variables for "for" loops
	int i, j;

	//prompt for first string
	puts("Enter String #1: ");
	fgets(str1, 100, stdin);
	//prompt for second string
	puts("Enter String #2: ");
	fgets(str2, 100, stdin);

	//tolower str1 until \0 is reached
	for(i = 0; str1[i] != '\0'; i++)
	{
		str1[i] = tolower(str1[i]);
	}
	//tolower str2 until \0 is reached
	for(i = 0; str2[i] != '\0'; i++)
	{
		str2[i] = tolower(str2[i]);
	}

	//ignore spaces in str1
	for(i = 0; str1[i] != '\0'; i++)
	{
		while(str1[i] == ' ')
		{
			for(j = i; str1[j] != '\0'; j++)
			{
				str1[j] = str1[j+1];
			}
		}
	}
	//ignore spaces in str2
	for(i = 0; str2[i] != '\0'; i++)
	{
		while(str2[i] == ' ')
		{
			for(j = i; str2[j] != '\0'; j++)
			{
				str2[j] = str2[j+1];
			}
		}
	}

	//puts(str1);
	//puts(str2);

	//compare the strings using strcmp() function
	if(strcmp(str1, str2) == 0)
	{
		puts("Both strings are same!");
	}
	else
	{
		puts("The strings are not the same!");
	}


}
