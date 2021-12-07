
#include "../libs/parser.h"

#define RError(d) fprintf(stderr,"%s %d ",__func__, __LINE__); RaiseError(d); DebbugPrintToken(nextToken); exit(d);

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
#define IFIdentif (nextToken->type == _identifier)                                  
#define IFKwWhile (nextToken->type == _keyword && nextToken->data.kw == _while)
#define IFKwIf (nextToken->type == _keyword && nextToken->data.kw == _if)
#define IFKwGlobal (nextToken->type == _keyword && nextToken->data.kw == _global)
#define IFKwLocal (nextToken->type ==_keyword && nextToken->data.kw==_local)
#define IFKwInteger (nextToken->type == _keyword && nextToken->data.kw == _k_integer)
#define IFKwNumber (nextToken->type == _keyword && nextToken->data.kw == _k_number)
#define IFKwString (nextToken->type == _keyword && nextToken->data.kw == _k_string)
#define IFKwAssign (nextToken->type == _misc && nextToken->data.msc == _assign)
#define IFEof (nextToken->type==_misc && nextToken->data.msc == _EOF)

int param_cter;

//Function to free all the allocs - TBD
void FreeTheseBees(TreeSupport *ts){
    // Bees Knees 
    (void)ts;

}

//DNT
dataType* PushDataType(dataType *toPush,dataType newType){
    int count = 1;
    while (toPush[count-1]!=_ender){
        count+=1;
    }
    dataType* help;
    help = realloc(toPush,sizeof(dataType)*count + 1);
    if (help==NULL)return NULL;
    toPush=help;
    toPush[count-1]=newType;
    toPush[count]=_ender;

    return toPush;
}

