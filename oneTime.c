#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void encrypt(void);
void decrypt(void);
int getLine (char* buffer, size_t sz); // array, sizeof array, 
FILE* getFilePtr(char * prompt, int * charCount);
void flush (void);
FILE* keyGen(int);


int main(int argc, char const *argv[])
{
	char input [3];
	int estate = 0;
	
	do
	{
		printf("en: Encrypt / de: Decrypt: ");
		fgets(input,3, stdin);

		if (!strcmp(input, "en"))
			estate = 1;
		if (!strcmp(input, "de"))
			estate = 2;
	} while(estate == 0);

	switch (estate)
	{
		case 1: // ENCRYPTION
		printf("got to the Encrypt case\n" );
		encrypt();

		break;

		case 2: 
		printf("%s\n", "got to the decryption case");
		break;
	}

}

void encrypt(void)
{
	char answer;
	FILE* key;
	int keyLen = 0, messageLen = 0;

	FILE* message = getFilePtr("Enter name of file to encript: ", &messageLen);

	while(answer != 'y' && answer != 'n')
	{
		printf("Generate new key? y/n: ");
		answer = getchar();
	}

	if (answer == 'y')
	{
		printf("answered yes\n");
		key = keyGen(messageLen * 8);
	}

	if (answer == 'n')
	{
		key = getFilePtr("Enter key filename: ", &keyLen);
	}

	short binMessage [8 * messageLen];

}

FILE* getFilePtr(char * prompt, int * lenth)
{
	char fileName [20];
	FILE *f;
	printf("%s", prompt);
	int read = 0;
	flush();

	do{
		if (read == 0)
		{
			read = getLine(fileName, sizeof(fileName)); // returns 0 for error, 1 for calid string
		}
		if (read == 1)
		{
			f = fopen(fileName, "r"); // returns 0 for invalid filename, 2 for good file
			if (f == NULL)
			{
				printf("Could not open %s. Try different name? y/n: ", fileName);
				if (fgetc(stdin) == 'y')
				{
					read = 0;
					flush(); // Flushing stdin
					printf("Enter Filename: ");
				}
				else
					return f;
			}
			else
			{
				printf("File read successful. The contents of the file are: \n");
				char c = fgetc(f);			
				while(c != EOF)
				{
					(*lenth)++;
					printf("%c", c);
					c = fgetc(f);
				}
				rewind(f);
				printf("\nChar count: %d\n", *lenth);
				read = 2;
			}
		}

	}while(read != 2);

	return f;
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

void flush(void)
{
	while (getchar() != '\n' && getchar() != EOF) { }
}

FILE* keyGen(int lenth)
{	
	flush();
	char fileName [20];
	printf("Enter target filename: ");
	getLine(fileName, sizeof(fileName));

	FILE *f = fopen(fileName, "w");
	srand((unsigned)time(NULL));
	for (int i = 0; i < lenth; ++i)
	{
		fprintf(f, "%d", ((int)rand()) % 2);
	}
	return f;
}