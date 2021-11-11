#include "parser.h"

//TODO ERROR GENERATION AND REMOVE SOMETHING_SOMETHING_CHIMICHANGAS

#define IFLeftBracket (nextToken->type ==_misc && nextToken->data.msc==_bracketL)
#define IFDoubleKomma (nextToken->type ==_misc && nextToken->data.msc==_doubleKomma)
#define IfKomma (nextToken->type ==_misc && nextToken->data.msc == _komma)
#define IFRightBracket (nextToken->type ==_misc && nextToken->data.msc==_bracketR)
#define IFKwFunction (nextToken->type ==_keyword && nextToken->data.kw==_function)
#define IFKwEnd (nextToken->type ==_keyword && nextToken->data.kw==_end)
#define IFKwDo (nextToken->type ==_keyword && nextToken->data.kw==_do)
#define IFKwElse (nextToken->type ==_keyword && nextToken->data.kw==_else)
#define IFKwNil (nextToken->type ==_keyword && nextToken->data.kw==_nil)
#define IFKwRead (nextToken->type ==_keyword && nextToken->data.kw==_read)
#define IFKwRequire (nextToken->type ==_keyword && nextToken->data.kw==_require)
#define IFKwReturn (nextToken->type ==_keyword && nextToken->data.kw==_return)
#define IFKwThen (nextToken->type==_keyword && nextToken->data.kw==_then)
#define IFKwWrite (nextToken->type==_keyword && nextToken->data.kw==_write)
#define IFIdentif (nextToken->type == _identifier)                                   //TU SA MODLI ABY DENIS NEPOSLAL NULL4
#define IFKwWhile (nextToken->type == _keyword && nextToken->data.kw == _while)
#define IFKwIf (nextToken->type == _keyword && nextToken->data.kw == _if)
#define IFKwGlobal (nextToken->type == _keyword && nextToken->data.kw == _global)
#define IFKwLocal (nextToken->type ==_keyword && nextToken->data.kw==_local)
#define IFKwInteger (nextToken->type == _keyword && nextToken->data.kw == _k_integer)
#define IFKwNumber (nextToken->type == _keyword && nextToken->data.kw == _k_number)
#define IFKwString (nextToken->type == _keyword && nextToken->data.kw == _k_string)
#define IFKwAssign (nextToken->type == _misc && nextToken->data.msc == _assign)
#define IFEof (nextToken.type==_misc && nextToken.data == _EOF)

//Function to free all the allocs - TBD
void FreeTheseBees(TreeSupport *ts){

}
//Function to generate errors - TBD
void GiveMeAnError(int errCode){

}

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
        if (!IFIdentif) return INVALID_TOKEN;
        if (TS_LookFunction(ts,nextToken->data.str)!=NULL) return REDECLARATION_OF_FUNCTION;
        user_func* newData = CreateFunctionData();
        if (newData==NULL)return MALLOC_ERR_CODE;
        if(TS_InsertFunction(ts,nextToken->data.str,w_func,newData)==MALLOC_ERR_CODE)return MALLOC_ERR_CODE;
        NEXT;
        if (!IFDoubleKomma)return INVALID_TOKEN;
        NEXT;
        if (!IFKwFunction)return INVALID_TOKEN;
        NEXT;
        if (!IFLeftBracket)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_Params,nextToken);
        if (test!=0)return test;
        if (!IFRightBracket)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_ParamsR,nextToken);
        if (test != 0) return test;
        return 0;
        
    }
    else if(nextToken->type ==_keyword && nextToken->data.kw == _function){
        NEXT;
        if (!IFIdentif) return INVALID_TOKEN;
        if (TS_LookFunction(ts,nextToken->data.str)!=NULL) return REDECLARATION_OF_FUNCTION;
        user_func* newData = CreateFunctionData();
        if (newData==NULL) return MALLOC_ERR_CODE;
        if(TS_InsertFunction(ts,nextToken->data.str,w_func,newData)==MALLOC_ERR_CODE) return MALLOC_ERR_CODE;
        NEXT;
        if (!IFLeftBracket)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_Params,nextToken);
        if (test != 0) return test;
        if (!IFRightBracket)return INVALID_TOKEN;
        NEXT;
        test = workNotTerminal(N_ParamsR,nextToken);
        if (test!= 0) return test;
        test = workNotTerminal(N_StList,nextToken);
        if (test!= 0) return test;
        if (!IFKwEnd)return INVALID_TOKEN;
        NEXT;
        return 0;

    }
    else if((nextToken->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL)||
            (nextToken->type == _identifier && TS_LookVariable(ts,nextToken->data.str)!=NULL)||
            IFKwWhile||
            IFKwIf)){
        test = workNotTerminal(N_Statement,nextToken);
        if(test!=0) return test;
    }else if(IFEof)
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

int F_Exprb(token* nextToken);

int F_ExprAfterLoc(token* nextToken);

int F_Expression(token* nextToken){
    int test = CallTheCommander(token* nextToken);
    return test;
}

int F_SentPar(token* nextToken);

int F_SPar(token* nextToken);

int F_SExpr(token* nextToken);

int F_SVar(token* nextToken);

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
    case N_Expr_AfterLoc: return F_ExprAfterLoc(token *nextToken); break;
    case N_Expression: return F_Expression(token *nextToken); break;
    case N_Exprb: return F_Exprb(token *nextToken); break;
    case N_SentPar: return F_SentPar(token *nextToken); break;
    case N_SPar: return F_SPar(token *nextToken); break;
    case N_SExpr: return F_SExpr(token *nextToken); break;
    case N_SVar: return F_SVar(token *nextToken); break;    
    
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
    while (!IFEof){
        int temp = workNotTerminal(N_Prog);
        if (temp != 0){
            flag = 1;
            if (temp == MALLOC_ERR_CODE){ 
                mallocErr=1;
                break;
            }else{
                                                                                //Also not sure if this works                                        //DO SOMETHING SIMMILIAR IN ST_LISTE
                while(!((nextToken->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL)||
                        (nextToken->type == _identifier && TS_LookVariable(ts,nextToken->data.str)!=NULL)||
                        (IFKwWhile)||
                        (IFKwIf)||
                        (IFKwGlobal)||
                        (IFKwFunction))){
                    NEXT;
                    if (IFEof)break;
                }
            }

        }
    }
    FreeTheseBees(ts);
}