char** PushParamName(char** param_names, char* param_name)
{
    char** curAdr=param_names;
    int count = 1;
    while (*curAdr!=NULL){
        count+=1;
        curAdr=curAdr+1;
    }
    param_names = realloc(param_names, (count + 1)* (sizeof(char*)));
    if (param_names == NULL)return NULL; // < so is one here
    param_names[count-1] = malloc(100*sizeof(char));
    if (param_names[count-1] == NULL)
        return NULL;
    if (param_name != NULL)
        strcpy(param_names[count-1], param_name);
    else
        param_names[count-1][0] = '\0';
    param_names[count] = NULL;    
    return param_names;
}
/*
int update_param_name(char* param_name, char** param_names, int position)
{
    int index = 0;
    int max = 0;

    while(param_names[max] != NULL)
        {
            max++;
        }

    if (position < 0)
    {
        while(param_names[index][0] != '\0')
        {
            index++;
            if (index == max )
                break;
        }

        if (param_names[index] == NULL)
            return CONFLICTING_NUMBER_OF_PARAMS;   // define Conflicting number of parameters ' ' new integer 
        
        if (param_names[index][0] == '\0')
            strcpy(param_names[index], param_name);
    }
    else
    {
        if ( position < max )
            strcpy(param_names[position], param_name);
        else
            return INTERNAL_UPDATE_PARAM_NAME_ERROR;   // internal error, should not happen
    }

    return 0;

}
*/
//DNT
user_func* CreateFunctionData(){
    user_func* newData = malloc(sizeof(user_func));
    if (newData == NULL) return NULL;
    newData->params=malloc(sizeof(dataType));
    if(newData->params==NULL){free(newData); return NULL;}

    newData->param_names = malloc (sizeof(char*));
    if ( newData->param_names == NULL )
    {
        free(newData->params);
        free(newData);
        return NULL;
    }

    newData->param_names[0] = malloc (100 * sizeof(char));
    if ( newData->param_names[0] == NULL )
    {
        free(newData->param_names);
        free(newData->params);
        free(newData);
        return NULL;
    }

    newData->returns=malloc(sizeof(dataType));
    if(newData->returns==NULL){free(newData->params);free(newData->param_names);free(newData->param_names[0]);free(newData);return NULL;}
    *(newData->params)=_ender;
    newData->param_names[0] = NULL;
    *(newData->returns)=_ender;
    newData->vDefined=0;
    newData->vUsed=0;
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
void F_Prog(token* nextToken){                                   
    if (nextToken->type==_keyword && nextToken->data.kw==_global){
        param_cter = -1;
        NEXT;
        if (!IFIdentif){RError(2)}//<---------------------------------------_Error
        if (TS_LookFunction(ts,nextToken->data.str)!=NULL){ RError(4)}//<---------------------------------------_Error
        user_func* newData = CreateFunctionData();
        char *newStr = malloc(sizeof(char)*strlen(nextToken->data.str));
        if(newStr==NULL){RError(99)}//<---------------------------------------_Error
        strcpy(newStr,nextToken->data.str);
        if (newData==NULL){free(newStr); RError(99);}//<---------------------------------------_Error
        if(TS_InsertFunction(ts,newStr,w_func,newData)==MALLOC_ERR_CODE){RError(99)}//<---------------------------------------_Error
        TreeElement* func = TS_LookFunction(ts,newStr);
        NEXT;
        if (!IFDoubleKomma){RError(2)}//<---------------------------------------_Error
        NEXT;
        if (!IFKwFunction){RError(2)}//<---------------------------------------_Error
        NEXT;
        if (!IFLeftBracket){RError(2)}//<---------------------------------------_Error
        NEXT;
        F_ParamsGlobal(nextToken, func);
        if (!IFRightBracket){RError(2)}//<---------------------------------------_Error
        NEXT;
        param_cter=-1;
        F_ParamsR(nextToken, newStr,0);
        return;
        
    }
    else if(nextToken->type ==_keyword && nextToken->data.kw == _function){
        param_cter = -1;
        char *newStr;
        int globaled = 0;
        NEXT;
        if (!IFIdentif){RError(2)}//<---------------------------------------_Error
        TreeElement *oldFunc = TS_LookFunction(ts,nextToken->data.str);
        if (oldFunc!=NULL && ((user_func*)oldFunc->data)->vDefined==1) {RError(4)}//<---------------------------------------_Error
        else if (oldFunc!=NULL && ((user_func*)oldFunc->data)->vDefined==0){
            globaled = 1;
            newStr=oldFunc->name;
        }else{
            user_func* newData = CreateFunctionData();
            if (newData==NULL) {RError(99)}//<---------------------------------------_Error
            newStr = malloc(sizeof(char)*strlen(nextToken->data.str));
            if(newStr==NULL){free(newData);RError(99)}//<---------------------------------------_Error
            strcpy(newStr,nextToken->data.str);
            if(TS_InsertFunction(ts,newStr,w_func,newData)==MALLOC_ERR_CODE) {RError(99)}//<---------------------------------------_Error
        }
        TreeElement* func = TS_LookFunction(ts,newStr);
        NEXT;
        if (!IFLeftBracket){RError(2)}//<---------------------------------------_Error
        NEXT;
        F_ParamsFunction(nextToken, func,globaled);
        if (!IFRightBracket){RError(2)}//<---------------------------------------_Error
        NEXT;
        param_cter=-1;
        F_ParamsR(nextToken, newStr,globaled);
        TreeElement* function = TS_LookFunction(ts,newStr);
        if(TS_OpenLayer(ts)!=0){RError(99)}
        int i=0;
        while(((user_func*)function->data)->params[i]!=_ender){
            variable *new=CreateVariableData(((user_func*)function->data)->params[i]);
            if(new==NULL){RError(99)}
            if(TS_InsertVariable(ts,((user_func*)function->data)->param_names[i],((user_func*)function->data)->params[i],new)!=0){free(new); RError(99)}
            i++;
        }

        G_function_bgn(function);//<---------------------------------------------_LK

        F_StList(nextToken,TS_LookFunction(ts,newStr));
        TS_CloseLayer(ts);
        if (!IFKwEnd){RError(2)}//<---------------------------------------_Error
        ((user_func*)func->data)->vDefined=1;                              
        NEXT;
        G_function_end(function->name);
        return;
    }
    else if((nextToken->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL)){
            TreeElement* func = TS_LookFunction(ts,nextToken->data.str);
            ((user_func*)func->data)->vUsed=1;
            NEXT;
            if(!IFLeftBracket){RError(2)}
            NEXT;
            BubbleStack_t paramStack;
            BubbleStack_t returnStack;
            BS_Init(&paramStack);
            BS_Init(&returnStack);
            F_SentPar(nextToken,&paramStack);
            if (!IFRightBracket){BS_Dispose(&paramStack); BS_Dispose(&returnStack);RError(2)}//<---------------------------------------_Error
            NEXT;
            int help = ParamCheck(func,&paramStack);
            if(help!=1){ BS_Dispose(&paramStack);BS_Dispose(&returnStack);RError(5)}
            G_CallFunc(func,&paramStack,&returnStack );//<---------------------------------------------------------_Lukasova funkci
            BS_Dispose(&paramStack);
            if(!BS_IsEmpty(&returnStack)){BS_Dispose(&returnStack); RError(5)}
            BS_Dispose(&returnStack);
            return;
    }
    else if(IFEof)
        return;
    else 
        {RError(2)}
}

void F_ParamsGlobal(token *nextToken,TreeElement* func){
    param_cter++;
    if(IFIdentif){
        char** test = PushParamName(((user_func*)func->data)->param_names, nextToken->data.str);
        if (test == NULL) {RError(99)}
        ((user_func*)func->data)->param_names = test;
        NEXT;
        if (!IFDoubleKomma){RError(2)}
        NEXT;
        if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil ){
            dataType tmp =_ender;
            F_Type(nextToken, &tmp);
            dataType* newParamAddr = PushDataType(((user_func*)func->data)->params,tmp);
            if(newParamAddr==NULL){RError(99)}
            ((user_func*)func->data)->params = newParamAddr;
        }else {
            RError(2)
        }
        NEXT;
        F_SecondParamGlobal(nextToken, func);
    }else
    if (IFKwNil||IFKwInteger||IFKwNumber||IFKwString){
        char** test = PushParamName(((user_func*)func->data)->param_names, NULL);
        if (test == NULL) {RError(99)}
        ((user_func*)func->data)->param_names = test;
        dataType tmp = _ender;
        F_Type(nextToken, &tmp);
        dataType *newParamAddr=PushDataType(((user_func*)func->data)->params,tmp);
        if(newParamAddr==NULL){RError(99)}
        ((user_func*)func->data)->params = newParamAddr;
        NEXT;
        F_SecondParamGlobal(nextToken, func);
    }else
    if(IFRightBracket){
        return;
    }else
    {RError(2)}
}

