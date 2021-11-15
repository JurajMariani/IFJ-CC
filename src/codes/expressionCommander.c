#include "../libs/expressionCommander.h"

//TODO Juro ide menit ten errflag, alebo mozno ne nwm
/* TODO FIND OUT HOW TO DO THIS
//Only used to shorten the code in tab search <-- YES, It's dumb but what you gonna do
#define sortRest 
        else if (nextToken->type == _operator)
            return _tab_terminalize;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if (IsEndSymbol(nextToken))
            return _tab_terminalize;
        else
            return _tab_error;
*/

//DNT
int IsEndSymbol(token *nextToken){
    if ((nextToken->type == _keyword && nextToken->data.kw == _end) ||
    (nextToken ->type == _keyword && nextToken->data.kw == _return)||
    (nextToken ->type == _keyword && nextToken->data.kw == _while) ||
    (nextToken ->type == _keyword && nextToken->data.kw == _if)||
    (nextToken ->type == _keyword && nextToken->data.kw == _local)||
    (nextToken ->type == _identifier && TS_LookFunction(ts,nextToken->data.str)!=0))
    return 1;
    else return 0; 
}

//DNT
int ConvertToBlock(expression_block *block, token* nextToken){
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
    }else
    if (nextToken->type==_const){
        block->blockType=_operand_expr;
        block->operType=_constant_oper;
        block->dt=nextToken->data.type;
        //_integer,_number,_string
        if (block->dt==_integer)
            block->_integer=nextToken->data._integer;
        else if (block->dt==_number)
            block->_double=nextToken->data._double;
        else if (block->dt==_string)
            block->str=nextToken->data.str;
    }
    return 0;
}

//DNT
expression_block* FillBeginMark(){
    expression_block* block=malloc(sizeof(expression_block));
    if (block==NULL)return block;
    block->blockType=_misc_expr;
    block->em=_bgnMark;
    return block;
}

//DNT
expression_block* FillEndMark(){
    expression_block* block=malloc(sizeof(expression_block));
    if (block==NULL)return block;
    block->blockType=_misc_expr;
    block->em=_endMark;
    return block;
}

//DNT
dataType ResultData(expression_block* operand1, expression_block* operand2, expression_block *sign){
    if (sign->oper==_less || sign->oper == _lessEq || sign->oper == _great || sign->oper == _greatEq || sign->oper == _Eq || sign->oper == _nEq) return _bool;
    else if (sign->oper==_konk) return _integer;
    else if (operand1->dt == _integer && operand2->dt == _integer) return _integer;
    else if ((operand1->dt == _integer && operand2->dt == _number) || (operand1->dt == _number && operand2->dt == _integer)) return _number;
    else if (operand1->dt == _number && operand2->dt == _number) return _number;
    else if (operand1->dt == _string && operand2->dt == _string) return _string;
    else return _nan;
}

//DNT
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
            else if ((operand1->dt==_number && operand2->dt == _integer) || (operand1->dt==_integer && operand2->dt == _number)) return 0; //TODO -- NOT SURE ABOUT THIS ONE
            else if (operand1->dt == _number && operand2->dt == _number) return FLOAT_INT_COMPARISM; //DAJ SEM MOZNO ROVNO VYPIS
            else if (operand1->dt==_string && operand2->dt == _string) return 1;
            else if (operand1->dt==_bool && operand2->dt == _bool) return 1;
            else return 0;
        }else
            return 0;  
    }else if (sign->blockType==_operator_expr){
        if(operand1->dt == _integer && operand2->dt==_integer)return 1;
        else if(operand1->dt == _integer && operand2->dt==_number) {/*ConvertToFloat(operand1);*/ return 1;} //<----------------------------------------------------------------------------------------------- JUROVE FUNKCIE
        else if(operand1->dt == _number && operand2->dt ==_integer) {/*ConvertToFloat(operand2);*/ return 1;}
        else if(operand1->dt == _number && operand2->dt == _number) return 1;
        else return 0;
    }
    else return 0;

}

