MANUAL Y EJEMPLO DE UN PARSER EN YACC/LEX
=========================================

Este proyecto contiene las fuentes de un programa de ejemplo que mediante
YACC/LEX implementa una calculadora básica. Esta calculadora es capaz
de parsear e interpretar el siguiente ejemplo:

    pi = 3.14;           1.000000
    sin(pi/2);     --->  func cos:
    cos(2*pi*w);             func *:
    2+2*3+2;                     6.280000
    (2+2)*(3+2);                 w
                         10.000000
                         20.000000

Esto es, el interprete para si hay una variable sin definir. Conoce
sumas y las funciones trigonométricas sin y cos. Es delimitado por
"semicolons" y conoce como agrupar operaciones.

Puedes ver el código fuente en [./c-minicalc](./c-minicalc). Este sigue
la típica estructura de un interprete.

        [ejemplos/a.mc]
             |
        _____V_________
       | Lexer (lex.l) |  (Auto-generado por LEX: lex.yy.c)
       |_______________|
             |
        _____V_____________
       | Parser (parser.y) | (Auto-generado por YACC: y.tab.h y.tab.c)
       |___________________|
         ____|________
        / AST (ast.h) \     Abstract Syntax Tree (ast.c)
        \_____________/
             |
        _____V_______________  ___________________________
       | Interpreter (int.c) || Librería estándar (lib.c) |
       |                     || (+,-,*,/,sin,cos)         |
       |_____________________||___________________________|
             |
             V
         [Salida]

Algunos puntos se han anotado en [./YACC_LEX_REFERENCIA.md](./YACC_LEX_REFERENCIA.md) y en
[AST_CONSIDERACIONES.md](./AST_CONSIDERACIONES.md).

## Como aprender YACC/LEX en un día

Se recomienda implementar un parser propio, una calculadora es un
buen ejemplo. También se puede implementar un parser de un JSON
simplificado.

Se puede seguir el siguiente proceso:

1. Leer manuales de [yacc(1p)](https://raw.githubusercontent.com/harkaitz/c-yacc-lex-ejemplo/master/doc/yacc.1p.pdf) y [lex(1p)](https://raw.githubusercontent.com/harkaitz/c-yacc-lex-ejemplo/master/doc/lex.1p.pdf) a medida
   que se leen las fuentes [lex.l](./c-minicalc/lex.l) y [parser.y](./c-minicalc/parser.l).

2. Una vez entendido la estructura general, escribir unos "lex.l" y "parser.y" propios, pero
   que en vez de rellenar el AST solamente impriman mensajes. Hacer un main rudimentario.

3. Definir un AST, y rellenarlo en el parser. Se recomienda también escribir un "node_print()"
   para poder visualizar el árbol generado.

4. Escribir el interprete.

En principio se puede en un día, se recomienda leer la [referencia](./YACC_LEX_REFERENCIA.md) y
las [consideraciones](./AST_CONSIDERACIONES.md) antes de empezar a escribir el código.

## Bison

El código generado con yacc utiliza variables globales, para un código que sea re-entrante y
thread safe utiliza Bison.
