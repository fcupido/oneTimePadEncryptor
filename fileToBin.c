#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

void FILEToBin (FILE* message, FILE* dest);

int main(void)
{
	FILE* message = fopen("testKey.txt", "r");
	FILE* dest = fopen("destination.txt","w");

	FILEToBin(message, dest);

	printf("Saved as: destination.txt\n");
}


void FILEToBin (FILE* message, FILE* dest)
{
	int c = 0;
	for(int j = 0; 1; j++)
	{
		c = fgetc(message);
		if (c == EOF)
		{
			break;
		}
		for (int i = 0; i < 8; ++i)
		{
			fprintf(dest, "%c", (c % 2) + '0');
			c = c / 2;
		}
	}
	fprintf(stderr,"\nPlaintext binary message generated.\n");
}