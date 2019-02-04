%option noyywrap
%option nodefault
%option nounistd
%option never-interactive

%{
#include <stdlib.h>

#include "tree.h"
#include "parser.h"
%}


DIGIT [0-9]
ID [a-zA-Z][A-Za-z0-9]*
WS [ \r\n\t]*


%%

{WS}         { }

"int main()" { return(MAIN); }

";"          { return(SEMICOLON); }

"="          { return(ASSIGN); }
"=="         { return(EQ); }
">"          { return(G); }
"<"          { return(L); }
"<="         { return(LEQ); }
">="         { return(GEQ); }
"!="         { return(NOTEQ); }

"||"         { return(OR); }
"&&"         { return(AND); }

"true"       { return(TRUE); }
"false"      { return(FALSE); }

"+"          { return(Plus); }
"-"          { return(Minus); }
"*"          { return(Times); }
"/"          { return(Div); }

"if"         { return(IF); }
"else"       { return(ELSE); }
"while"      { return(WHILE); }


"int"        { return(INT); }
"bool"       { return(BOOL); }

"("          { return(PL); }
")"          { return(PR); }

"{"          { return(CBL); }
"}"          { return(CBR); }

"return"     { return(RETURN); }

{DIGIT}+     { yylval.number = atoi(yytext); return(NUM); }
{ID}+        { yylval.identifier = strdup(yytext); return(VAR); }

.            { printf("Unknown character [%c]\n",yytext[0]);}
