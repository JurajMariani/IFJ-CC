
#include "../libs/parser.h"

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
#define IFEof (nextToken->type==_misc && nextToken->data.msc == _EOF)

int param_cter;

//Function to free all the allocs - TBD
void FreeTheseBees(TreeSupport *ts){
    // Bees Knees 
    (void)ts;

}
//Function to generate errors - TBD
void GiveMeAnError(int errCode){
    (void) errCode;
}

//DNT
dataType* PushDataType(dataType *toPush,dataType newType){
    int count = 1;
    while (toPush[count-1]==_ender){
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
    while (curAdr!=NULL){
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
        param_cter = -1;
        NEXT;
        if (!IFIdentif){INVALID_TOKEN}
        if (TS_LookFunction(ts,nextToken->data.str)!=NULL) return REDECLARATION_OF_FUNCTION;
        user_func* newData = CreateFunctionData();
        char *newStr = malloc(sizeof(char)*strlen(nextToken->data.str));
        if(newStr==NULL)return MALLOC_ERR_CODE;
        strcpy(newStr,nextToken->data.str);
        if (newData==NULL){free(newStr); return MALLOC_ERR_CODE;}
        if(TS_InsertFunction(ts,newStr,w_func,newData)==MALLOC_ERR_CODE)return MALLOC_ERR_CODE;
        NEXT;
        if (!IFDoubleKomma){INVALID_TOKEN}
        NEXT;
        if (!IFKwFunction){INVALID_TOKEN}
        NEXT;
        if (!IFLeftBracket){INVALID_TOKEN}
        NEXT;
        test = F_Params(nextToken, newStr);
        //test = workNotTerminal(N_Params,nextToken);
        if (test!=0)return test;
        if (!IFRightBracket){INVALID_TOKEN}
        NEXT;
        //test = workNotTerminal(N_ParamsR,nextToken);
        test = F_ParamsR(nextToken, newStr);
        if (test != 0) return test;
        // TreeElement* function = TS_LookFunction(ts,name_func);   <- uncomment when CodeGen ready
        //G_function_bgn(function);                                 <- uncomment when CodeGen ready 
        return 0;
        
    }
    else if(nextToken->type ==_keyword && nextToken->data.kw == _function){
        param_cter = -1;
        NEXT;
        if (!IFIdentif){INVALID_TOKEN}
        TreeElement *oldFunc = TS_LookFunction(ts,nextToken->data.str);
        if (oldFunc!=NULL && ((user_func*)oldFunc->data)->vDefined==1) return REDECLARATION_OF_FUNCTION;
        else if (oldFunc!=NULL && ((user_func*)oldFunc->data)->vDefined==1){
            printf("pls");
        }////////////////////////////////////////////////////////////////////////////////////////////TODO
        user_func* newData = CreateFunctionData();
        if (newData==NULL) return MALLOC_ERR_CODE;
        char *newStr = malloc(sizeof(char)*strlen(nextToken->data.str));
        if(newStr==NULL){free(newData);return MALLOC_ERR_CODE;}
        strcpy(newStr,nextToken->data.str);
        if(TS_InsertFunction(ts,newStr,w_func,newData)==MALLOC_ERR_CODE) return MALLOC_ERR_CODE;
        char* name_func = nextToken->data.str;
        NEXT;
        if (!IFLeftBracket){INVALID_TOKEN}
        NEXT;
        //test = workNotTerminal(N_Params,nextToken);
        test = F_Params(nextToken, name_func);
        if (test != 0) return test;
        if (!IFRightBracket){INVALID_TOKEN}
        NEXT;
        //test = workNotTerminal(N_ParamsR,nextToken);
        test = F_ParamsR(nextToken, name_func);
        if (test!= 0) return test;
        
        test = F_StList(nextToken);
        if (test!= 0) return test;
        if (!IFKwEnd){INVALID_TOKEN}
        // TreeElement* function = TS_LookFunction(ts,name_func);   <- uncomment when CodeGen ready
        //G_function_bgn(function);
        newData->vDefined=1;                                 //<- uncomment when CodeGen ready 
        NEXT;
        return 0;

    }
    else 
        if((nextToken->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL))
        {
            TreeElement* func = TS_LookFunction(ts,nextToken->data.str);
            NEXT;
            if(!IFLeftBracket){INVALID_TOKEN;}
            NEXT;
            if(!IFRightBracket){INVALID_TOKEN;}
            NEXT;
            // Fill params & returns                                    <- TODO + uncomment when CodeGent ready
            // G_CallFunc(nextToken->data.str, #params#, #returns#);    <- TODO + uncomment when CodeGent ready
        }
        else 
            if(IFEof)
                return DONE;
            else 
            {
                INVALID_TOKEN
            }
    return 0;
}


int F_Params(token* nextToken, char* func_name)
{ 
    int output = 0;
    char** test;

    TreeElement* func = TS_LookFunction(ts, func_name);
    dataType* param_array = ((user_func*)(func->data))->params;

    if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil )
    {
        param_cter++;
        test = PushParamName(((user_func*)func->data)->param_names, NULL);
        if (test == NULL)
            return MALLOC_ERR_CODE;
        ((user_func*)func->data)->param_names = test;
        dataType tmp = _ender;
        F_Type(nextToken, &tmp);
        

        ((user_func*)func->data)->params = param_array;

        NEXT
        output = F_SecondParam(nextToken, func_name);
        if (!output)
            return 0;
    }
    else
        if ( IFIdentif )
        {
            param_cter++;
            test = PushParamName(((user_func*)func->data)->param_names, NULL);
            if (test == NULL)
                return MALLOC_ERR_CODE;
            ((user_func*)func->data)->param_names = test;
            output = update_param_name(nextToken->data.str, ((user_func*)func->data)->param_names, param_cter);
            if ((output == INTERNAL_UPDATE_PARAM_NAME_ERROR ) || (output == CONFLICTING_NUMBER_OF_PARAMS))
                return output;
            NEXT
            if (!IFDoubleKomma)
            {
                INVALID_TOKEN
            }
            NEXT
            if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil ){
                dataType tmp =_ender;
                F_Type(nextToken, &tmp);
                param_array = PushDataType(param_array,tmp);
                if(param_array==NULL)return MALLOC_ERR_CODE;
            }else
            {
                INVALID_TOKEN
            }

            ((user_func*)func->data)->params = param_array;
            
            NEXT
            output = F_SecondParam(nextToken, func_name);
            if (output)
                return output;
        }
        else
        {
            if ( IFRightBracket )
                return 0;
            else
            {
                INVALID_TOKEN
            }
        }

    INVALID_TOKEN
}