void F_SecondParamGlobal(token *nextToken,TreeElement* func){
    if ( IfKomma )
    {
        NEXT;
        F_ParamsGlobal(nextToken, func);
    }
    else if( IFRightBracket )
        return;
    else
        {RError(2)}
}

void F_ParamsFunction(token *nextToken,TreeElement* func,int globaled){
    param_cter++;
    if(globaled){
        if(IFIdentif){
            if(((user_func*)func->data)->params[param_cter]==_ender){
                RError(3)
            }else if(((user_func*)func->data)->param_names[param_cter][0]=='\0'){
                strcpy(((user_func*)func->data)->param_names[param_cter],nextToken->data.str);
            }else{
                if(strcmp(((user_func*)func->data)->param_names[param_cter],nextToken->data.str)!=0){RError(3)}
            }
            NEXT;
            if (!IFDoubleKomma){RError(2)}
            NEXT;
            if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil ){
                dataType tmp =_ender;
                F_Type(nextToken, &tmp);
                if(tmp!=((user_func*)func->data)->params[param_cter]){RError(3)}
            }else {
                RError(2)
            }
            NEXT;
            F_SecondParamFunction(nextToken, func,globaled);
        }else if(IFRightBracket){
            return;
        }else{
            RError(2)
        }
    }else{
        if(IFIdentif){
            char** test = PushParamName(((user_func*)func->data)->param_names, nextToken->data.str);
            if (test == NULL) {RError(99)}
            ((user_func*)func->data)->param_names = test;
            NEXT;
            if (!IFDoubleKomma){RError(2)}
            NEXT;
            if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil ){
                dataType tmp =_ender;
                F_Type(nextToken, &tmp);
                dataType* newParamAddr = PushDataType(((user_func*)func->data)->params,tmp);
                if(newParamAddr==NULL){RError(99)}
                ((user_func*)func->data)->params = newParamAddr;
            }else {
                RError(2)
            }
            NEXT;
            F_SecondParamFunction(nextToken, func,globaled);
        }else if(IFRightBracket){
            return;   
        }else{
            RError(2)
        }
    }
}

void F_SecondParamFunction(token *nextToken,TreeElement* func,int globaled){
     if ( IfKomma )
    {
        NEXT
        F_ParamsFunction(nextToken, func,globaled);
    }
    else if( IFRightBracket ){
        if (globaled && ((user_func*)func->data)->params[param_cter+1]!=_ender){RError(3)}
        return;
    }else
        {RError(2)}
}

