#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "diff_tree.h"
#include "buffer.h"


int main() {
	char expr[] = "3/sin(x)*pow(4/12,x*2+1)+5*x";

	int syntaxErr = 0;
	tree_t exprTree = ExprToTree(expr, &syntaxErr);

	int a = ShowTree(&exprTree);
}