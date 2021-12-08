/**
 * @file expressionCommander.c
 * @author Tomas Lukac xlukac16 xlukac16@fit.vutbr.cz
 * @brief 
 * @version 1.0
 * @date 2021-12-08
 * 
 * 
 */

#include "../libs/expressionCommander.h"

//error macro
#define comError(d) RaiseError(d); exit(d);

//allocates and returns pointer to array of variables, used in assign commands
TreeElement** VL_INIT(){
    TreeElement **vl;
    vl=malloc(sizeof(TreeElement*));
    *vl = NULL;
    return(vl);
}

//appends new variable to given array
TreeElement** VL_PUSH(TreeElement **vl,TreeElement* new){
    int i=0;
    while (vl[i]!=NULL){
        i++;
    }
    vl[i]=new;
    i++;
    TreeElement** tmp = vl;
    vl=realloc(vl,sizeof(TreeElement*)*(i+1));
    if (vl==NULL){free(tmp);return vl;}
    vl[i]=NULL;
    return vl;
}

//disposes of array of variables
void VL_Dispose(TreeElement **vl){
    free(vl);
}

//checks params sent to function
int ParamCheck(TreeElement* func, BubbleStack_t *stack){
    if(strcmp(func->name,"write")==0){fflush(stderr); return 1;} //no requirements for write function
    BubbleStack_t helper;
    BS_Init(&helper);
    if (stack_err_flag){BS_Dispose(stack); comError(99)}

    while (!BS_IsEmpty(stack)) //we need reversed stack
    {
        BS_Push(&helper,BS_TopStack(stack));
        BS_Pop(stack);
    }
    int i=0;
    int bad = 0;
    while (!BS_IsEmpty(&helper)){
        if(((user_func*)func->data)->params[i]==_ender){BS_Dispose(stack);BS_Dispose(&helper);return 0;} //less params needed than send
        expression_block* tmp = BS_TopStack(&helper);
        if(tmp->dt!=((user_func*)func->data)->params[i]){ // sent param is not requred param
            if(tmp->dt==_number && ((user_func*)func->data)->params[i]==_integer)convert_to_float(tmp); //sent param is convertable
            else if(tmp->dt==_integer && ((user_func*)func->data)->params[i]==_number)convert_to_int(tmp); //sent param is convertable
            else if(tmp->dt!=_nan){bad=1;break;} //we can still use nil params
        }

        BS_Push(stack,tmp);
        BS_Pop(&helper);
        i++;
    }
    if(bad){BS_Dispose(stack);BS_Dispose(&helper);return 0;} // bad param sent
    if(((user_func*)func->data)->params[i]!=_ender){BS_Dispose(stack);BS_Dispose(&helper);return 0;} //not enough params sent
    BS_Dispose(&helper);
    if(bad==0)return 1;
    else return 0;
}

//assign command check type compability
int SemanticCheck(TreeElement** vl,BubbleStack_t *stack){
    BubbleStack_t helper;
    BS_Init(&helper);
    if(helper.BS_Element==NULL){BS_Dispose(stack); comError(99)} 
    int bad = 0;
    int length=0;
    while (vl[length]!=NULL){length+=1;}
    while(!BS_IsEmpty(stack)){BS_Push(&helper,BS_TopStack(stack));BS_Pop(stack);} //we need reversed stack
    for(int i=length-1;i>=0;i--){
        if(BS_IsEmpty(&helper)){BS_Dispose(stack);BS_Dispose(&helper);comError(4)} //more variables than expressions
        variable* temp = (variable*)vl[i]->data;
        if(BS_TopStack(&helper)->dt!= temp->type){ //unexpected type
            if(BS_TopStack(&helper)->dt==_integer && temp->type==_number)convert_to_float(BS_TopStack(&helper)); //sent param is convertable
            else if(BS_TopStack(&helper)->dt==_number && temp->type==_integer) convert_to_int(BS_TopStack(&helper));//sent param is convertable
            else if(BS_TopStack(&helper)->dt!=_nan){ bad=1;break;}// nil can be used
        }
        BS_Push(stack,BS_TopStack(&helper));
        BS_Pop(&helper);
    }
    
    if(!BS_IsEmpty(&helper)){BS_Dispose(stack);BS_Dispose(&helper);comError(4)} // more expressions than variables

    BS_Dispose(&helper);
    if(bad)
        return 0;
    else
        return 1;
}