int F_ParamsR(token *nextToken, char* name_func)
{
    int output = 0;
    if ( IFDoubleKomma )
    {
        NEXT
        TreeElement* func = TS_LookFunction(ts, name_func);
        dataType* return_array =  ((user_func*)func->data)->returns;

        if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil )
        {
            dataType tmp= _ender;
            output = F_Type(nextToken, &tmp);
            if(output!=0){INVALID_TOKEN;}
            return_array=PushDataType(return_array,tmp);
            if(return_array==NULL)return MALLOC_ERR_CODE;
             ((user_func*)func->data)->returns = return_array;
            NEXT
            return(F_SecondParamR(nextToken, name_func));
        }
    }
    else
        if ( IFKwGlobal || IFKwFunction || IFIdentif || IFKwLocal || IFKwWhile || IFKwIf )
            return 0;
        else
        {
            INVALID_TOKEN
        }
    INVALID_TOKEN
}

int F_SecondParam(token* nextToken, char* name_func)
{ 
    if ( IfKomma )
    {
        NEXT
        F_Params(nextToken, name_func);
    }
    else
        if( IFRightBracket )
            return 0;
        else
        {
            INVALID_TOKEN
        }
    INVALID_TOKEN
}


int F_SecondParamR(token* nextToken, char* name_func)
{
    int output = 0;

    if ( IfKomma )
    {
        NEXT
        TreeElement* func = TS_LookFunction(ts, name_func);
        dataType* return_array =  ((user_func*)func->data)->returns;

        if ( IFKwInteger || IFKwString || IFKwNumber || IFKwNil )
        {
            dataType temp=_ender;
            output = F_Type(nextToken, &temp);
            if(output!=0){INVALID_TOKEN;}
            return_array = PushDataType(return_array,temp);
            if(return_array==NULL)return MALLOC_ERR_CODE;

             ((user_func*)func->data)->returns = return_array;
            NEXT

            output = F_SecondParamR(nextToken, name_func);

        }
    }
    else
        if( IFKwGlobal || IFKwFunction || IFIdentif || IFKwLocal || IFKwWhile || IFKwIf )
            return 0;
        else
        {
            INVALID_TOKEN
        }
    INVALID_TOKEN
}


