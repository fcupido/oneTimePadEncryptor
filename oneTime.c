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
	fprintf(stderr,"en: Encrypt / de: Decrypt: ");
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
		fprintf(stderr,"got to the Encrypt case\n" );
		encrypt();

		break;

		case 2: 
		fprintf(stderr,"%s\n", "got to the decryption case");
		decrypt();

		break;
	}

}

void decrypt()
{
	char answer;
	int cryptoLength;
	FILE* crypto;

	do{
		cryptoLength = 0;
		crypto = getFilePtr("Enter cryptograph filename: ", &cryptoLength);
		fprintf(stderr,"Used this file? y/n: \n");
		answer = getchar();
	}while(answer != 'y');

	int keyLen = 0;
	FILE* key = getFilePtr("Enter KEY filename: ", &keyLen);

	char binMessage [cryptoLength];

	xorDE (crypto, key, binMessage, cryptoLength);
	printf("\n\n Converting to plaintext.\n\n");
	
	
	for (int i = 0; i < cryptoLength; i += 8)
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

		fprintf(stderr, "%c", answer );
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
		fprintf(stderr,"Generate new key? y/n: ");
		answer = getchar();
	}

	if (answer == 'y')
	{
		fprintf(stderr,"answered yes\n");
		key = keyGen(messageLen * 8);
	}

	if (answer == 'n')
	{
		key = getFilePtr("Enter key filename: ", &keyLen);
	}
	
	char binMessage [8 * messageLen];
	char crypto [8 * messageLen];

	FILEToBin(message, binMessage, messageLen);

	xor (binMessage, key, crypto, messageLen);

	fprintf(stderr,"\nCryto Message generated, enter target filename: ");
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
			fprintf(stderr,"Line Read: [%s] readValue = %d\n", fileName, read);
		}
		if (read == 1)
		{
			f = fopen(fileName, "r"); // returns 0 for invalid filename, 2 for good file
			if (f == NULL)
			{
				fprintf(stderr,"Could not open %s. Try different name? y/n: ", fileName);
				if (fgetc(stdin) == 'y')
				{
					read = 0;
					flush(); // Flushing stdin
					fprintf(stderr,"Enter Filename: ");
				}
				else
					return f;
			}
			else
			{
				fprintf(stderr,"File read successful. The contents of the file are: \n\n");
				char c = fgetc(f);			
				while(c != EOF)
				{
					(*lenth)++;
					fprintf(stderr,"%c", c);
					c = fgetc(f);
				}
				rewind(f);
				fprintf(stderr,"\nChar count: %d\n\n", *lenth);
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
		fprintf(stderr,"invalid Input. Try again: ");
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
	fprintf(stderr,"Enter target filename: ");
	getLine(fileName, sizeof(fileName));

	FILE *f = fopen(fileName, "w");
	srand((unsigned)time(NULL));
	for (int i = 0; i < lenth; ++i)
	{
		fprintf(f, "%d", ((int)rand()) % 2);
	}

	fclose(f);

	f = fopen(fileName, "r");

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
	fprintf(stderr,"Plaintext Binary message:\n");
	char * temp = dest;
	int c;
	for(int j = 0; j < charCount; j++)
	{
		fprintf(stderr,"%5d  ", j); // for debug
		c = fgetc(message);
		for (int i = 0; i < 8; ++i)
		{
			*temp = c % 2 + '0';
			c = c / 2;
			fprintf(stderr,"%c", *temp); // fpr debug
			temp++; 
		}
		fprintf(stderr,"\n"); // for debug

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
	fprintf(stderr,"Plaintext Binary message xorDe:\n");
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
		printf("%c", plain[i]);
	}
}