//on return command
int ReturnsCheck(TreeElement *func, BubbleStack_t * returnedStack){
    int bad=0;
    BubbleStack_t helperStack;
    BS_Init(&helperStack);
    if(helperStack.BS_Element==NULL){BS_Dispose(returnedStack);comError(99)}
    int lengthStack=0;
    while(!BS_IsEmpty(returnedStack)){ // we need reverse stack + we count the return expressions
        BS_Push(&helperStack,BS_TopStack(returnedStack));
        BS_Pop(returnedStack);
        lengthStack++;
    }
    int lengthReturns=0;
    user_func* data=(user_func*)func->data;
    while (data->returns[lengthReturns]!=_ender) // count function expected returns
    {
        lengthReturns++;
    }
    int i = 0;
    if(lengthReturns>=lengthStack){ // more or equal returns expected
        while (lengthReturns>lengthStack) // if more was expected this pushes nil values to stack
        {
            expression_block* tmp = malloc(sizeof(expression_block));
            if (tmp==NULL){BS_Dispose(returnedStack);BS_Dispose(&helperStack);return MALLOC_ERR_CODE;}//<---------------Error
            tmp->blockType=_operand_expr;
            tmp->operType=_not_terminal_oper;
            tmp->dt=data->returns[i];
            tmp->_double = exprCounter;
            G_FillWithNil(tmp);//<--------------------------------------------------------------------------------------------------------------------LK
            BS_Push(returnedStack,tmp);
            lengthStack++;
            i++;
        }
        while(!BS_IsEmpty(&helperStack)){ //Checks the returns
            expression_block* tmp = BS_TopStack(&helperStack);
            if(tmp->dt!=data->returns[i]){
                if(tmp->dt==_number && data->returns[i]==_integer){convert_to_int(tmp);}//sent param is convertable
                else if(tmp->dt==_integer && data->returns[i]==_number){convert_to_float(tmp);}//sent param is convertable
                else if(tmp->dt!=_nan) bad=1; // nil is acceptable
            }
            BS_Push(returnedStack,tmp);
            BS_Pop(&helperStack);
            i++;
        }
    }else{
        while (i<lengthReturns) //no nills needed
        {
            expression_block* tmp = BS_TopStack(&helperStack);
            if(tmp->dt!=data->returns[i]){
                if(tmp->dt==_number && data->returns[i]==_integer){convert_to_int(tmp);}//sent param is convertable
                else if(tmp->dt==_integer && data->returns[i]==_number){convert_to_float(tmp);}//sent param is convertable
                else if(tmp->dt!=_nan) bad=1; //nil is acceptable
            }
            BS_Push(returnedStack,tmp);
            BS_Pop(&helperStack);
            i++;
        }
    }
    BS_Dispose(&helperStack);
    if(bad)return 0;
    else return 1;
}

//appends contents to stack
void MoveStack(BubbleStack_t* dest, BubbleStack_t* target){
    BubbleStack_t helper;
    BS_Init(&helper);
    if(helper.BS_Element==NULL){BS_Dispose(dest);BS_Dispose(target);comError(99)}
    while(!BS_IsEmpty(target)){ //moves to temp stack
        BS_Push(&helper,BS_TopStack(target));
        BS_Pop(target);
    }
    while (!BS_IsEmpty(&helper)) //temp stack to target stack
    {
        if(BS_IsFull(dest)){BS_Dispose(dest);BS_Dispose(target);comError(99)}
        BS_Push(dest,BS_TopStack(&helper));
        BS_Pop(&helper);
    }

    BS_Dispose(&helper);
    return;
}