//DNT
int GetClosestTerminal(BubbleStack_t *stack, expression_block *block){
    BS_TopStack(stack,block);
    if (err_flag == 1) return err_flag;
    if (IsTerminal) return 0;
    else{
        expression_block *helper;
        helper = block;
        BS_Pop(stack);
        BS_TopStack(stack,block);
        if (err_flag == 1) return err_flag;
        BS_Push(stack,helper);
    }
    return 0;
}

//DNT
int TAB_Shift(BubbleStack_t *stack,token *nextToken){
    expression_block *block;
    if(nextToken->type==_identifier || nextToken->type==_const || (nextToken->type == _misc && nextToken->data.msc == _bracketL)){
        block = FillBeginMark();
        if(block == NULL) return MALLOC_ERR_CODE;
        BS_Push(stack,block);
        if (err_flag != 0) return MALLOC_ERR_CODE;
    }else
    if (nextToken->type==_operator){
        block = FillBeginMark();
        if (block == NULL) return MALLOC_ERR_CODE;
        expression_block *helper=NULL;
        BS_TopStack(stack,helper);
        BS_Pop(stack);
        BS_Push(stack,block);
        if (err_flag != 0) return MALLOC_ERR_CODE;
        BS_Push(stack,helper);
        if (err_flag != 0) return MALLOC_ERR_CODE;
    }
    int test = ConvertToBlock(block,nextToken);
    if (test != 0) return test;
    NEXT;
    BS_Push(stack,block);
    if (err_flag != 0) return MALLOC_ERR_CODE;
    return 0;
}

//DNT
int TAB_Equals(BubbleStack_t *stack, token *nextToken){
    expression_block *nt=NULL;
    BS_TopStack(stack,nt);
    BS_Pop(stack);                                          //TU by sa mozno hodili kontroly popovanych veci
    BS_Pop(stack);
    BS_Pop(stack);
    BS_Push(stack,nt);
    if (err_flag != 0) return MALLOC_ERR_CODE;
    NEXT;
    return 0;
}

//DNT
int TAB_Terminalize(BubbleStack_t *stack,token *nextToken,int* termNumber){
    (void)nextToken;//Remove later
    expression_block *operand1 = NULL,*operand2=NULL,*sign=NULL,*bumper=NULL;
    BS_TopStack(stack,operand2);
    BS_Pop(stack);
    BS_TopStack(stack,sign);
    BS_Pop(stack);
    //Var Na Non term
    if (sign->blockType==_misc_expr && sign->em==_bgnMark){
        expression_block *newTerm = malloc(sizeof(expression_block));
        if(newTerm==NULL)return MALLOC_ERR_CODE;
        newTerm->blockType=_operand_expr;
        newTerm->operType=_not_terminal_oper;
        newTerm->dt=operand2->dt;
        newTerm->_integer=*termNumber;
        (*termNumber)++;
        //VarToTermAssign(newTerm,operand2); //<------------------------------------------------------------ JUROVA FUNKCIA
        BS_Push(stack,newTerm);
        if(err_flag==1)return MALLOC_ERR_CODE;
        return 0;
    }
    BS_TopStack(stack,operand1);
    BS_Pop(stack);
    //Unary Operand
    if (operand1->blockType==_misc_expr && operand1->em==_bgnMark){
        if(TypeCheck(operand2,sign,NULL)==0)return INVALID_OPERATION;
        expression_block *newTerm = malloc(sizeof(expression_block));
        if(newTerm==NULL)return MALLOC_ERR_CODE;
        newTerm->blockType=_operand_expr;
        newTerm->operType=_not_terminal_oper;
        newTerm->dt=_integer;
        newTerm->_integer=*termNumber;
        (*termNumber)++;
        //UnaryOperator(newTerm,operand2); //<----------------------------------------------------------------------- JUROVA FUNKCIA
        BS_Push(stack,newTerm);
        if(err_flag==1)return MALLOC_ERR_CODE;
        return 0;
    }
    BS_TopStack(stack,bumper);
    BS_Pop(stack);
    //diary operand *duary *bilingual *binary
    if(bumper->blockType==_misc_expr && bumper->em == _bgnMark){
        if (TypeCheck(operand1,sign,operand2)==0)return INVALID_OPERATION;
        expression_block *newTerm = malloc(sizeof(expression_block));
        if(newTerm==NULL)return MALLOC_ERR_CODE;
        newTerm->blockType=_operand_expr;
        newTerm->operType=_not_terminal_oper;
        newTerm->dt=ResultData(operand1,operand2,sign);
        newTerm->_integer=*termNumber;
        (*termNumber)++;
        //OperationQuickAction(newTerm,operand1,sign,operand2);<--------------------------------------------------------Jurov funkca
        BS_Push(stack,newTerm);
        if(err_flag==1)return MALLOC_ERR_CODE;
        return 0;
    }
    return INVALID_EXPRESSION;
}