/*
void F_Params(token* nextToken, char* func_name)
{
    int output = 0;
    char** test;

    TreeElement* func = TS_LookFunction(ts, func_name);
    dataType* param_array = ((user_func*)(func->data))->params;

    if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil )
    {
        param_cter++;
        test = PushParamName(((user_func*)func->data)->param_names, "\0");
        if (test == NULL) {RError(99)}
        ((user_func*)func->data)->param_names = test;
        dataType tmp = _ender;
        F_Type(nextToken, &tmp);
        dataType *newParamAddr=PushDataType(((user_func*)func->data)->params,tmp);
        if(newParamAddr==NULL){RError(99)}
        ((user_func*)func->data)->params = newParamAddr;
        NEXT;
        F_SecondParam(nextToken, func_name);
    }
    else if ( IFIdentif ){
        param_cter++;
        test = PushParamName(((user_func*)func->data)->param_names, NULL);
        if (test == NULL) {RError(99)}
        ((user_func*)func->data)->param_names = test;
        output = update_param_name(nextToken->data.str, ((user_func*)func->data)->param_names, param_cter);
        if(output==INTERNAL_UPDATE_PARAM_NAME_ERROR) {RError(99)}//<---------------------------------------_Error
        else if (output == CONFLICTING_NUMBER_OF_PARAMS){RError(4)}//<---------------------------------------_Error
        NEXT;
        if (!IFDoubleKomma){RError(2)}
        NEXT;
        if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil ){
            dataType tmp =_ender;
            F_Type(nextToken, &tmp);
            dataType* newParamAddr = PushDataType(param_array,tmp);
            if(newParamAddr==NULL){RError(99)}
            ((user_func*)func->data)->params = newParamAddr;
        }else {
            RError(2)
        }
        NEXT;
        F_SecondParam(nextToken, func_name);
    }else
    {
        if ( !IFRightBracket )
            {RError(2)}
    }
}*/

void F_ParamsR(token *nextToken, char* name_func,int globaled)
{   
    param_cter++;
    if ( IFDoubleKomma )
    {
        NEXT
        TreeElement* func = TS_LookFunction(ts, name_func);
        dataType* return_array =  ((user_func*)func->data)->returns;

        if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil )
        {
            dataType tmp= _ender;
            F_Type(nextToken, &tmp);
            if(!globaled){
                return_array=PushDataType(return_array,tmp);
                if(return_array==NULL){RError(99)}
                ((user_func*)func->data)->returns = return_array;
            }else{
                if(tmp!=return_array[param_cter]){RError(3)}
            }
            NEXT;
            F_SecondParamR(nextToken, name_func,globaled);
            return;
        }
        else
            {RError(2)}
    }
    else
        if ( IFKwGlobal || IFKwFunction || IFIdentif || IFKwLocal || IFKwWhile || IFKwIf || IFKwEnd || IFKwReturn)
            return;
    else
        {RError(2)}
}
/*
void F_SecondParam(token* nextToken, char* name_func)
{ 
    if ( IfKomma )
    {
        NEXT
        F_Params(nextToken, name_func);
    }
    else if( IFRightBracket )
        return;
    else
        {RError(2)}
}
*/

void F_SecondParamR(token* nextToken, char* name_func,int  globaled)
{
    param_cter++;
    if ( IfKomma )
    {
        NEXT;
        TreeElement* func = TS_LookFunction(ts, name_func);
        dataType* return_array =  ((user_func*)func->data)->returns;

        if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil )
        {
            dataType temp=_ender;
            F_Type(nextToken, &temp);
            if(!globaled){
                return_array = PushDataType(return_array,temp);
                if(return_array==NULL){RError(99)}
                ((user_func*)func->data)->returns = return_array;
            }else{
                if(return_array[param_cter]!=temp){RError(3)}
            }
            NEXT;
            F_SecondParamR(nextToken, name_func,globaled);

        }else
            {RError(2)}   
    }
    else 
        return;
}


void F_Type(token* nextToken, dataType* array)
{
    if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil )
    {
        switch(nextToken->data.kw)
        {
            case _k_string:
                *array=_string;
                break;
            case _k_number:
                *array=_number;
                break;
            case _k_integer:
                *array=_integer;
                break;
            case _nil:
                *array=_nan;
                break;
            default:
                RError(2) break;
        }
        return;
    }else
    {RError(2)}

}

void F_StList(token* nextToken,TreeElement* curFunc)
{
    if ( IFKwEnd || IFKwElse )
        return;
    else
    {
        if ( IFIdentif || IFKwLocal || IFKwWhile || IFKwIf || IFKwReturn ){ 
            F_Statement(nextToken,curFunc);
            F_StList(nextToken,curFunc);
            return;
        }else
            {
                RError(2)}
    }
}

