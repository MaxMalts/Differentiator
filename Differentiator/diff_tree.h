#pragma once


#define LEFT_CHILD 1
#define RIGHT_CHILD 2

const int treeStrMaxSize = 101;

typedef char value_t[treeStrMaxSize];

enum node_type {
	unknown_node,
	num_node,
	var_node,
	op_node,
	func_node,
	diff_node
};

struct node_t {
	node_type type = unknown_node;
	value_t value = {};
	node_t* left = NULL;
	node_t* right = NULL;
	node_t* parent = NULL;
};

struct tree_t {

	node_t* root = {};                       ///<������
	int size = 0;                            ///<��������� ������ ������

#ifdef _DEBUG
	char name[30] = "";                      ///<��� ������
	int err = 0;                             ///<��� ������, ������������ � ������:\n
											 ///0 - ��� ������\n
	                                         ///1 - ������ �������������
	                                         ///2 - ��� �����
	                                         ///3 - �������� � ������
	                                         ///4 - ���������� ����� �� ������������� �������
#endif
};



#ifdef _DEBUG
#define PrintTree_OK(tree) TreeDump(&tree, __FILE__, __LINE__, __FUNCTION__, "just looking");
#define PrintTree_NOK(tree) TreeDump(&tree, __FILE__, __LINE__, __FUNCTION__, "tree has an error");
#else
#define PrintTree_OK(tree) ;
#define PrintTree_NOK(tree) ;
#endif


/**
*	������� ���������� � ������. ��� ������������ ������, ����������� ShowTree().
*
*	@param[in] tree ������
*	@param[in] file �������� �����, ����� ������� �������
*	@param[in] line ����� ������, �� ������� ������� �������
*	@param[in] function ��� �������, �� ������� ������� �������
*	@param[in] reason �������, �� ������� ������� �������
*/

#ifdef _DEBUG
void TreeDump(tree_t* tree, const char* file, const int line, const char* function, const char* reason);
#endif


/**
*	��������� ������ � ���������� � ���� ��� ������
*
*	@param[in] tree ������
*
*	@return 0 - ������ ������������; 1 - ������ ����������
*/

#ifdef _DEBUG
int TreeOk(tree_t* tree);
#endif


/**
*	������� ���������� �������
*
*	@param[in] node ����
*
*	@return ���������� ������� (0, 1, 2)
*/

int NodeChildsCount(node_t* node);


/**
*	�������� ������� ���� value_t
*
*	@param[out] dest ���� ��������
*	@param[in] source ������ ��������
*
*	@return 0 - ��� ������ ���������
*/

int CopyValue_t(value_t dest, value_t source);


/**
*	����������� value_t � ������
*
*	@param[in] value ������� ���� value_t
*
*	@return ��������� �� ������. �� �������� ���������� ������ �� ����� ���������!
*/

char* Value_tToStr(const value_t value);


/**
*	���������� �������� value_t
*
*	@param[in] value1 ������ ��������
*	@param[in] value2 ������ ��������
*
*	@return < 0 - ������ �������� ������ �������; 0 - �������� �����; > 0 - ������ �������� ������ �������
*/

int ValueCmp(value_t* value1, value_t* value2);

/**
*	����������� ������ � value_t
*
*	@param[in] valueS ������
*	@param[out] value �������� value_t
*
*	@return 1 - � ������ �������� ������������� value_t; 0 - ��� ������ ���������
*/

int StrToValue_t(const char* valueS, value_t* value);



/**
*	������� ����� ������. ����� ������, �������� �������� �����\
 �� ������ � ������� ������� ChangeNodeValue()!
 *
 *	@param[in] name ��� ������
 *
 *	@return ��������� �� ��������� ������
*/

tree_t TreeConstructor(const char* name);


/**
*	������������� ������ ������
*
*	@param tree ������
*
*	@return ��. ������������ �������� CalcNodesCount
*/

int RecalcTreeSize(tree_t* tree);


/**
*	������� ����� ������ � �������� ������
*
*	@param[in] root ������
*	@param[in] name ��� ������
*
*	@return ��������� ������
*/

tree_t TreeRootConstructor(node_t* root, const char* name);


/**
*	�������������� �������� ����
*
*	@param node ����
*	@param[in] value ��������
*
*	@return 0 - ��� ������ ���������
*/

int ChangeNodeValue(node_t* node, value_t value);


/**
*	������ �������� ����
*
*	@param[in] node ����
*	@param[out] value ��������
*
*	@return 0 - ��� ������ ���������
*/

int GetNodeValue(node_t* node, value_t* value);