int F_Type(token* nextToken, dataType* array)
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
                *array=_nil;
                break;
            default:
                return 1;
        }
        return 0;
    }
    INVALID_TOKEN

}

int F_StList(token* nextToken)
{
    if ( IFKwEnd || IFKwElse )
        return 0;
    else
    {
        if ( IFIdentif || IFKwLocal || IFKwWhile || IFKwIf || IFKwReturn )
        { 
            int output=0;
            output = F_Statement(nextToken);
            if (output == MALLOC_ERR_CODE)
                exit(99);
            output=F_StList(nextToken);
            if (output == MALLOC_ERR_CODE)
                exit(99);
            else return 0;
        }
    }
    INVALID_TOKEN
}

int F_Statement(token* nextToken){
    if(IFKwWhile){
        while_counter+=1;
        NEXT;
        //G_WhileBGN();<---------------------------------------------------------------------LF
        expression_block *ret=F_Expression(nextToken);
        if(ret == NULL) return MALLOC_ERR_CODE;
        else if (ret->operType==_std_error)generate_code=0;
        else if (ret->dt==_bool){
            //G_CompareBool(ret);<------------------------------------------------------------LF
        }else{
            //G_CompareNull(ret);<--------------------------------------------------------------LF
        }
        free(ret);
        if(!IFKwDo){INVALID_TOKEN}
        NEXT;
        int test = F_StList(nextToken);
        if(test!=0)return test;
        if(!IFKwEnd){INVALID_TOKEN}
        //G_WhileEND();<------------------------------------------------------------------------LF
        NEXT;
        return 0;

    }else if(IFKwIf){
        if_counter+=1;
        NEXT;
        expression_block *ret=F_Expression(nextToken);
        if (ret == NULL){ return MALLOC_ERR_CODE;}
        if(ret->operType==_std_error)generate_code=0;
        //G_IfBGN(ret);
        free(ret);
        NEXT;
        int help = F_StList(nextToken);
        if(help!=0)return help;
        if (IFKwElse){
            help = F_Else(nextToken);
            if(help!=0)return help;
        }else{
            //G_IfELSE();<----------------------------------------------------------LF
        }
        //G_IfEND();<----------------------------------------------------------LF
        if(!IFKwEnd){INVALID_TOKEN;}
        NEXT;
        return 0;

    }else if(IFKwLocal){
        int help;
        NEXT;
        if(!IFIdentif){INVALID_TOKEN}
        if(TS_LookLayerVariable(ts,nextToken->data.str)!=NULL)return REDECLARATION_OF_VARIABLE;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! We should check the rest thou
        char* newVarName = malloc((strlen(nextToken->data.str)+1)*sizeof(char));
        if (newVarName==NULL)return MALLOC_ERR_CODE;
        strcpy(newVarName,nextToken->data.str);
        NEXT;
        if(!IFDoubleKomma){free(newVarName); INVALID_TOKEN;}
        NEXT;
        dataType type=_ender;
        int test = F_Type(nextToken,&type);
        if (test != 0)return MALLOC_ERR_CODE;
        variable* newData = CreateVariableData(type);
        if(newData==NULL){free(newVarName); return MALLOC_ERR_CODE;}
        test = TS_InsertVariable(ts,newVarName,w_var,newData);
        if (test == MALLOC_ERR_CODE) {free(newVarName); return MALLOC_ERR_CODE;}
        NEXT;
        if (IFKwAssign){
            help = F_ExprAfterLoc(nextToken,TS_LookVariable(ts,newVarName));
            if(help!=0){return help;}
        }
        return 0;
    }else if(IFKwReturn){

        NEXT;
        BubbleStack_t returnStack;
        BS_Init(&returnStack);
        int help=F_Exprb(nextToken,&returnStack);
        if(help!=0){BS_Dispose(&returnStack); return help;}
        help = F_SExpr(nextToken,&returnStack);
        if(help!=0){BS_Dispose(&returnStack);return help;}
        //void G_RETUR_TERM(BubbleStack_t *)<----------Dole je prva polozka--------------------------------------------------------------------nova lukasova
        BS_Dispose(&returnStack);
        return 0;
    }else if(IFIdentif && TS_LookFunction(ts,nextToken->data.str)!=NULL){
        TreeElement* func=TS_LookFunction(ts,nextToken->data.str);
        NEXT;
        if(!IFLeftBracket){INVALID_TOKEN}
        NEXT;
        BubbleStack_t paramStack;
        BubbleStack_t returnStack;
        BS_Init(&paramStack);
        BS_Init(&returnStack);
        int help = F_SentPar(nextToken,&paramStack);
        if (help!=0){BS_Dispose(&paramStack); BS_Dispose(&returnStack); return help;}
        if (!IFRightBracket){BS_Dispose(&paramStack); BS_Dispose(&returnStack); return help;}
        NEXT;
        //G_CallFunc(TreeElement* func,&paramStack,&returnStack );<---------------------------------------------------------_Lukasova funkci
        BS_Dispose(&paramStack);
        BS_Dispose(&returnStack);
        return 0;
    }else if(IFIdentif && TS_LookVariable(ts,nextToken->data.str)!=NULL){
        TreeElement **vl=VL_INIT();
        if(vl==NULL)return MALLOC_ERR_CODE;
        vl=VL_PUSH(vl,TS_LookVariable(ts,nextToken->data.str));
        if(vl==NULL)return MALLOC_ERR_CODE;
        int help = F_SVar(nextToken,&vl);
        if(help==MALLOC_ERR_CODE || help==INVALID_TOKEN_CONST_CODE){VL_Dispose(vl);return MALLOC_ERR_CODE;}
        if(!IFKwAssign){VL_Dispose(vl); INVALID_TOKEN }
        NEXT;
        BubbleStack_t returnStack;
        BS_Init(&returnStack);
        help=F_Exprb(nextToken,&returnStack);
        if(help==MALLOC_ERR_CODE){VL_Dispose(vl); BS_Dispose(&returnStack); return MALLOC_ERR_CODE;}
        help=F_SExpr(nextToken, &returnStack);
        if(help==MALLOC_ERR_CODE){VL_Dispose(vl); BS_Dispose(&returnStack); return MALLOC_ERR_CODE;}

        help = SemanticCheck(vl,&returnStack);
        if(help)
            printf("placeHolder");
            //G_AssignToVars(vl,&returnStack);<----------------------------------------------------------------------LK
        BS_Dispose(&returnStack);
        VL_Dispose(vl);
        return 0;
    }else{INVALID_TOKEN}

    INVALID_TOKEN
}

