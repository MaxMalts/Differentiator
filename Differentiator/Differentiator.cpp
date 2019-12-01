#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "diff_tree.h"
#include "buffer.h"


/**
*	���� �����
*
*	@param[in] var
*
*	@return -1 - �����; 1 - ����
*/

int Sign(float var) {
	if (var < 0) {
		return -1;
	}

	return 1;
}


/**
*	������� ����� foat �� ��� ����� � ������� ������
*
*	@param[in] intPart ����� ����� (����� ���� �������������)
*	@param[in] fracPart ������� ����� (������ �������������)
*
*	@return ����� float
*/

float CreateFloat(int intPart, int fracPart) {
	int tenPow = 1;
	while (fracPart / tenPow != 0) {
		tenPow *= 10;
	}

	float res = intPart + Sign(intPart) * (float)fracPart / tenPow;

	return res;
}


#define assert_syntax(syntaxErr) \
	if(syntaxErr) {\
		return {};\
	}


const char* curSequence = "";

node_t* GetG(int* syntaxErr);

node_t* GetE(int* syntaxErr);

node_t* GetT(int* syntaxErr);

node_t* GetP(int* syntaxErr);

node_t* GetFunc(int* syntaxErr);

node_t* GetId(int* syntaxErr);

node_t* GetFNode(int* syntaxErr);

float GetF(int* syntaxErr);

int GetI(int* syntaxErr);

int GetN(int* syntaxErr);



/**
*	������� ��� ��������� ��������� (G::=E'\0')
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return �������� ���������
*/

node_t* GetG(int* syntaxErr) {
	assert(syntaxErr != NULL);

	node_t* res = GetE(syntaxErr);
	assert_syntax(*syntaxErr);

	if (*curSequence != '\0') {
		*syntaxErr = 1;
		free(res);
	}
	assert_syntax(*syntaxErr);
	curSequence++;

	return res;
}


/**
*	������� ��� ��������� �����, �������� (E::=T{[+-]T}*)
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return �������� ���������
*/

node_t* GetE(int* syntaxErr) {
	assert(syntaxErr != NULL);

	node_t* res = NULL;

	node_t* node1 = GetT(syntaxErr);
	assert_syntax(*syntaxErr);

	if (*curSequence == '+' || *curSequence == '-') {
		char op = *curSequence;
		curSequence++;

		node_t* node2 = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		res = CreateNode();

		res->type = op_node;
		res->value[0] = op;
		res->value[1] = '\0';

		res->left = node1;
		res->right = node2;
		node1->parent = res;
		node2->parent = res;
	}
	else {
		res = node1;
	}

	return res;
}


/**
*	������� ��� ��������� ���������, ������� (T::=P{[/*]P}*)
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return �������� ���������
*/

node_t* GetT(int* syntaxErr) {
	assert(syntaxErr != NULL);

	node_t* res = NULL;

	node_t* node1 = GetP(syntaxErr);
	assert_syntax(*syntaxErr);

	if (*curSequence == '*' || *curSequence == '/') {
		char op = *curSequence;
		curSequence++;

		node_t* node2 = GetT(syntaxErr);
		assert_syntax(*syntaxErr);

		res = CreateNode();

		res->type = op_node;
		res->value[0] = op;
		res->value[1] = '\0';

		res->left = node1;
		res->right = node2;
		node1->parent = res;
		node2->parent = res;
	}
	else {
		res = node1;
	}

	return res;
}


/**
*	������� ��� ��������� ��������� � �������, ������ ��� �����(P::='('E')'|Func|F).\
 ����� ����������, ��� ������ ��������, ������� ������ ������.
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return �������� ���������
*/

node_t* GetP(int* syntaxErr) {
	assert(syntaxErr != NULL);

	node_t* res = NULL;

	if (*curSequence == '(') {
		curSequence++;

		res = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
			free(res);
		}
		assert_syntax(*syntaxErr);
		curSequence++;
	}

	else if (isalpha(*curSequence)) {
		if (isalpha(*(curSequence + 1))) {
			res = GetFunc(syntaxErr);
			assert_syntax(*syntaxErr);
		}
		else {
			res = GetId(syntaxErr);
			assert_syntax(*syntaxErr);
		}
	}

	else {
		res = GetFNode(syntaxErr);
		assert_syntax(*syntaxErr);
	}

	return res;
}


