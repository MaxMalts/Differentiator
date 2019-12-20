#include <stdio.h>
#include "diff_tree.h"
#include "Functions_enum.h"


#define STANDARD_FUNC_OUTPUT \
	assert(curNode->right == NULL);      \
	assert(curNode->left != NULL);       \
	                                     \
	fprintf(fout, "%s(", funcS);         \
	NodesToLatex(fout, curNode->left);   \
	fprintf(fout, ")");


node_t* DSLSetDiff(node_t* node) {
	assert(node != NULL);

	node->diff = 1;

	return node;
}


/**
*	Записывает число типа float в value_t как оно записано в памяти
*
*	@param[in] num Число
*
*	@return Указатель на value_t
*/

value_t* FloatToValue_t(float num) {
	static value_t res = {};
	memcpy(res, &num, sizeof(float));

	return &res;
}


/**
*	Записывает символ в value_t
*
*	@param[in] ch Символ
*
*	@return Указатель на value_t
*/

value_t* CharToValue_t(char ch) {
	static value_t res = {};
	res[0] = ch;
	res[1] = 0;

	return &res;
}



#define DIFF(node)\
	DSLSetDiff(node)

#define CLONE(node)\
	CloneNodes(node)

#define VAR(parent, var)\
	CreateNodeProp(parent, var_node, *CharToValue_t(var), NULL, NULL)

#define NUM(parent, num)\
	CreateNodeProp(parent, num_node, *FloatToValue_t(num), NULL, NULL)

#define PLUS(parent, left, right)\
	CreateNodeProp(parent, op_node, *CharToValue_t('+'), left, right)

#define MINUS(parent, left, right)\
	CreateNodeProp(parent, op_node, *CharToValue_t('-'), left, right)

#define MUL(parent, left, right)\
	CreateNodeProp(parent, op_node, *CharToValue_t('*'), left, right)

#define DIV(parent, left, right)\
	CreateNodeProp(parent, op_node, *CharToValue_t('/'), left, right)

#define SIN(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(sin_funcI), left, NULL)

#define COS(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(cos_funcI), left, NULL)

#define TG(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(tg_funcI), left, NULL)

#define CTG(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(ctg_funcI), left, NULL)

#define ARCSIN(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(arcsin_funcI), left, NULL)

#define ARCCOS(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(arccos_funcI), left, NULL)

#define ARCTG(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(arctg_funcI), left, NULL)

#define ARCCTG(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(arcctg_funcI), left, NULL)

#define EXP(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(exp_funcI), left, NULL)

#define ABS(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(abs_funcI), left, NULL)

#define FLOOR(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(floor_funcI), left, NULL)

#define SQRT(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(sqrt_funcI), left, NULL)

#define LN(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(ln_funcI), left, NULL)

#define LG(parent, left)\
	CreateNodeProp(parent, func_node, *CharToValue_t(lg_funcI), left, NULL)

#define LOG(parent, left, right)\
	CreateNodeProp(parent, func_node, *CharToValue_t(log_funcI), left, right)

#define POW(parent, left, right)\
	CreateNodeProp(parent, func_node, *CharToValue_t(pow_funcI), left, right)