void F_Statement(token* nextToken, TreeElement* curFunc){
    if(IFKwWhile){
        while_counter+=1;
        int thisWhile = while_counter;
        NEXT;
        G_WhileBGN();//<---------------------------------------------------------------------LF
        expression_block *ret=F_Expression(nextToken);
        if (ret->dt==_bool){
            G_CompareBool(ret);//<------------------------------------------------------------LF
        }else{
            G_CompareNull(ret);//<--------------------------------------------------------------LF
        }
        free(ret);
        if(!IFKwDo){RError(2)}//<-------------------------------------------------Error
        NEXT;
        F_StList(nextToken,curFunc);
        if(!IFKwEnd){RError(2)}//<-------------------------------------------------Error
        G_WhileEND(thisWhile);//<------------------------------------------------------------------------LF
        NEXT;
        return;

    }else if(IFKwIf){
        if_counter+=1;
        int thisIf = if_counter;
        NEXT;
        expression_block *ret=F_Expression(nextToken);
        G_IfBGN(ret);//<---------------------------------------------LF
        free(ret);
        NEXT;
        if(TS_OpenLayer(ts)!=0){RError(99)}
        F_StList(nextToken,curFunc);
        TS_CloseLayer(ts);
        G_IfELSE(thisIf);
        if (IFKwElse)
            F_Else(nextToken,curFunc);
        G_IfEND(thisIf); //<----------------------------------------------------------LF
        if(!IFKwEnd){RError(2)}//<-------------------------------------------------Error
        NEXT;
        return;

    }else if(IFKwLocal){
        NEXT;
        if(!IFIdentif){RError(2)}//<----------------------------------------------Error
        if(TS_LookLayerVariable(ts,nextToken->data.str)!=NULL){RError(3)}//<----------------------------------------------Error
        if(TS_LookFunction(ts,nextToken->data.str)!=NULL){RError(3)}//<----------------------------------------------Error
        char* newVarName = malloc((strlen(nextToken->data.str)+1)*sizeof(char));
        if (newVarName==NULL){RError(99)}//<---------------------------------------_Error
        strcpy(newVarName,nextToken->data.str);
        NEXT;
        if(!IFDoubleKomma){free(newVarName); RError(2)}//<---------------------------------------_Error
        NEXT;
        dataType type=_ender;
        F_Type(nextToken,&type);
        variable* newData = CreateVariableData(type);
        if(newData==NULL){free(newVarName); RError(99)}//<---------------------------------------_Error
        if (TS_InsertVariable(ts,newVarName,w_var,newData) != 0) {free(newVarName); RError(99)}//<---------------------------------------_Error
        def_var(TS_LookVariable(ts,newVarName));
        NEXT;
        if (IFKwAssign)
            F_ExprAfterLoc(nextToken,TS_LookVariable(ts,newVarName));
        return;
    }else if(IFKwReturn){
        NEXT;
        BubbleStack_t returnStack;
        BS_Init(&returnStack);
        if(stack_err_flag!=0){RError(99)}
        Returner(nextToken,&returnStack);
        if(ReturnsCheck(curFunc,&returnStack)!=1){RError(6)}//<---------------------------------------_Error
        G_RetrunTerm(&returnStack,curFunc->name);
        BS_Dispose(&returnStack);
        return;
    }else if(IFIdentif && TS_LookFunction(ts,nextToken->data.str)!=NULL){
        TreeElement* func=TS_LookFunction(ts,nextToken->data.str);
        ((user_func*)func->data)->vUsed=1;
        NEXT;
        if(!IFLeftBracket){RError(2)}
        NEXT;
        BubbleStack_t paramStack;
        BubbleStack_t returnStack;
        BubbleStack_t helperS;
        BS_Init(&paramStack);
        if(stack_err_flag){RError(99)}
        BS_Init(&returnStack);
        if(stack_err_flag){RError(99)}
        BS_Init(&helperS);
        if(stack_err_flag){RError(99)}
        F_SentPar(nextToken,&paramStack);
        if (!IFRightBracket){BS_Dispose(&paramStack); BS_Dispose(&returnStack);BS_Dispose(&helperS); RError(2)}//<---------------------------------------_Error
        NEXT;
        int help = ParamCheck(func,&paramStack);
        if(help!=1){RError(5)}

        while(!BS_IsEmpty(&paramStack)){
            expression_block* rr = BS_TopStack(&paramStack);
            BS_Push(&helperS,rr);
            BS_Pop(&paramStack);
        }
        G_CallFunc(func,&helperS,&returnStack );//<---------------------------------------------------------_Lukasova funkci
        BS_Dispose(&paramStack);
        BS_Dispose(&returnStack);
        BS_Dispose(&helperS);
        return;
    }else if(IFIdentif && TS_LookVariable(ts,nextToken->data.str)!=NULL){
        TreeElement **vl=VL_INIT();
        if(vl==NULL){RError(99)}//<---------------------------------------_Error
        vl=VL_PUSH(vl,TS_LookVariable(ts,nextToken->data.str));
        if(vl==NULL){RError(99)}//<---------------------------------------_Error
        F_SVar(nextToken,&vl);
        if(!IFKwAssign){VL_Dispose(vl); RError(2) }//<---------------------------------------_Error
        NEXT;
        BubbleStack_t returnStack;
        BS_Init(&returnStack);
        if(stack_err_flag!=0){RError(99)}
        F_Exprb(nextToken,&returnStack);
        F_SExpr(nextToken, &returnStack);
        int help = SemanticCheck(vl,&returnStack);
        if(help)
            G_AssignToVars(vl,&returnStack);//<----------------------------------------------------------------------LK
        else
            {RError(4)}
        BS_Dispose(&returnStack);
        VL_Dispose(vl);
        return;
    }else{
        RError(2)
    }
}

