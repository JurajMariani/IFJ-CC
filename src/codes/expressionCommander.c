#include "../libs/expressionCommander.h"

#define comError(d) fprintf(stderr,"%s %d ",__func__, __LINE__); RaiseError(d); exit(d);

TreeElement** VL_INIT(){
    TreeElement **vl;
    vl=malloc(sizeof(TreeElement*));
    *vl = NULL;
    return(vl);
}

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

void VL_Dispose(TreeElement **vl){
    free(vl);
}

int ParamCheck(TreeElement* func, BubbleStack_t *stack){
    BubbleStack_t helper;
    BS_Init(&helper);
    if (helper.BS_Element==NULL){BS_Dispose(stack); comError(99)}

    while (!BS_IsEmpty(stack))
    {
        BS_Push(&helper,BS_TopStack(stack));
        BS_Pop(stack);
    }
    int i=0;
    int bad = 0;
    user_func* data=(user_func*)func->data;
    while (!BS_IsEmpty(&helper)){
        dataType paramPointer=data->params[i];
        if(paramPointer==_ender){BS_Dispose(stack);BS_Dispose(&helper);return 0;}
        expression_block* tmp = BS_TopStack(&helper);
        if(tmp->dt!=data->params[i]){
            if(tmp->dt==_number && data->params[i]==_integer)convert_to_float(tmp);
            else if(tmp->dt==_integer && data->params[i]==_number)convert_to_int(tmp);
            else bad=1;
        }

        BS_Push(stack,tmp);
        BS_Pop(&helper);
        i++;
    }
    dataType paramPointer=data->params[i];
    if(paramPointer!=_ender){BS_Dispose(stack);BS_Dispose(&helper);return 0;}
    BS_Dispose(&helper);
    if(bad==0)return 1;
    else return 0;
}

int SemanticCheck(TreeElement** vl,BubbleStack_t *stack){
    BubbleStack_t helper;
    BS_Init(&helper);
    if(helper.BS_Element==NULL){BS_Dispose(stack); comError(99)} 
    int bad = 0;
    int length=0;
    while (vl[length]!=NULL){length+=1;}
    for(int i=length-1;i>=0;i--){
        if(BS_IsEmpty(stack)){BS_Dispose(stack);BS_Dispose(&helper);comError(4)}
        BS_Push(&helper,BS_TopStack(stack));
        BS_Pop(stack);
        variable* temp = (variable*)vl[i]->data;
        if(BS_TopStack(&helper)->dt!= temp->type){
            if(BS_TopStack(&helper)->dt==_integer && temp->type==_number)convert_to_float(BS_TopStack(&helper)); //<-----------------LK
            else if(BS_TopStack(&helper)->dt==_number && temp->type==_integer) convert_to_int(BS_TopStack(&helper));
            else bad=1;
        }
    }
    if(!BS_IsEmpty(stack)){BS_Dispose(stack);BS_Dispose(&helper);comError(4)}

    while(!BS_IsEmpty(&helper)){BS_Push(stack,BS_TopStack(&helper));BS_Pop(&helper);}

    BS_Dispose(&helper);
    if(bad)
        return 0;
    else
        return 1;
}