//Checks single variable assign command , used after local
int C_AssignToVar(TreeElement* target, expression_block* source){
    if(((variable*)target->data)->type==_string){
        if(source->dt == _string)return 1;
        else if(source->dt == _nan)return 1;
        else return 0;
    }else
    if(((variable*)target->data)->type==_nan){
        if(source->dt == _nan)return 1;
        else return 0;
    }else
    if (((variable*)target->data)->type==_number){
        if(source->dt == _nan)return 1;
        if (source->dt==_number) 
            return 1;
        else if(source->dt==_integer){
            convert_to_float(source);
            return 1;
        }
        else
            return 0;
    }else
    if(((variable*)target->data)->type==_integer){
        if(source->dt == _nan)return 1;
        if(source->dt==_integer){
            return 1;
        }else
        if(source->dt==_number){
            convert_to_int(source);
            return 1;
        }else
            return 0;
    }else
        return 0;
}

//check if all used functions are defined
void TS_Looper(TreeElement* root){
    if(root==NULL)return;
    if(((user_func*)root->data)->vUsed==1 && ((user_func*)root->data)->vDefined==0){comError(3)}
    TS_Looper(root->left);
    TS_Looper(root->right);
}

//calls the recursive function to check if all used functions are defined
void TS_FunctionCaller(){
    TS_Looper(ts->functionLayer->tree->root);
}

//Checks if nextToken means end of expression
int IsEndSymbol(token *nextToken){
    if ((nextToken->type == _keyword && nextToken->data.kw == _end) ||
    (nextToken ->type == _keyword && nextToken->data.kw == _return)||
    (nextToken ->type == _keyword && nextToken->data.kw == _while) ||
    (nextToken ->type == _keyword && nextToken->data.kw == _if)||
    (nextToken ->type == _keyword && nextToken->data.kw == _local)||
    (nextToken ->type == _keyword && nextToken->data.kw == _then)||
    (nextToken ->type == _keyword && nextToken->data.kw == _do)||
    (nextToken ->type == _keyword && nextToken->data.kw == _else)||
    (nextToken ->type == _misc && nextToken->data.msc == _komma)||
    (nextToken ->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL)||
    (nextToken ->type == _misc && nextToken->data.msc == _EOF))
    return 1;
    else return 0; 
}

//Converts token to expression block
int ConvertToBlock(expression_block *block, token* nextToken){
    if (nextToken->type==_misc){ //bracket
        if (nextToken->data.msc == _bracketL){
            block->em=_lbr; block->blockType=_misc_expr; return 0;
        }else if (nextToken->data.msc == _bracketR){
            block->em=_rbr; block->blockType=_misc_expr; return 0;
        }else return INVALID_TOKEN;
    }else
    if (nextToken->type==_operator){ //operator
        block->blockType=_operator_expr;
        block->oper = nextToken->data.oper;
    }else
    if (nextToken->type==_identifier){ //identifier
        block->blockType=_operand_expr;
        block->operType=_variable_oper;
        block->TSPointer=TS_LookVariable(ts,nextToken->data.str);
        if(block->TSPointer == NULL){return UNKNOWN_IDENTIF;}
        block->dt = ((variable*)(block->TSPointer->data))->type;
        block->str=malloc(sizeof(char)*strlen(nextToken->data.str)+1);
        strcpy(block->str,nextToken->data.str);
    }else
    if (nextToken->type==_const){ //constant
        block->blockType=_operand_expr;
        block->operType=_constant_oper;
        block->dt=nextToken->data.type;
        block->_double=nextToken->data._double;
        block->_integer=nextToken->data._integer;
        if(block->dt==_string){
            block->str=malloc(sizeof(char)*strlen(nextToken->data.str)+1);
            strcpy(block->str,nextToken->data.str);
        }
    }else
    if (nextToken->type==_keyword && nextToken->data.kw==_nil){ //nill
        block->blockType=_operand_expr;
        block->operType=_constant_oper;
        block->dt=_nan;
    }
    return 0;
}