void F_Else(token* nextToken,TreeElement* curFunc){
    NEXT;
    if(TS_OpenLayer(ts)!=0){RError(99)}
    F_StList(nextToken,curFunc);
    TS_CloseLayer(ts);
}

void Returner(token* nextToken,BubbleStack_t *returnStack){
    if((IFIdentif && (TS_LookVariable(ts,nextToken->data.str)!=NULL  || TS_LookFunction(ts,nextToken->data.str)!=NULL))|| (nextToken->type==_keyword && nextToken->data.kw == _nil)||IFLeftBracket|| nextToken->type==_const){
        if(stack_err_flag!=0){RError(99)}
        F_Exprb(nextToken,returnStack);
        F_SExpr(nextToken, returnStack);
    }
}

void F_Exprb(token* nextToken,BubbleStack_t *stack){
    if(IFIdentif && TS_LookFunction(ts,nextToken->data.str)!= NULL){
        TreeElement* func = TS_LookFunction(ts,nextToken->data.str);
        ((user_func*)func->data)->vUsed=1;
        NEXT;
        if(!IFLeftBracket){BS_Dispose(stack); RError(2)}//<-----------------------------------------------------___Error
        NEXT;
        BubbleStack_t paramStack;
        BS_Init(&paramStack);
        if(stack_err_flag!=0){BS_Dispose(&paramStack);RError(99)}
        F_SentPar(nextToken,&paramStack);
        if(ParamCheck(func,&paramStack)==0){BS_Dispose(&paramStack);RError(3)}
        BubbleStack_t mockStack;
        BS_Init(&mockStack);
        if(stack_err_flag){RError(99)}
        if(!IFRightBracket){BS_Dispose(stack); RError(2)}
        NEXT;
        int help = ParamCheck(func,&paramStack);
        if(help==0){RError(5)}
        help = G_CallFunc(func, &paramStack, &mockStack);
        BS_Dispose(&paramStack);
        if(help!=0){BS_Dispose(stack); BS_Dispose(&mockStack); RError(99)}//<-------------------------------------------------Error
        help=ReturnsCheck(func,&mockStack);
        if(help!=1){BS_Dispose(stack); BS_Dispose(&mockStack); RError(5)} //<--------------------------------------------------Error invalid return

        BubbleStack_t help2;
        BS_Init(&help2);
        while (!BS_IsEmpty(&mockStack))
        {
            BS_Push(&help2,BS_TopStack(&mockStack));
            BS_Pop(&mockStack);
        }
        
        MoveStack(stack,&help2);
        BS_Dispose(&mockStack);
        BS_Dispose(&help2);
        
        return;

    }else if((IFIdentif && TS_LookVariable(ts,nextToken->data.str)!=NULL) || IFLeftBracket || nextToken->type==_const || (nextToken->type==_keyword && nextToken->data.kw==_nil)){
        expression_block* result = F_Expression(nextToken);
        if (result == NULL){BS_Dispose(stack); RError(99)}//<-------------------------------------------------Error
        else{
            BS_Push(stack,result);
            if (stack_err_flag!=0){BS_Dispose(stack); RError(99)}//<-------------------------------------------------Error
            return;
        }
    }else if(IFIdentif){
        RError(3)//<-------------------------------------------------Error 
    }else{RError(2)}//<-------------------------------------------------Error
}

