%option noyywrap

%{
#include "tree.h"
#include "parser.h"
#include <stdlib.h>
%}


DIGIT [0-9]
ID [a-zA-Z][A-Za-z0-9]*


%%
[\t\n]+



"int main()"	{return(MAIN);}

";"		      {return(SEMICOLON);}

"="		      {return(ASSIGN);}
"=="	         {return(EQ);}
">"		      {return(G);}
"<"		      {return(L);}
"<="	         {return(LEQ);}
">="	         {return(GEQ);}
"!=" 	         {return(NOTEQ);}

"||"	         {return(OR);}
"&&"	         {return(AND);}

"true"	      {return(TRUE);}
"false"	      {return(FALSE);}

"+"		      {return(Plus);}
"-"		      {return(Minus);}
"*" 	         {return(Times);}
"/" 	         {return(Div);}

"if" 	         {return(IF);}
"else" 	      {return(ELSE);}
"while"	      {return(WHILE);}


"int" 	      {return(INT);}
"bool"	      {return(BOOL);}

"("		      {return(P0);}
")"		      {return(P1);}

"{"		      {return(CB0);}
"}"		      {return(CB1);}

"return"       {return(RETURN);}

{DIGIT}+  { yylval.numero = atoi(yytext);   return(NUM);}
{ID}+ 	 { yylval.ident  = strdup(yytext); return(VAR);}
