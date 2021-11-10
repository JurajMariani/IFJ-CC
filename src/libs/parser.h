#include "Defs.h"
#include "TS.h"
#include "BubbleStack.h"

#define NEXT nextToken=GetNextToken(nextToken)
#define DONE 2

typedef enum{N_Prog,N_Params,N_ParamsR,N_SecondParamVar,N_SecondParamType,N_SecondParamR,N_Type,N_StList,N_Statement,
                N_Else,N_SOperExpr,N_Expr_AfterLoc,N_Expr,N_ExprC,N_ExprE,N_SentPar,N_SPar,N_SExpr,
                    N_SVar,N_Operand,N_OperandWs}notTerminal;

TreeSupport ts; //global variable
/**
 * This is main recursive function that turns not terminals to terminals, by seeing the next token
 * next token is handled as adress, so it can be updated by calling get next token anywhere
 * I will use multiple functions to delegate work
 * @param notTerminal to process
 * @param token* next unprocessed token
 * @return 0 on sucess, 1 on invalid termianl find
 */ 
int workNotTerminal(notTerminal terminal,token* nextToken);

int F_Prog(token* nextToken);

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

//Will see if needed,mainly testing purposes
int mainParseFunction(); 

int PushDataType(dataType *toPush,dataType newType);