void F_ExprAfterLoc(token* nextToken,TreeElement* targetVar){
    if (IFKwAssign){
        NEXT;
        if (IFIdentif){
            if(TS_LookFunction(ts,nextToken->data.str)!=NULL){
                TreeElement* func = TS_LookFunction(ts,nextToken->data.str);
                NEXT;
                if(!IFLeftBracket){RError(2)}       //<-------------------------------------------------Error
                NEXT;
                BubbleStack_t stack;
                BS_Init(&stack);
                F_SentPar(nextToken,&stack);
                if(!IFRightBracket){BS_Dispose(&stack); RError(2)}//<-------------------------------------------------Error
                NEXT;
                ((variable*)targetVar->data)->vDefined=1;
                TreeElement** vl = VL_INIT();
                if(vl==NULL){BS_Dispose(&stack); RError(99)}//<-------------------------------------------------Error
                vl=VL_PUSH(vl,targetVar);
                if(vl==NULL){BS_Dispose(&stack); RError(99)}//<-------------------------------------------------Error
                BubbleStack_t mockStack;
                BS_Init(&mockStack);
                int help = ParamCheck(func,&stack);
                if(help!=1){RError(5)}
                help = G_CallFunc(func,&stack,&mockStack);//<--------------------------------------------------------------LK
                if(help!=0){BS_Dispose(&mockStack);BS_Dispose(&stack);RError(99)}//<--------------------------------------------Error
                if(SemanticCheck(vl,&mockStack)!=1){BS_Dispose(&mockStack);BS_Dispose(&stack);RError(4)}//<--------------------------Error
                G_AssignToVar(targetVar,BS_TopStack(&mockStack));
                BS_Dispose(&mockStack);
                BS_Dispose(&stack);
                return;       
            }else{
                expression_block* block=F_Expression(nextToken);
                if (C_AssignToVar(targetVar,block)){
                    G_AssignToVar(targetVar,block);//<----------------------------------------------------LK
                    ((variable*)targetVar->data)->vDefined=1;
                    return;
                }else{
                    RError(4)//<------------------------------------------------------------------------------Error
                }
            }
        }else if (nextToken->type==_const || (nextToken->type==_misc && nextToken->data.msc==_bracketL)||(nextToken->type==_operator && nextToken->data.oper==_length)|| (nextToken->type == _keyword && nextToken->data.kw==_nil)){
            expression_block* block=F_Expression(nextToken);
                if (C_AssignToVar(targetVar,block)){
                    G_AssignToVar(targetVar,block);//<------------------------------------------------------------___LK
                    ((variable*)targetVar->data)->vDefined=1;
                    return;
                }else{
                    RError(4)// <-------------------------------------------------------------Error
                }
        }else
            {RError(2)}//<----------------------------------------------------------------------Error
    }
    else{
        return;
    }
}

expression_block* F_Expression(token* nextToken){
    return CallTheCommander(nextToken);
}

void F_SExpr(token* nextToken, BubbleStack_t * stack){
    if(nextToken->type==_misc && nextToken->data.msc==_komma){
        NEXT;
        F_Exprb(nextToken,stack);
        F_SExpr(nextToken,stack);
    }
}

void F_SentPar(token* nextToken,BubbleStack_t *stack){
    if(IFRightBracket){return;}
    F_Exprb(nextToken,stack);
    F_SPar(nextToken,stack);
    return;
}

void F_SPar(token* nextToken, BubbleStack_t *stack){
    if(IFRightBracket){return;}
    if(!IfKomma){BS_Dispose(stack); RError(2)}//<-------------------------------------------------Error
    NEXT;
    F_Exprb(nextToken,stack);
    F_SPar(nextToken,stack);
    return;
}

void F_SVar(token* nextToken,TreeElement ***vl){
    NEXT;
    if(IfKomma){
        NEXT;
        if(IFIdentif && TS_LookVariable(ts,nextToken->data.str)!=NULL){
            *vl=VL_PUSH(*vl,TS_LookVariable(ts,nextToken->data.str));
            if(*vl == NULL){RError(99)}//<-------------------------------------------------Error
            F_SVar(nextToken,vl);
            return;
        }
        else 
        {
            VL_Dispose(*vl);
            RError(2)//<-------------------------------------------------Error
        }
    }else{ 
        return;
    }
}

void ashes_to_ashes_dust_to_dust()
{
    //Can I interest you in everything all of the time?
    // free ( everything and anything and everything and anything and everything and anything and everything and anything amd all of the time )
    
    FreeTheseBees(ts);
    //dtroy_variable_name_catcher();
    return;
}

