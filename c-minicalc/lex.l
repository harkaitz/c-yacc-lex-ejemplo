%{
#include "y.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int zzline = 1;
%}
%option noyywrap
%option nounput
%option noinput
%%

[a-zA-Z][a-zA-Z0-9]* { strncpy(yylval.str, yytext, sizeof(yylval.str)-1); return IDENTIFIER; }
[0-9\.]+             { yylval.num = atof(yytext); return NUMBER; }

"+"                  { return PLUS; }
"-"                  { return MINUS; }
"*"                  { return TIMES; }
"/"                  { return DIVIDE; }

"="                  { return EQUALS; }
"("                  { return LPAREN; }
")"                  { return RPAREN; }
";"                  { return SEMICOLON; }
[ \t\r]              { /* Whitespace ignored */ }
\n                   { zzline++; }

%%
