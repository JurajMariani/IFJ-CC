#include "parser.h"

//TODO ERROR GENERATION AND REMOVE SOMETHING_SOMETHING_CHIMICHANGAS

void FreeTheseBees(TreeSupport *ts);

//DNT
int PushDataType(dataType *toPush,dataType newType){
    dataType* curAdr=toPush;
    int count = 1;
    while (curAdr!=NULL){
        count+=1;
        curAdr=curAdr+1;
    }
    toPush = realloc(toPush,sizeof(dataType)*count + 1);
    if (toPush==NULL)return MALLOC_ERR_CODE;
    toPush[count-1]=newType;
    toPush[count]=NULL;
    return 0;
}

//DNT
user_func* CreateFunctionData(){
    user_func* newData = malloc(sizeof(user_func));
    if (newData == NULL) return NULL;
    newData->params=malloc(sizeof(dataType));
    if(newData->params==NULL){free(newData); return NULL;}
    newData->returns=malloc(sizeof(dataType));
    if(newData->returns==NULL){free(newData->params);free(newData);return NULL;}
    newData->params=NULL;
    newData->returns=NULL;
    newData->vDefined=0;
    return newData;
}

//DNT
variable* CreateVariableData(dataType newType){
    variable *newData = malloc(sizeof(variable));
    if(newData==NULL)return NULL;
    newData->type=newType;
    newData->vDefined=0;
    return newData;
}

//DNT
int F_Prog(token* nextToken){                                     
    int test;
    if (nextToken->type==_keyword && nextToken->data.kw==_global){
        NEXT;
        if (nextToken->type != _identifier) return INVALID_TOKEN;
        if (TS_LookFunction(ts,nextToken->data.str)!=NULL) return REDECLARATION_OF_FUNCTION;
        user_func* newData = CreateFunctionData();
        if (newData==NULL)return MALLOC_ERR_CODE;
        if(TS_InsertFunction(ts,nextToken->data.str,w_func,newData)==MALLOC_ERR_CODE)return MALLOC_ERR_CODE;
        NEXT;
        if (nextToken->type!=_misc || nextToken->data.msc!=_doubleKomma)return INVALID_TOKEN;
        NEXT;
        if (nextToken->type!=_keyword || nextToken->data.kw!=_function)return INVALID_TOKEN;
        NEXT;
        if (nextToken->type!=_misc || nextToken->data.msc!=_bracketL)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_Params,nextToken);
        if (test!=0)return SOMETHING_SOMETHING_CHIMICHANGAS;
        if (nextToken->type!=_misc || nextToken->data.msc!=_bracketR)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_ParamsR,nextToken);
        if (test != 0) return SOMETHING_SOMETHING_CHIMICHANGAS;
        return 0;
        
    }
    else if(nextToken->type ==_keyword && nextToken->data.kw == _function){
        NEXT;
        if (nextToken->type!=_identifier) return INVALID_TOKEN;
        if (TS_LookFunction(ts,nextToken->data.str)!=NULL) return REDECLARATION_OF_FUNCTION;
        user_func* newData = CreateFunctionData();
        if (newData==NULL) return MALLOC_ERR_CODE;
        if(TS_InsertFunction(ts,nextToken->data.str,w_func,newData)==MALLOC_ERR_CODE) return MALLOC_ERR_CODE;
        NEXT;
        if (nextToken->type!=_misc || nextToken->data.msc!=_bracketL)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_Params,nextToken);
        if (test != 0) return SOMETHING_SOMETHING_CHIMICHANGAS;
        if (nextToken->type!=_misc || nextToken->data.msc!=_bracketR)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_ParamsR,nextToken);
        if (test!= 0) return SOMETHING_SOMETHING_CHIMICHANGAS;
        test = workNotTerminal(N_StList,nextToken);
        if (test!= 0) return SOMETHING_SOMETHING_CHIMICHANGAS;
        if (nextToken->type!=_keyword && nextToken->data.kw!=_end)return INVALID_TOKEN;
        NEXT;
        return 0;

    }
    else if((nextToken->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL)||
            (nextToken->type == _identifier && TS_LookVariable(ts,nextToken->data.str)!=NULL)||
            (nextToken->type == _keyword && nextToken->data.kw == _while)||
            (nextToken->type == _keyword && nextToken->data.kw == _if)){
        test = workNotTerminal(N_Statement,nextToken);
        if(test!=0) return SOMETHING_SOMETHING_CHIMICHANGAS;
    }else if(nextToken->type == _misc && nextToken->data.msc == _EOF)
        return DONE;
    else return INVALID_TOKEN;
}

