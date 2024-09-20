#ifndef MINICALC_H
#define MINICALC_H

#include <stdio.h>

typedef struct Node Node; /* ast.h */
extern void node_free(Node *_node); /* ast.c */
extern int  node_count; /* ast.c */

extern Node *mc_parse(char const *_file); /* parser.y */

extern void  mc_load_library(void); /* lib.c */

extern void  node_print(Node *_node, int _prefix, FILE *_fp); /* int.c */
extern void  node_eval(Node *_node); /* int.c */
extern void  mc_add_rule(char const *_name, void (*eval) (Node *)); /* int.c */
extern void  mc_clear(void);

#endif
