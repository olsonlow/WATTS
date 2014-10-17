/*CS 460 Fall 2014 Project 1 Part 1
 *Author: Lowell Olson
 *Development time 36 hours
 */




#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "dfa.h"
#include "SetLimits.h"

int main (int argc, char * argv[])
{
  SetLimits();
  token t;
  if (argc < 2)
    {
      printf ("format: proj1 <filename>\n");
      exit (1);
    }
  init (argv[1]);
  while ((t = get_token()) != EOFT){
    printf ("\t%-12s%s\n", token_names[t], get_lexeme());
  }
  end_lex();
  return 0;
}
