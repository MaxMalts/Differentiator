#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "diff_tree.h"
#include "buffer.h"



/*  �� ��� ������������
*	���������, ������������� �� �������� ����� �� ������ ����
*
*	@param[in] side �������
*
*	@return 1 (true) - �������������; 0 (false) - �� �������������
*/

int SideIsCorrect(const int side) {
	if (side == LEFT_CHILD || side == RIGHT_CHILD) {
		return 1;
	}
	return 0;
}


/**
*	������� ���������� �������
*
*	@param[in] node ����
*
*	@return ���������� ������� (0, 1, 2)
*/

int NodeChildsCount(node_t* node) {
	assert(node != NULL);

	if (node->left != NULL && node->right != NULL) {
		return 2;
	}
	else if (node->left != NULL || node->right != NULL) {
		return 1;
	}
	return 0;
}


/**
*	�������� ������� ���� value_t
*
*	@param[out] dest ���� ��������
*	@param[in] source ������ ��������
*
*	@return 0 - ��� ������ ���������
*/

int CopyValue_t(value_t dest, value_t source) {
	assert(dest != NULL);
	assert(source != NULL);

	for (int i = 0; i < treeStrMaxSize; i++) {
		dest[i] = source[i];
	}

	return 0;
}


/**
*	����������� value_t � ������
*
*	@param[in] value ������� ���� value_t
*
*	@return ��������� �� ������. �� �������� ���������� ������ �� ����� ���������!
*/

char* Value_tToStr(const value_t value) {
	assert(value != NULL);

	const int value_tMaxStrSize = treeStrMaxSize;

	char* str = (char*)calloc(value_tMaxStrSize, sizeof(char));
	strcpy(str, value);

	return str;
}


/**
*	���������� �������� value_t
*
*	@param[in] value1 ������ ��������
*	@param[in] value2 ������ ��������
*
*	@return < 0 - ������ �������� ������ �������; 0 - �������� �����; > 0 - ������ �������� ������ �������
*/

int ValueCmp(value_t* value1, value_t* value2) {
	assert(value1 != NULL);
	assert(value2 != NULL);

	return strcmp(*value1, *value2);
}

/**
*	����������� ������ � value_t
*
*	@param[in] valueS ������
*	@param[out] value �������� value_t
*
*	@return 1 - � ������ �������� ������������� value_t; 0 - ��� ������ ���������
*/

int StrToValue_t(const char* valueS, value_t* value) {
	assert(valueS != NULL);
	assert(value != NULL);
	
	strcpy(*value, valueS);

	return 0;
}


/*  �� ��� ������������
*	��������� ���� ���� ��������� � ������� �� ����������
*
*	@param[in] node ����
*	@param[out] NNodes ���������� �����. �� ����� �������� ������ ���� 0!
*
*	@return 1 - ������ ����; 0 - ������ ���
*/

#ifdef _DEBUG
int NodesOk(node_t* node, int* NNodes) {
	assert(node != NULL);
	assert(NNodes != NULL);
	
	(*NNodes)++;
	int err = 0;
	if (node->left != NULL) {
		if (node->left->parent != node) {
			return 1;
		}
		err = NodesOk(node->left, NNodes);
		if (err != 0) {
			return err;
		}
	}
	if (node->right != NULL) {
		if (node->right->parent != node) {
			return 1;
		}
		err = NodesOk(node->right, NNodes);
		if (err != 0) {
			return err;
		}
	}
	
	return 0;
}
#endif


/**
*	��������� ������ � ���������� � ���� ��� ������
*
*	@param[in] tree ������
*
*	@return 0 - ������ ������������; 1 - ������ ����������
*/

#ifdef _DEBUG
int TreeOk(tree_t* tree) {
	if (tree == NULL) {
		return 0;
	}
	if (tree->size < 0) {
		tree->err = 1;
		return 0;
	}
	if (tree->root == NULL) {
		tree->err = 2;
		return 0;
	}
	int actSize = 0;
	if (NodesOk(tree->root, &actSize) != 0) {
		tree->err = 3;
		return 0;
	}
	if (actSize != tree->size) {
		tree->err = 4;
		return 0;
	}

	return 1;
}
#endif


/*  �� ��� ������������
*	������� ���������� � ������. ��� ������������ ������, ����������� ShowTree().
*
*	@param[in] tree ������
*	@param[in] file �������� �����, ����� ������� �������
*	@param[in] line ����� ������, �� ������� ������� �������
*	@param[in] function ��� �������, �� ������� ������� �������
*	@param[in] reason �������, �� ������� ������� �������
*/