/**
*	������� ��� ��������� ������� ( Func::="sin"'('E')'|"cos"'('E')'...|"pow"'('E','E')' )
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return �������� �������
*/

node_t* GetFunc(int* syntaxErr) {
	assert(syntaxErr != NULL);

	node_t* res = NULL;

#define DEF_FUNC(str, NParams, funcI) \
	                                                               \
	else if (strncmp(curSequence, #str, sizeof(#str) - 1) == 0) {  \
		assert(NParams == 1 || NParams == 2);                      \
		                                                           \
		curSequence = curSequence + sizeof(#str) - 1;              \
		if (*curSequence != '(') {                                 \
			*syntaxErr = 1;                                        \
		}                                                          \
		assert_syntax(*syntaxErr);                                 \
		curSequence++;                                             \
		                                                           \
		res = CreateNode();                                        \
		res->type = func_node;                                     \
		res->value[0] = funcI;                                     \
		                                                           \
		res->left = GetE(syntaxErr);                               \
		assert_syntax(*syntaxErr);                                 \
		res->left->parent = res;                                   \
		                                                           \
		if (NParams == 2) {                                        \
			if (*curSequence != ',') {                             \
				*syntaxErr = 1;                                    \
				free(res->left);                                   \
				free(res);                                         \
			}                                                      \
			assert_syntax(*syntaxErr);                             \
			curSequence++;                                         \
			                                                       \
			res->right = GetE(syntaxErr);                          \
			assert_syntax(*syntaxErr);                             \
			res->right->parent = res;                              \
		}                                                          \
		                                                           \
		if (*curSequence != ')') {                                 \
			*syntaxErr = 1;                                        \
			                                                       \
			free(res->left);                                       \
			if (res->right) {                                      \
				free(res->right);                                  \
			}                                                      \
			free(res);                                             \
		}                                                          \
		assert_syntax(*syntaxErr);                                 \
		curSequence++;                                             \
	}

	if (0) {}
#include "Functions.h"
	else {
		*syntaxErr = 1;
	}
	assert_syntax(*syntaxErr);

#undef DEF_FUNC

	return res;
}


node_t* GetId(int* syntaxErr) {
	assert(syntaxErr != NULL);

	if (!isalpha(*curSequence)) {
		*syntaxErr = 1;
	}
	assert_syntax(*syntaxErr);

	node_t* res = CreateNode();
	res->type = var_node;
	res->value[0] = *curSequence;
	res->value[1] = '\0';

	curSequence++;

	return res;
}


node_t* GetFNode(int* syntaxErr) {
	assert(syntaxErr != NULL);

	float val = GetF(syntaxErr);
	assert_syntax(*syntaxErr);

	node_t* res = CreateNode();
	res->type = num_node;
	memcpy(res->value, &val, sizeof(val));

	return res;
}


/**
*	������� ��� ��������� ����� float ( F::=I{'.'N}? )
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return ����� float
*/

float GetF(int* syntaxErr) {
	assert(syntaxErr != NULL);

	float val = 0;

	int intPart = GetI(syntaxErr);
	assert_syntax(*syntaxErr);

	int fracPart = 0;
	if (*curSequence == '.') {
		curSequence++;

		fracPart = GetN(syntaxErr);
		assert_syntax(*syntaxErr);
	}

	val = CreateFloat(intPart, fracPart);

	return val;
}


/**
*	������� ��� ��������� ������ ����� ( I::=[+-]?N )
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return ����� �����
*/

int GetI(int* syntaxErr) {
	assert(syntaxErr != NULL);

	int val = 0;
	int sign = 1;

	if (*curSequence == '-') {
		sign = -1;
		curSequence++;
	}
	else if (*curSequence == '+') {
		curSequence++;
	}

	val = GetN(syntaxErr);
	assert_syntax(*syntaxErr);

	val *= sign;

	return val;
}


/**
*	������� ��� ��������� ������ �������������� ����� ( N::=['0'-'9']+ )
*
*	@param[out] syntaxErr �������� �� �������������� ������ (1 ��� 0)
*
*	@return ����� ������������� �����
*/

int GetN(int* syntaxErr) {
	assert(syntaxErr != NULL);

	int val = 0;

	if (!('0' <= *curSequence && *curSequence <= '9')) {
		*syntaxErr = 1;
	}
	assert_syntax(*syntaxErr);

	while ('0' <= *curSequence && *curSequence <= '9') {
		val = val * 10 + (*curSequence - '0');
		curSequence++;
	}

	return val;
}


/**
*	������� ������ �� ���������
*
*	@param[in] expr ���������
*
*	@return ������
*/

tree_t ExprToTree(char* expr, int* syntaxErr = NULL) {
	assert(expr != NULL);

	curSequence = expr;
	int syntErr = 0;
	node_t* root = GetG(&syntErr);
	if (syntaxErr != NULL) {
		*syntaxErr = syntErr;
	}
	if (syntErr) {
		return {};
	}

	tree_t tree = TreeRootConstructor(root, "expression_tree");

#ifdef _DEBUG
	if (TreeOk(&tree)) {
		PrintTree_OK(tree);
	}
	else {
		PrintTree_NOK(tree);
	}
#endif

	return tree;
}


void DifferentiateNode(node_t* curNode) {
	assert(curNode != NULL);

	switch (curNode->type) {
	case num_node: {
		float newNum = 0;
		memcpy(curNode->value, &newNum, sizeof(float));
		break;
	}
	case var_node: {
		curNode->type = num_node;
		float newNum = 1;
		memcpy(curNode->value, &newNum, sizeof(float));
		break;
	}
	case op_node:
		switch (curNode->value[0]) {
		case '+':
		case '-':
			DifferentiateNode(curNode->left);
			DifferentiateNode(curNode->right);
			break;
		case '*': {
			curNode->value[0] = '+';

			node_t* leftOld = curNode->left;
			node_t* rightOld = curNode->right;

			node_t* leftNew = CreateNodeProp(curNode, op_node, (char*)"*", leftOld, rightOld);
			node_t* rightNew = CreateNodeProp(curNode, op_node, (char*)"*", CloneNodes(leftOld), CloneNodes(rightOld));

			curNode->left = leftNew;
			curNode->right = rightNew;

			DifferentiateNode(leftNew->left);
			DifferentiateNode(rightNew->right);
			break;
		}
		case '/': {
			curNode->value[0] = '/';

			node_t* leftOld = curNode->left;
			node_t* rightOld = curNode->right;

			node_t* minusLeft = CreateNodeProp(NULL, op_node, (char*)"*", leftOld, rightOld);
			node_t* minusRight = CreateNodeProp(NULL, op_node, (char*)"*", CloneNodes(leftOld), CloneNodes(rightOld));

			node_t* leftNew = CreateNodeProp(curNode, op_node, (char*)"-", minusLeft, minusRight);

			node_t* rightNew = CreateNodeProp(curNode, op_node, (char*)"*", CloneNodes(rightOld), CloneNodes(rightOld));

			curNode->left = leftNew;
			curNode->right = rightNew;

			DifferentiateNode(minusLeft->left);
			DifferentiateNode(minusRight->right);
			break;
		}
		}
	}
}

void Differentiate(tree_t* exprTree) {
	assert(exprTree != NULL);

	DifferentiateNode(exprTree->root);

	RecalcTreeSize(exprTree);

#ifdef _DEBUG
	if (TreeOk(exprTree)) {
		PrintTree_OK(*exprTree);
	}
	else {
		PrintTree_NOK(*exprTree);
	}
#endif
}



int main() {
	//char expr[] = "3/sin(x)*pow(4/12,x*2+1)+5*x";
	char expr[] = "1/x*2";

	int syntaxErr = 0;
	tree_t diffTree = ExprToTree(expr, &syntaxErr);
	
	int a = ShowTree(&diffTree);

	Differentiate(&diffTree);

	a = ShowTree(&diffTree);
}