int F_Else(token* nextToken){
    NEXT;
    int help = F_StList(nextToken);
    return help;
}

int F_Exprb(token* nextToken,BubbleStack_t *stack){
    if(IFIdentif && TS_LookFunction(ts,nextToken->data.str)!= NULL){

        TreeElement* func = TS_LookFunction(ts,nextToken->data.str);
        NEXT;
        if(!IFLeftBracket){BS_Dispose(stack); INVALID_TOKEN;}
        NEXT;
        int help = F_SentPar(nextToken,stack);
        if (help!=0){BS_Dispose(stack); return help;}
        if(!IFRightBracket){BS_Dispose(stack); INVALID_TOKEN;}
        NEXT;
        BubbleStack_t mockStack;
        BS_Init(&mockStack);
        
        //help = G_CALLFUNCTION(func,stack,&mockStack);<--------------------------------------------------------------LK
        if(help!=0){BS_Dispose(stack); BS_Dispose(&mockStack); return help;}
        help=ReturnsCheck(func,&mockStack);
        if(help!=1){BS_Dispose(stack); BS_Dispose(&mockStack); return help;} //<--------------------------------------------------Error invalid return
        help = MoveStack(stack,&mockStack);
        BS_Dispose(&mockStack);
        return 0;

    }else if((IFIdentif && TS_LookVariable(ts,nextToken->data.str)!=NULL) || IFLeftBracket || nextToken->type==_const){
        expression_block* result = F_Expression(nextToken);
        if (result == NULL){BS_Dispose(stack); return MALLOC_ERR_CODE;}
        else{
            BS_Push(stack,result);
            if (stack_err_flag!=0){BS_Dispose(stack); return MALLOC_ERR_CODE;}
            return 0;
        }
    }else if(IFIdentif){
        return UNKNOWN_IDENTIF; 
    }else{INVALID_TOKEN}

    INVALID_TOKEN
}