//fills begin mark to expression block
expression_block* FillBeginMark(){
    expression_block* block=malloc(sizeof(expression_block));
    if (block==NULL)return block;
    block->blockType=_misc_expr;
    block->em=_bgnMark;
    block->_double=exprCounter;
    return block;
}

//fills end mark to expression block
expression_block* FillEndMark(){
    expression_block* block=malloc(sizeof(expression_block));
    if (block==NULL)return block;
    block->blockType=_misc_expr;
    block->em=_endMark;
    block->_double=exprCounter;
    return block;
}

//Determines type of operation result
dataType ResultData(expression_block* operand1, expression_block* operand2, expression_block *sign){
    if (sign->oper==_less || sign->oper == _lessEq || sign->oper == _great || sign->oper == _greatEq || sign->oper == _Eq || sign->oper == _nEq) return _bool;
    else if (sign->oper==_konk) return _string;
    else if (sign->oper==_length) return _integer;
    else if (operand1->dt == _integer && operand2->dt == _integer) return _integer;
    else if ((operand1->dt == _integer && operand2->dt == _number) || (operand1->dt == _number && operand2->dt == _integer)) return _number;
    else if (operand1->dt == _number && operand2->dt == _number) return _number;
    else return _nan;
}

//Checks if operation is feasable
int TypeCheck(expression_block* operand1,expression_block* sign, expression_block* operand2){
    if (sign->blockType==_operator_expr && sign->oper==_length){ //#
        if (operand2==NULL && operand1->blockType==_operand_expr && operand1->dt==_string)
            return 1;
        else
            return 0;
    }else if (sign->blockType==_operator_expr && sign->oper==_konk){ //..
        if (operand1->blockType==_operand_expr && operand1->dt==_string && operand2->blockType==_operand_expr && operand2->dt==_string)
            return 1;
        else
            if(operand1->dt==_nan || operand2->dt==_nan)
                return -1;
            else
                return 0;
                                                                                        //compare operations
    }else if (sign-> blockType==_operator_expr && (sign->oper==_Eq || sign->oper==_nEq || sign->oper==_less || sign->oper==_lessEq || sign->oper==_great || sign->oper==_greatEq)){
        if (operand1->blockType==_operand_expr && operand2->blockType==_operand_expr){
            if (operand1->dt==_integer && operand2->dt== _integer)return 1;
            else if (operand1->dt==_number && operand2->dt == _integer){convert_to_float(operand2); return 1;} //convertable
            else if (operand1->dt==_integer && operand2->dt == _number) {convert_to_float(operand1); return 1;} //convertable
            else if (operand1->dt == _number && operand2->dt == _number) return 1; //This probably doesnt work
            else if (operand1->dt==_string && operand2->dt == _string) return 1;
            else if (operand1->dt==_bool && operand2->dt == _bool) return 1;
            else if ((sign->oper==_Eq || sign->oper==_nEq) && ( operand1->dt==_nan || operand2->dt==_nan)) return 1; //compare with nills
            else if(operand1->dt==_nan || operand2->dt==_nan) // other oper with nil
                return -1;
            else
                return 0;
        }else
            if(operand1->dt==_nan || operand2->dt==_nan)  //other opers with nills
                return -1;
            else
                return 0; 
    }else if (sign->blockType==_operator_expr){ // other operations
        if((sign->oper==_div||sign->oper==_div2)&&operand2->isZero==1)return -2;
        if(operand1->dt == _integer && operand2->dt==_integer)return 1;
        else if(operand1->dt == _integer && operand2->dt==_number) {convert_to_float(operand1); return 1;} //<----------------------------------------------------------------------------------------------- JUROVE FUNKCIE
        else if(operand1->dt == _number && operand2->dt ==_integer) {convert_to_float(operand2); return 1;}
        else if(operand1->dt == _number && operand2->dt == _number) return 1;
        else if(operand1->dt==_nan || operand2->dt==_nan)
                return -1;
            else
                return 0;
    }
    else return 0;

}

