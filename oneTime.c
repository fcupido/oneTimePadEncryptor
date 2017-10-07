#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{
	char input [3];
	int estate = 0;
	
	do
	{
		printf("en: Encrypt / de: Decrypt\n");
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

		break;

		case 2: 
		printf("got to the Decryption stage\n");
	}

}
