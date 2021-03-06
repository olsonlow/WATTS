#include "dfa.h"
#include <stdio.h>
#include <string.h>

struct file{
  FILE *input;
  char *line;
  size_t len;
  ssize_t read;
  int linecount;
  int errorcount;
  char *i;
  char *lex;
  valid_t nc;
  int position;
  int count;
}static filein;

char * token_names [] = {"ERROR", "IDENT", "NUMLIT", "PLUS", 
			 "MINUS", "MULT", "DIV", "MOD", "EXP", "NOT", "AND", "OR", 
			 "XOR", "ASSIGN", "LT", "GT", "SHIFTL", "SHIFTR", "PLUSPLUS", 
			 "PLUSEQ", "MINUSMINUS", "MINUSEQ", "MULTEQ", "DIVEQ", "MODEQ", 
			 "EXPEQ", "NOTEQ", "LOGAND", "ANDEQ", "LOGOR", "OREQ", "XOREQ", 
			 "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "TILDE", "RPAREN",
			 "LPAREN", "SEMI", "ERROR", "NUMLIT", "INTTYPE", "DBLTYPE"};

valid_t reduce_char(char c);

int get_special_type(char * id)
{
  char * i = "int";
  char * d = "double";
  if(strcmp(id, i) == 0)
    return 43;
  else if(strcmp(id, d) == 0)
    return 44;
  return -2;
}

char* concat(char *str, char c)
{
  size_t len = strlen(str);
  char *updatedstr = malloc( len + 2 );
  strcpy(updatedstr, str);
  updatedstr[len] = c;
  updatedstr[len+1] = '\0';
  return updatedstr;
}

size_t stringLen(const char *str)
{
  const char *s;
  for(s = str; *s; ++s);
  return(s - str);
}

char * get_lexeme()
{ 
  return filein.lex;
}

void reportID_error()
{
  filein.errorcount++;
  printf("Error found on line %d: Invalid IDENT found: %s\n", filein.linecount, filein.lex);
}

void report_error()
{
  filein.errorcount++;
  printf("Error at %d,%d: Invalid character found: %c\n", filein.linecount, filein.position, *filein.lex);
}