/**
*	������� ����� ����
*
*	@return ��������� �� ����� ����. �� �������� ���������� ������ �� ����� ���������!
*/

node_t* CreateNode();


/*  �� ��� ������������
*	������� ����� ���� � ������������ ������
*
*	@return ��������� �� ����� ����. �� �������� ���������� ������ �� ����� ���������!
*/

node_t* CreateNodeProp(node_t* parent, node_type type, value_t value, node_t* left, node_t* right);


/**
*	��������� ���� � ��� ��� �������� ����
*
*	@param[in] srcNode �������� ����
*
*	@return ��������� �� ����� ���� (���� parnt ��������� ����� ��, ��� � ���������).\
 �� �������� ���������� ������ �� ����� ���������!
*/

node_t* CloneNodes(node_t* srcNode);


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

int AddChild(tree_t* tree, node_t* node, value_t elem, const int side, node_t** createdNode = NULL);


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

int DeleteChild(tree_t* tree, node_t* node, const int side);


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

int AddSubtree(tree_t* tree, tree_t* subtree, node_t* node, const int side);


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

int DeleteSubtree(tree_t* tree, node_t* node, const int side);


/**
*	���������� ������� ������, ������� ��� ��� ����.\
 ��������, ���� ������ ������ �������� ���������� �\
 ������ ������, �� ����������� ���� ��������.
*
*	@param[in] tree ������
*
*	@return 0 - ��� ������ ���������
*/

int DeleteTree(tree_t* tree);


/**
*	������������� ������. ��������, ������ ������� �� ������� ���� ������!\
 ����� ������� ����, �������������� �������� DeleteTree()!
*
*	@param tree ������
*
*	@return 0 - ��� ������ ���������
*/

int TreeDestructor(tree_t* tree);


/**
*	������� ������ ����� �� ���������� "���������" �����
*
*	@param[in] tree ������
*	@param[out] words ������ ����� (�� ����� ������ ���� NULL). �� ��������\
 ���������� ������ ��� ���� ��������� ����� ������� � ��� ������ �������!
*	@param[out] NNodes ���������� �����
*
*	@return 1 - �� ���� �������� ������������ ������; 2 - words != NULL;\
 3 - �������� ��� ����������� words; 4 - ������ ������; 0 - ��� ������ ���������
*/

int LastNodesWords(tree_t* tree, char**& words, int* NNodes);


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

int CreateTreeImage(tree_t* tree, const char foutName[] = "tree.png", const char gvFileName[] = "tree.gv");


/**
*	������� ����������� ������ � ��������� ���. ������� ���������, �� ����������� �����!
*
*	@param[in] tree ������
*
*	@return 1 - �������� ��� �������� �����������; 2 - �������� ��� �������� �����������;\
 3 - �� ���� �������� ������ � ������� (������ � ������ �������); 0 - ��� ������ ���������
*/

int ShowTree(tree_t* tree);



/**
*	������� ��� �� ������
*
*	@param[in] tree ������
*	@param[out] size ����� ����������� ���� (��� ����� '\0').\
 ���� �������� �������������, �� �������� ��������� ������:\
 -1 - �� ���� �������� ������ � ������� (������ � ������ �������);\
 -2 -�������� ��� �������� ������; 0 - ��� ������ ���������
*
*	@return ��������� �� ����� � �����. � ������ ������ ����� NULL.\
 �� �������� ���������� ������ �� ����� ���������!
*/

char* TreeToCode(tree_t* tree, int* size = NULL);


/**
*	������� ������ �� ����
*
*	@param[in] code ���
*	@param[in] treeName ��� ������ (�� ��������� "tree_from_code")
*	@param[out] err ��� ������ (�� �������): 1 - ������������ ������;\
 2 - ������ � ����; 0 - ��� ������ ���������
*
*	@return ��������������� ������
*/

tree_t CodeToTree(char* code, const char* treeName="tree_from_code", int* err = NULL);


/**
*	���� ���� � ��������� ���������
*
*	@param[in] tree ������
*	@param[in] value ��������
*	@param[out] err ������: 1 - ������������ ������; 2 - ���� � ����� ��������� �� ������
*
*	@return ��������� �� ������ � ����� �� ���������� ���� (���� �������) � ���� '0' � '1',\
 ��� '0' �������� ������ ����, '1' - �������. � ����� ������ �������� '\0'.\
 �� �������� ���������� ������ �� ����� ���������! ���� �������� ������, ���������� NULL.
*/

char* FindNodeByValue(tree_t* tree, value_t* value, node_t*& foundNode, int* err = NULL);