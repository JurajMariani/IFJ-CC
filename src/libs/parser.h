#ifndef PARSER_HH
#define PARSER_HH

#include "Defs.h"
#include "TS.h"
#include "BubbleStack.h"
#include "expressionCommander.h"
#include <string.h>

#define DONE 2 //<-- NOt sure if this should be used
#define INTERNAL_UPDATE_PARAM_NAME_ERROR 1001011

typedef enum{N_Prog,N_Params,N_ParamsR,N_SecondParamVar,N_SecondParam, N_SecondParamType,N_SecondParamR,N_Type,N_StList,N_Statement,
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

int F_Params(token* nextToken, char* );

int F_ParamsR(token *nextToken, char* );

int F_SecondParam(token* nextToken, char* );

int F_SecondParamR(token* nextToken, char* );

int F_Type(token* nextToken, dataType* );

int F_StList(token* nextToken);

int F_Statement(token* nextToken);

int F_Else(token* nextToken);

int F_Exprb(token* nextToken, BubbleStack_t *);

int F_ExprAfterLoc(token* nextToken, TreeElement*);

expression_block* F_Expression(token* nextToken);

int F_SentPar(token* nextToken, BubbleStack_t *);

int F_SPar(token* nextToken, BubbleStack_t *);

int F_SExpr(token* nextToken, BubbleStack_t *);

int F_SVar(token* nextToken, TreeElement ***);

//Will see if needed,mainly testing purposes
int mainParseFunction(); 

dataType* PushDataType(dataType *toPush,dataType newType);
#endif