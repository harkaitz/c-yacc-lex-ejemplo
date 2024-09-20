#include "ast.h"
#include "y.tab.h"
#include "ext/utlist.h"
#include <stdarg.h>
#include <assert.h>

int node_count = 0;

/* -----------------------------------------------------------------------------
 * ---- METHODS FOR THE INTERPRETER --------------------------------------------
 * -------------------------------------------------------------------------- */

void
node_clear(Node *_node)
{
	Node *elem, *tmp;
	LL_FOREACH_SAFE(_node->children, elem, tmp) {
		LL_DELETE(_node->children, elem);
		node_free(elem);
	}
	_node->terminal = 0;
	_node->nterminal = 0;
	_node->text[0] = 0;
	_node->children = NULL;
}

void
node_free(Node *_node)
{
	node_clear(_node);
	node_count--;
	free(_node);
}

Node *
node_set_error(Node *_node, char const *_fmt, ...)
{
	node_clear(_node);
	_node->nterminal = NT_ERROR;
	va_list args;
	va_start(args, _fmt);
	vsnprintf(_node->text, sizeof(_node->text)-1, _fmt, args);
	va_end(args);
	return _node;
}

bool
node_is_nil(Node *_node)
{
	return _node->terminal == 0 && _node->nterminal == 0;
}

Node *
node_copy(Node *_to, Node *_fr)
{
	Node *elem,*copy;
	node_clear(_to);
	_to->terminal = _fr->terminal;
	_to->nterminal = _fr->nterminal;
	memcpy(_to->text, _fr->text, sizeof(_fr->text));
	LL_FOREACH(_fr->children, elem) {
		copy = node_copy(node_alloc(), elem);
		LL_APPEND(_to->children, copy);
	}
	return _to;
}

/* -----------------------------------------------------------------------------
 * ---- METHODS FOR THE PARSER -------------------------------------------------
 * -------------------------------------------------------------------------- */

Node *
node_alloc(void)
{
	Node *ret = calloc(1, sizeof(struct Node));
	assert(ret);
	node_count++;
	return ret;
}

Node *
node_set_tree(Node *_node)
{
	node_clear(_node);
	_node->nterminal = NT_TREE;
	return _node;
}

Node *
node_set_number(Node *_node, int _terminal, double _number)
{
	node_clear(_node);
	_node->terminal = _terminal;
	_node->number = _number;
	return _node;
}

Node *
node_set_text(Node *_node, int _terminal, char const _text[])
{
	node_clear(_node);
	_node->terminal = _terminal;
	strncpy(_node->text, _text, sizeof(_node->text)-1);
	return _node;
}

Node *
node_set_nterminal(Node *_node, int _nterminal, char const _text[], Node *_child, ...)
{
	va_list args; Node *child;
	
	node_clear(_node);
	_node->nterminal = _nterminal;
	strncpy(_node->text, _text, sizeof(_node->text)-1);
	if (_child) {
		node_add(_node, _child);
		va_start(args, _child);
		while ((child = va_arg(args, Node *))) {
			node_add(_node, child);
		}
		va_end(args);
	}
	
	return _node;
}

Node *
node_add(Node *_node, Node *_elem)
{
	LL_APPEND(_node->children, _elem);
	return _node;
}