#ifdef _DEBUG
void TreeDump(tree_t* tree, const char* file, const int line, const char* function, const char* reason) {
	assert(tree != NULL);
	assert(file != NULL);
	assert(line > 0);
	assert(function != NULL);
	assert(reason != NULL);

	char status[10] = "ok";
	if (tree->err != 0) {
		strcpy(status, "ERR");
	}

	printf("\nInfo about a tree from file: %s, function: %s, line: %d, reason: %s:\n", file, function, line, reason);
	printf("tree_t \"%s\" (%p):    (%s)\n", tree->name, tree, status);

	printf("\tsize = %d\n", tree->size);

	char* valueS = Value_tToStr(tree->root->value);
	printf("\troot (%p):\n", tree->root);
	printf("\t\tvalue: %s;\n", valueS);
	printf("\t\ttype: %d;\n", tree->root->type);
	printf("\t\tleft: %p;\n", tree->root->left);
	printf("\t\tright: %p;\n", tree->root->right);
	printf("\t\tparent: %p;\n", tree->root->parent);
	free(valueS);

	printf("err = %d\n\n\n", tree->err);
}
#endif


/*  �� ��� ������������
*	���������� ���������� ���������� ��� ���� ����� ��� ��������� Graphviz.\
 ����������, ���������� ���� �������� �� �������� ����������� ����������� ������.
*
*	@param[in] gvFile ���� ��� ������
*	@param[in] node �������� ����
*
*	@return 0 - ��� ������ ���������
*/

int NodesOutput(FILE* gvFile, node_t* node) {
	assert(gvFile != NULL);
	assert(node != NULL);

	char valueS[100] = "";
	if (node->type == num_node) {
		float num = 0;
		memcpy((char*)&num, node->value, sizeof(float));
		sprintf(valueS, "%f", num);
	}
	else if (node->type == func_node) {
		itoa(node->value[0], valueS, 10);
	}
	else {
		strcpy(valueS, Value_tToStr(node->value));
	}

#ifdef _DEBUG
	fprintf(gvFile, "\t%d [label=\"{%p|%p|%d|%s|{%p|%p}}\"]\n",
		    (int)node, node, node->parent, node->type, valueS, node->left, node->right);
#else
	fprintf(gvFile, "\t%d [label=\"{%d|%s}\"]", (int)node, node->type valueS);
#endif
	//free(valueS);

	if (node->left != NULL) {
		NodesOutput(gvFile, node->left);
		fprintf(gvFile, "\t%d -> %d\n", (int)node, (int)node->left);
	}
	if (node->right != NULL) {
		NodesOutput(gvFile, node->right);
		fprintf(gvFile, "\t%d -> %d\n", (int)node, (int)node->right);
	}

	return 0;
}


/*  �� ��� ������������
*	����������� ������� ��� �������� ������� ����� "���������" �����
*
*	@param[in] curNode ������� ����
*	@param[out] words ������ ����
*	@param[out] NNodes ���������� ����
*
*	@return 1 - �������� ��� ����������� words; 0 - ��� ������ ���������
*/

int LastNodesValuesRec(node_t* curNode, char**& words, int* NNodes) {
	assert(curNode != NULL);
	assert(words != NULL);
	assert(NNodes != NULL);

	if (NodeChildsCount(curNode) == 0) {
		(*NNodes)++;
		words = (char**)realloc(words, *NNodes * sizeof(char*));
		if (words == NULL) {
			return 1;
		}

		words[*NNodes - 1] = (char*)calloc(treeStrMaxSize, sizeof(char));
		if (words[*NNodes - 1] == NULL) {
			return 1;
		}
		strncpy(words[*NNodes - 1], curNode->value, treeStrMaxSize - 1);
	}
	else {
		if (curNode->left != NULL) {
			int err = LastNodesValuesRec(curNode->left, words, NNodes);
			if (err != 0) {
				return err;
			}
		}
		if (curNode->right != NULL) {
			int err = LastNodesValuesRec(curNode->right, words, NNodes);
			if (err != 0) {
				return err;
			}
		}
	}

	return 0;
}


/**
*	������� ������ ����� �� ���������� "���������" �����
*
*	@param[in] tree ������
*	@param[out] words ������ ����� (�� ����� ������ ���� NULL)
*	@param[out] NNodes ���������� �����
*
*	@return 1 - �� ���� �������� ������������ ������; 2 - words != NULL;\
 3 - �������� ��� ����������� words; 4 - ������ ������; 0 - ��� ������ ���������
*/

int LastNodesWords(tree_t* tree, char**& words, int* NNodes) {
	assert(tree != NULL);
	assert(NNodes != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
		return 1;
	}
#endif

	if (words != NULL) {
		return 2;
	}

	*NNodes = 0;
	words = (char**)calloc(1, sizeof(char*));
	int err = LastNodesValuesRec(tree->root, words, NNodes);

	if (err != 0) {
		return 3;
	}
	if (*NNodes > tree->size) {
		return 4;
	}
	return 0;
}

/**
*	���������� ������ ����� � ������ ������� '\r'
*
*	@param[in] f ����
*
*	@return ������ �����. ���� ������ �������������, �� ��������� ������
*/

