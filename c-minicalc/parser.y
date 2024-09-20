%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int yyerror(const char *s);
int yylex(void);
extern int zzline;
struct Node *zztree = NULL;
%}

%union {
    char   str[60];
    double num;
    struct Node *node;
}

%token  <str> IDENTIFIER
%token  <num> NUMBER
%token EQUALS
%token LPAREN
%token RPAREN
%token SEMICOLON

%token PLUS
%token MINUS
%token TIMES
%token DIVIDE

%right EQUALS
%left PLUS MINUS
%left TIMES DIVIDE
                        
%type <node> expression
%type <node> statement
%%


program:        program statement { node_add(zztree, $2);  }
        |       statement         { node_add(zztree, $1); }
                ;

statement:
                expression SEMICOLON                   { $$ = $1; }
        |       IDENTIFIER EQUALS expression SEMICOLON { $$ = node_set_nterminal(node_alloc(), NT_SET, $1, $3, NULL); }
                ;

expression:     expression TIMES  expression { $$ = node_set_nterminal(node_alloc(), NT_FUNCTION, "*", $1, $3, NULL); }
        |       expression MINUS  expression { $$ = node_set_nterminal(node_alloc(), NT_FUNCTION, "-", $1, $3, NULL); }
        |       expression PLUS   expression { $$ = node_set_nterminal(node_alloc(), NT_FUNCTION, "+", $1, $3, NULL); }
        |       expression DIVIDE expression { $$ = node_set_nterminal(node_alloc(), NT_FUNCTION, "/", $1, $3, NULL); }
        |       LPAREN expression RPAREN     { $$ = $2; }
        |       IDENTIFIER LPAREN expression RPAREN { $$ = node_set_nterminal(node_alloc(), NT_FUNCTION, $1, $3, NULL); } 
        |       NUMBER                       { $$ = node_set_number(node_alloc(), NUMBER, $1); }
        |       IDENTIFIER                   { $$ = node_set_text(node_alloc(), IDENTIFIER, $1); }
                ;
%%
extern int zzline;
char const *zzfile;

int yyerror(const char *s) {
    fprintf(stderr, "error: %s:%i: %s\n", zzfile, zzline, s);
    return 1;
}

Node *
mc_parse(char const *_file)
{
	int ret;
	extern FILE *yyin;
	zzfile = _file; zzline = 1;
	yyin = fopen(_file, "r");
	if (yyin) {
		zztree = node_set_tree(node_alloc());
		ret = yyparse();
		fclose(yyin);
		return (ret)?NULL:zztree;
	} else {
		yyerror(strerror(errno));
		return NULL;
	}
}
