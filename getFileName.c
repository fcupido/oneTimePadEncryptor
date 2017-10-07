#include <stdio.h>
#include <string.h>
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
/*
A function that promps the user to enter the name of a file in the working directory
checks to see if the file exits, if it doesn't then the function asks wheter to try again, 
if the user enters y then function loops
*/

void getLine (char *buff, size_t sz);


int main (void) {
    char buff[20];

    getLine (buff, sizeof(buff));



    printf ("OK [%s]\n", buff);

    FILE *f = fopen(buff, "r");
    	if (f == NULL)
    	{
    		printf("NO SUCH FILE\n");
    	}
    	else

    	{
    		char c = fgetc(f);
    		while(c != EOF)
    		{
    			printf("%c", c);
    			c = fgetc(f);
    		}
    	}

    	printf("end of program\n");


    return 0;
}

void getLine (char *buff, size_t sz) {
    int ch, extra, rc;
    do
    {
    	printf("\nEnter Filename> ");
    	if (fgets (buff, sz, stdin) == NULL)
    	    rc = NO_INPUT;

    	if (buff[strlen(buff)-1] != '\n') {
    	    extra = 0;
    	    while (((ch = getchar()) != '\n') && (ch != EOF))
    	        extra = 1;
    	    rc = (extra == 1) ? TOO_LONG : OK;
    	}

    	buff[strlen(buff)-1] = '\0';
    	rc = OK;


	} while(rc != OK);

}