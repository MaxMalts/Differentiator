#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "diff_tree.h"
#include "buffer.h"
#include "Functions_enum.h"
#include "DSL.h"


int DifferentiateNode(node_t*& curNode);

int NodesToLatex(FILE* fout, node_t* curNode);


int Fact(int val) {
	assert(val >= 0);

	if (val == 0) {
		return 1;
	}

	return val * Fact(val - 1);
}

/**
*	Читает заданное количество символов из входного потока со спецификатором формата
*
*	@param[out] buf Буфер
*	@param[in] formSpec Спецификатор формата
*	@param[in] NChars Количество символов. Внимание, в конце может дописаться еще один '\0'!
*
*	@return 1 - ошибка; 0 - все прошло нормально
*/

int ScanNChars(char* buf, const char* formSpec, const int NChars) {
	assert(buf != NULL);
	assert(NChars >= 0);

	char format[100] = "";
	sprintf(format, "%%%s%ds", formSpec, NChars);
	int err = scanf(format, buf);
	if (err != 1 && err != 0) {
		return 1;
	}
	fseek(stdin, 0, SEEK_END);

	return 0;
}

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


//node_t* DSLDiffNode(FILE* fout, node_t* node) {
//	assert(node != NULL);
//
//	DifferentiateNode(fout, node);
//
//	return node;
//}


int DifferentiateNode(node_t*& curNode) {
	assert(curNode != NULL);

	node_t* newNode = NULL;

	if (curNode->diff) {
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
	}

	int differentiated = curNode->diff;

	if (differentiated) {
		node_t* curNodeBckp = curNode;
		UpdateParentChild(curNode, newNode);
		DeleteNodes(curNodeBckp);
		curNode = newNode;
	}
	else {
		if (curNode->left != NULL) {
			differentiated |= DifferentiateNode(curNode->left);
		}
		if (!differentiated && curNode->right != NULL) {
			differentiated |= DifferentiateNode(curNode->right);
		}
	}

	return differentiated;
}


