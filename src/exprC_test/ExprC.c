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
}

int GetNextToken(token* nextToken){
    if (nextToken->type==_misc && nextToken->data.msc==_EOF)return 0;
    nextToken->type=tokenArr[curToken].type;
    nextToken->data.kw=tokenArr[curToken].data.kw;
    nextToken->data.msc=tokenArr[curToken].data.msc;
    nextToken->data.oper=tokenArr[curToken].data.oper;
    nextToken->data.str=tokenArr[curToken].data.str;
    nextToken->data.type=tokenArr[curToken].data.type;
    nextToken->data._double=tokenArr[curToken].data._double;
    nextToken->data._integer=tokenArr[curToken].data._integer;
    curToken++;
    return 0;
    
}

int main(){
    TS_Init(ts);
    token* nextToken=malloc(sizeof(token));
    Chain1Init();
    GetNextToken(nextToken);
    BubbleStack_t stack;
    BS_Init(&stack);
    //for(int i=0;i<25;i++){DebbugPrintToken(nextToken);GetNextToken(nextToken);}
    //printf("\n");
    expression_block* newBlock=NULL;
    for (int i=0;i<25;i++){
        newBlock=malloc(sizeof(expression_block));
        
        //ConvertToBlock(newBlock,nextToken);
        DebbugPrintExpress(newBlock);
        GetNextToken(nextToken);
    }
    free(nextToken);
    TS_COLLAPSE(ts);
    return 0;
}
