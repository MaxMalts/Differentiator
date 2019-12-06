#include <stdio.h>
#include "diff_tree.h"
#include "Functions_enum.h"


#define PLUS(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"+", left, right);

#define MINUS(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"-", left, right);

#define MUL(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"*", left, right);

#define DIV(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"/", left, right);

#define SIN(parent, left, right)\
	CreateNodeProp(parent, func_node, sin_funcI, left, right);