int PushBuiltInFunctions(){
    //readi
    user_func* new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    new->returns = PushDataType( new->returns,_integer);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    char *readi = malloc(sizeof(char)*6);
    if(readi==NULL)return MALLOC_ERR_CODE;
    strcpy(readi,"readi");
    int help = TS_InsertFunction(ts,readi,w_func,new);
    if (help!=0){free(readi);free(new);return MALLOC_ERR_CODE;}
    //readn
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    new->returns = PushDataType( new->returns,_number);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    char *readn = malloc(sizeof(char)*6);
    if(readn==NULL)return MALLOC_ERR_CODE;
    strcpy(readn,"readn");
    help = TS_InsertFunction(ts,readn,w_func,new);
    if (help!=0){free(readn);free(new);return MALLOC_ERR_CODE;}
    //reads
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    new->returns = PushDataType( new->returns,_string);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    char *reads = malloc(sizeof(char)*6);
    if(reads==NULL)return MALLOC_ERR_CODE;
    strcpy(reads,"reads");
    help = TS_InsertFunction(ts,reads,w_func,new);
    if (help!=0){free(reads);free(new);return MALLOC_ERR_CODE;}
    //toInt
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    new->returns = PushDataType( new->returns,_integer);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    new->params = PushDataType( new->params,_number);
    if(new->params==NULL){free(new); return MALLOC_ERR_CODE;}
    char *tointeger = malloc(sizeof(char)*10);
    if(tointeger==NULL)return MALLOC_ERR_CODE;
    strcpy(tointeger,"tointeger");
    help = TS_InsertFunction(ts,tointeger,w_func,new);
    if (help!=0){free(tointeger);free(new);return MALLOC_ERR_CODE;}
    //substr
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    new->returns = PushDataType( new->returns,_string);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    new->params = PushDataType( new->params,_string);
    if(new->params==NULL){free(new); return MALLOC_ERR_CODE;}
    new->params = PushDataType( new->params,_integer);
    if(new->params==NULL){free(new); return MALLOC_ERR_CODE;}
    new->params = PushDataType( new->params,_number);//<----------------------------------_NOT SURE ABOUT THIS
    if(new->params==NULL){free(new); return MALLOC_ERR_CODE;}
    char *substr = malloc(sizeof(char)*7);
    if(substr==NULL)return MALLOC_ERR_CODE;
    strcpy(substr,"substr");
    help = TS_InsertFunction(ts,substr,w_func,new);
    if (help!=0){free(substr);free(new);return MALLOC_ERR_CODE;}
    //ord
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    new->returns = PushDataType( new->returns,_integer);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    new->params = PushDataType( new->params,_string);
    if(new->params==NULL){free(new); return MALLOC_ERR_CODE;}
    new->params = PushDataType( new->params,_integer);
    if(new->params==NULL){free(new); return MALLOC_ERR_CODE;}
    char *ord = malloc(sizeof(char)*4);
    if(ord==NULL)return MALLOC_ERR_CODE;
    strcpy(ord,"ord");
    help = TS_InsertFunction(ts,ord,w_func,new);
    if (help!=0){free(ord);free(new);return MALLOC_ERR_CODE;}
    //chr
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    new->returns = PushDataType( new->returns,_string);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    new->params = PushDataType( new->params,_integer);
    if(new->params==NULL){free(new); return MALLOC_ERR_CODE;}
    char *chr = malloc(sizeof(char)*4);
    if(chr==NULL)return MALLOC_ERR_CODE;
    strcpy(chr,"chr");
    help = TS_InsertFunction(ts,chr,w_func,new);
    if (help!=0){free(chr);free(new);return MALLOC_ERR_CODE;}
    //write
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->vDefined=1;
    char *write = malloc(sizeof(char)*6);
    if(write==NULL)return MALLOC_ERR_CODE;
    strcpy(write,"write");
    help = TS_InsertFunction(ts,write,w_func,new);
    if (help!=0){free(write);free(new);return MALLOC_ERR_CODE;}
    return 0;
    //TODO - others
}

void key_arr_init(){
    strcpy(arr_keywords[0],"do");
    strcpy(arr_keywords[1],"else");
    strcpy(arr_keywords[2],"end");
    strcpy(arr_keywords[3],"function");
    strcpy(arr_keywords[4],"global");
    strcpy(arr_keywords[5],"if");
    strcpy(arr_keywords[6],"local");
    strcpy(arr_keywords[7],"nil");
    strcpy(arr_keywords[8],"read");
    strcpy(arr_keywords[9],"require");
    strcpy(arr_keywords[10],"return");
    strcpy(arr_keywords[11],"then");
    strcpy(arr_keywords[12],"while");
    strcpy(arr_keywords[13],"write");
    strcpy(arr_keywords[14],"integer");
    strcpy(arr_keywords[15],"number");
    strcpy(arr_keywords[16],"string");            
}

void initGlobals(){
    generate_code = 1;
    stack_err_flag=0;
    lex_err_flag=0;
    execute_counter=0;
    while_counter=0;
    if_counter=0;
    logic_counter=0;
    line_cnt=1;
    exprCounter = 0.0;
    key_arr_init();
}

int flag = 0;
int mainParseFunction(){
    initGlobals();
    atexit( ashes_to_ashes_dust_to_dust );
    generate_header();
    TS_Init(&ts);
    token* nextToken = (token*) malloc(sizeof(token));
    if(nextToken==NULL){RError(99)}
    if(PushBuiltInFunctions()!=0){free(nextToken); RError(99)}
    NEXT;
    if(nextToken->type==_keyword && nextToken->data.kw==_require){
        NEXT;
        if (nextToken->type!=_const||nextToken->data.type!=_string||strcmp(nextToken->data.str,"ifj21")!=0) {RError(7)}
        NEXT;
    }
    if(IFEof){RError(2)}
    while (!IFEof){
        F_Prog(nextToken);
    }
    TS_FunctionCaller();
    FreeTheseBees(ts);
    return 0;
}