//Gets closest terminal expression block on stack
void GetClosestTerminal(BubbleStack_t *stack, expression_block **block){
    *block=BS_TopStack(stack);
    if (stack_err_flag == 1){BS_Dispose(stack);comError(99)}
    if (!((*block)->blockType==_operand_expr && (*block)->operType == _not_terminal_oper)){ // terminal on top 
        return;
    }else{                                                          //not terminal on top
        expression_block *helper;
        helper = *block;
        BS_Pop(stack);
        *block=BS_TopStack(stack);
        if (stack_err_flag == 1) {BS_Dispose(stack);comError(99)}
        BS_Push(stack,helper);
    }
    return;
}

//Shift symbol
void TAB_Shift(BubbleStack_t *stack,token *nextToken){
    expression_block *block;
    if(nextToken->type==_identifier || nextToken->type==_const || (nextToken->type == _misc && nextToken->data.msc == _bracketL)){ //shift identif,constant,(
        block = FillBeginMark();
        if(block == NULL) {BS_Dispose(stack);comError(99)}
        BS_Push(stack,block);
        if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
    }else
    if (nextToken->type==_operator){ // shift operator
        block = FillBeginMark();
        if (block == NULL) {BS_Dispose(stack);comError(99)}
        if(nextToken->data.oper==_length){
            BS_Push(stack,block);
            if(stack_err_flag!=0){BS_Dispose(stack);comError(99)}
        }else{
            expression_block *helper=NULL;
            helper=BS_TopStack(stack);
            BS_Pop(stack);
            BS_Push(stack,block);
            if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
            BS_Push(stack,helper);
            if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
        }
    }else 
    if (nextToken->type == _keyword && nextToken->data.kw==_nil){ // shift nil
        block = FillBeginMark();
        if(block == NULL) {BS_Dispose(stack);comError(99)}
        BS_Push(stack,block);
        if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
    }
    block=malloc(sizeof(expression_block));//new block
    if(block==NULL){BS_Dispose(stack);comError(99)}
    int test = ConvertToBlock(block,nextToken);
    if (test != 0) {BS_Dispose(stack);comError(2)}
    NEXT;
    BS_Push(stack,block);
    if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
}

//= sign in tab
void TAB_Equals(BubbleStack_t *stack, token *nextToken){
    expression_block *nt=NULL;
    expression_block *del=NULL;
    nt=BS_TopStack(stack);
    BS_Pop(stack);
    del=BS_TopStack(stack);
    BS_Pop(stack);
    free(del);
    del=BS_TopStack(stack);
    BS_Pop(stack);
    free(del);
    BS_Push(stack,nt);
    if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
    NEXT;
}

