/* CS460 Fall 2014 Project 1 Part 2
 * Author: Lowell Olson
 * Lexical Analyzer
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dfa.h"
#include "SetLimits.h"


int main(int argc, char *argv[])
{
  SetLimits();
  int i;
  if(argc < 2)
    {
      printf("Format: proj1 <filename>\n");
      exit (1);
    }
  init (argv[1]);  
  return 0;
}