int ReturnsCheck(TreeElement *func, BubbleStack_t * returnedStack){
    int bad=0;
    BubbleStack_t helperStack;
    BS_Init(&helperStack);
    if(helperStack.BS_Element==NULL){BS_Dispose(returnedStack);comError(99)}
    int lengthStack=0;
    while(!BS_IsEmpty(returnedStack)){
        BS_Push(&helperStack,BS_TopStack(returnedStack));
        BS_Pop(returnedStack);
        lengthStack++;
    }
    int lengthReturns=0;
    user_func* data=(user_func*)func->data;
    printf("%s %d",func->name,data->returns[0]);
    while (data->returns[lengthReturns]!=_ender)
    {
        lengthReturns++;
    }
    int i = 0;
    if(lengthReturns>=lengthStack){
        while (lengthReturns>lengthStack)
        {
            expression_block* tmp = malloc(sizeof(expression_block));
            if (tmp==NULL){BS_Dispose(returnedStack);BS_Dispose(&helperStack);return MALLOC_ERR_CODE;}
            tmp->blockType=_operand_expr;
            tmp->operType=_not_terminal_oper;
            tmp->dt=data->returns[i];
            tmp->_double = exprCounter;
            G_FillWithNil(tmp);//<--------------------------------------------------------------------------------------------------------------------LK TODO
            BS_Push(returnedStack,tmp);
            lengthStack++;
            i++;
        }
        while(!BS_IsEmpty(&helperStack)){
            expression_block* tmp = BS_TopStack(&helperStack);
            if(tmp->dt!=data->returns[i]){
                if(tmp->dt==_number && data->returns[i]==_integer){convert_to_int(tmp);}//<--------------------------------------------LK
                else if(tmp->dt==_integer && data->returns[i]==_number){convert_to_float(tmp);}
                else bad=1;
            }
            BS_Push(returnedStack,tmp);
            BS_Pop(&helperStack);
            i++;
        }
    }else{
        while (i<lengthReturns)
        {
            expression_block* tmp = BS_TopStack(&helperStack);
            if(tmp->dt!=data->returns[i]){
                if(tmp->dt==_number && data->returns[i]==_integer){convert_to_int(tmp);}//<--------------------------------------------LK
                else if(tmp->dt==_integer && data->returns[i]==_number){convert_to_float(tmp);}
                else bad=1;
            }
            BS_Push(returnedStack,tmp);
            i++;
        }
    }
    BS_Dispose(&helperStack);
    if(bad)return 0;
    else return 1;
}

void MoveStack(BubbleStack_t* dest, BubbleStack_t* target){
    BubbleStack_t helper;
    BS_Init(&helper);
    if(helper.BS_Element==NULL){BS_Dispose(dest);BS_Dispose(target);comError(99)}
    while(!BS_IsEmpty(target)){
        BS_Push(&helper,BS_TopStack(target));
        BS_Pop(target);
    }
    while (!BS_IsEmpty(&helper))
    {
        if(BS_IsFull(dest)){BS_Dispose(dest);BS_Dispose(target);comError(99)}
        BS_Push(dest,BS_TopStack(&helper));
        BS_Pop(&helper);
    }

    BS_Dispose(&helper);
    return;
}

int C_AssignToVar(TreeElement* target, expression_block* source){
    if(((variable*)target->data)->type==_string){
        if(source->dt == _string)return 1;
        else return 0;
    }else
    if (((variable*)target->data)->type==_number){
        if(source->dt==_integer){
            convert_to_float(source);//<-------------------------------LK
            return 1;
        }else
        if (source->dt==_number) 
            return 1;
        else
            return 0;
    }else
    if(((variable*)target->data)->type==_integer){
        if(source->dt==_integer){
            return 1;
        }else
        if (source->dt==_number){ 
            convert_to_int(source);//<_----------------------------LK
            return 1;
        }else
            return 0;
    }else
        return 0;
}

//DNT
int IsEndSymbol(token *nextToken){
    if ((nextToken->type == _keyword && nextToken->data.kw == _end) ||
    (nextToken ->type == _keyword && nextToken->data.kw == _return)||
    (nextToken ->type == _keyword && nextToken->data.kw == _while) ||
    (nextToken ->type == _keyword && nextToken->data.kw == _if)||
    (nextToken ->type == _keyword && nextToken->data.kw == _local)||
    (nextToken ->type == _keyword && nextToken->data.kw == _then)||
    (nextToken ->type == _keyword && nextToken->data.kw == _do)||
    (nextToken ->type == _misc && nextToken->data.msc == _komma)||
    (nextToken ->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=0)||
    (nextToken ->type == _misc && nextToken->data.msc == _EOF))
    return 1;
    else return 0; 
}