//> sign in tab
void TAB_Terminalize(BubbleStack_t *stack,token *nextToken,int* termNumber){
    (void)nextToken;
    expression_block *operand1 = NULL,*operand2=NULL,*sign=NULL,*bumper=NULL;
    operand2=BS_TopStack(stack);
    BS_Pop(stack);
    sign=BS_TopStack(stack);
    BS_Pop(stack);
    if (sign->blockType==_misc_expr && sign->em==_bgnMark){//var -> non terminal
        free(sign);
        expression_block *newTerm = malloc(sizeof(expression_block));
        if(newTerm==NULL){free(operand2);BS_Dispose(stack);comError(99)}
        newTerm->blockType=_operand_expr;
        newTerm->operType=_not_terminal_oper;
        
        if(operand2->operType == _variable_oper){
            if(((variable*)(operand2->TSPointer->data))->isNil==1)
                newTerm->dt=_nan;
            else
                newTerm->dt=operand2->dt;
        }else
            newTerm->dt=operand2->dt;

        newTerm->_integer=*termNumber;
        newTerm->_double=exprCounter;
        newTerm->TSPointer=NULL;
        newTerm->str=NULL;
        if(operand2->dt==_integer && operand2->_integer==0) newTerm->isZero=1;
        else newTerm->isZero=0;
        (*termNumber)++;
        var_to_term_assign(newTerm,operand2);//<-----------------------------------------------------------LK FIXME
        BS_Push(stack,newTerm);
        if(stack_err_flag==1){free(operand2);BS_Dispose(stack);comError(99)}
        if(operand2->operType==_variable_oper || (operand2->operType==_constant_oper && operand2->dt==_string))free(operand2->str);
        free(operand2);
        return;
    }
    operand1=BS_TopStack(stack);
    BS_Pop(stack);
    if (operand1->blockType==_misc_expr && operand1->em==_bgnMark){ //unary operand
        free(operand1);
        int tp = TypeCheck(operand2,sign,NULL);
        if(tp!=1){free(operand2);free(sign); BS_Dispose(stack); if(tp==-1){comError(8)}else {comError(6)}}
        expression_block *newTerm = malloc(sizeof(expression_block));
        if(newTerm==NULL){free(operand2);free(sign);BS_Dispose(stack);comError(6)}
        newTerm->blockType=_operand_expr;
        newTerm->operType=_not_terminal_oper;
        newTerm->dt=_integer;
        newTerm->_integer=*termNumber;
        newTerm->_double=exprCounter;
        (*termNumber)++;

        unary_operator(newTerm,operand2);//<---------------------------------------------------------LK
        
        BS_Push(stack,newTerm);
        free(operand2);
        free(sign);
        if(stack_err_flag==1){BS_Dispose(stack);comError(6)}
        return;
    }
    bumper=BS_TopStack(stack);
    BS_Pop(stack);
    if(bumper->blockType==_misc_expr && bumper->em == _bgnMark){//diary operand *duary *bilingual *binary
        free(bumper);
        int tp = TypeCheck(operand1,sign,operand2);
        if (tp!=1){free(operand1);free(operand2);free(sign); BS_Dispose(stack); if(tp==-2){comError(9)}else if(tp == -1){comError(8)} else{ comError(6)}}
        expression_block *newTerm = malloc(sizeof(expression_block));
        if(newTerm==NULL){free(operand1);free(operand2);free(sign);BS_Dispose(stack);comError(99)}
        newTerm->blockType=_operand_expr;
        newTerm->operType=_not_terminal_oper;      
        newTerm->dt=ResultData(operand1,operand2,sign);
        newTerm->_integer=*termNumber;
        newTerm->_double=exprCounter;
        (*termNumber)++;

        operation_quick_action(newTerm,operand1,operand2,sign);

        BS_Push(stack,newTerm);
        free(operand1);
        free(operand2);
        free(sign);
        if(stack_err_flag==1){free(operand1);free(operand2);free(sign);BS_Dispose(stack);comError(99)}
        return;
    }else{free(bumper);free(operand1);free(operand2);free(sign);BS_Dispose(stack);comError(6)} // Error
}

