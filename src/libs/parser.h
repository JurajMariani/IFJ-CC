#ifndef PARSER_HH
#define PARSER_HH

#include "Defs.h"
#include "TS.h"
#include "BubbleStack.h"
#include "expressionCommander.h"

#define DONE 2 //<-- NOt sure if this should be used

typedef enum{N_Prog,N_Params,N_ParamsR,N_SecondParamVar,N_SecondParamType,N_SecondParamR,N_Type,N_StList,N_Statement,
                N_Else,N_Expr_AfterLoc,N_Exprb,N_Expression,N_SentPar,N_SPar,N_SExpr,
                    N_SVar}notTerminal;

/**
 * @brief frees all the allocs
 * @param ts pointer
 */
void FreeTheseBees(TreeSupport *ts);

/**
 * @brief Generates and writes error from parameter
 * @param errCode 
 */
void GiveMeAnError(int errCode);

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

int F_Exprb(token* nextToken);

int F_ExprAfterLoc(token* nextToken);

int F_Expression(token* nextToken);

int F_SentPar(token* nextToken);

int F_SPar(token* nextToken);

int F_SExpr(token* nextToken);

int F_SVar(token* nextToken);

//Will see if needed,mainly testing purposes
int mainParseFunction(); 

int PushDataType(dataType *toPush,dataType newType);
#endif