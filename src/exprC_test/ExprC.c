#include "../libs/expressionCommander.h"

int curToken = 0;
token tokenArr[25];

void Chain1Init(){/*
     tokenArr[0].type=_operator;
     tokenArr[0].data.oper=_lessEq;
     tokenArr[1].type=_operator;
     tokenArr[1].data.oper=_mul;
     tokenArr[2].type=_const;
     tokenArr[2].data.type=_string;
     tokenArr[2].data.str="vraj nemozem nadavat";
     tokenArr[3].type=_misc;
     tokenArr[3].data.msc=_EOF;
     */
    /*
    tokenArr[0].type=_const;
    tokenArr[0].data.type=_integer;
    tokenArr[0].data._integer=1;
    tokenArr[1].type=_operator;
    tokenArr[1].data.oper=_add;
    tokenArr[2].type=_const;
    tokenArr[2].data.type=_integer;
    tokenArr[2].data._integer=1;
    tokenArr[3].type=_misc;
    tokenArr[3].data.msc=_EOF;
    */
    /*
    tokenArr[0].type=_const;
    tokenArr[0].data.type=_integer;
    tokenArr[0].data._integer=1;
    tokenArr[1].type=_operator;
    tokenArr[1].data.oper=_add;
    tokenArr[2].type=_const;
    tokenArr[2].data.type=_integer;
    tokenArr[2].data._integer=1;
    tokenArr[3].type=_operator;
    tokenArr[3].data.oper=_mul;
    tokenArr[4].type=_misc;
    tokenArr[4].data.msc=_bracketL;
    tokenArr[5].type=_identifier;
    tokenArr[5].data.type=_integer;
    tokenArr[5].data.str="a";
    tokenArr[6].type=_operator;
    tokenArr[6].data.oper=_div2;
    tokenArr[7].type=_identifier;
    tokenArr[7].data.str="tvoja mama";
    tokenArr[7].data.type=_number;
    tokenArr[8].type=_misc;
    tokenArr[8].data.msc=_bracketR;
    tokenArr[9].type=_operator;
    tokenArr[9].data.oper=_div;
    tokenArr[10].type=_operator;
    tokenArr[10].data.oper=_length;
    tokenArr[11].type=_misc;
    tokenArr[11].data.msc=_bracketL;
    tokenArr[12].type=_identifier;
    tokenArr[12].data.str="this";
    tokenArr[12].data.type=_string;
    tokenArr[13].type=_operator;
    tokenArr[13].data.oper=_konk;
    tokenArr[14].type=_const;
    tokenArr[14].data.type=_string;
    tokenArr[14].data.str="not this";
    tokenArr[15].type=_misc;
    tokenArr[15].data.msc=_bracketR;
    tokenArr[16].type=_misc;
    tokenArr[16].data.msc=_EOF;
    */
    /*
    tokenArr[0].type=_const;
    tokenArr[0].data.type=_integer;
    tokenArr[0].data._integer=1;
    tokenArr[1].type=_operator;
    tokenArr[1].data.oper=_less;
    tokenArr[2].type=_const;
    tokenArr[2].data.type=_integer;
    tokenArr[2].data._integer=2;
    tokenArr[3].type=_misc;
    tokenArr[3].data.msc=_EOF;
    */
    tokenArr[0].type=_const;
    tokenArr[0].data.type=_integer;
    tokenArr[0].data._integer=1;
    tokenArr[1].type=_operator;
    tokenArr[1].data.oper=_less;
    tokenArr[2].type=_operator;
    tokenArr[2].data.oper=_less;
    tokenArr[2].data._double=0.2;
    tokenArr[3].type=_misc;
    tokenArr[3].data.msc=_EOF;
    
}

void GetNextToken(token* nextToken){
    if (nextToken->type==_misc && nextToken->data.msc==_EOF)return;
    nextToken->type=tokenArr[curToken].type;
    nextToken->data.kw=tokenArr[curToken].data.kw;
    nextToken->data.msc=tokenArr[curToken].data.msc;
    nextToken->data.oper=tokenArr[curToken].data.oper;
    nextToken->data.str=tokenArr[curToken].data.str;
    nextToken->data.type=tokenArr[curToken].data.type;
    nextToken->data._double=tokenArr[curToken].data._double;
    nextToken->data._integer=tokenArr[curToken].data._integer;
    curToken++;
    return;
    
}

int main(){
    lex_err_flag=0;
	stack_err_flag=0;
    ts = NULL;
    TS_Init(&ts);
    BubbleStack_t stack;
    BS_Init(&stack);

    variable* newVar = CreateVariableData(_integer);
    TS_InsertVariable(ts,"a",w_var,newVar);
    variable* newVar2 = CreateVariableData(_number);
    TS_InsertVariable(ts,"tvoja mama",w_var,newVar2);
    variable* newVar3 = CreateVariableData(_string);
    TS_InsertVariable(ts,"this",w_var,newVar3);
    token* nextToken=malloc(sizeof(token));

    Chain1Init();

    GetNextToken(nextToken);
    
    CallTheCommander(nextToken);
    /* write
    expression_block *newBlock=FillBeginMark();
    DebbugPrintExpress(newBlock);
    free(newBlock);
    for (int i=0;i<25;i++){
        newBlock=malloc(sizeof(expression_block));
        if(ConvertToBlock(newBlock,nextToken)==0){
            DebbugPrintExpress(newBlock);
        }
        GetNextToken(nextToken);
        free(newBlock);
    }*/
    /* single s
    expression_block *op1 = malloc(sizeof(expression_block));
    expression_block *op2 = malloc(sizeof(expression_block));
    expression_block *sign = malloc(sizeof(expression_block));
    ConvertToBlock(op1,nextToken);
    GetNextToken(nextToken);
    ConvertToBlock(sign,nextToken);
    GetNextToken(nextToken);
    ConvertToBlock(op2,nextToken);
    GetNextToken(nextToken);
    printf("%d",TypeCheck(op1,sign,op2));
    newBlock=FillEndMark();
    DebbugPrintExpress(newBlock);
    */
    free(nextToken);
    TS_COLLAPSE(ts);
    BS_Dispose(&stack);
    return 0;
}
