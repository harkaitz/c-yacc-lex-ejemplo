#include "mc.h"
#include "ast.h"
#include "y.tab.h"
#include "ext/utlist.h"
#include <stdio.h>
#include <strings.h>
#ifndef MINICALC_MAX_RULES
#  define MINICALC_MAX_RULES 100
#endif

typedef struct Rule Rule;
struct Rule {
	char const *name;
	void (*eval) (Node *_node);
};

Rule   rules[MINICALC_MAX_RULES+1];
size_t rulesz = 0;

Node  *binds = NULL;

void
node_print(Node *_node, int _prefix, FILE *_fp)
{
	Node *elem;
	char const *tabs =
	    "                                                          "
	    "                                                          "
	    "                                                          "
	    "                                                          "
	    ;
	switch (_node->nterminal) {
	case NT_FUNCTION:
		fprintf(_fp, "%.*s" "func %s:\n", _prefix, tabs, _node->text);
		LL_FOREACH(_node->children, elem) {
			node_print(elem, _prefix+4, _fp);
		}
		break;
	case NT_ERROR:
		fprintf(_fp, "%.*s" "error: %s.\n", _prefix, tabs, _node->text);
		break;
	case NT_TREE:
		LL_FOREACH(_node->children, elem) {
			node_print(elem, _prefix, _fp);
		}
		break;
	case NT_SET:
		fprintf(_fp, "%.*s" "SET %s =\n", _prefix, tabs, _node->text);
		LL_FOREACH(_node->children, elem) {
			node_print(elem, _prefix+4, _fp);
		}
		break;
	}
	switch (_node->terminal) {
	case NUMBER:
		fprintf(_fp, "%.*s" "%f\n", _prefix, tabs, _node->number);
		break;
	case IDENTIFIER:
		fprintf(_fp, "%.*s" "%s\n", _prefix, tabs, _node->text);
		break;
	}
}

void
node_eval(Node *_node)
{
	size_t i; Node *elem;
	
	/* Variable. */
	if (_node->terminal == IDENTIFIER) {
		LL_FOREACH(binds, elem) {
			if (!strcmp(_node->text, elem->text)) {
				node_copy(_node, elem->children);
				return;
			}
		}
		return;
	}
	
	/* Set variable. */
	if (_node->nterminal == NT_SET) {
		elem = node_copy(node_alloc(), _node);
		if (elem->children) {
			node_eval(elem->children);
		}
		LL_APPEND(binds, elem);
		node_clear(_node);
		return;
	}
	
	/* Evaluate children. */
	if (_node->nterminal == NT_FUNCTION || _node->nterminal == NT_TREE) {
		LL_FOREACH(_node->children, elem) {
			node_eval(elem);
		}
	}
	
	/* Functions. */
	if (_node->nterminal == NT_FUNCTION) {
		for (i=0; i<rulesz; i++) {
			if (!strcasecmp(_node->text, rules[i].name)) {
				rules[i].eval(_node);
				break;
			}
		}
	}
	
	return;
}

void
mc_add_rule(char const *_name, void (*eval) (Node *))
{
	rules[rulesz].name = _name;
	rules[rulesz].eval = eval;
	rulesz++;
}

void
mc_clear(void)
{
	Node *node, *tmp;
	LL_FOREACH_SAFE(binds, node, tmp) {
		node_free(node);
	}
	binds = NULL;
	rulesz = 0;
}