int F_ExprAfterLoc(token* nextToken,TreeElement* targetVar){
    if (IFKwAssign){
        NEXT;
        //int returnVal;
        if (IFIdentif){
            if(TS_LookFunction(ts,nextToken->data.str)!=NULL){
                TreeElement* func = TS_LookFunction(ts,nextToken->data.str);
                NEXT;
                if(!IFLeftBracket){INVALID_TOKEN}       //return tu bol
                NEXT;
                BubbleStack_t stack;
                BS_Init(&stack);
                int help = F_SentPar(nextToken,&stack);
                if(help!=0){BS_Dispose(&stack);return help;}
                if(!IFRightBracket){BS_Dispose(&stack);INVALID_TOKEN;}       // return tu bol
                NEXT;
                ((variable*)targetVar->data)->vDefined=1;
                TreeElement** vl = VL_INIT();
                if(vl==NULL){BS_Dispose(&stack); return MALLOC_ERR_CODE;}
                vl=VL_PUSH(vl,targetVar);
                if(vl==NULL){BS_Dispose(&stack); return MALLOC_ERR_CODE;}
                
                BubbleStack_t mockStack;
                BS_Init(&mockStack);
                help = 0;
                 //help = G_CALLFUNCTION(func,stack,&mockStack);<--------------------------------------------------------------LK
                if(help!=0){BS_Dispose(&mockStack);BS_Dispose(&stack);return help;}
                if(SemanticCheck(vl,&mockStack)!=1)if(help!=0){BS_Dispose(&mockStack);BS_Dispose(&stack);return help;}
                BS_Dispose(&mockStack);
                BS_Dispose(&stack);
                return 0;       
            }else{
                expression_block* block=F_Expression(nextToken);
                if (block==NULL)return MALLOC_ERR_CODE;
                else if(IsErrMark){INVALID_EXPRESSION}
                else{
                    if (C_AssignToVar(targetVar,block)){
                    //G_AssignToVar(block,targetVar); <------------------------------------------------------------Lukasova funkcia
                    ((variable*)targetVar->data)->vDefined=1;
                    }else{
                    printf("invalid data type");
                    //RaiseError() <-------------------------------------------------------------Error
                    }
                }
            }
        }else if (nextToken->type==_const || (nextToken->type==_misc && nextToken->data.msc==_bracketL)){
            expression_block* block=F_Expression(nextToken);
                if (block==NULL)return MALLOC_ERR_CODE;
                else if(IsErrMark){ INVALID_EXPRESSION}
                else if (C_AssignToVar(targetVar,block)){
                    //G_AssignToVar(block,targetVar); <------------------------------------------------------------Lukasova funkcia
                    ((variable*)targetVar->data)->vDefined=1;
                    return 0;
                }else{
                    printf("invalid data type");
                    //RaiseError() <-------------------------------------------------------------Error
                }
        }
    }
    else{
        return 0;
    }
    INVALID_TOKEN
}

