#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "utilities.h"

/* Print a usage message on stderr 
   and exit with failure. */
static void usage(const char *cmdname)
{
    fprintf(stderr,
	    "Usage: %s code-filename\n",
	    cmdname);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    const char *cmdname = argv[0];
    --argc;
    /* 1 non-option argument */
    if (argc != 1 || argv[1][0] == '-') {
	    usage(cmdname);
    }
    lexer_init(argv[1]);
    lexer_output();
    if (errors_noted) {
	return EXIT_FAILURE;
    } else {
	return EXIT_SUCCESS;
    }
}
