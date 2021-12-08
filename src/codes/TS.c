
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libs/TS.h"


/*
	Tree
*/
void TreeInit(Tree *tree){
	tree->root=NULL;
}

void TreeDive(TreeElement *element,int func){
	TreeElement *hElement=element->left;
	if(hElement!=NULL){
		TreeDive(hElement,func);
	}
	hElement=element->right;
	if(hElement!=NULL){
		TreeDive(hElement,func);
	}
	if(func){
		user_func* dt=element->data;
		int i=0;
		while (dt->params[i]!=_ender)
		{
			free(dt->param_names[i]);
			i++;
		}
		free(dt->param_names);
		free(dt->params);
		free(dt->returns);
	}
	free(element->data);
	free(element->name);
	free(element);
}

void TreeDestroy(Tree *tree,int func){
	if(tree->root!=NULL)
		TreeDive(tree->root,func);
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

int TS_Init(TreeSupport **ts){
	*ts=malloc(sizeof(TreeSupport));
	if(*ts==NULL) return MALLOC_ERR_CODE;
    (*ts)->curLayer=TS_GetLayer();
    if((*ts)->curLayer==NULL){free(*ts); return MALLOC_ERR_CODE;}
	(*ts)->functionLayer=(*ts)->curLayer;
	if(TS_OpenLayer(*ts)!=0){
		TS_CloseLayer(*ts,0);
		return MALLOC_ERR_CODE;
	}
	(*ts)->bounceLayer=(*ts)->curLayer;
    return 0;
}

int TS_OpenLayer(TreeSupport *ts){
	TreeLayer *newLayer=TS_GetLayer();
	if(newLayer==NULL)return MALLOC_ERR_CODE;
	newLayer->prevLayer=ts->curLayer;
	ts->curLayer=newLayer;
	return 0;
}

void TS_CloseLayer(TreeSupport *ts,int func){
	TreeLayer *toDelete = ts->curLayer;
	ts->curLayer=ts->curLayer->prevLayer;
	TreeDestroy(toDelete->tree,func);
	free(toDelete->tree);
	free(toDelete);
}

void TS_COLLAPSE(TreeSupport **ts){
	if(*ts == NULL)return;
	while((*ts)->curLayer!=NULL){
		if((*ts)->curLayer==(*ts)->functionLayer)
			TS_CloseLayer(*ts,1);
		else
			TS_CloseLayer(*ts,0);
	}
	free(*ts);
	*ts=NULL;
	//fprintf(stderr,"got");fflush(stderr);
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
/*
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWNWWWWWWWWWWWWWWWWWWWWWWXOKWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWXdlONWWWWWWWWWWWWWWWWWW0:.cXWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW0,.,dXWWWWWWWWWWWWWWWNx:,.;0WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWMWWk;:l;l0WWWWWWWWWWWWWXo;lkc,kWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWNx,dNO:;xXXKK00000KKOc;dNXl,xWMMWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWMWWWMWWMMMWWWWMMWWWWWWWWWWWWMWMWWMWMNo,dKOl''coollllllll:,;ldd:'dNMMWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWMWWWWWWWWWWWWMWWMMMMWWWWWWWWWWMMWWWWW0c.,codkOKXNNNNNNNNNXX0kxo:';dOXWMWWWWWWWWMWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
MMMMMMMMMMMMMMMMMMMMMMMMWMWWWWWWWWWWWWWWWWWWWWWNNNNNNXXXXXKkc'.':d0NMMMMMMMMMWN0kxxxk0XNXOocco0NWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMXkoooooooolllllllccccc:::::;;;;;,...',;;,';dKWMMMWNKo;',,;;,',lONWNOl:lONWWMWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMWO,.:llllllllllllloooooooooodddddc..lkO0Oxd:.,xNMNxc'.:dkO0Okdc..oXMMN0l;lKWMWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMM0;;x0O0OOO0OOOO0OOOOO00OOOOOOOO0x,.l0KKKOkOo.'xKx' .oOkO0KK0kOx'.oXNWMNk::OWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMKc,dOOOOOOOOOOkdxkkkodkOOOOOOOOOOc'oOkxOKOkk:..,...:Ok0KOxkKKOOc..,dNMMW0:;kWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMWWMMMMMMMMMNl'cO0OOOOOOOk:',;;;''lOOOOOOOOOOo':O0O0KOOx,.:d:. ,kOOK0O0K0OO:.,d0NMMMWO;;OWMMMMMMMMMMWWWWWWWWMMMMMMMMMMMMWWWWWWMMMMMMMMMMM
WWWWWWWWWWWWWWWWWWWWWWWWWNd':k0OOOOOOkc,oOOO0Oc,lOOOOOOOOOd,'d000OOx;.cKWK:..;xkO000Okx:.,OWMMMMMMNd'cKWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWNWWWWWWWWWWWWWW
NWWNNNNWNWNNNNNNNNNNNNNNNNk;;x0OOOOOOl'lXMMMMW0:,dOOOOOOOOk;.;oool;.':xNN0xdc'.;loool:''c0WMMMMMMMW0,'kNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
NNNNNNNNNNNNNNNNNNNNNNNNNN0:,dOOOOOOOl'lKMMMMWO;,xOOOOOOOOOc..,,,',o0k:cc:d0XKxc:;,,;cd0NWWMMMMMMMMK;.xNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
NNNNNNNNNNNNNNNNNNNNNNNNNNKc'lO0OOOOOkc;:odxdl;;dOOOOOOOOO0o':ONXx;l0O:..lXWWXxxXNXNNNN0kONMMMMMMMWO,,kNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
NNNNNNNNNNNNNNNNNNNNNNNNNNXl.ck0OOOOOOOkdlcccldkOOOOOOOOOO0x',0MWXkl,,lxl:cooc:kWWN0kdodkXWMMMMMMMNo'lKNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
NNNNNNNNNNNNNNNNNNNNNNNNNNNd.;k0OOOOOOOOOO00OOOOOOOOOOOOOO0k;'kWMMMNo,xNNd'.;kKN0dcldOXWMMNK0NMMMWx,c0NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
NNNNNNNNNNNNNNNNNNNNNNNWNNNO;.:odddddddooooooooooooooooooool;.ck0OOko,'lo,';oOOl',okO0OOxddx0NMMWk;cONNNNNNNNNNWWWWWNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
NNNNNNNNNNNNNNNNNNNNNNNWNNNXOl..';;,;;,,;;;;;;;;;;;;;;;;;;;::'..;::cc::;;;:::::;,;;;;'..lOXWMMWXd;l0NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWXd';dkkkkkkOOOOOOOOOOkkOOOkkOOOOd'.cxOOOOOOOOOOOkkkkkkkko.,0MMMWXx::xXWNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWXd,,::::::cccclc:::::::::::::;;;...';::::::::::::::;::;,'cKWN0o:cxKNWWWWNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWNWWWN0kddooooooooooooddddddxxxxddc...,:odxxkkkkOOOOOOOOkOkkkkko;':kXNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWXo':oollldxO000KKK000Okxolllo:.:0NWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO;:KW0:.cddoooooooooodc.;ONKc,xNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWXl,xNx,lXWNNNNNNNNNNNWKc,kXd,oXWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
MMWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWNKOkkl.':'.:oolllllllllllll;.':.'lxkkO0XNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWNKkdol:;,,;;;,,,,,,,,,,,,,,,,,,,;ccloxOXNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWNNNNNNNXXXXXXXXXXXXXXXXXXNNNNNNWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
*/
//End of this file
