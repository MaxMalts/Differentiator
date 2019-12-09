#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "diff_tree.h"
#include "buffer.h"
#include "Functions_enum.h"
#include "DSL.h"


void DifferentiateNode(FILE* fout, node_t*& curNode);

int NodesToLatex(FILE* fout, node_t* curNode);


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

	node_t* res = GetT(syntaxErr);
	assert_syntax(*syntaxErr);

	while (*curSequence == '+' || *curSequence == '-') {
		char op = *curSequence;
		curSequence++;

		node_t* right = GetT(syntaxErr);
		assert_syntax(*syntaxErr);

		node_t* left = res;

		res = CreateNode();
		res->type = op_node;
		res->value[0] = op;
		res->value[1] = '\0';

		res->left = left;
		res->right = right;
		left->parent = res;
		right->parent = res;
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

	node_t* res = GetP(syntaxErr);
	assert_syntax(*syntaxErr);

	while (*curSequence == '*' || *curSequence == '/') {
		char op = *curSequence;
		curSequence++;

		node_t* right = GetP(syntaxErr);
		assert_syntax(*syntaxErr);

		node_t* left = res;

		res = CreateNode();
		res->type = op_node;
		res->value[0] = op;
		res->value[1] = '\0';

		res->left = left;
		res->right = right;
		left->parent = res;
		right->parent = res;
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
	//int found = 0;

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
#include "functions.h"
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


node_t* DSLDiffNode(FILE* fout, node_t* node) {
	assert(node != NULL);

	DifferentiateNode(fout, node);

	return node;
}


void DifferentiateNode(FILE* fout, node_t*& curNode) {
	assert(fout != NULL);
	assert(curNode != NULL);

	node_t* newNode = NULL;

	switch (curNode->type) {
	case num_node: {
		assert(NodeChildsCount(curNode) == 0);

		newNode = NUM(curNode->parent, 0);

		break;
	}
	case var_node: {
		assert(NodeChildsCount(curNode) == 0);

		newNode = NUM(curNode->parent, 1);

		break;
	}
	case op_node: {
		assert(NodeChildsCount(curNode) == 2);

		switch (curNode->value[0]) {
		case '+': {
			newNode = PLUS(curNode->parent, DIFF(CLONE(curNode->left)), DIFF(CLONE(curNode->right)));
			break;
		}

		case '-': {
			newNode = MINUS(curNode->parent, DIFF(CLONE(curNode->left)), DIFF(CLONE(curNode->right)));
			break;
		}

		case '*': {
			newNode = PLUS(curNode->parent, NULL, NULL);
			newNode->left = MUL(newNode, DIFF(CLONE(curNode->left)), CLONE(curNode->right));
			newNode->right = MUL(newNode, CLONE(curNode->left), DIFF(CLONE(curNode->right)));
			break;
		}

		case '/': {
			newNode = DIV(curNode->parent, NULL, NULL);
				newNode->left = MINUS(newNode, NULL, NULL);
					newNode->left->left = MUL(newNode->left, DIFF(CLONE(curNode->left)), CLONE(curNode->right));
					newNode->left->right = MUL(newNode->left, CLONE(curNode->left), DIFF(CLONE(curNode->right)));
				newNode->right = MUL(newNode, CLONE(curNode->right), CLONE(curNode->right));
			break;
		}

		default:
			assert(0);
		}
		break;
	}
	case func_node: {

#define DEF_FUNC(str, NParams, funcI, code) \
	case (funcI): {                                 \
		code                                        \
		break;                                      \
	}

		switch (curNode->value[0]) {
#include "functions.h"
		default:
			assert(0);
		}
#undef DEF_FUNC

		break;
	}
	}

	fprintf(fout, "\\begin{flalign*}\n(");
	NodesToLatex(fout, curNode);
	fprintf(fout, ")' = \n");

	UpdateParentChild(curNode, newNode);
	DeleteNodes(curNode);
	curNode = newNode;

	NodesToLatex(fout, curNode);
	fprintf(fout, "; &&\n\\end{flalign*}\n");

}


void Differentiate(FILE* fout, tree_t* exprTree) {
	assert(fout != NULL);
	assert(exprTree != NULL);

	DifferentiateNode(fout, exprTree->root);

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


int SimplifyExprNodes(node_t*& curNode) {
	assert(curNode != NULL);

	node_t* newNode = NULL;
	int simplified = 0;

	if (curNode->type == op_node) {
		if (curNode->left->type == num_node && curNode->right->type == num_node) {
			float opRes = 0;
			float leftNum = *(float*)curNode->left->value;
			float rightNum = *(float*)curNode->right->value;

			switch (curNode->value[0]) {
			case '+':
				opRes = leftNum + rightNum;
				break;
			case '-':
				opRes = leftNum - rightNum;
				break;
			case '*':
				opRes = leftNum * rightNum;
				break;
			case '/':
				opRes = leftNum / rightNum;
				break;
			default:
				assert(0);
			}

			newNode = NUM(curNode->parent, opRes);
			simplified = 1;
		}

		else {
			switch (curNode->value[0]) {
			case '+': {
				if (*(float*)curNode->left->value == 0) {
					newNode = CLONE(curNode->right);
					simplified = 1;
				}
				else if (*(float*)curNode->right->value == 0) {
					newNode = CLONE(curNode->left);
					simplified = 1;
				}
				break;
			}

			case '-': {
				if (*(float*)curNode->right->value == 0) {
					newNode = CLONE(curNode->left);
					simplified = 1;
				}
				else if (*(float*)curNode->left->value == 0) {
					newNode = MUL(curNode->parent, NULL, CLONE(curNode->right));
					newNode->left = NUM(newNode, -1);
					simplified = 1;
				}
				break;
			}

			case '*': {
				if (*(float*)curNode->left->value == 1) {
					newNode = CLONE(curNode->right);
					simplified = 1;
				}
				else if (*(float*)curNode->right->value == 1) {
					newNode = CLONE(curNode->left);
					simplified = 1;
				}
				break;
				}

			case '/': {
				if (*(float*)curNode->right->value == 1) {
					newNode = CLONE(curNode->left);
					simplified = 1;
				}
				break;
			}
			default:
				assert(0);
			}
		}
	}

	if (simplified) {
		node_t* curNodeBckp = curNode;
		newNode->parent = curNode->parent;
		UpdateParentChild(curNode, newNode);
		DeleteNodes(curNodeBckp);
		curNode = newNode;
	}

	if (curNode->left != NULL) {
		simplified |= SimplifyExprNodes(curNode->left);
	}
	if (curNode->right != NULL) {
		simplified |= SimplifyExprNodes(curNode->right);
	}

	return simplified;
}

int SimplifyExprTree(tree_t* exprTree) {
	assert(exprTree != NULL);

#ifdef _DEBUG
	if (!TreeOk(exprTree)) {
		PrintTree_NOK(*exprTree);
		return 1;
	}
#endif


	while (SimplifyExprNodes(exprTree->root)) {}

	RecalcTreeSize(exprTree);


#ifdef _DEBUG
	if (TreeOk(exprTree)) {
		PrintTree_OK(*exprTree);
	}
	else {
		PrintTree_NOK(*exprTree);
	}
#endif

	return 0;
}


void LatexStructBeg(FILE* fout) {
	assert(fout != NULL);

	fprintf(fout, "\\documentclass{article}\n\\pagestyle{empty}\n\n");
	fprintf(fout, "\\usepackage[margin=10px]{geometry}\n\\usepackage{mathtools}\n\n");
	fprintf(fout, "\\begin{document}\n");
}


void LatexStructEnd(FILE* fout) {
	assert(fout != NULL);
	fprintf(fout, "\\end{document}\n");
}


int NodesToLatex(FILE* fout, node_t* curNode) {
	assert(fout != NULL);
	assert(curNode != NULL);

	switch (curNode->type) {
	case num_node: {
		if (*(float*)curNode->value < 0) {
			fprintf(fout, "(%g)", *(float*)curNode->value);
		}
		else {
			fprintf(fout, "%g", *(float*)curNode->value);
		}
		break;
	}
	
	case var_node: {
		fprintf(fout, "%c", curNode->value[0]);
		break;
	}

	case op_node: {
		assert(NodeChildsCount(curNode) == 2);

		switch (curNode->value[0]) {
		case '+':
		case '-': {
			fprintf(fout, "(");
			NodesToLatex(fout, curNode->left);
			fprintf(fout, "%c", curNode->value[0]);
			NodesToLatex(fout, curNode->right);
			fprintf(fout, ")");
			break;
		}

		case '*': {
			NodesToLatex(fout, curNode->left);
			fprintf(fout, "\\cdot ");
			NodesToLatex(fout, curNode->right);
			break;
		}

		case '/': {
			fprintf(fout, "\\frac{");
			NodesToLatex(fout, curNode->left);
			fprintf(fout, "}{");
			NodesToLatex(fout, curNode->right);
			fprintf(fout, "}");
			break;
		}
		default:
			assert(0);
		}
		break;
	}

	case func_node: {
#define DEF_FUNC(str, NParams, funcI, diffCode, outpCode) \
		case funcI: {                                                \
			char funcS[100] = #str;                                  \
			outpCode;                                                \
			break;                                                   \
		}

		switch (curNode->value[0]) {
#include "functions.h"
		default:
			assert(0);
		}
		break;
#undef DEF_FUNC
	}
	}

	return 0;
}


int TreeToLatex(FILE* fout, tree_t* exprTree) {
	assert(fout != NULL);
	assert(exprTree != NULL);

#ifdef _DEBUG
	if (!TreeOk(exprTree)) {
		PrintTree_NOK(*exprTree);
		return 1;
	}
#endif
	
	fprintf(fout, "\\begin{flalign*}\n");

	NodesToLatex(fout, exprTree->root);

	fprintf(fout, " &&\n\\end{flalign*}\n");

	return 0;
}


void CompileLatex(const char* finName = "LatexFiles\\expression.tex", const char* outDir = "LatexFiles") {
	assert(finName != NULL);
	assert(outDir != NULL);

	char finNameInvSlash[200] = "";
	strcpy(finNameInvSlash, finName);

	char outDirInvSlash[200] = "";
	strcpy(outDirInvSlash, outDir);

	char* foundSlash = strchr(finNameInvSlash, '\\');
	while (foundSlash) {
		*foundSlash = '/';
		foundSlash = strchr(finNameInvSlash, '\\');
	}

	foundSlash = strchr(outDirInvSlash, '\\');
	while (foundSlash) {
		*foundSlash = '/';
		foundSlash = strchr(outDirInvSlash, '\\');
	}

	char sysComm[200] = "";
	sprintf(sysComm, "start /WAIT cmd /k \"pdflatex -output-directory=%s %s & exit\"", outDirInvSlash, finNameInvSlash);
	system(sysComm);
}


void OpenExprPdf(const char* fName = "LatexFiles\\expression.pdf") {
	assert(fName != NULL);

	char sysComm[200] = "";
	sprintf(sysComm, "start /WAIT cmd /k \"%s & exit\"", fName);
	system(sysComm);
}


//int ShowExpr(tree_t* exprTree) {
//	assert(exprTree != NULL);
//
//	if (TreeToLatex(exprTree) != 0) {
//		return 1;
//	}
//	
//	CompileLatex();
//	OpenExprPdf();
//
//	return 0;
//}


void PrintHelp() {
	printf("Available functions:\n");

#define DEF_FUNC(str)\
	printf("%s\n", #str);

#include "functions.h"

#undef DEF_FUNC
}


int StartDifferentiator() {
	const char foutName[] = "LatexFiles\\expression.tex";

	printf("Hello, I can differentiate almost every function.\n");

	char expr[1000] = "";
	while (1) {

		printf("\nType in your function. If you need help type \"help\". For exit type \"exit\":\n");

		scanf("%s[^\n]", expr);
		fseek(stdin, 0, SEEK_END);

		if (strcmp(expr, "help") == 0) {
			PrintHelp();
			continue;
		}
		if (strcmp(expr, "exit") == 0) {
			break;
		}

		int syntaxErr = 0;
		tree_t diffTree = ExprToTree(expr, &syntaxErr);
		if (syntaxErr) {
			printf("Syntax error. Check the expression correctness.\n");
			continue;
		}

		FILE* fout = fopen(foutName, "w");
		if (fout == NULL) {
			perror("Latex output file open error");
			return 1;
		}

		LatexStructBeg(fout);
		fprintf(fout, "You entered:\n");
		TreeToLatex(fout, &diffTree);

		SimplifyExprTree(&diffTree);
		fprintf(fout, "\\\\After simplification:\n");
		TreeToLatex(fout, &diffTree);

		fprintf(fout, "\\\\\\\\Let's differentiate:\n");
		Differentiate(fout, &diffTree);

		fprintf(fout, "\\\\\\\\The result is:\n");
		TreeToLatex(fout, &diffTree);

		SimplifyExprTree(&diffTree);
		fprintf(fout, "\\\\Final result after simplification:\n");
		TreeToLatex(fout, &diffTree);
		
		LatexStructEnd(fout);
		fclose(fout);

		CompileLatex();
		OpenExprPdf();
	}
}

int main() {
	
	return StartDifferentiator();

}