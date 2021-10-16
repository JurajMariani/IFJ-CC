#include "TS.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
	Tree
*/
void TreeInit(Tree *tree){
	tree->root=NULL;
}

void TreeDive(TreeElement *element);
void TreeDive(TreeElement *element){
	TreeElement *hElement=element->left;
	if(hElement!=NULL){
		TreeDive(hElement);
	}
	hElement=element->right;
	if(hElement!=NULL){
		TreeDive(hElement);
	}
	free(element->var);
	free(element);
}

void TreeDestroy(Tree *tree){
	if(tree->root!=NULL)
		TreeDive(tree->root);
	tree->root=NULL;
}

int TreeInsert(Tree *tree,variable *gVar){
	TreeElement *newElement = malloc(sizeof(TreeElement));
	if(newElement==NULL)return MALLOC_ERR_CODE;
	newElement->var=gVar;
	newElement->left=NULL;
	newElement->right=NULL;

	if(tree->root==NULL){
		tree->root=newElement;
		return 0;
	}

	TreeElement *targetElement=tree->root;
	while (1){
		if(strcmp(newElement->var->name,targetElement->var->name)<0){
			if(targetElement->left!=NULL){
				targetElement=targetElement->left;
			}else{
				targetElement->left=newElement;
				return 0;
			}
		}else{
			if(targetElement->right!=NULL){
				targetElement=targetElement->right;
			}else{
				targetElement->right=newElement;
				return 0;
			}
		}
	}
}

variable* TreeLookup(Tree *tree, char *name){
	if(tree->root==NULL)return NULL;
	TreeElement *hElement = tree->root;
	int nFlag=0;
	while(1){
		int help=strcmp(name,hElement->var->name);
		if(help==0){
			break;
		}else if(help<0){
			if(hElement->left==NULL){
				nFlag=1;
				break;
			}
			hElement=hElement->left;
		}else{
			if(hElement->right==NULL){
				nFlag=1;
				break;
			}
			hElement=hElement->right;
		}
	}
	if(nFlag)
		return NULL;
	else
		return hElement->var;
}

/*
	TreeSupports

*/

//TODO
variable* Var_Create(char *name, dataType type){
	variable *nVar = malloc(sizeof(variable));
	if(nVar==NULL) return NULL;
	nVar->type=type;
	strcpy(nVar->name,name);
	return nVar;	
}

int TS_Insert(TreeSupport *ts,variable *gVar){
	int errCode = TreeInsert(ts->curLayer->tree,gVar);
	return errCode;
}

TreeLayer *TS_GetLayer(){
	TreeLayer *newLayer = malloc(sizeof(TreeLayer));
	if(newLayer==NULL)return NULL;
	newLayer->tree = malloc(sizeof(Tree));
	if(newLayer->tree==NULL){free(newLayer); return NULL;}
	TreeInit(newLayer->tree);
	newLayer->prevLayer=NULL;
	return newLayer;
}

int TS_Init(TreeSupport *ts){
        ts->curLayer=TS_GetLayer();
        if(ts->curLayer==NULL)return MALLOC_ERR_CODE;
        return 0;
}

int TS_OpenLayer(TreeSupport *ts){
	TreeLayer *newLayer=TS_GetLayer();
	if(newLayer==NULL)return MALLOC_ERR_CODE;
	newLayer->prevLayer=ts->curLayer;
	ts->curLayer=newLayer;
	return 0;
}

void TS_CloseLayer(TreeSupport *ts){
	TreeLayer *toDelete = ts->curLayer;
	ts->curLayer=ts->curLayer->prevLayer;
	TreeDestroy(toDelete->tree);
	free(toDelete->tree);
	free(toDelete);
}

void TS_COLLAPSE(TreeSupport *ts){
	while(ts->curLayer!=NULL)
		TS_CloseLayer(ts);
}

variable *TS_LookLayer(TreeSupport *ts, char *name){
	return TreeLookup(ts->curLayer->tree,name);
}

//End of this file
