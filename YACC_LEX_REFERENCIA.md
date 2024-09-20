YACC PARSER CREATOR
===================

## GNU Emacs

    package-install bison-mode

## Ejemplos LEX:

Token string (Luego manualmente habrá que quitar los " y \).

    \"(\\.|[^\"])*\" { strcpy(yylval.str, yytext); return STRING; }

Símbolo literal.

    "sin" { yylval.str = "sin"; return FUNCTION; }
    ";"   { return SEMICOLON; }

Numero float.

    [0-9\.]+ { yylval.num = atof(yytext); /* stdlib */; return NUMBER; }

Ignorar otros caracteres (poner al final).

    . { /**/ }

## Ejemplos YACC

La prevalencia de los operadores se hace con %right y %left

    %right EQUALS
    %left PLUS MINUS
    %left TIMES DIVIDE

El tipo de "$$" de un no terminal se define con %type. Cuando el no
terminal se encuentre a la derecha $1 ... cogerán también ese tipo.

Los tipos de tokens que retorna el lexer se definen con %token. El
lexer podrá acceder a ello desde `yylval`.

Los tipos se definen con %union para que estén disponibles en `yylval`.

    %union {
        char   str[60];
        double num;
        struct Node *node;
    }
    
    %token  <str> IDENTIFIER
    %token  <num>
    %type <node> expression
    %type <node> statement

## Compatibilidad de compiladores.

### Compatibilidad C99.

El código generado comprueba si la entrada es un tty con fileno para
saber si leer el stream con "getc" o "fread". Se puede forzar uno u
otro con sed.

    sed -i '/yy_is_interactive =.*fileno/s|=.*\;|=1\;|' lex.yy.c

También se puede hacer "fileno" disponible añadiendo el "_POSIX_C_SOURCE".

    %top{
        #define _POSIX_C_SOURCE 200809L
    }

O finalmente poner su declaración.

    %{
        #include <stdio.h>
        extern int fileno(FILE *);
    %}

### Compatibilidad GCC

El código generado define tres funciones estáticas que no se usan, se
puede desactivar su generación o desactivar el warning.

    %top{
    #ifdef __GNUC__
    #  pragma GCC diagnostic ignored "-Wunused-function"
    #endif
    }

    %option noyywrap
    %option nounput
    %option noinput

## Reporte de errores.

Cuando hay un error se ejecuta `yyerror()`. Se puede definir las variables
globales "zzfile" "zzline" para gestionar la linea del error.

    | [lex.l]                | [yacc.y]
    |                        |
    | %{                     | %%
    | int zzline = 1;        | %%
    | %}                     | extern int zzline;
    | %%                     | char const *zzfile;
    | [ \t\r] {            } | int yyerror(const char *s) {
    | \n      { zzline ++; } |     fprintf(stderr, "error: %s:%i: %s\n", zzfile, zzline, s);
    | %%                     |     return 1;
    |                        | }

## Ejecución del parser.

El lexer utiliza "yyin" para leer. Se puede definir una función en
el `parser.y` que reinicie el árbol AST y coloque en yyin la entrada
que queremos.

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

