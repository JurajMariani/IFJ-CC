
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "TS.h"

/*
	Tree
*/
void TreeInit(Tree *tree){
	tree->root=NULL;
}

void TreeDive(TreeElement *element){
	TreeElement *hElement=element->left;
	if(hElement!=NULL){
		TreeDive(hElement);
	}
	hElement=element->right;
	if(hElement!=NULL){
		TreeDive(hElement);
	}
	free(element->data);
	free(element);
}

void TreeDestroy(Tree *tree){
	if(tree->root!=NULL)
		TreeDive(tree->root);
	tree->root=NULL;
}

int TreeInsert(Tree *tree, char* name,treeElementType type,void* data){
	TreeElement *newElement = malloc(sizeof(TreeElement));
	if(newElement==NULL)return MALLOC_ERR_CODE;
	newElement->data=data;
	newElement->type=type;
	newElement->name=name;
	newElement->left=NULL;
	newElement->right=NULL;

	if(tree->root==NULL){
		tree->root=newElement;
		return 0;
	}

	TreeElement *targetElement=tree->root;
	while (1){
		if(strcmp(newElement->name,targetElement->name)<0){
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

TreeElement* TreeLookup(Tree *tree, char *name){
	if(tree->root==NULL)return NULL;
	TreeElement *hElement = tree->root;
	int nFlag=0;
	while(1){
		int help=strcmp(name,hElement->name);
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
		return hElement;
}

/*
	TreeSupports

*/

//TODO
void* Data_Create(dataType type, dataType *pType, dataType *rType){
	if (type != _nan){
		variable *nVar = malloc(sizeof(variable));
		if(nVar==NULL) return NULL;
		nVar->type=type;
		nVar->vDefined = 0;
		return nVar;
	}else{
		user_func *nFunc = malloc(sizeof(user_func));
		if(nFunc==NULL)return NULL;
		nFunc->vDefined = 0;
		nFunc->params = pType;
		nFunc->returns = rType;
		return nFunc;
	}	
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
		ts->functionLayer=ts->curLayer;
		if(TS_OpenLayer(ts)!=0){
			TS_CloseLayer(ts);
			return MALLOC_ERR_CODE;
		}
		ts->bounceLayer=ts->curLayer;
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
	while(ts->curLayer!=NULL){
		TS_CloseLayer(ts);
	}
}

int TS_InsertVariable(TreeSupport *ts, char* name,treeElementType type,void* data){
	int errCode = TreeInsert(ts->curLayer->tree,name,type,data);
	return errCode;
}

int TS_InsertFunction(TreeSupport *ts, char* name,treeElementType type,void* data){
	int errCode = TreeInsert(ts->functionLayer->tree,name,type,data);
	return errCode;
}

TreeElement* TS_LookVariable(TreeSupport *ts, char *name){
	TreeElement *returnEl;
	TreeLayer *searchedLayer=ts->curLayer;
	while ((returnEl=TreeLookup(searchedLayer->tree,name))==NULL && searchedLayer!=ts->bounceLayer)
	{
		searchedLayer=searchedLayer->prevLayer;
	}
	
	return returnEl;
}

TreeElement* TS_LookLayerVariable(TreeSupport *ts, char *name){
	return TreeLookup(ts->curLayer->tree,name);
}

TreeElement* TS_LookFunction(TreeSupport *ts, char *name){
	return TreeLookup(ts->functionLayer->tree,name);
}

//End of this file
