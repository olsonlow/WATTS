#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "lex.h"
FILE fp;
init_lex(file) fp = open(file)
  get_token() -> uses fp to get next token

int main (int argc, char * argv[])
{
	enum token_type t;
	if (argc < 2)
	{
		printf ("format: proj1 <filename>\n");
		exit (1);
	}
	init_lex (argv[1]);
	while ((t = get_token()) != EOFT)
		printf ("\t%-12s%s\n", token_names[t], get_lexeme());
	end_lex();
	return 0;
}
