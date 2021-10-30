
#include "Defs.h"

//TreeStuff

typedef enum {w_var,w_func}treeElementType;

typedef struct TreeElement{
	char *name;
	treeElementType type;
	void *data;
	struct TreeElement *left;
	struct TreeElement *right;
}TreeElement;

typedef struct{
	TreeElement *root;	
}Tree;


//TreeSupprot

typedef struct TreeLayer{
	struct TreeLayer *prevLayer;
	Tree *tree;
}TreeLayer;

typedef struct TreeSupport{
	TreeLayer *curLayer;
	TreeLayer *functionLayer;
	TreeLayer *bounceLayer;
}TreeSupport;


//Binary search tree functions

/**	
 *	Tree Initialization
 *	Just fill pointer with NULL
 *	@param Tree pointer
 */
void TreeInit(Tree *tree);

/**
 * 	Tree Destruction / memory free
 * 	Frees the allocated memory
 * 	@param Tree pointer
 */
void TreeDestroy(Tree *tree);

/**
 * 	Inserting new variable to tree
 * 	Adds new node to binary tree
 * 	@param Tree pointer
 * 	@param Variable* data type pointer
 */ 
int TreeInsert(Tree *tree, char* name,treeElementType type,void* data);

/**
 * 	Looks if the variable is present in tree
 * 	returns pointer if yes, NULL if no
 * 	took 3h to make :X
 * 	@param Tree pointer
 * 	@param pointer to name of variable
 * 	@return pointer to found variable or NULL
 */
TreeElement* TreeLookup(Tree *tree, char *name);


//layered tree control

/**
 * 	Initiate TS
 * 	GIVES YOU FIRST LAYER PREPARED! DO NOT CALL A NEW LAYER FROM GETGO!!!
 * 	@param TS pointer
 * 	@return 0 on succes, err code on memory fail
 */
int TS_Init(TreeSupport *ts);

/**
 * 	Opens a new layer on top of the one before
 * 	(Sprouts a brend new tree if you will :))
 * 	@param TS pointer
 * 	@return 0 on succes, err code on  memory fail
 */
int TS_OpenLayer(TreeSupport *ts);

/**
 * 	Closes a curently opened layer and delets the current tree
 * 	@param TS pointer
 */
void TS_CloseLayer(TreeSupport *ts);

/**
 * 	Closes all Layers and frees memory
 * 	@param TS pointer
 */
void TS_COLLAPSE(TreeSupport *ts);

/**
 * 	Creates a variable for thee to insert to a tree
 * 	This function is in dire need
 * 	This comment of it to yeet
 * 	#TOEXPAND if needed TODO
 * 	@param defined *var / func defined
 * 	@param dataType var type / if func _nan
 *  @param param_type NULL if variable
 *  @param return_type NULL if variable
 * 	@return void* NULL if alloc error, pointer on succes
 */
void* Data_Create(dataType type, dataType *pType, dataType *rType);

/**
 * 	Looks if variable currently exists at all
 * 	@param TS pointer
 * 	@param char* name of searched variable
 * 	@return TreeElement* on found, NULL on not found
 */
TreeElement* TS_LookVariable(TreeSupport *ts,char *name);

/**
 * 	Looks if element is in current tree
 * 	@param TS pointer
 * 	@param char* name of searched variable
 * 	@return TreeElement* on found, NULL on not found
 */
TreeElement* TS_LookLayerVariable(TreeSupport *ts, char *name);

/**
 * 	Looks if function was declared
 * 	@param TS pointer
 * 	@param char* name of searched variable
 * 	@return TreeElement* on found, NULL on not found
 */
TreeElement* TS_LookFunction(TreeSupport *ts,char *name);

/**
 * 	Insert new element to curently opened layer
 * 	@param TS pointer
 * 	@param @param char* variable to add
 * 	@param void* data to add
 * 	@return 0 on succes, err code on memory fail
 */
int TS_InsertVariable(TreeSupport *ts, char* name,treeElementType type, void* data);

/**
 * 	Insert new element to curently opened layer
 * 	@param TS pointer
 * 	@param char* variable to add
 * 	@param void* data to add
 * 	@return 0 on succes, err code on memory fail
 */
int TS_InsertFunction(TreeSupport *ts, char* name,treeElementType type,void* data);

//LIBARY ENDO
