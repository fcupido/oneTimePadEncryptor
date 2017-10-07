#include <stdio.h>
#include <string.h>

int getLine (char* buffer, size_t sz);

FILE* main(void)
{
	char fileName [20];
	FILE *f;
	printf("Enter Filename\n");

	do{
		int read = 0;

		if (read = 0)
		{
			n = getLine(fileName); // returns 0 for error, 1 for calid string
		}
		if (n = 1)
		{
			n = fileExists(fileName, *f); // returns 0 for invalid filename, 2 for good file
		}

	}while(read != 2);

	return f;
}

int getLine (char* buffer, size_t sz)
{
	fgets(buffer, sz, stdin);
	if (isalnum(buffer[0]) == 0)
	{
		printf("invalid Input\n");
	}
	return 0;
}