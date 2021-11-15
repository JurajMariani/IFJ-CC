#include "../libs/expressionCommander.h"

int curToken = 0;
token tokenArr[25];

token* Chain1Init(){
     return NULL;
}

int GetNextToken(token* nextToken){
    while (nextToken->type!=_misc && nextToken->data.msc!=_EOF)
    {
        nextToken=&(tokenArr[curToken]);
        curToken++;
        return 0;
    }
    return 0;
    
}

int main(){

    return 0;
}