int F_Params(token* nextToken);

int F_ParamsR(token *nextToken);

int F_SecondParam(token* nextToken);

int F_SecondParamR(token* nextToken);

int F_Type(token* nextToken);

int F_StList(token* nextToken);

int F_Statement(token* nextToken);

int F_Else(token* nextToken);

int F_SOperExpr(token *nextToken);

int F_Expr(token* nextToken);

int F_ExprAfterLoc(token* nextToken);

int F_ExprC(token* nextToken);

int F_ExprE(token* nextToken);

int F_SentPar(token* nextToken);

int F_SPar(token* nextToken);

int F_SExpr(token* nextToken);

int F_SVar(token* nextToken);

int F_Operand(token* nextToken);

int F_OperandWs(token* nextToken);

int workNotTerminal(notTerminal terminal,token* nextToken){
    switch (termainal)
    {
    case N_Prog: return F_Prog(token *nextToken); break;
    case N_Params: return F_Params(token *nextToken); break;
    case N_ParamsR: return F_ParamsR(token *nextToken); break;
    case N_SecondParam : return F_SecondParam(token *nextToken); break;
    case N_SecondParamR: return F_SecondParamR(token *nextToken); break;
    case N_Type: return F_Type(token *nextToken); break;
    case N_StList: return F_StList(token *nextToken); break;
    case N_Statement: return F_Statement(token *nextToken); break;
    case N_Else: return F_Else(token *nextToken); break;
    case N_SOperExpr: return F_SOperExpr(token *nextToken); break;
    case N_Expr: return F_Expr(token *nextToken); break;
    case N_Expr_AfterLoc: return F_ExprAfterLoc(token *nextToken); break;
    case N_ExprC: return F_ExprC(token *nextToken); break;
    case N_ExprE: return F_ExprE(token *nextToken); break;
    case N_SentPar: return F_SentPar(token *nextToken); break;
    case N_SPar: return F_SPar(token *nextToken); break;
    case N_SExpr: return F_SExpr(token *nextToken); break;
    case N_SVar: return F_SVar(token *nextToken); break;
    case N_Operand: return F_Operand(token *nextToken); break;
    case N_OperandWs: return F_OperandWs(token *nextToken); break;
    
    
    default:
        //NO FUTURE
        break;
    }
}

int flag = 0;
int mainParseFunction(){
    int mallocErr = 0;
    token nextToken;
    TS_Init(&ts);
    NEXT;
    while (nextToken.type!=_misc && nextToken.data != _EOF){
        int temp = workNotTerminal(N_Prog);
        if (temp != 0){
            flag = 1;
            if (temp == MALLOC_ERR_CODE){ 
                mallocErr=1;
                break;
            }else{
                                                                                                                        //DO SOMETHING SIMMILIAR IN ST_LISTE
                while(!((nextToken->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL)||
                        (nextToken->type == _identifier && TS_LookVariable(ts,nextToken->data.str)!=NULL)||
                        (nextToken->type == _keyword && nextToken->data.kw == _while)||
                        (nextToken->type == _keyword && nextToken->data.kw == _if)||
                        (nextToken->type == _keyword && nextToken->data.kw == _global)||
                        (nextToken->type == _keyword && nextToken->data.kw == _function)))
                NEXT;
            }

        }
    }
    FreeTheseBees(ts);
}