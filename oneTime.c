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
	
	do // Encript or decript
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
		decrypt();

		break;
	}

}

void decrypt()
{
	int cryptoLength;
	FILE* crypto = getFilePtr("Enter cryptograph filename: ", &cryptoLength);
	int keyLen;
	FILE* key = getFilePtr("Enter KEY filename: ", &keyLen);

	char binMessage [cryptoLength / 8];

	xorDE (crypto, key, binMessage, cryptoLength);

	genFile("binDecrypted", binMessage, cryptoLength / 8);

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
	printf("Got to file to bin function\n");
	
	char binMessage [8 * messageLen];
	char crypto [8 * messageLen];

	FILEToBin(message, binMessage, messageLen);

	xor (binMessage, key, crypto, messageLen);

	printf("\nCryto Message generated, enter target name: ");
	char cryptoName [20];
	getLine(cryptoName, sizeof(cryptoName));

	genFile(cryptoName, crypto, 8 * messageLen);
	printf("File Created\n");

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
			printf("Line Read: [%s] readValue = %d\n", fileName, read);
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
	fflush(stdin);
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
	printf("Plaintext Binary message:\n");
	char * temp = dest;
	int c;
	for(int j = 0; j < charCount; j++)
	{
		printf("%5d  ", j); // for debug
		c = fgetc(message);
		for (int i = 0; i < 8; ++i)
		{
			*temp = c % 2 + '0';
			c = c / 2;
			printf("%c", *temp); // fpr debug
			temp++; 
		}
		printf("\n"); // for debug

	}
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
	printf("Plaintext Binary message:\n");
	char k;
	char c;
	for (int i = 0; i < charCount; ++i)
	{	
		if (i % 8 == 0)
		{
			printf("\n%5d   ", i / 8);
		}
		c = fgetc(crypto);
		k = fgetc(key);
		if(c == k)
			plain[i] = '0';
		else
			plain[i] = '1';
		printf("%c", plain[i]);
	}

}