//Should work
int ConvertToBlock(expression_block *block, token* nextToken, int *termNumber){
    if (nextToken->type==_misc){
        if (nextToken->data.msc == _bracketL){
            block->em=_lbr; block->blockType=_misc_expr; return 0;
        }else if (nextToken->data.msc == _bracketR){
            block->em=_rbr; block->blockType=_misc_expr; return 0;
        }else return INVALID_TOKEN;
    }else
    if (nextToken->type==_operator){
        block->blockType=_operator_expr;
        block->oper = nextToken->data.oper;
    }else
    if (nextToken->type==_identifier){
        block->blockType=_operand_expr;
        block->operType=_variable_oper;
        block->TSPointer=TS_LookVariable(ts,nextToken->data.str);
        if(block->TSPointer == NULL)return UNKNOWN_IDENTIF;
        block->dt = ((variable*)(block->TSPointer->data))->type;
        block->str=block->TSPointer->name;
    }else
    if (nextToken->type==_const){
        block->blockType=_operand_expr;
        block->operType=_constant_oper;
        block->dt=nextToken->data.type;
        block->_double=nextToken->data._double;
        block->_integer=nextToken->data._integer;
        if(block->dt==_string){
            block->str=malloc(sizeof(char)*strlen(nextToken->data.str));
            strcpy(block->str,nextToken->data.str);
        }
    }else
    if (nextToken->type==_keyword && nextToken->data.kw==_nil){
        block->blockType=_operand_expr;
        block->operType=_constant_oper;
        block->_double=exprCounter;
        block->_integer=*termNumber;
        (*termNumber)++;
        block->dt=_nan;
    }
    return 0;
}

//Should work
expression_block* FillBeginMark(){
    expression_block* block=malloc(sizeof(expression_block));
    if (block==NULL)return block;
    block->blockType=_misc_expr;
    block->em=_bgnMark;
    block->_double=exprCounter;
    return block;
}

//Should work
expression_block* FillEndMark(){
    expression_block* block=malloc(sizeof(expression_block));
    if (block==NULL)return block;
    block->blockType=_misc_expr;
    block->em=_endMark;
    block->_double=exprCounter;
    return block;
}

//Should probably work
dataType ResultData(expression_block* operand1, expression_block* operand2, expression_block *sign){
    if (sign->oper==_less || sign->oper == _lessEq || sign->oper == _great || sign->oper == _greatEq || sign->oper == _Eq || sign->oper == _nEq) return _bool;
    else if (sign->oper==_konk) return _string;
    else if (sign->oper==_length) return _integer;
    else if (operand1->dt == _integer && operand2->dt == _integer) return _integer;
    else if ((operand1->dt == _integer && operand2->dt == _number) || (operand1->dt == _number && operand2->dt == _integer)) return _number;
    else if (operand1->dt == _number && operand2->dt == _number) return _number;
    else return _nan;
}

//Should probably work
int TypeCheck(expression_block* operand1,expression_block* sign, expression_block* operand2){
    if (sign->blockType==_operator_expr && sign->oper==_length){
        if (operand2==NULL && operand1->blockType==_operand_expr && operand1->dt==_string)
            return 1;
        else
            return 0;
    }else if (sign->blockType==_operator_expr && sign->oper==_konk){
        if (operand1->blockType==_operand_expr && operand1->dt==_string && operand2->blockType==_operand_expr && operand2->dt==_string)
            return 1;
        else
            return 0;
    }else if (sign-> blockType==_operator_expr && (sign->oper==_Eq || sign->oper==_nEq || sign->oper==_less || sign->oper==_lessEq || sign->oper==_great || sign->oper==_greatEq)){
        if (operand1->blockType==_operand_expr && operand2->blockType==_operand_expr){
            if (operand1->dt==_integer && operand2->dt== _integer)return 1;
            else if (operand1->dt==_number && operand2->dt == _integer){convert_to_float(operand2); return 1;}
            else if (operand1->dt==_integer && operand2->dt == _number) {convert_to_float(operand1); return 1;} //TODO -- NOT SURE ABOUT THIS ONE
            else if (operand1->dt == _number && operand2->dt == _number) return 1; //DAJ SEM MOZNO ROVNO VYPIS
            else if (operand1->dt==_string && operand2->dt == _string) return 1;
            else if (operand1->dt==_bool && operand2->dt == _bool) return 1;
            else if (operand1->dt==_nan || operand2->dt==_nan) return 1;
            return 0;
        }else
            return 0;  
    }else if (sign->blockType==_operator_expr){
        if(operand1->dt == _integer && operand2->dt==_integer)return 1;
        else if(operand1->dt == _integer && operand2->dt==_number) {convert_to_float(operand1); return 1;} //<----------------------------------------------------------------------------------------------- JUROVE FUNKCIE
        else if(operand1->dt == _number && operand2->dt ==_integer) {convert_to_float(operand2); return 1;}
        else if(operand1->dt == _number && operand2->dt == _number) return 1;
        else return 0;
    }
    else return 0;

}