int GetFileSizeTr(FILE* f) {
	assert(f != NULL);
	if (fseek(f, 0, SEEK_END) != 0) {
		return -1;
	}

	int fileSize = ftell(f);

	if (fseek(f, 0, SEEK_SET) != 0) {
		return -1;
	}

	return fileSize;
}


/**
*	������� ������ UTF-8 �� ����� (������������ �� UTF-8 BOM � UTF-8)
*
*	@param[in] fName ��� �����
*
*	@return 1 - �� ������� ������� ���� �� ������; 2 - �� ������� ���������� ������ �����;\
 3 - �� ������� ��������� ����; 4 - �� ������� ������� ���� �� ������;\
 5 - �� ������� �������� � ����; 0 - ��� ������ ���������
*/

int DeleteUtfMarker(const char* fName) {
	assert(fName != NULL);

	FILE* f = fopen(fName, "rb");
	if (f == NULL) {
		return 1;
	}

	int fSize = GetFileSizeTr(f);
	if (fSize < 0) {
		return 2;
	}

	char* buf = (char*)calloc(fSize + 1, sizeof(char));
	if (fread(buf, sizeof(char), fSize, f) != fSize) {
		return 3;
	}
	fclose(f);

	f = fopen(fName, "wb");
	if (f == NULL) {
		return 4;
	}
	if (fwrite(&buf[3], sizeof(char), fSize - 3, f) != fSize - 3) {
		return 5;
	}
	fclose(f);

	return 0;
}


/**
*	������ ������ � ������� ��������� Graphviz.
*
*	@param[in] tree ������
*	@param[in] foutName ��� ����� � �������� (�� ��������� - "tree.png")
*	@param[in] gvFileName ��� ����� � ��������� ����� ��� Graphviz (�� ��������� - "tree.gv")
*
*	@return 1 - �� ������� ������� ���� ��� ������; 2 - �� ���� �������� ������\
� ������� (������ � ������ �������); 0 - ��� ������ ���������
*/

int CreateTreeImage(tree_t* tree, const char foutName[], const char gvFileName[]) {
	assert(tree != NULL);
	assert(gvFileName != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
		return 2;
	}
#endif

	char* tempGvFileName = (char*)calloc(strlen(gvFileName) + 7, sizeof(char));
	sprintf(tempGvFileName, "%s.temp", gvFileName);
	FILE* tempGvFile = fopen(tempGvFileName, "w");
	if (NULL == tempGvFile) {
		return 1;
	}
	
#ifdef _DEBUG
	fprintf(tempGvFile, "digraph %s {\n", tree->name);
	fprintf(tempGvFile, "\tnode [shape=record]\n\n");
	fprintf(tempGvFile, "\tformat_node [label=\"{adress|parent|type|value|{left|right}}\"]\n\n");
#else
	fprintf(tempGvFile, "digraph {\n");
	fprintf(tempGvFile, "\tnode [shape=record]\n\n");
	fprintf(tempGvFile, "\tformat_node [label=\"{type|value}\"]\n\n");
#endif

	NodesOutput(tempGvFile, tree->root);

	fprintf(tempGvFile, "}");
	fclose(tempGvFile);

	char sysCommand[1000] = "";
	sprintf(sysCommand, "start /wait powershell -command \"get-content -path %s | \
out-file %s -encoding utf8\"", tempGvFileName, gvFileName);
	system(sysCommand);

	DeleteUtfMarker(gvFileName);
	
	sprintf(sysCommand, "del %s", tempGvFileName);
	system(sysCommand);
	free(tempGvFileName);

	sprintf(sysCommand, "start /wait dot -Tpng %s -o %s", gvFileName, foutName);
	system(sysCommand);

	return 0;
}


/**
*	������� ����������� ������ � ��������� ���. ������� ���������, �� ����������� �����!
*
*	@param[in] tree ������
*
*	@return 1 - �������� ��� �������� �����������; 2 - �������� ��� �������� �����������;\
 3 - �� ���� �������� ������ � ������� (������ � ������ �������); 0 - ��� ������ ���������
*/

int ShowTree(tree_t* tree) {
	assert(tree != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
		return 3;
	}
#endif

	//����� �� ������������ Sleep() �� windows.h.
	//���� ����� �� �������, �� ���������� ����������� ����������� ��� �����.
	int a = 0;
	for (int i = 0; i < 1000000; i++) {
		a++;
		if (a % 100 == 0) {
			i -= 99;
		}
	}

	if (CreateTreeImage(tree) == 1) {
		return 1;
	}
	if (system("tree.png") != 0) {
		return 2;
	}
	return 0;
}


/**
*	������� ����� ������. ����� ������, �������� �������� �����\
 �� ������ � ������� ������� ChangeNodeValue()!
 *
 *	@param[in] name ��� ������
 *
 *	@return ��������� �� ��������� ������
*/

