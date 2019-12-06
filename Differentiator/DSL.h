#include <stdio.h>
#include "diff_tree.h"
#include "Functions_enum.h"


#define DIFF(node)\
	DSLDiffNode(node)

#define CLONE(node)\
	CloneNodes(node)

#define PLUS(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"+", left, right)

#define MINUS(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"-", left, right)

#define MUL(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"*", left, right)

#define DIV(parent, left, right)\
	CreateNodeProp(parent, op_node, (char*)"/", left, right)

#define SIN(parent, left)\
	CreateNodeProp(parent, func_node, sin_funcI, left, NULL)

#define COS(parent, left)\
	CreateNodeProp(parent, func_node, cos_funcI, left, NULL)

#define TG(parent, left)\
	CreateNodeProp(parent, func_node, tg_funcI, left, NULL)

#define CTG(parent, left)\
	CreateNodeProp(parent, func_node, ctg_funcI, left, NULL)

#define ARCSIN(parent, left)\
	CreateNodeProp(parent, func_node, arcsin_funcI, left, NULL)

#define ARCCOS(parent, left)\
	CreateNodeProp(parent, func_node, arccos_funcI, left, NULL)

#define ARCTG(parent, left)\
	CreateNodeProp(parent, func_node, arctg_funcI, left, NULL)

#define ARCCTG(parent, left)\
	CreateNodeProp(parent, func_node, arcctg_funcI, left, NULL)

#define EXP(parent, left)\
	CreateNodeProp(parent, func_node, exp_funcI, left, NULL)

#define ABS(parent, left)\
	CreateNodeProp(parent, func_node, abs_funcI, left, NULL)

#define FLOOR(parent, left)\
	CreateNodeProp(parent, func_node, floor_funcI, left, NULL)

#define SQRT(parent, left)\
	CreateNodeProp(parent, func_node, sqrt_funcI, left, NULL)

#define LN(parent, left)\
	CreateNodeProp(parent, func_node, ln_funcI, left, NULL)

#define LG(parent, left)\
	CreateNodeProp(parent, func_node, lg_funcI, left, NULL)

#define LOG(parent, left, right)\
	CreateNodeProp(parent, func_node, log_funcI, left, right)

#define POW(parent, left, right)\
	CreateNodeProp(parent, func_node, pow_funcI, left, right)