//DNT
void GetClosestTerminal(BubbleStack_t *stack, expression_block **block){
    *block=BS_TopStack(stack);
    if (stack_err_flag == 1){BS_Dispose(stack);comError(99)}
    if ((*block)->operType != _not_terminal_oper){ 
        return;
    }else{
        expression_block *helper;
        helper = *block;
        BS_Pop(stack);
        *block=BS_TopStack(stack);
        if (stack_err_flag == 1) {BS_Dispose(stack);comError(99)}
        BS_Push(stack,helper);
    }
    return;
}

//DNT
void TAB_Shift(BubbleStack_t *stack,token *nextToken, int *termNumber){
    expression_block *block;
    if(nextToken->type==_identifier || nextToken->type==_const || (nextToken->type == _misc && nextToken->data.msc == _bracketL)){
        block = FillBeginMark();
        if(block == NULL) {BS_Dispose(stack);comError(99)}
        BS_Push(stack,block);
        if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
    }else
    if (nextToken->type==_operator){
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
    if (nextToken->type == _keyword && nextToken->data.kw==_nil){
        block = FillBeginMark();
        if(block == NULL) {BS_Dispose(stack);comError(99)}
        BS_Push(stack,block);
        if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
    }
    block=malloc(sizeof(expression_block));
    if(block==NULL){BS_Dispose(stack);comError(99)}
    int test = ConvertToBlock(block,nextToken,termNumber);
    if (test != 0) {BS_Dispose(stack);comError(2)}
    NEXT;
    BS_Push(stack,block);
    if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
}

//DNT
void TAB_Equals(BubbleStack_t *stack, token *nextToken){
    expression_block *nt=NULL;
    expression_block *del=NULL;
    nt=BS_TopStack(stack);
    BS_Pop(stack);
    del=BS_TopStack(stack);                                          //TU by sa mozno hodili kontroly popovanych veci
    BS_Pop(stack);
    free(del);
    del=BS_TopStack(stack);
    BS_Pop(stack);
    free(del);
    BS_Push(stack,nt);
    if (stack_err_flag != 0) {BS_Dispose(stack);comError(99)}
    NEXT;
}

//DNT
void TAB_Terminalize(BubbleStack_t *stack,token *nextToken,int* termNumber){
    (void)nextToken;//Remove later
    expression_block *operand1 = NULL,*operand2=NULL,*sign=NULL,*bumper=NULL;
    operand2=BS_TopStack(stack);
    BS_Pop(stack);
    sign=BS_TopStack(stack);
    BS_Pop(stack);
    //Var Na Non term
    if (sign->blockType==_misc_expr && sign->em==_bgnMark){
        free(sign);
        expression_block *newTerm = malloc(sizeof(expression_block));
        if(newTerm==NULL){free(operand2);BS_Dispose(stack);comError(99)}
        newTerm->blockType=_operand_expr;
        newTerm->operType=_not_terminal_oper;
        newTerm->dt=operand2->dt;
        newTerm->_integer=*termNumber;
        newTerm->_double=exprCounter;
        newTerm->TSPointer=operand2->TSPointer;
        newTerm->str=operand2->str;
        (*termNumber)++;

        var_to_term_assign(newTerm,operand2);//<-----------------------------------------------------------LK FIXME
        if(operand2->dt==_string)free(operand2->str);
        BS_Push(stack,newTerm);
        if(stack_err_flag==1){free(operand2);BS_Dispose(stack);comError(99)}
        free(operand2);
        return;
    }
    operand1=BS_TopStack(stack);
    BS_Pop(stack);
    //Unary Operand
    if (operand1->blockType==_misc_expr && operand1->em==_bgnMark){
        free(operand1);
        if(TypeCheck(operand2,sign,NULL)==0){free(operand2);free(sign); BS_Dispose(stack);comError(6)}
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
    //diary operand *duary *bilingual *binary
    if(bumper->blockType==_misc_expr && bumper->em == _bgnMark){
        free(bumper);
        if (TypeCheck(operand1,sign,operand2)==0){free(operand1);free(operand2);free(sign); BS_Dispose(stack);comError(6)}
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
    }else{free(bumper);free(operand1);free(operand2);free(sign);BS_Dispose(stack);comError(6)}
}

//DNT
actions ChooseAction(BubbleStack_t *stack,token* nextToken){
    expression_block *block=NULL;
    GetClosestTerminal(stack,&block);
    //#
    if (block->blockType == _operator_expr && block->oper == _length){
        if (nextToken->type == _operator && nextToken->data.oper == _length)
            return _tab_terminalize; //NOT SURE ABOUT THIS ONE, CHECK LATER, MIGHT BE STRAIGHT OUT ERROR
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr && (block->oper == _mul || block->oper==_div || block->oper==_div2)){
        if (nextToken->type == _operator && nextToken->data.oper == _length)
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr && (block->oper == _sub || block->oper == _add)){
        if (nextToken->type == _operator && (nextToken->data.oper == _length || nextToken->data.oper == _mul || nextToken->data.oper == _div || nextToken->data.oper == _div2))
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr && block->oper == _konk){
        if (nextToken->type == _operator && (nextToken->data.oper == _length || nextToken->data.oper == _mul || nextToken->data.oper == _div || nextToken->data.oper == _div2 || nextToken->data.oper == _sub || nextToken->data.oper == _add))
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (nextToken->type==_const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operator_expr){
        if (nextToken->type == _operator && (nextToken->data.oper == _length || nextToken->data.oper == _mul || nextToken->data.oper == _div || nextToken->data.oper == _div2 || nextToken->data.oper == _sub || nextToken->data.oper == _add || nextToken->data.oper == _konk ))
            return _tab_shift;
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (nextToken->type==_const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            {BS_Dispose(stack);comError(2)}
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _misc_expr && block->em == _lbr){
        if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_equals;
        else if (nextToken->type == _operator)
            return _tab_shift;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _misc_expr && block->em == _rbr){
        if (nextToken->type == _operator)
            return _tab_terminalize;
        else if(nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if(IsEndSymbol(nextToken))
            return _tab_end;
        else if (nextToken->type ==_identifier)
            return _tab_end;    //IT SHOULD END HERE, NOT SURE IF THIS IS ENOUGH
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _operand_expr && (block->operType == _variable_oper || block->operType==_constant_oper)){
        if (nextToken->type == _operator)
            return _tab_terminalize;
        else if(nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if(IsEndSymbol(nextToken)){
            return _tab_end;
        }
        else if (nextToken->type ==_identifier)
            return _tab_end;    //IT SHOULD END HERE, NOT SURE IF THIS IS ENOUGH
        else
            {BS_Dispose(stack);comError(2)}
    }else
    if (block->blockType == _misc_expr && block->em== _endMark){
        if (nextToken->type == _operator)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (nextToken->type == _const)
            return _tab_shift;
        else if (nextToken->type == _keyword && nextToken->data.kw==_nil)
            return _tab_shift;
        else if(IsEndSymbol(nextToken))
            return _tab_end; // TODO AGAIN NOT SURE ABOUT THIS
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_end;
        else
            {BS_Dispose(stack);comError(2)}
    }else
        {BS_Dispose(stack);comError(2)}
}

expression_block* SolveCycle(BubbleStack_t* stack,token *nextToken,int *termNumber){
    actions curAction = ChooseAction(stack,nextToken);
    while (curAction!=_tab_end){
        if(curAction==_tab_shift){
            TAB_Shift(stack,nextToken,termNumber);
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

expression_block* CallTheCommander(token *nextToken){
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Add undefined and unknown var support and also the floats
    expression_block *block = NULL;
    BubbleStack_t stack;
    int termNumber = 0;
    BS_Init(&stack);
    if(stack_err_flag){comError(99)}
    block = FillEndMark();
    if(block==NULL){BS_Dispose(&stack); comError(99)}
    BS_Push(&stack,block);
    expression_block* result = SolveCycle(&stack,nextToken,&termNumber);
    BS_Dispose(&stack);
    return result;
}