void DifferentiateTree(tree_t* exprTree, FILE* fout = NULL) {
	assert(exprTree != NULL);

	DIFF(exprTree->root);

	if (fout != NULL) {
		fprintf(fout, "\\begin{math}\n$$ ");
		NodesToLatex(fout, exprTree->root);
	}

	while (DifferentiateNode(exprTree->root)) {
		if (fout != NULL) {
			fprintf(fout, " =\\\\\n$$ = ");
			NodesToLatex(fout, exprTree->root);
		}
	}

	if (fout != NULL) fprintf(fout, "\n\\end{math}\n");

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
				if (curNode->left->type == num_node && *(float*)curNode->left->value == 0) {
					newNode = CLONE(curNode->right);
					simplified = 1;
				}
				else if (curNode->right->type == num_node && *(float*)curNode->right->value == 0) {
					newNode = CLONE(curNode->left);
					simplified = 1;
				}
				break;
			}

			case '-': {
				if (curNode->left->type == num_node && *(float*)curNode->right->value == 0) {
					newNode = CLONE(curNode->left);
					simplified = 1;
				}
				else if (curNode->right->type == num_node && *(float*)curNode->left->value == 0) {
					newNode = MUL(curNode->parent, NULL, CLONE(curNode->right));
					newNode->left = NUM(newNode, -1);
					simplified = 1;
				}
				break;
			}

			case '*': {
				if (curNode->left->type == num_node && *(float*)curNode->left->value == 1) {
					newNode = CLONE(curNode->right);
					simplified = 1;
				}
				else if (curNode->right->type == num_node && *(float*)curNode->right->value == 1) {
					newNode = CLONE(curNode->left);
					simplified = 1;
				}
				else if (curNode->left->type == num_node && *(float*)curNode->left->value == 0) {
					newNode = NUM(curNode->parent, 0);
					simplified = 1;
				}
				else if (curNode->right->type == num_node && *(float*)curNode->right->value == 0) {
					newNode = NUM(curNode->parent, 0);
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
	else if (curNode->type == func_node) {
#define DEF_FUNC(str, NParams, funcI, diffCode, outpCode, simplifCode) \
		case funcI: {       \
			simplifCode;    \
			break;          \
		}

		switch (curNode->value[0]) {
#include "functions.h"
		default:
			assert(0);
		}

#undef DEF_FUNC
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
	fprintf(fout, "\\usepackage[margin=10px]{geometry}\n\n");
	fprintf(fout, "\\begin{document}\n");
}


void LatexStructEnd(FILE* fout) {
	assert(fout != NULL);
	fprintf(fout, "\\end{document}\n");
}


int NodesToLatex(FILE* fout, node_t* curNode) {
	assert(fout != NULL);
	assert(curNode != NULL);

	if (curNode->diff) {
		fprintf(fout, "(");
	}

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
			NodesToLatex(fout, curNode->left);
			fprintf(fout, "%c", curNode->value[0]);
			NodesToLatex(fout, curNode->right);
			break;
		}

		case '*': {
			if (curNode->left->type == op_node && (curNode->left->value[0] == '+' || curNode->left->value[0] == '-')) {
				fprintf(fout, "(");
				NodesToLatex(fout, curNode->left);
				fprintf(fout, ")");
			}
			else {
				NodesToLatex(fout, curNode->left);
			}

			fprintf(fout, "\\cdot ");

			if (curNode->right->type == op_node && (curNode->right->value[0] == '+' || curNode->right->value[0] == '-')) {
				fprintf(fout, "(");
				NodesToLatex(fout, curNode->right);
				fprintf(fout, ")");
			}
			else {
				NodesToLatex(fout, curNode->right);
			}
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

	if (curNode->diff) {
		fprintf(fout, ")'");
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
	
	fprintf(fout, "\\begin{math}\n$$ ");

	NodesToLatex(fout, exprTree->root);

	fprintf(fout, "\n\\end{math}\n");

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


void PrintHelp() {
	printf("\nAvailable functions:\n");

#define DEF_FUNC(str)\
	printf("%s\n", #str);

#include "functions.h"

#undef DEF_FUNC
}


void ReplaceVars(node_t* curNode, float point) {
	assert(curNode != NULL);

	if (curNode->type == var_node) {
		curNode->type = num_node;
		memcpy(curNode->value, &point, sizeof(float));
		return;
	}
	
	if (curNode->left != NULL) {
		ReplaceVars(curNode->left, point);
	}
	if (curNode->right != NULL) {
		ReplaceVars(curNode->right, point);
	}
}


int CalcExprValueRec(node_t*& curNode) {
	assert(curNode != NULL);

	node_t* newNode = NULL;
	int calced = 0;

	switch (curNode->type) {
	case op_node: {
		if (curNode->left->type == num_node && curNode->right->type == num_node) {
			float val1 = *(float*)curNode->left->value;
			float val2 = *(float*)curNode->right->value;

			switch (curNode->value[0]) {
			case '+':
				newNode = NUM(curNode->parent, val1 + val2);
				calced = 1;
				break;
			case '-':
				newNode = NUM(curNode->parent, val1 - val2);
				calced = 1;
				break;
			case '*':
				newNode = NUM(curNode->parent, val1 * val2);
				calced = 1;
				break;
			case '/':
				newNode = NUM(curNode->parent, val1 / val2);
				calced = 1;
				break;
			default:
				assert(0);
			}
		}
		break;
	}

	case func_node: {

#define DEF_FUNC(str, NParams, funcI, diffCode, outpCode, simplCode, calcCode)\
	case funcI: {                        \
		calcCode;                        \
		break;                           \
	}

		switch (curNode->value[0]) {
#include "functions.h"
		}
#undef DEF_FUNC

		break;
	}
	}

	if (calced) {
		node_t* curNodeBckp = curNode;
		newNode->parent = curNode->parent;
		UpdateParentChild(curNode, newNode);
		DeleteNodes(curNodeBckp);
		curNode = newNode;
	}

	if (curNode->left != NULL) {
		calced |= CalcExprValueRec(curNode->left);
	}
	if (curNode->right != NULL) {
		calced |= CalcExprValueRec(curNode->right);
	}

	return calced;
}

float CalcExprValue(tree_t* exprTree) {
	assert(exprTree != NULL);

#ifdef _DEBUG
	if (!TreeOk(exprTree)) {
		PrintTree_NOK(*exprTree);
		return 0;
	}
#endif

	while (CalcExprValueRec(exprTree->root)) {}

	RecalcTreeSize(exprTree);
	assert(exprTree->size == 1);

	float res = *(float*)&exprTree->root->value;
	return res;
}

float CalcValInPoint(tree_t* exprTree, float point) {
	assert(exprTree != NULL);

#ifdef _DEBUG
	if (!TreeOk(exprTree)) {
		PrintTree_NOK(*exprTree);
		return 0;
	}
#endif

	ReplaceVars(exprTree->root, point);

	float res = CalcExprValue(exprTree);

	return res;
}

tree_t Taylor(tree_t* exprTree, float point, int accur) {
	assert(exprTree != NULL);
	assert(accur > 0);
	
#ifdef _DEBUG
	if (!TreeOk(exprTree)) {
		PrintTree_NOK(*exprTree);
		return {};
	}
#endif

	tree_t calcDiffTree = TreeConstructor("calcDiffTree");
	free(calcDiffTree.root);
	tree_t taylorTree = TreeConstructor("taylorTree");
	free(taylorTree.root);

	tree_t curDiffTree = TreeConstructor("curDiffTree");
	free(curDiffTree.root);
	curDiffTree.root = CloneNodes(exprTree->root);
	curDiffTree.size = exprTree->size;

	calcDiffTree.root = CloneNodes(curDiffTree.root);
	calcDiffTree.size = curDiffTree.size;
	CalcValInPoint(&calcDiffTree, point);

	float curVal = *(float*)calcDiffTree.root->value;
	free(calcDiffTree.root);

	taylorTree.root = NUM(NULL, curVal);

	for (int i = 1; i <= accur; i++) {
		DifferentiateTree(&curDiffTree);
		calcDiffTree.root = CloneNodes(curDiffTree.root);
		calcDiffTree.size = curDiffTree.size;
		CalcValInPoint(&calcDiffTree, point);

		float curVal = *(float*)calcDiffTree.root->value;
		free(calcDiffTree.root);

		curVal = curVal / Fact(i);

		taylorTree.root = PLUS(NULL, taylorTree.root, NULL);
			taylorTree.root->right = MUL(taylorTree.root, NULL, NULL);
				taylorTree.root->right->left = NUM(taylorTree.root->right, curVal);
				taylorTree.root->right->right = POW(taylorTree.root->right, NULL, NULL);
					taylorTree.root->right->right->left = MINUS(taylorTree.root->right->right, NULL, NULL);
						taylorTree.root->right->right->left->left = VAR(taylorTree.root->right->right->left, 'x');
						taylorTree.root->right->right->left->right = NUM(taylorTree.root->right->right->left, point);
					taylorTree.root->right->right->right = NUM(taylorTree.root->right->right, (float)i);

	}
	RecalcTreeSize(&taylorTree);
	SimplifyExprTree(&taylorTree);

	DeleteTree(&curDiffTree);

#ifdef _DEBUG
	if (TreeOk(&taylorTree)) {
		PrintTree_OK(taylorTree);
	}
	else {
		PrintTree_NOK(taylorTree);
	}
#endif

	return taylorTree;
}

enum mode_t {
	undef_mode,
	diff_mode,
	diffp_mode,
	taylor_mode
};

mode_t DetMode(char* buf) {
	assert(buf != NULL);

	if (strcmp(buf, "diff") == 0) {
		return diff_mode;
	}
	if (strcmp(buf, "diffp") == 0) {
		return diffp_mode;
	}
	if (strcmp(buf, "taylor") == 0) {
		return taylor_mode;
	}
	return undef_mode;
}

int StartDifferentiator() {
	const char foutName[] = "LatexFiles\\expression.tex";

	printf("Hello, I can differentiate or calculate in a point almost every function.\n");

	char expr[1000] = "";
	while (1) {

		printf("\nWhat do you want to do?\nType \"diff\" to differentiate function;\n"
			   "\"diffp\" to calculate the differentiated function in specified point;\n"
			   "\"taylor\" to get the taylor formula in specified point\n");
		char modeS[100] = "";
		ScanNChars(modeS, "[^\n]", sizeof(modeS) - 1);
		mode_t mode = DetMode(modeS);
		while (mode == undef_mode) {
			printf("Wrong mode. Retry:\n", sizeof(expr) - 1);
			ScanNChars(modeS, "[^\n]", sizeof(modeS) - 1);
			mode = DetMode(modeS);
		}

		printf("\nType in your function. If you need help type \"help\":\n");
		ScanNChars(expr, "[^\n]", sizeof(expr) - 1);
		while (expr[sizeof(expr) - 2] != '\0') {
			printf("Function too long (maximum function size is %d characters. "
				   "Type in another function:\n", sizeof(expr) - 1);
			memset(expr, 0, sizeof(expr));
			ScanNChars(expr, "[^\n]", sizeof(expr) - 1);
		}

		if (strcmp(expr, "help") == 0) {
			PrintHelp();
			continue;
		}

		int syntaxErr = 0;
		tree_t diffTree = ExprToTree(expr, &syntaxErr);
		if (syntaxErr) {
			printf("Syntax error. Check the expression correctness.\n\n");
			continue;
		}

		FILE* fout = fopen(foutName, "w");
		if (fout == NULL) {
			perror("Latex output file open error");
			getchar();
			return 1;
		}

		switch (mode) {
		case diff_mode:
		case diffp_mode: {
			printf("How many times to you want to differentiate?:\n");
			int diffTimes = 0;
			scanf("%d%*c", &diffTimes);

			LatexStructBeg(fout);
			fprintf(fout, "You entered:\\\\\n");
			TreeToLatex(fout, &diffTree);

			SimplifyExprTree(&diffTree);
			fprintf(fout, "\\\\\\\\After simplification:\\\\\n");
			TreeToLatex(fout, &diffTree);

			fprintf(fout, "\\\\\\\\\\\\Let's differentiate:\n");
			for (int i = 0; i < diffTimes; i++) {
				fprintf(fout, "\\\\\\\\Derivative %d:\\\\\n", i + 1);
				DifferentiateTree(&diffTree, fout);

				SimplifyExprTree(&diffTree);
				fprintf(fout, "\\\\After simplification:\\\\\n");
				TreeToLatex(fout, &diffTree);
			}

			fprintf(fout, "\\\\\\\\\\\\The result is:\\\\\n");
			TreeToLatex(fout, &diffTree);

			if (mode == diffp_mode) {
				printf("In which pont do you want to calculate?\n");
				float point = 0;
				scanf("%f%*c", &point);

				fprintf(fout, "\\\\\\\\In point %g the function is:\\\\\n", point);
				CalcValInPoint(&diffTree, point);
				TreeToLatex(fout, &diffTree);
			}

			LatexStructEnd(fout);
			fclose(fout);

			break;
		}

		case taylor_mode: {
			printf("Till which accuracy do you wand to get the formula?:\n");
			int accur = 0;
			scanf("%d%*c", &accur);

			tree_t backupTree = TreeConstructor("backupTree");
			free(backupTree.root);
			backupTree.root = CloneNodes(diffTree.root);
			backupTree.size = diffTree.size;

			LatexStructBeg(fout);
			fprintf(fout, "You entered:\\\\\n");
			TreeToLatex(fout, &diffTree);

			SimplifyExprTree(&diffTree);
			fprintf(fout, "\\\\\\\\After simplification:\\\\\n");
			TreeToLatex(fout, &diffTree);

			fprintf(fout, "\\\\\\\\\\\\Let's differentiate:\n");
			for (int i = 0; i < accur; i++) {
				fprintf(fout, "\\\\\\\\Derivative %d:\\\\\n", i + 1);
				DifferentiateTree(&diffTree, fout);

				SimplifyExprTree(&diffTree);
				fprintf(fout, "\\\\After simplification:\\\\\n");
				TreeToLatex(fout, &diffTree);
			}

			printf("In which point do you want to get the formula?\n");
			float point = 0;
			scanf("%f%*c", &point);

			fprintf(fout, "\\\\\\\\In point %g the Taylor formula is:\\\\\n", point);
			tree_t taylorTree = Taylor(&backupTree, point, accur);
			fprintf(fout, "\\begin{math}\n");
			NodesToLatex(fout, taylorTree.root);
			fprintf(fout, " + o(x^{%d})\n\\end{math}", accur);

			LatexStructEnd(fout);
			fclose(fout);

			break;
		}
		}

		CompileLatex();
		OpenExprPdf();
	}
}

int main() {
	
	return StartDifferentiator();

}