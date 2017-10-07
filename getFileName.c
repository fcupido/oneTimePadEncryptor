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

static int getLine (char *prmpt, char *buff, size_t sz);


int main (void) {
    int rc;
    char buff[20];

    rc = getLine ("Enter filename> ", buff, sizeof(buff));
    if (rc == NO_INPUT) {
        // Extra NL since my system doesn't output that on EOF.
        printf ("\nNo input\n");
        return 1;
    }

    if (rc == TOO_LONG) {
        printf ("Input too long [%s]\n", buff);
        return 1;
    }

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

static int getLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}