//Choose action from tab
actions ChooseAction(BubbleStack_t *stack,token* nextToken){
    expression_block *block=NULL;
    GetClosestTerminal(stack,&block);
    //#
    if (block->blockType == _operator_expr && block->oper == _length){ //unary operand
        if (nextToken->type == _operator && nextToken->data.oper == _length)
            return _tab_terminalize;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (BS_TopStack(stack)->blockType == _operand_expr && BS_TopStack(stack)->operType==_not_terminal_oper && nextToken->type== _identifier && TS_LookFunction(ts,nextToken->data.str)!=NULL)
           return _tab_end;
        else if (nextToken->type == _identifier)
            if(TS_LookVariable(ts,nextToken->data.str)!=NULL)
                return _tab_shift;
            else
                {BS_Dispose(stack);comError(3)}
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr && (block->oper == _mul || block->oper==_div || block->oper==_div2)){ // *,/,//
        if (nextToken->type == _operator && nextToken->data.oper == _length)
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            if(TS_LookVariable(ts,nextToken->data.str)!=NULL)
                return _tab_shift;
            else
                {BS_Dispose(stack);comError(3)}
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            if(BS_TopStack(stack)->blockType==_operand_expr){
                return _tab_end;
            }else
                {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr && (block->oper == _sub || block->oper == _add)){ // +,-
        if (nextToken->type == _operator && (nextToken->data.oper == _length || nextToken->data.oper == _mul || nextToken->data.oper == _div || nextToken->data.oper == _div2))
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            if(TS_LookVariable(ts,nextToken->data.str)!=NULL)
                return _tab_shift;
            else
                {BS_Dispose(stack);comError(3)}
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr && block->oper == _konk){ // ..
        if (nextToken->type == _operator && (nextToken->data.oper == _length || nextToken->data.oper == _mul || nextToken->data.oper == _div || nextToken->data.oper == _div2 || nextToken->data.oper == _sub || nextToken->data.oper == _add))
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            if(TS_LookVariable(ts,nextToken->data.str)!=NULL)
                return _tab_shift;
            else
                {BS_Dispose(stack);comError(3)}
        else if (nextToken->type==_const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr){ //>,>=,<,<=,==,~=
        if (nextToken->type == _operator && (nextToken->data.oper == _length || nextToken->data.oper == _mul || nextToken->data.oper == _div || nextToken->data.oper == _div2 || nextToken->data.oper == _sub || nextToken->data.oper == _add || nextToken->data.oper == _konk ))
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            if(TS_LookVariable(ts,nextToken->data.str)!=NULL)
                return _tab_shift;
            else
                {BS_Dispose(stack);comError(3)}
        else if (nextToken->type==_const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _misc_expr && block->em == _lbr){ //(
        if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_equals;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_shift;
        else if (nextToken->type == _identifier)
            if(TS_LookVariable(ts,nextToken->data.str)!=NULL)
                return _tab_shift;
            else
                {BS_Dispose(stack);comError(3)}
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _misc_expr && block->em == _rbr){//)
        if (nextToken->type == _operator)
            return _tab_terminalize;
        else if(nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if(IsEndSymbol(nextToken))
            return _tab_end;
        else if (nextToken->type ==_identifier)
            return _tab_end;
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operand_expr && (block->operType == _variable_oper || block->operType==_constant_oper)){ // identif,var,constant
        if (nextToken->type == _operator)
            return _tab_terminalize;
        else if(nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if(IsEndSymbol(nextToken)){
            return _tab_end;
        }
        else if (nextToken->type ==_identifier)
            return _tab_end;
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _misc_expr && block->em== _endMark){ //$
        if (nextToken->type == _operator)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _identifier)
            if(TS_LookVariable(ts,nextToken->data.str)!=NULL)
                return _tab_shift;
            else
                {BS_Dispose(stack);comError(3)}
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if(IsEndSymbol(nextToken))
            if(BS_TopStack(stack)->blockType==_operand_expr){ 
                return _tab_end; 
            }
            else
                {BS_Dispose(stack);comError(2)}
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_end;
        else
            {BS_Dispose(stack);comError(2)}
    }else
        {BS_Dispose(stack);comError(2)}
}

//main expression cycle
expression_block* SolveCycle(BubbleStack_t* stack,token *nextToken,int *termNumber){
    actions curAction = ChooseAction(stack,nextToken);
    while (curAction!=_tab_end){
        if(curAction==_tab_shift){
            TAB_Shift(stack,nextToken);
        }else if (curAction==_tab_terminalize){
            TAB_Terminalize(stack,nextToken,termNumber);
        }else if (curAction == _tab_equals){
            TAB_Equals(stack,nextToken);
        }
        curAction=ChooseAction(stack,nextToken);
    }
    while(stack->BS_TopIndex!=1){
        TAB_Terminalize(stack,nextToken,termNumber);
    }
    expression_block* result=NULL;
    result=BS_TopStack(stack);
    BS_Pop(stack);
    return result;
}

//main expression function
expression_block* CallTheCommander(token *nextToken){
    expression_block *block = NULL;
    BubbleStack_t stack;
    int termNumber = 0;
    exprCounter+=1.0f;
    BS_Init(&stack);
    if(stack_err_flag){comError(99)}
    block = FillEndMark();
    if(block==NULL){BS_Dispose(&stack); comError(99)}
    BS_Push(&stack,block);
    expression_block* result = SolveCycle(&stack,nextToken,&termNumber);
    BS_Dispose(&stack);
    return result;
}
/*
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWWWMW0ooKMWX00NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXNNkcd0x;.  ,c;'..:oooxXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKXKc.''  ..                'ck0KXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXo...                           ..,llclO0kXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXNXxo;                                      ...:kNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXNXxdkl','   .:'                                       .,:xXNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWOc,:'  .  .,ldk0No                                          ..',:cldk0NWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl.        'kNXd:;ko                                                   .':dONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMK:         .kMX:  :d'                                                       .,lONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNl          .dO:.'cl'                                                           .,d0WMMMMMN0dllld0NMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMO.           ...ld;                                                                .,clll:'.     .;kXWMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXc               ..                                                                   .lxOOxl'      .xWMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMXl.                                                                                    .lXMMMMNx.     ':ldkNMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMNk,      .''.                                                                              cXMMMMMKo,;c,;dddONMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMWKkl,                                                                                           oWMMMMMMNNWXKNMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMNk:.                                                                                              ,KMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMNO:.                            ..                                                                  ,KMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMXkdc'       ..                     ..                                                                .'oXMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMWd. .,,.    :o.                     ',                                                                 cXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMXx,....   .xK:..                   'c.                                                                .dNMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMNOc;;lc'..:c,.                    ,o.                                                               .d0XMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMWWMMXkoc:::lodddl;'.     .'... lo.                                          ..                  'kXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNOl,;,:OXKKxdo.                ..                    ..   '::.                .:kKXWMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNNMMMMWk.             .:;cOl      .c:;'.....';lx0x.    .;;'                 ..,dNMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNd.            .lkNWWWO.     lWMWNXKK0KNWMMMWk.     .,c:.                 :XMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNx;             .lXWMMMMNo     dMMMMMMMMMMMMMMMWO.      oNKxc'.   .,;'.     lWMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNl          ..:dk0XWMMMMMMX:    cNMMMMMMMMMMMMMW0:.   .:xXMMMWN0OkOXWMNk,    oWMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMk.     .;lxOKXWMMMMMMMMMMMM0'   .OMMMMMMMMMMWXx:.  .ckXWMMMMMMMMMMMMMMMMO.   lWMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM0,   .ckXWMMMMMMMMMMMMMMMMMWx.    cNMMMMMMMNOl.   .lXMMMMMMMMMMMMMMMMMMMMK,   :NMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKd.  .:0WMMMMMMMMMMMMMMMMMMMWO'    .cXMMMMWKd;'..   :XMMMMMMMMMMMMMMMMMMMMWd.   ;KMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKx:.   ,kWMMMMMMMMMMMMMMMMMMMMXo..'.  :XMMMMMXl  .'...:0MMMMMMMMMMMMMMMMMMMMKl.    '0MMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNkc;..   ,OMMMMMMMMMMMMMMMMMMMMMMXl.':cldXMMMMMMMKl:;;:lkNMMMMMMMMMMMMMMMMMMMNx,... .;xXMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMK; .'.'ckKWMMMMMMMMMMMMMMMMMMMMMMMNXXNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM0,....'oXMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMXkkk0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKdodkXWMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
*/

/*file end*/