token get_next_state(token current, valid_t c)
{
//alp 0, dig 1, pls 2, mns 3, mul 4, mod 5, div 6, eql 7, lt 8, gt 9, bang 10, amp 11, pipe 12, carr 13, tilde 14, dot 15, lpar 16, rpar 17, semi 18, inv 19
  int state_table[43][22] = {{1, 2, 3, 4, 5, 7, 6, 13, 14, 15, 9, 10, 11, 12, 37, 42, 39, 38, 40, 41, 41, 0},            //START 0
			     {1, 1, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},    //IDENT 1
			     {41, 2, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 42, 41, 41, 41, 41, 41, 41},    //NUMLIT 2
			     {41, 41, 18, 41, 41, 41, 41, 19, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //PLUS   3
			     {41, 41, 41, 20, 41, 41, 41, 21, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //MINUS  4
			     {41, 41, 41, 41, 8, 41, 41, 22, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //MULT 5
			     {41, 41, 41, 41, 41, 41, 41, 23, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //DIV 6
			     {41, 41, 41, 41, 41, 41, 41, 24, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //MOD 7
			     {41, 41, 41, 41, 41, 41, 41, 25, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},   //EXP 8
			     {41, 41, 41, 41, 41, 41, 41, 26, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //NOT 9
 			     {41, 41, 41, 41, 41, 41, 41, 28, 41, 41, 41, 27, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //AND 10
			     {41, 41, 41, 41, 41, 41, 41, 30, 41, 41, 41, 41, 29, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //OR 11
			     {41, 41, 41, 41, 41, 41, 41, 31, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //XOR 12 
			     {41, 41, 41, 41, 41, 41, 41, 32, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //ASSIGN 13
			     {41, 41, 41, 41, 41, 41, 41, 34, 16, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //LT 14
			     {41, 41, 41, 41, 41, 41, 41, 36, 41, 17, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //GT 15
			     {41, 41, 41, 41, 41, 41, 41, 33, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //SHIFTL 16
			     {41, 41, 41, 41, 41, 41, 41, 35, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //SHIFTR 17
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //PLUSPLUS 18
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //PLUSEQ 19
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //MINUSMINUS 20
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //MINUSEQ 21
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //MULTEQ 22
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //DIVEQ 23
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //MODEQ 24
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //EXPEQ 25
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //NOTEQ 26
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //LOGAND 27
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //ANDEQ 28
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //LOGOR 29
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //OREQ 30
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //XOREQ 31
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //EQUALTO 32
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //SHIFTLEQ 33
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //LTE 34
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //SHIFTREQ 35
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //GTE 36
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //TILDE 37
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //RPAREN 38
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //LPAREN 39
			     {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},  //SEMI 40
			     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //ERROR 41
			     {41, 42, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41}}; //NUMNUT 42
  
  
  token ret = (state_table[current][c]);
  //  printf("the return value = %d\n", ret);
  return ret;

}
void init (char * filename)
{
  filein.input = fopen(filename, "r");
  filein.position = 0;
  filein.linecount = 0;
  filein.errorcount = 0;
  filein.lex = malloc(sizeof(char *));
  filein.read = getline(&filein.line, &filein.len, filein.input);
  printf("%d. %s",filein.linecount, filein.line);
}

char read_character()
{
  char c = filein.line[filein.position];
  if(c == '\n'){
    filein.read = getline(&filein.line, &filein.len, filein.input);
    filein.position = 0;
    filein.linecount ++;
    if(filein.read > 0){
      c = filein.line[filein.position];
      printf("%d. %s",filein.linecount, filein.line);
    }
    else{
      return 0;
    }
  }
  filein.position++;
  filein.nc = reduce_char(filein.line[filein.position]);
  return c;
}

token get_token(){
  filein.lex = "";
  token cs = ERROR;
  token ns = ERROR;
  token ks = ERROR;
  valid_t cc;
  char c = read_character();
    
  while(ns != 41){
    if(c == '\0')
      return EOFT;
    cc = (reduce_char(c));
    ns = (get_next_state(cs, cc));
    ks = (get_next_state(ns, filein.nc));
    if(cc != ws){
      filein.lex = concat(filein.lex, c);
      if(cc == invalid){
	report_error();
	return 0;
      }
      if(ks == 41){
	if(cc == dot && strlen(filein.lex) < 2){
	  report_error();
	  return 0;
	}
	else if(cs == IDENT && strlen(filein.lex) > 32){
	  reportID_error();
	  return 0;
	}
	else if(cs == IDENT && get_special_type(filein.lex) >= 43){
	  return get_special_type(filein.lex);
	}
	else if(cs == IDENT && strlen(filein.lex) < 2 && cc != ws)
	  return cs;
	else
	  return ns;
      }
      else{
	if(ns != ERROR)
	  cs = ns;
      }
    }
    c = read_character();
  }
  
}

valid_t reduce_char(char c)
{
  if(c > 126 || c < 0)
    return invalid;
  
  static valid_t lookup[256] = {
    eof,          //0
    invalid,      //1
    invalid,      //2
    invalid,      //3
    invalid,      //4
    invalid,      //5
    invalid,      //6
    invalid,      //7
    invalid,      //8
    ws,           //9
    ws,           //10
    ws,           //11
    ws,           //12
    ws,           //13
    invalid,      //14
    invalid,      //15
    invalid,      //16
    invalid,      //17
    invalid,      //18
    invalid,      //19
    invalid,      //20
    invalid,      //21
    invalid,      //22
    invalid,      //23
    invalid,      //24
    invalid,      //25
    invalid,      //26
    invalid,      //27
    invalid,      //28
    invalid,      //29
    invalid,      //30
    invalid,      //31
    ws,           //32
    bang,         //33
    invalid,      //34
    invalid,      //35
    invalid,      //36
    mod,          //37
    amp,          //38
    invalid,      //39
    rpar,         //40
    lpar,         //41
    mult,         //42
    pls,          //43
    invalid,      //44
    mns,          //45
    dot,          //46
    divide,       //47
    digit,        //48
    digit,        //49
    digit,        //50
    digit,        //51
    digit,        //52
    digit,        //53
    digit,        //54
    digit,        //55
    digit,        //56
    digit,        //57
    invalid,      //58
    semi,         //59
    lt,           //60
    eql,          //61
    gt,           //62
    invalid,      //63
    invalid,      //64
    alpha,        //65
    alpha,        //66
    alpha,        //67
    alpha,        //68
    alpha,        //69
    alpha,        //70
    alpha,        //71
    alpha,        //72
    alpha,        //73
    alpha,        //74
    alpha,        //75
    alpha,        //76
    alpha,        //77
    alpha,        //78
    alpha,        //79
    alpha,        //80
    alpha,        //81
    alpha,        //82
    alpha,        //83
    alpha,        //84
    alpha,        //85
    alpha,        //86
    alpha,        //87
    alpha,        //88
    alpha,        //89
    alpha,        //90
    invalid,      //91
    invalid,      //92
    invalid,      //93
    carrot,       //94
    alpha,        //95
    invalid,      //96
    alpha,        //97
    alpha,        //98
    alpha,        //99
    alpha,        //100
    alpha,        //101
    alpha,        //102
    alpha,        //103
    alpha,        //104
    alpha,        //105
    alpha,        //106
    alpha,        //107
    alpha,        //108
    alpha,        //109
    alpha,        //110
    alpha,        //111
    alpha,        //112
    alpha,        //113
    alpha,        //114
    alpha,        //115
    alpha,        //116
    alpha,        //117
    alpha,        //118
    alpha,        //119
    alpha,        //120
    alpha,        //121
    alpha,        //122
    invalid,      //123
    pipe,         //124
    invalid,      //125
    tilde,        //126
  };
  return lookup[c];
}

void end_lex(){
  printf("%d errors found in input file\n", filein.errorcount);
  fclose(filein.input);
  return;
}

