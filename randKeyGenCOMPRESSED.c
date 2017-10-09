#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

void keyGen(int);

int main(int argc, char const *argv[])
{
	keyGen(1000);
	return 0;
}

void keyGen(int lenth)
{	
	char fileName [] = "testKey.txt";
	char c = '\0';
	FILE *f;
	FILE *p;

	//fprintf(stderr,"\nEnter target filename: ");
	//getLine(fileName, sizeof(fileName));

	f = fopen(fileName, "w");
	p = fopen("testKeyPLAIN.txt", "w");
	srand((unsigned)time(NULL));
	int temp = 0;


	for (int i = 0; i < lenth; ++i)
	{
		if (i % 8 == 0)
			c = 0;

		temp = ((int)rand() % 2);

		c += ((int)pow(2, i % 8)) * temp;
		fprintf(p, "%d", temp);

		if((i + 1) % 8 == 0)
		fprintf(f, "%c", c);
	}
	fprintf(f, "%c", EOF);
	fclose(f);
	f = fopen(fileName, "r");

	fprintf(stderr,"\nKey generated, saved as: %s.", fileName);
	return;
}