expression_block* F_Expression(token* nextToken){
    return CallTheCommander(nextToken);
}

int F_SExpr(token* nextToken, BubbleStack_t * stack){
    if(nextToken->type==_misc && nextToken->data.msc==_komma){
        NEXT;
        int test = F_Exprb(nextToken,stack);
        if (test==MALLOC_ERR_CODE)return test;
        return F_SExpr(nextToken,stack);
    }else{
        return 0;
    }
    INVALID_TOKEN
}

int F_SentPar(token* nextToken,BubbleStack_t *stack){
    if(IFRightBracket){NEXT; return 0;}
    int help=F_Exprb(nextToken,stack);
    if(help==MALLOC_ERR_CODE){BS_Dispose(stack); return MALLOC_ERR_CODE;}
    return F_SPar(nextToken,stack);
}

int F_SPar(token* nextToken, BubbleStack_t *stack){
    if(IFRightBracket){return 0;}
    if(!IfKomma){ return INVALID_TOKEN;}
    NEXT;
    int help=F_Exprb(nextToken,stack);
    if(help==MALLOC_ERR_CODE){BS_Dispose(stack); return MALLOC_ERR_CODE;}
    return F_SPar(nextToken,stack);
}

int F_SVar(token* nextToken,TreeElement ***vl){
    NEXT;
    if(IfKomma){
        NEXT;
        if(IFIdentif && TS_LookVariable(ts,nextToken->data.str)!=NULL){
            *vl=VL_PUSH(*vl,TS_LookVariable(ts,nextToken->data.str));
            if(*vl == NULL)return MALLOC_ERR_CODE;
            return F_SVar(nextToken,vl);
        }
    }else{ return 0;}
    INVALID_TOKEN;
}

void ashes_to_ashes_dust_to_dust()
{
    // free ( everything and anything and everything and anything and everything and anything and everything and anything amd all of the time )
    return;
}

int PushBuiltInFunctions(){
    user_func* new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    new->returns = PushDataType( new->returns,_integer);
    if(new->returns==NULL){free(new); return MALLOC_ERR_CODE;}
    char *readi = malloc(sizeof(char)*6);
    if(readi==NULL)return MALLOC_ERR_CODE;
    strcpy(readi,"readi");
    int help = TS_InsertFunction(ts,readi,w_func,new);
    if (help!=0){free(readi);free(new);return MALLOC_ERR_CODE;}
    new = CreateFunctionData();
    if(new==NULL)return MALLOC_ERR_CODE;
    char *write = malloc(sizeof(char)*6);
    if(write==NULL)return MALLOC_ERR_CODE;
    strcpy(write,"write");
    help = TS_InsertFunction(ts,write,w_func,new);
    if (help!=0){free(write);free(new);return MALLOC_ERR_CODE;}

    return 0;
    //TODO - others
}

int flag = 0;
int mainParseFunction(){

    atexit( ashes_to_ashes_dust_to_dust );
    generate_code = 1;
    stack_err_flag=0;
    lex_err_flag=0;
    execute_counter=0;
    while_counter=0;
    if_counter=0;
    logic_counter=0;
    line_cnt=1;
    key_arr_init();
    token* nextToken = (token*) malloc(sizeof(token));
    if(nextToken==NULL)return -1;
    TS_Init(&ts);
    if(PushBuiltInFunctions()!=0){free(nextToken); return MALLOC_ERR_CODE;}
    NEXT;
    if(nextToken->type==_keyword && nextToken->data.kw==_require){
        NEXT;
        if (nextToken->type!=_const||nextToken->data.type!=_string||strcmp(nextToken->data.str,"ifj21"))generate_code=0;//RaiseError()
        NEXT;
    }
    while (!IFEof){
        int temp = F_Prog(nextToken);
        if (temp != 0){
            DebbugPrintToken(nextToken);
            NEXT;
        }
    }
    FreeTheseBees(ts);
    return 0;
}