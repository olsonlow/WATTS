#ifndef DFA_H
#define DFA_H

#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>

typedef enum{
  ERROR, IDENT, NUMLIT, PLUS, MINUS, MULT,
  DIV, MOD, EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL,
  SHIFTR, PLUSPLUS, PLUSEQ, MINUSMINUS, MINUSEQ, MULTEQ,
  DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
  XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDA, RPAREN,
  LPAREN, SEMI, INTTYPE, DBLTYPE, EOFT
}token;

typedef enum{
  alpha = 0, digit, pls, mns, mult, mod, divide, eql, 
  lt, gt, bang, amp, pipe, carrot, tilde, 
  dot, lpar, rpar, semi, invalid, eof, ws
}valid_t;

char* concat(char *str,char c);

extern char * token_names[];

extern token get_token();

extern char * get_lexeme();

extern void init(char * filename);

extern void end_lex ();

#endif
