#ifndef MINICALC_AST_H
#define MINICALC_AST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node Node;

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

extern int node_count;

/* Required by the interpreter. (int.c) */
extern void  node_clear(Node *_node); /* Leaves malloced, next. */
extern void  node_free(Node *_node);
extern Node *node_set_error(Node *_node, char const *_fmt, ...);
extern bool  node_is_nil(Node *_node);
extern Node *node_copy(Node *_to, Node *_fr);

/* Required by the parser. (yacc.y) */
extern Node *node_alloc(void);
extern Node *node_set_tree(Node *_node);
extern Node *node_set_number(Node *_node, int _token, double _number);
extern Node *node_set_text(Node *_node, int _token, char const _text[]);
extern Node *node_set_nterminal(Node *_node, int nterminal, char const _text[], Node *child, ...);
extern Node *node_add(Node *node, Node *elem);

#endif
