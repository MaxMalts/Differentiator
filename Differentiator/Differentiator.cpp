#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "diff_tree.h"
#include "buffer.h"
#include "Functions_enum.h"
#include "DSL.h"


void DifferentiateNode(node_t*& curNode);


/**
*	Знак числа
*
*	@param[in] var
*
*	@return -1 - минус; 1 - плюс
*/

int Sign(float var) {
	if (var < 0) {
		return -1;
	}

	return 1;
}


/**
*	Создает число foat по его целой и дробной частям
*
*	@param[in] intPart Целая часть (может быть отрицательная)
*	@param[in] fracPart Дробная часть (всегда положительная)
*
*	@return Число float
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
*	Правило для получения вырвжения (G::=E'\0')
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Значение выражения
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
*	Правило для получения суммы, разности (E::=T{[+-]T}*)
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Значение выражения
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
*	Правило для получения умножения, деления (T::=P{[/*]P}*)
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Значение выражения
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
*	Правило для получения выражения в скобках, функии или числа(P::='('E')'|Func|F).\
 Чтобы определить, что именно получать, смотрит первый символ.
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Значение выражения
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
*	Правило для получения функции ( Func::="sin"'('E')'|"cos"'('E')'...|"pow"'('E','E')' )
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Значение функции
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
*	Правило для получения числа float ( F::=I{'.'N}? )
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Число float
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
*	Правило для получения целого числа ( I::=[+-]?N )
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Целое число
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
*	Правило для получения целого положительного числа ( N::=['0'-'9']+ )
*
*	@param[out] syntaxErr Возникла ли синтаксическая ошбика (1 или 0)
*
*	@return Целое положительное число
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
*	Создает дерево по выражению
*
*	@param[in] expr Выражение
*
*	@return Дерево
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


node_t* DSLDiffNode(node_t* node) {
	assert(node != NULL);

	DifferentiateNode(node);

	return node;
}


void DifferentiateNode(node_t*& curNode) {
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

	UpdateParentChild(curNode, newNode);
	DeleteNodes(curNode);
	curNode = newNode;
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

int TreeToLatex(tree_t* exprTree, const char* foutName = "LatexFiles\\expression.tex") {
	assert(exprTree != NULL);

#ifdef _DEBUG
	if (!TreeOk(exprTree)) {
		PrintTree_NOK(*exprTree);
		return 1;
	}
#endif

	FILE* fout = fopen(foutName, "w");
	if (fout == NULL) {
		perror("Latex output file open error");
		return 1;
	}
	
	fprintf(fout, "\\documentclass{article}\n\\pagestyle{empty}\n\n");
	fprintf(fout, "\\usepackage{mathtools}\n\n");
	fprintf(fout, "\\begin{document}\n\\begin{equation*}\n");

	NodesToLatex(fout, exprTree->root);

	fprintf(fout, "\n\\end{equation*}\n\\end{document}");
	
	fclose(fout);
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
	sprintf(sysComm, "pdflatex -output-directory=%s %s", outDirInvSlash, finNameInvSlash);
	system(sysComm);
}

void OpenExprPdf(const char* fName = "LatexFiles\\expression.pdf") {
	assert(fName != NULL);

	system(fName);
}

int ShowExpr(tree_t* exprTree) {
	assert(exprTree != NULL);

	if (TreeToLatex(exprTree) != 0) {
		return 1;
	}
	
	CompileLatex();
	OpenExprPdf();

	return 0;
}

int main() {
	//char expr[] = "3/sin(x)*pow(4/12,x*2+1)+5*x";
	char expr[] = "pow(x,x*3*sin(1/x+3*ln(log(3*pow(x,2),abs(x)))))";
	//char expr[] = "6+7";

	int syntaxErr = 0;
	tree_t diffTree = ExprToTree(expr, &syntaxErr);
	int a = ShowTree(&diffTree);

	ShowExpr(&diffTree);

	/*SimplifyExprTree(&diffTree);
	int a = ShowTree(&diffTree);

	Differentiate(&diffTree);
	a = ShowTree(&diffTree);

	SimplifyExprTree(&diffTree);
	a = ShowTree(&diffTree);*/

	return 0;
}