tree_t TreeConstructor(const char* name) {
	assert(name != NULL);

	tree_t tree = {};
	
#ifdef _DEBUG
	strcpy(tree.name, name);
	tree.err = 0;
#endif
	
	tree.root = (node_t*)calloc(1, sizeof(node_t));
	tree.size = 1;

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


/*  �� ��� ������������
*	������� ���������� �����-�������� � ������ �������� ���� (����������)
*
*	@param[in] node ����
*	@param[out] size ����������
*
*	@return 0 - ��� ������ ���������
*/

int CalcNodesCount(node_t* node, int* size) {
	assert(node != NULL);
	assert(size != NULL);

	if (node->left != NULL) {
		int err = CalcNodesCount(node->left, size);
		if (err != 0) {
			return err;
		}
	}
	if (node->right != NULL) {
		int err = CalcNodesCount(node->right, size);
		if (err != 0) {
			return err;
		}
	}
	(*size)++;
	return 0;
}


/*  �� ��� ������������
*	������������� ������ ������
*
*	@param tree ������
*
*	@return ��. ������������ �������� CalcNodesCount
*/

int RecalcTreeSize(tree_t* tree) {
	assert(tree != NULL);
	assert(tree->root != NULL);

	int newSize = 0;
	CalcNodesCount(tree->root, &newSize);
	tree->size = newSize;

	return newSize;
}


/*  �� ��� ������������
*	������� ����� ������ � �������� ������
*
*	@param[in] root ������
*	@param[in] name ��� ������
*
*	@return ��������� ������
*/

tree_t TreeRootConstructor(node_t* root, const char* name) {
	assert(root != NULL);
	assert(root->parent == NULL);
	assert(name != NULL);

	tree_t tree = {};

#ifdef _DEBUG
	strcpy(tree.name, name);
	tree.err = 0;
#endif

	tree.root = root;
	RecalcTreeSize(&tree);

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


/**
*	�������������� �������� ����
*
*	@param node ����
*	@param[in] value ��������
*
*	@return 0 - ��� ������ ���������
*/

int ChangeNodeValue(node_t* node, value_t value) {
	assert(node != NULL);

	CopyValue_t(node->value, value);

	return 0;
}


/**
*	������ �������� ����
*
*	@param[in] node ����
*	@param[out] value ��������
*
*	@return 0 - ��� ������ ���������
*/

int GetNodeValue(node_t* node, value_t* value) {
	assert(node != NULL);
	assert(value != NULL);

	strcpy(*value, node->value);

	return 0;
}


/*  �� ��� ������������
*	������� ����� ����
*
*	@return ��������� �� ����� ����. �� �������� ���������� ������ �� ����� ���������!
*/

node_t* CreateNode() {

	node_t* node = (node_t*)calloc(1, sizeof(node_t));
	node->type = {};
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;

	return node;
}


/*  �� ��� ������������
*	������� ����� ���� � ������������ ������, ���� left ��� right != NULL,\
 �� ������ �� ���� parent �� ���������������
*
*	@return ��������� �� ����� ����. �� �������� ���������� ������ �� ����� ���������!
*/

node_t* CreateNodeProp(node_t* parent, node_type type, value_t value, node_t* left, node_t* right) {
	node_t* node = CreateNode();

	node->parent = parent;
	CopyValue_t(node->value, value);
	node->type = type;
	node->left = left;
	node->right = right;

	if (left != NULL) {
		left->parent = node;
	}
	if (right != NULL) {
		right->parent = node;
	}

	return node;
}


/**
*	��������� ����
*
*	@param[in] srcNode �������� ����
*
*	@return ��������� �� ����� ����. �� �������� ���������� ������ �� ����� ���������!
*/

node_t* CloneNode(node_t* srcNode) {
	assert(srcNode != NULL);

	node_t* dstNode = CreateNodeProp(srcNode->parent, srcNode->type, srcNode->value, srcNode->left, srcNode->right);
	if (srcNode->left != NULL) {
		srcNode->left->parent = srcNode;
	}
	if (srcNode->right != NULL) {
		srcNode->right->parent = srcNode;
	}

	return dstNode;
}


/**
*	��������� ���� � ��� ��� �������� ����
*
*	@param[in] srcNode �������� ����
*
*	@return ��������� �� ����� ���� (���� parnt ��������� ����� ��, ��� � ���������).\
 �� �������� ���������� ������ �� ����� ���������!
*/

node_t* CloneNodes(node_t* srcNode) {
	assert(srcNode != NULL);

	node_t* dstNode = CloneNode(srcNode);
	
	if (srcNode->left != NULL) {
		dstNode->left = CloneNodes(srcNode->left);
		dstNode->left->parent = dstNode;
	}
	if (srcNode->right != NULL) {
		dstNode->right = CloneNodes(srcNode->right);
		dstNode->right->parent = dstNode;
	}

	return dstNode;
}


/**
*	��������� ���� � ��������������� �������
*
*	@param[in] oldChild ������ ���, �������� �������� ����� ��������
*	@param[in] child ����� ���
*
*	@return 1 - ������; 0 - ��� ������ ���������
*/

int UpdateParentChild(node_t* oldChild, node_t* newChild) {
	assert(oldChild != NULL);

	if (oldChild->parent != NULL) {
		if (oldChild->parent->left == oldChild) {
			oldChild->parent->left = newChild;
		}
		else if (oldChild->parent->right == oldChild) {
			oldChild->parent->right = newChild;
		}
		else {
			return 1;
		}
	}
	else {
		return 1;
	}

	return 0;
}

/**
*	��������� ���� � ��������� ������� �� ��������
*
*	@param tree ������, � ������� ��������� ����
*	@param node ����, � �������� ���������
*	@param[in] value �������� ������ ����
*	@param[in] side �������, � ������� ��������� (LEFT_CHILD, RIGHT_CHILD)
*	@param[out] newNode ����� ������ ����
*
*	@return 1 - � ���� ��� ��� �������� ���� � ��������������� �������;\
 2 - �������� side ���� ������������ �������; 3 - �� ���� �������� ������ �\
  ������� (������ � ������ �������); 0 - ��� ������ ���������
*/

int AddChild(tree_t* tree, node_t* node, value_t value, const int side, node_t** createdNode) {
	assert(tree != NULL);
	assert(node != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
		return 3;
	}
#endif

	if (!SideIsCorrect(side)) {
		return 2;
	}

	node_t* newNode = CreateNode();
	strcpy(newNode->value, value);
	newNode->parent = node;

	switch (side) {
	case LEFT_CHILD:
		if (node->left != NULL) {
			return 1;
		}
		node->left = newNode;
		break;
	case RIGHT_CHILD:
		if (node->right != NULL) {
			return 1;
		}
		node->right = newNode;
		break;
	}

	tree->size++;

	if (createdNode != NULL) {
		*createdNode = newNode;
	}

#ifdef _DEBUG
	if (TreeOk(tree)) {
		PrintTree_OK(*tree);
	}
	else {
		PrintTree_NOK(*tree);
	}
#endif

	return 0;
}


///**
//*	��������� �������� ���� � ��������� ������� �� ������
//*
//*	@param tree ������, � ������� ��������� ����
//*	@param parentNode ����, � �������� ���������
//*	@param[in] childNode �������� ����
//*	@param[in] side �������, � ������� ��������� (LEFT_CHILD, RIGHT_CHILD)
//*
//*	@return 1 - � ���� ��� ��� �������� ���� � ��������������� �������;\
// 2 - �������� side ���� ������������ �������; 0 - ��� ������ ���������
//*/
//
//int AddNode(tree_t* tree, node_t* parentNode, node_t* childNode, const int side) {
//	assert(tree != NULL);
//	assert(parentNode != NULL);
//	assert(childNode != NULL);
//
//	if (!SideIsCorrect(side)) {
//		return 2;
//	}
//
//	switch (side) {
//	case LEFT_CHILD:
//		if (parentNode->left != NULL) {
//			return 1;
//		}
//		parentNode->left = childNode;
//		break;
//	case RIGHT_CHILD:
//		if (parentNode->right != NULL) {
//			return 1;
//		}
//		parentNode->right = childNode;
//		break;
//	}
//
//	return 0;
//}

/**
*	������� �������� ����
*
*	@param tree ������, � ������� ��������� ����
*	@param node ����, � ������� ����������� ������� ����
*	@param[in] side �������, � ������� ������� (LEFT_CHILD, RIGHT_CHILD)
*
*	@return 1 - �� ���� ��������� ����; 2 - �������� ���� �������� ����������;\
 3 - �������� side ���� ������������ �������; 4 - �� ���� �������� ������ �\
 ������� (������ � ������ �������); 0 - ��� ������ ���������
*/

int DeleteChild(tree_t* tree, node_t* node, const int side) {
	assert(tree != NULL);
	assert(node != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
		return 4;
	}
#endif

	if (!SideIsCorrect(side)) {
		return 3;
	}

	switch (side) {
	case LEFT_CHILD:
		if (node->left == NULL) {
			return 1;
		}
		else if (NodeChildsCount(node->left) > 0) {
			return 2;
		}

		free(node->left);
		node->left = NULL;
		break;

	case RIGHT_CHILD:
		if (node->right == NULL) {
			return 1;
		}
		else if (NodeChildsCount(node->right) > 0) {
			return 2;
		}

		free(node->right);
		node->right = NULL;
		break;
	}

	tree->size--;

#ifdef _DEBUG
	if (TreeOk(tree)) {
		PrintTree_OK(*tree);
	}
	else {
		PrintTree_NOK(*tree);
	}
#endif

	return 0;
}


/**
*	��������� ��������� � ������ �� ���������� ����.
*
*	@param tree ������, � ������� ���������
*	@param subtree ���������, ������� ���������. ����� ������������� ����������������!
*	@param node ����, � �������� ���������
*	@param[in] side ������� ����, � ������� ���������
*
*	@return 1 - � ���� ��� ��� �������� ���� � ��������������� �������;\
 2 - �������� side ���� ������������ �������; 3, 4 - �� ���� ��������\
 ������ ��� �������������� ��������� � ������� (������ � ������ �������);\
 0 - ��� ������ ���������
*/	

int AddSubtree(tree_t* tree, tree_t* subtree, node_t* node, const int side) {
	assert(tree != NULL);
	assert(subtree != NULL);
	assert(node != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
		return 3;
	}
#endif
#ifdef _DEBUG
	if (!TreeOk(subtree)) {
		PrintTree_NOK(*subtree);
		return 4;
	}
#endif

	if (!SideIsCorrect(side)) {
		return 2;
	}

	switch (side) {
	case LEFT_CHILD:
		if (node->left != NULL) {
			return 1;
		}
		node->left = subtree->root;
		break;

	case RIGHT_CHILD:
		if (node->right != NULL) {
			return 1;
		}
		node->right = subtree->root;
		break;
	}
	subtree->root->parent = node;
	tree->size += subtree->size;

	TreeDestructor(subtree);

#ifdef _DEBUG
	if (TreeOk(tree)) {
		PrintTree_OK(*tree);
	}
	else {
		PrintTree_NOK(*tree);
	}
#endif

	return 0;
}


/*  �� ��� ������������
*	������� ��� �������� ���� � ��� ����
*
*	@param ����
*	@param[in] count ���������� ��� ��������� �����\
 (��� ��������� ������ ��������, ���������� ������ ��� ������������ ������)
*
*	@return ���������� ��������� �����
*/

int DeleteNodes(node_t* node, int count) {
	assert(node != NULL);
	assert(count >= 0);

	if (node->left != NULL) {
		count = DeleteNodes(node->left, count);
	}
	if (node->right != NULL) {
		count = DeleteNodes(node->right, count);
	}
	free(node);

	return count + 1;
}


/**
*	������� ���������
*
*	@param tree ������
*	@param node ����, ��������� ��������� ������� � ��������� ����
*	@param side ������� ��������� ����
*
*	@return 1 - �� ���� ��������� ����; 2 - �������� side ���� ������������ �������;\
 3 - �� ���� �������� ������ � ������� (������ � ������ �������); 0 - ��� ������ ���������
*/

int DeleteSubtree(tree_t* tree, node_t* node, const int side) {
	assert(tree != NULL);
	assert(node != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
		return 3;
	}
#endif

	if (!SideIsCorrect(side)) {
		return 2;
	}

	int deletedCount = 0;
	switch (side) {
	case LEFT_CHILD:
		if (node->left == NULL) {
			return 1;
		}

		deletedCount = DeleteNodes(node->left);
		node->left = NULL;
		break;

	case RIGHT_CHILD:
		if (node->right == NULL) {
			return 1;
		}

		deletedCount = DeleteNodes(node->right);
		node->right = NULL;
		break;
	}

	tree->size -= deletedCount;
	assert(tree->size >= 0);

#ifdef _DEBUG
	if (TreeOk(tree)) {
		PrintTree_OK(*tree);
	}
	else {
		PrintTree_NOK(*tree);
	}
#endif

	return 0;
}


/**
*	���������� ������� ������, ������� ��� ��� ����.\
 ��������, ���� ������ ������ �������� ���������� �\
 ������ ������, �� ����������� ���� ��������.
*
*	@param[in] tree ������
*
*	@return 0 - ��� ������ ���������
*/

int DeleteTree(tree_t* tree) {
	assert(tree != NULL);

#ifdef _DEBUG
	if (!TreeOk(tree)) {
		PrintTree_NOK(*tree);
	}
#endif

	DeleteNodes(tree->root);
	TreeDestructor(tree);

	return 0;
}


/**
*	������������� ������. ��������, ������ ������� �� ������� ���� ������!\
 ����� ������� ����, �������������� �������� DeleteTree()!
*
*	@param tree ������
*
*	@return 0 - ��� ������ ���������
*/

int TreeDestructor(tree_t* tree) {
	assert(tree != NULL);

	tree->root = NULL;
	tree->size = 0;

	return 0;
}


///*  �� ��� ������������
//*	������� ��� �� ����
//*
//*	@param[in] node ����
//*	@param[out] codeBuf ����� � �����
//*
//*	@return 0 - ��� ������ ���������
//*/
//
//int NodesToCode(node_t* node, buf_t* codeBuf) {
//	assert(node != NULL);
//	assert(codeBuf != NULL);
//
//	char* valueS = Value_tToStr(node->value);
//	
//	if (NodeChildsCount(node) > 0) {
//		Bufcat(codeBuf, valueS);
//		free(valueS);
//		Bufcat(codeBuf, "{");
//
//		if (node->left != NULL) {
//			NodesToCode(node->left, codeBuf);
//		}
//		else {
//			Bufcat(codeBuf, "@");
//		}
//		Bufcat(codeBuf, ",");
//		if (node->right != NULL) {
//			NodesToCode(node->right, codeBuf);
//		}
//		else {
//			Bufcat(codeBuf, "@");
//		}
//		Bufcat(codeBuf, "}");
//	}
//	else {
//		Bufcat(codeBuf, "{");
//		Bufcat(codeBuf, valueS);
//		Bufcat(codeBuf, "}");
//		free(valueS);
//	}
//
//	return 0;
//}
//
//
///**
//*	������� ��� �� ������
//*
//*	@param[in] tree ������
//*	@param[out] size ����� ����������� ���� (��� ����� '\0').\
// ���� �������� �������������, �� �������� ��������� ������:\
// -1 - �� ���� �������� ������ � ������� (������ � ������ �������);\
// -2 -�������� ��� �������� ������; 0 - ��� ������ ���������
//*
//*	@return ��������� �� ����� � �����. � ������ ������ ����� NULL.\
// �� �������� ���������� ������ �� ����� ���������!
//*/
//
//char* TreeToCode(tree_t* tree, int* size) {
//	assert(tree != NULL);
//
//#ifdef _DEBUG
//	if (!TreeOk(tree)) {
//		PrintTree_NOK(*tree);
//		if (size != NULL) {
//			*size = -1;
//		}
//		return NULL;
//	}
//#endif
//
//	int bufConstrErr = 0;
//	buf_t codeBuf = BufConstructor('w', &bufConstrErr);
//	if (bufConstrErr != 0) {
//		if (size != NULL) {
//			*size = -2;
//		}
//		return NULL;
//	}
//	
//	Bufcat(&codeBuf, "{");
//	NodesToCode(tree->root, &codeBuf);
//	Bufcat(&codeBuf, "}");
//
//	char* res = (char*)calloc(codeBuf.lastChar + 2, sizeof(char));
//	strncpy(res, codeBuf.str, codeBuf.lastChar + 1);
//	if (size != NULL) {
//		*size = codeBuf.lastChar + 1;
//	}
//
//	BufDestructor(&codeBuf);
//
//	return res;
//}
//
//
///*  �� ��� ������������
//*	���������� ������� ���� �� ����
//*
//*	@param[in] buf ����� � �����
//*	@param[out] node ������� ����. ��������, ��� ��������� ������\
// ������ ���� NULL, ���� ��������� �������� ����!
//*	@param[out] size
//*
//*	@return 1 - ������ � ����; 0 - ��� ������ ���������
//*/
//
//int CodeToNodes(buf_t* buf, node_t*& node, int* size) {
//	assert(buf != NULL);
//	assert(size != NULL);
//
//	char curCh = Bgetc(buf);
//	if (curCh == '{') {
//		char nextCh = Bgetc(buf);
//		if (nextCh == EOB) {
//			return 1;
//		}
//		if (nextCh != '{') {
//			Bseek(buf, -1, BSEEK_CUR);
//		}
//
//		if (Bgetc(buf) == '@') {
//			node->left = NULL;
//
//			int err = 0;
//			err = CodeToNodes(buf, node, size);
//			if (err != 0) {
//				return err;
//			}
//		}
//		else {
//			Bseek(buf, -1, BSEEK_CUR);
//
//			char valueS[100] = "";
//			ReadToChar(valueS, buf, "{}");
//
//			node_t* newNode = CreateNode();
//
//			value_t value = {};
//			StrToValue_t(valueS, &value);
//			ChangeNodeValue(newNode, value);
//			if (node != NULL) {
//				node->left = newNode;
//			}
//			newNode->parent = node;
//			if (node == NULL) {
//				node = newNode;
//			}
//			(*size)++;
//
//			int err = 0;
//			err = CodeToNodes(buf, newNode, size);
//			if (err != 0) {
//				return err;
//			}
//		}
//	}
//	else if (curCh == ',') {
//		char nextCh = Bgetc(buf);
//		if (nextCh == EOB) {
//			return 1;
//		}
//		if (nextCh != '{') {
//			Bseek(buf, -1, BSEEK_CUR);
//		}
//
//		if (Bgetc(buf) == '@') {
//			node->right = NULL;
//
//			int err = 0;
//			err = CodeToNodes(buf, node, size);
//			if (err != 0) {
//				return err;
//			}
//		}
//		else {
//			Bseek(buf, -1, BSEEK_CUR);
//
//			char valueS[100] = "";
//			ReadToChar(valueS, buf, "{}");
//
//			node_t* newNode = CreateNode();
//
//			value_t value = {};
//			StrToValue_t(valueS, &value);
//			ChangeNodeValue(newNode, value);
//			node->right = newNode;
//			newNode->parent = node;
//			(*size)++;
//
//			int err = 0;
//			err = CodeToNodes(buf, newNode, size);
//			if (err != 0) {
//				return err;
//			}
//		}
//	}
//	else if (curCh == '}') {
//
//		if (node->parent != NULL) {
//			int err = 0;
//			err = CodeToNodes(buf, node->parent, size);
//			if (err != 0) {
//				return err;
//			}
//		}
//	}
//	else {
//		return 1;
//	}
//
//	return 0;
//}
//
//
///**
//*	������� ������ �� ����
//*
//*	@param[in] code ���
//*	@param[in] treeName ��� ������ (�� ��������� "tree_from_code")
//*	@param[out] err ��� ������ (�� �������): 1 - ������������ ������;\
// 2 - ������ � ����; 0 - ��� ������ ���������
//*
//*	@return ��������������� ������
//*/
//
//tree_t CodeToTree(char* code, const char* treeName, int* err) {
//	assert(code != NULL);
//
//	tree_t tree = TreeConstructor(treeName);
//
//#ifdef _DEBUG
//	if (TreeOk(&tree)) {
//		PrintTree_OK(tree);
//	}
//	else {
//		PrintTree_NOK(tree);
//	}
//#endif
//
//	int constructErr = 0;
//	buf_t codeBuf = BufConstructor('r', code, strlen(code), &constructErr);
//	if (constructErr != 0) {
//		*err = 1;
//		return tree;
//	}
//
//	free(tree.root);
//	tree.root = NULL;
//	tree.size = 0;
//	int retErr = CodeToNodes(&codeBuf, tree.root, &tree.size);
//	
//	if(retErr!=0){
//		if (err != NULL) {
//			*err = 2;
//		}
//		return tree;
//	}
//
//#ifdef _DEBUG
//	if (!TreeOk(&tree)) {
//		if (err != NULL) {
//			*err = 2;
//		}
//		return tree;
//	}
//#endif
//
//	return tree;
//}
//
//
///*  �� ��� ������������
//*	���� ���� � ��������� ��������� (����������)
//*
//*	@param[in] curNode ������� ���� (��� ��������� ������ ������ ���� ������ ������!)
//*	@param[in] value ��������
//*	@param[out] way ���� �� ���������� ���� (���� �������) � ������� ������ �� '0' � '1',\
// ��� '0' �������� ������ ����, '1' - �������. � ����� ������ �������� '\0'
//*
//*	@return 1 (true) - ���� ������; 0 (false) - ���� �� ������
//*/
//
//int NodeByValue(node_t* curNode, value_t* value, buf_t* way, node_t*& foundNode) {
//	assert(curNode != NULL);
//	assert(value != NULL);
//	assert(way != NULL);
//
//	if (ValueCmp(&curNode->value, value) == 0) {
//		Bputc(way, '\0');
//		foundNode = curNode;
//		return 1;
//	}
//	if (NodeChildsCount(curNode) == 0) {
//		Bseek(way, -1, SEEK_CUR);
//		Bputc(way, '\0');
//		Bseek(way, -1, SEEK_CUR);
//		return 0;
//	}
//
//	if (curNode->left != NULL) {
//		Bputc(way, '0');
//		if (NodeByValue(curNode->left, value, way, foundNode) == 1) {
//			return 1;
//		}
//	}
//	if (curNode->right != NULL) {
//		Bputc(way, '1');
//		if (NodeByValue(curNode->right, value, way, foundNode) == 1) {
//			return 1;
//		}
//	}
//	Bseek(way, -1, SEEK_CUR);
//	Bputc(way, '\0');
//	Bseek(way, -1, SEEK_CUR);
//
//	return 0;
//}
//
//
///**
//*	���� ���� � ��������� ���������
//*
//*	@param[in] tree ������
//*	@param[in] value ��������
//*	@param[out] err ������: 1 - ������������ ������; 2 - ���� � ����� ��������� �� ������
//*
//*	@return ��������� �� ������ � ����� �� ���������� ���� (���� �������) � ���� '0' � '1',\
// ��� '0' �������� ������ ����, '1' - �������. � ����� ������ �������� '\0'.\
// �� �������� ���������� ������ �� ����� ���������! ���� �������� ������, ���������� NULL.
//*/
//
//char* FindNodeByValue(tree_t* tree, value_t* value, node_t*& foundNode, int* err) {
//	assert(tree != NULL);
//	assert(value != NULL);
//
//	int constrErr = 0;
//	buf_t way = BufConstructor('w', &constrErr);
//	if (constrErr != 0) {
//		if (err != NULL) {
//			*err = 1;
//		}
//		return NULL;
//	}
//
//	if (!NodeByValue(tree->root, value, &way, foundNode)) {
//		if (err != NULL) {
//			*err = 2;
//		}
//		free(way.str);
//		BufDestructor(&way);
//		return NULL;
//	}
//
//	char* resWay = way.str;
//	BufDestructor(&way);
//	if (err != NULL) {
//		*err = 0;
//	}
//	return resWay;
//}