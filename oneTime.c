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
void xor (char * messageC, FILE * key, char* crypto, int charCount);
void FILEToBin (FILE* message, char* dest, int charCount);
void genFile (char * fileName, char * content, int charCount);
void decrypt(void);
void xorDE (FILE* messageC, FILE * key, char* crypto, int charCount);

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

	char binMessage [cryptoLen];
	xorDE (crypto, key, binMessage, cryptoLen);

	fclose(key);
	fclose(crypto);

	fprintf(stderr, "\nSave message as: ");
	getLine(fileName, sizeof(fileName));
	message = fopen(fileName, "w");

	printf("\n\nConverting to plaintext.\n\n");
	
	
	for (int i = 0; i < cryptoLen; i += 8)
	{
		answer = '0' - '0';
		answer += (binMessage[i] - '0');
		answer += (2*(binMessage[i+1] - '0'));
		answer += (4*(binMessage[i+2] - '0'));
		answer += (8*(binMessage[i+3] - '0'));
		answer += (16*(binMessage[i+4] - '0'));
		answer += (32*(binMessage[i+5] - '0'));
		answer += (64*(binMessage[i+6] - '0'));
		answer += (128*(binMessage[i+7] - '0'));

		fprintf(stderr, "%c", answer);
		fprintf(message, "%c", answer);
	}
}


void encrypt(void)
{
	char answer;
	int keyLen = 0, messageLen = 0;
	FILE* message;
	FILE* key;
	
	message = getFilePtr("\nEnter name of file to encript: ", &messageLen);

	while(answer != 'y' && answer != 'n')
	{
		fprintf(stderr,"\nGenerate new key? y/n: ");
		answer = getchar();
	}

	if (answer == 'y')
	{
		key = keyGen(messageLen * 8);
	}

	if (answer == 'n')
	{
		key = getFilePtr("\nEnter key filename: ", &keyLen);
	}
	
	char binMessage [8 * messageLen];
	char crypto [8 * messageLen];

	FILEToBin(message, binMessage, messageLen);

	xor (binMessage, key, crypto, messageLen);

	fprintf(stderr,"\nCryto Message generated. Save as: ");
	char cryptoName [20];
	getLine(cryptoName, sizeof(cryptoName));
	genFile(cryptoName, crypto, 8 * messageLen);
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
	fprintf(stderr,"\nEnter target filename: ");
	getLine(fileName, sizeof(fileName));

	FILE *f = fopen(fileName, "w");
	srand((unsigned)time(NULL));
	for (int i = 0; i < lenth; ++i)
	{
		fprintf(f, "%d", ((int)rand()) % 2);
	}

	fclose(f);
	f = fopen(fileName, "r");

	fprintf(stderr,"\nKey generated, saved as: %s.", fileName);
	return f;
}

void xor (char* messageC, FILE * key, char* crypto, int charCount)
{
	char k;
	char* message = messageC;

	for (int i = 0; i < charCount * 8; ++i)
	{
		k = fgetc(key);
		if(*message == k)
			crypto[i] = '0';
		else
			crypto[i] = '1';
		message++;
	}
}

void FILEToBin (FILE* message, char* dest, int charCount)
{
	char * temp = dest;
	int c;
	for(int j = 0; j < charCount; j++)
	{
		c = fgetc(message);
		for (int i = 0; i < 8; ++i)
		{
			*temp = c % 2 + '0';
			c = c / 2;
			temp++; 
		}
	}
	fprintf(stderr,"\nPlaintext binary message generated.\n");
}

void genFile (char * fileName, char * content, int charCount)
{
	FILE *f = fopen(fileName, "w");

	for (int i = 0; i < charCount; ++i)
	{
		fprintf(f, "%c", content [i]);
	}
}

void xorDE (FILE* crypto, FILE * key, char* plain, int charCount)
{
	fprintf(stderr,"\nDeciphering file: ");
	char k;
	char c;
	for (int i = 0; i < charCount; ++i)
	{	
		c = fgetc(crypto);
		k = fgetc(key);
		if(c == k)
			plain[i] = '0';
		else
			plain[i] = '1';
	}
	fprintf(stderr,"File Deciphered.");
}