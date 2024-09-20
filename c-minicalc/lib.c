#include "mc.h"
#include "ast.h"
#include "y.tab.h"
#include "ext/utlist.h"
#include <stdbool.h>
#include <math.h>

static void
node_reduce(Node *_node, double (*_f) (double a, double b))
{
	Node  *elem;
	double number = 1;
	int    count = 0;
	bool   all_numbers = true;
	
	LL_FOREACH(_node->children, elem) {
		if (elem->terminal == NUMBER) {
			number = (count)? _f(number, elem->number) : elem->number;
		} else {
			all_numbers = false;
		}
		count++;
	}
	if (all_numbers) {
		node_set_number(_node, NUMBER, number);
	}
	return;
}

static void
node_apply(Node *_node, double (*_f) (double a))
{
	if (_node->children && _node->children->terminal == NUMBER) {
		node_set_number(_node, NUMBER, _f(_node->children->number));
	}
}

static double f_multiply(double a, double b)  { return a*b; }
static double f_increment(double a, double b) { return a+b; }
static double f_decrement(double a, double b) { return a-b; }
static double f_divide(double a, double b)    { return a/b; }

static void e_multiply(Node *_node)  { node_reduce(_node, f_multiply); }
static void e_increment(Node *_node) { node_reduce(_node, f_increment); }
static void e_decrement(Node *_node) { node_reduce(_node, f_decrement); }
static void e_divide(Node *_node)    { node_reduce(_node, f_divide); }
static void e_sin(Node *_node)       { node_apply(_node, sin); }
static void e_cos(Node *_node)       { node_apply(_node, cos); }


void mc_load_library(void)
{
	mc_add_rule("*", e_multiply);
	mc_add_rule("+", e_increment);
	mc_add_rule("-", e_decrement);
	mc_add_rule("/", e_divide);
	mc_add_rule("sin", e_sin);
	mc_add_rule("cos", e_cos);
}
