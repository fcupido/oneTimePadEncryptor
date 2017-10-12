#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

void encrypt(void);
void decrypt(void);
int getLine (char* buffer, size_t sz);
FILE* getFilePtr(char * prompt, int * charCount);
void flush (void);
FILE* keyGen(int); // compressed
void xor (FILE* file1, FILE* file2, FILE* destination); // compressed
void FILEToBin (FILE* message, char* dest);	// compressed
void genFile (char * fileName, char * content, int charCount); // compressed
void decrypt(void);
int main(int argc, char const *argv[])
{
	char input [3];
	int estate = 0;
	fprintf(stderr,"\nen: Encrypt / de: Decrypt: ");
	do // Encript or decript
	{
		fgets(input,3, stdin);

		if (!strcmp(input, "en"))
			estate = 1;
		else if (!strcmp(input, "de"))
			estate = 2;

	} while(estate == 0);

	switch (estate)
	{
		case 1: // ENCRYPTION
		encrypt();

		break;

		case 2: 
		decrypt();
		break;
	}
}

void decrypt()
{
	char answer;
	char fileName [20];
	int cryptoLen = 0, keyLen = 0;
	FILE* crypto;
	FILE* key;
	FILE* message;

	do{
		cryptoLen = 0;
		crypto = getFilePtr("\nEnter cryptograph filename: ", &cryptoLen);
		fprintf(stderr,"\nUsed this file? y/n: ");
		answer = getchar();
	}while(answer != 'y');


	key = getFilePtr("\nEnter KEY filename: ", &keyLen);


	fprintf(stderr, "\nSave message as: ");
	getLine(fileName, sizeof(fileName));
	message = fopen(fileName, "w");

	xor (crypto, key, message);
	
}


void encrypt(void)
{
	char answer;
	int keyLen = 0, messageLen = 0;
	FILE* message;
	FILE* key;
	FILE* crypto;
	
	message = getFilePtr("\nEnter name of file to encript: ", &messageLen);

	while(answer != 'y' && answer != 'n')
	{
		fprintf(stderr,"\nGenerate new key? y/n: ");
		answer = getchar();
	}

	if (answer == 'y')
	{
		key = keyGen(messageLen);
	}

	if (answer == 'n')
	{
		key = getFilePtr("\nEnter key filename: ", &keyLen);
	}

	fprintf(stderr,"\nSave encrypted message as: ");
	char cryptoName [20];
	getLine(cryptoName, sizeof(cryptoName));
	crypto = fopen(cryptoName, "w");

	xor (message, key, crypto);

	fprintf(stderr,"File Created\n");
}

FILE* getFilePtr(char * prompt, int * lenth)
{
	char fileName [20];
	FILE *f;
	fprintf(stderr,"%s", prompt);
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
				fprintf(stderr,"\nCould not open %s. Try different name? y/n: ", fileName);
				if (fgetc(stdin) == 'y')
				{
					read = 0;
					flush(); // Flushing stdin
					fprintf(stderr,"\nEnter Filename: ");
				}
				else
				{
					printf("\nProgram terminated.\n");
					exit(-1);
				}
			}
			else
			{
				fprintf(stderr,"\nFile read successful. ");
				char c = fgetc(f);			
				while(c != EOF)
				{
					(*lenth)++;
					c = fgetc(f);
				}
				rewind(f);
				fprintf(stderr,"Char count: %d\n", *lenth);
				read = 2;
			}
		}
	}while(read != 2);
	return f;
}

int getLine (char* buffer, size_t sz)
{
	fflush(stdin);
	fgets(buffer, sz, stdin);
	if (isalnum(buffer[0]) == 0)
	{
		fprintf(stderr,"\ninvalid Input. Try again: ");
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
	FILE *f;

	fprintf(stderr,"\nEnter target filename: ");
	getLine(fileName, sizeof(fileName));

	f = fopen(fileName, "w");
	srand((unsigned)time(NULL));

	fprintf(stderr, "%s\n", "");

	for (int i = 0; i < lenth; ++i)
	{
		fprintf(f, "%c", rand() % 127 + 1);
	}
	fprintf(f, "%c", EOF);
	fclose(f);
	f = fopen(fileName, "r");

	fprintf(stderr,"\nKey generated, saved as: %s.", fileName);
	return f;
}

void genFile (char * fileName, char * content, int charCount)
{
	FILE *f = fopen(fileName, "w");
	char c = '\0';
	int temp = 0;

	for (int i = 0; i < charCount; ++i)
	{
		if (i % 8 == 0)
			c = 0;
		temp = content[i];
		c += ((int)pow(2, i % 8)) * temp;
		if((i + 1) % 8 == 0)
		fprintf(f, "%c", c);
	}
	fprintf(f, "%c", EOF);
	fclose(f);
	printf("File generated\n");
}

void xor (FILE* source, FILE * key, FILE * dest)
{
	char c = fgetc(source);
	char k = fgetc(key);
	while(c != EOF)
	{	
		fprintf(dest, "%c", c ^ k);
		c = fgetc(source);
		k = fgetc(key);
	}
	fprintf(stderr,"\nFile XORed.");
}