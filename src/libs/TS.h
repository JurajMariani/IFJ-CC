
#include "Defs.h"

//TreeStuff


typedef struct TreeElement{
	variable *var;
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
int TreeInsert(Tree *tree, variable* gVar);

/**
 * 	Looks if the variable is present in tree
 * 	returns pointer if yes, NULL if no
 * 	took 3h to make :X
 * 	@param Tree pointer
 * 	@param pointer to name of variable
 * 	@return pointer to found variable or NULL
 */
variable* TreeLookUp(Tree *tree, char *name);


//layered tree control

/**
 * 	Initiate TS
 * 	GIVES YOU FIRST LAYER PREPARED! DO NOT CALL A NEW LAYER FROM GETGO!!!
 * 	@param TS pointer
 * 	@return 0 on succes, err code on memory fail
 */
int TS_Init(TreeSupport *ts);

/**
 * 	Insert new element to curently opened layer
 * 	@param TS pointer
 * 	@param variable* variable to add
 * 	@return 0 on succes, err code on memory fail
 */
int TS_Insert(TreeSupport *ts,variable* gVar);

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
 * 	@param char *var name
 * 	@param dataType var type
 * 	@return variable* NULL if alloc error, pointer on succes
 */
variable* Var_Create(char* name, dataType type);

/**
 * 	Looks if element is in current tree
 * 	@param TS pointer
 * 	@param char* name of searched variable
 * 	@return variable* on found, NULL on not found
 */
variable* TS_LookLayer(TreeSupport *ts,char *name);



//WILL FINISH IF NEEDED #WIP
//variable* TS_LookAll(TreeSupport *ts, char *name);

//LIBARY ENDO
