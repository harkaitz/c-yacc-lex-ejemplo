#include "mc.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int
main(int _argc, char *_argv[])
{
	bool dryrun = false;
	bool refs = false;
	
	if (_argc == 1 || !strcmp(_argv[0], "--help") || !strcmp(_argv[0], "-h")) {
		printf("Usage: minicalc [-dr] FILE\n");
		return 0;
	}
	
	mc_load_library();
	
	int opt = 1;
	if (opt < _argc && _argv[opt][0] == '-') {
		dryrun = (strchr(_argv[opt], 'd'))?true:false;
		refs = (strchr(_argv[opt], 'r'))?true:false;
		opt++;
	}
	
	for (;opt < _argc; opt++) {
		Node *tree = mc_parse(_argv[opt]);
		if (refs) {
			printf("== Initial objects: %i\n", node_count);
		}
		if (!dryrun) {
			node_eval(tree);
		}
		if (refs) {
			printf("== Final objects: %i\n", node_count);
		}
		node_print(tree, 0, stdout);
		node_free(tree);
	}
	
	mc_clear();
	
	if (refs) {
		printf("== Leaked objects: %i\n", node_count);
	}
	return 0;
}
