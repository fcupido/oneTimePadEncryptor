#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* A program that promps the user to enter the name of a file in the working directory,
checks to see if the file exists, if it doesn't then the function asks wheter to try again, 
if the user enters 'y' the function loops.
*/

int getLine (char* buffer, size_t sz);


int main(void)
{
	char fileName [20];

	printf("Enter Filename: ");
	int read = 0;

	do{
		if (read == 0)
		{
			read = getLine(fileName, sizeof(fileName)); // returns 0 for error, 1 for calid string
		}
		if (read == 1)
		{
			FILE *f = fopen(fileName, "r"); // returns 0 for invalid filename, 2 for good file
			if (f == NULL)
			{
				printf("Could not open %s. Try different name? y/n: ", fileName);
				if (fgetc(stdin) == 'y')
				{
					read = 0;
					while (getchar() != '\n' && getchar() != EOF) { } // Flushing stdin
					printf("Enter Filename: ");
				}
				else
					return -1;					
			}
			else
			{
				printf("File read successful. The contents of the file are: \n");
				char c = fgetc(f);
				while(c != EOF)
				{
					printf("%c", c);
					c = fgetc(f);
				}
				read = 2;
			}
		}

	}while(read != 2);

	return 0;
}

int getLine (char* buffer, size_t sz)
{
	fgets(buffer, sz, stdin);
	if (isalnum(buffer[0]) == 0)
	{
		printf("invalid Input. Try again: ");
		return 0;
	}
	buffer[strlen(buffer) - 1] = '\0';
	return 1;
}