AST CONSIDERACIONES
===================

## Estructura del AST.

La estructura que se ha utilizado es el siguiente:

    struct Node {
            int   terminal;  /* Leaves. IDs from the lexer (lex). */
            int   nterminal; /* Branches. IDs from the parser (yacc). */
    #       define NT_FUNCTION 1
    #       define NT_ERROR    2
    #       define NT_TREE     3
    #       define NT_SET      4
            union {
            	double number;
            	char   text[100];
            };
            Node *children;
            Node *next;
    };

Las razones son las siguientes:

1. Los terminales normalmente ya tienen definido un identificador mediante
   `%token` en `parser.y`. Este genera defines en `y.tab.h` que se pueden
   utilizar para `int terminal`.

2. Los no terminales no tienen define asociado, por lo deben definirse
   en el `ast.h`.

3. Al mantener separados identificadores `terminal` y `nterminal` hace
   fácil discernir el tipo del nodo con construcciones `if (_node->terminal)` y
   `if (_node->nterminal)`.

4. En la `union` se ponen los datos del nodo, en children los hijos en
   caso de no terminales.

## Inicializadores

Se ha seguido el esquema de funciones siguiente:

    Node *node_alloc(void);
    Node *node_set_tree(Node *_node);
    Node *node_set_TIPO(Node *_node, ...);

en vez de:

    [NO] Node *node_new_tree(void);
    [NO] Node *node_new_...

para que el interprete pueda hacer *la evaluación destructivamente*. El
código va ha ser mucho mas limpio.

Se puede llevar una cuenta de `node_alloc` y `node_free`.

## Limpiadores.

Se define un `node_clear()` por separado del free el cual preserva la
variable `Node *next;`. Esto permite reemplazar un nodo por otro durante
la evaluación.

    void  node_clear(Node *_node);
    void  node_free(Node *_node);

Un nodo limpio se puede utilizar como si fuera un `nil`, por eso se ha
definido un `node_is_nil()` en vez de clear un identificador nuevo.

## Errores

Es mejor definir un nodo error y hacer la evaluación tal que evalúe hacia
el error, en vez de devolverlo por un camino distinto.

    void node_eval(Node *_node);

La función de evaluación devuelve el tipo error en caso de error.

## Copia profunda.

Implementa un copiador que haga una copia profunda, puede ser útil a
la hora de guardar datos en el interprete, como puede ser la operación "=".

    Node *node_copy(Node *_to, Node *_fr);

## Nodos estáticos

La idea es mala, porque los nodos se guardan en listas con next. Utilizar
arrays tampoco es buena idea, pues sacar y meter hijos es una operación
común en el interprete.

## Guardar fichero y linea

Puede ser buena idea guardar fichero y linea en el nodo, para que al
evaluar el error pueda reportarlo.