//DNT
actions ChooseAction(BubbleStack_t *stack,token* nextToken){
    expression_block *block=NULL;
    if (GetClosestTerminal(stack,block)!=0) return _tab_error;
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
        else if (IsEndSymbol(nextToken))
            return _tab_end;
        else
            return _tab_error;
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
        else if (IsEndSymbol(nextToken))
            return _tab_end;
        else
            return _tab_error;
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
        else if (IsEndSymbol(nextToken))
            return _tab_end;
        else
            return _tab_error;
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
        else if (IsEndSymbol(nextToken))
            return _tab_end;
        else
            return _tab_error;
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
        else if (IsEndSymbol(nextToken))
            return _tab_end;
        else
            return _tab_error;
    }else
    if (block->blockType == _misc_expr && block->em == _lbr){
        if (nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_equals;
        else if (nextToken->type == _operator)
            return _tab_shift;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else
            return _tab_error;
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
            return _tab_error;
    }else
    if (block->blockType == _operand_expr && block->operType == _variable_oper){
        if (nextToken->type == _operator)
            return _tab_terminalize;
        else if(nextToken->type == _misc && nextToken->data.msc == _bracketR)
            return _tab_terminalize;
        else if(IsEndSymbol(nextToken))
            return _tab_end;
        else if (nextToken->type ==_identifier)
            return _tab_end;    //IT SHOULD END HERE, NOT SURE IF THIS IS ENOUGH
        else
            return _tab_error;
    }else
    if (block->blockType == _misc_expr && block->em== _endMark){
        if (nextToken->type == _operator)
            return _tab_shift;
        else if (nextToken->type == _misc && nextToken->data.msc == _bracketL)
            return _tab_shift;
        else if (nextToken->type == _identifier)
            return _tab_shift;
        else if(IsEndSymbol(nextToken))
            return _tab_end; // TODO AGAIN NOT SURE ABOUT THIS
        else
            return _tab_error;
    }
    return _tab_error;

}

int SolveCycle(BubbleStack_t* stack,token *nextToken,int *termNumber){
    actions curAction = ChooseAction(stack,nextToken);
    while (curAction!=_tab_end){
        if(curAction==_tab_shift)
            TAB_Shift(stack,nextToken);
        else if (curAction==_tab_terminalize)
            TAB_Terminalize(stack,nextToken,termNumber);
        else if (curAction == _tab_equals)
            TAB_Equals(stack,nextToken);
        else return INVALID_EXPRESSION;
        curAction=ChooseAction(stack,nextToken);
    }
    while(stack->BS_TopIndex!=1)
        TAB_Terminalize(stack,nextToken,termNumber);
    expression_block* result=NULL;
    BS_TopStack(stack,result);
    //AssignToEndBlock(result); // <--------------------------------------------- JUROVA FUNKCIA
    //BS_Dispose(stack); <-------------------------------------------------------__TODO Restore
    return 0;
}

int CallTheCommander(token *nextToken){
    expression_block *block;
    BubbleStack_t stack;
    int termNumber = 0;
    BS_Init(&stack);
    block = FillEndMark();
    if(block==NULL)return MALLOC_ERR_CODE;
    BS_Push(&stack,block);
    SolveCycle(&stack,nextToken,&termNumber);
    return 0;
}