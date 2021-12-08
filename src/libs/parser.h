/**
 * @file expressionCommander.c
 * @author Tomas Lukac xlukac16 xlukac16@fit.vutbr.cz Juraj Mariani xmaria03 xmaria03@fit.vutbr.cz
 * @brief 
 * @version 1.0
 * @date 2021-12-08
 * 
 * 
 */

#ifndef PARSER_HH
#define PARSER_HH

#include "Defs.h"
#include "TS.h"
#include "BubbleStack.h"
#include "expressionCommander.h"
#include "analysis.h"
#include "Code_Assembly.h"
#include <string.h>

#define DONE 2 //<-- NOt sure if this should be used
#define INTERNAL_UPDATE_PARAM_NAME_ERROR 1001011

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
int PushBuiltInFunctions();

/**
 * @brief Inits global variables, that cannot be initiated elsewhere because of program linking
 * 
 */
void initGlobals();

/**
 * @brief helper function to add parameters to function 
 * 
 * @param toPush 
 * @param newType 
 * @return dataType* 
 */
dataType* PushDataType(dataType *toPush,dataType newType);

/**
 * @brief Create a Variable Data object
 * 
 * @param newType 
 * @return variable* 
 */
variable* CreateVariableData(dataType newType);

/**
 * @brief Create a Function Data object
 * 
 * @return user_func* 
 */
user_func* CreateFunctionData();

//Following functions are all used in analyses to analyze terminals and next token

/**
 * @brief  F_Prog decisions
 * 
 * @param nextToken 
 */
void F_Prog(token* nextToken);

/**
 * @brief F_Params decisions
 * 
 * @param nextToken 
 * @param func 
 */
void F_ParamsGlobal(token *nextToken, TreeElement* func);

/**
 * @brief F_Second_param decisions
 * 
 * @param nextToken 
 * @param func 
 */
void F_SecondParamGlobal(token *nextToken,TreeElement* func);

/**
 * @brief F_Param_type decisions
 * 
 * @param nextToken 
 * @param func 
 * @param globaled , if header global has been used and if it is correctly defined 
 */
void F_ParamsFunction(token *nextToken,TreeElement* func,int globaled);

/**
 * @brief F_Second_param_type dcisions
 * 
 * @param nextToken 
 * @param func 
 * @param globaled, if header global has been used and if it is correctly defined 
 */
void F_SecondParamFunction(token *nextToken,TreeElement* func, int globaled);

/**
 * @brief F_Params_R decisions
 * 
 * @param nextToken 
 */
void F_ParamsR(token *nextToken, char* ,int);

/**
 * @brief F_Second_Param_R decisions
 * 
 * @param nextToken 
 */
void F_SecondParamR(token* nextToken, char* ,int);

/**
 * @brief F_Type decisions
 * 
 * @param nextToken 
 */
void F_Type(token* nextToken, dataType* );

/**
 * @brief F_StList decisions
 * 
 * @param nextToken 
 * @param curFunc 
 */
void F_StList(token* nextToken, TreeElement* curFunc);

 /**
  * @brief F_Statement decisions
  * 
  * @param nextToken 
  * @param curFunc 
  */
void F_Statement(token* nextToken, TreeElement* curFunc);

/**
 * @brief F_Else decisions
 * 
 * @param nextToken 
 * @param curFunc 
 */
void F_Else(token* nextToken, TreeElement* curFunc);

/**
 * @brief F_Ret decisions
 * 
 * @param nextToken 
 * @param returnStack 
 */
void Returner(token* nextToken,BubbleStack_t *returnStack);

/**
 * @brief F_ExprB decisiona
 * 
 * @param nextToken 
 */
void F_Exprb(token* nextToken, BubbleStack_t *);

/**
 * @brief F_AfterLoc decisions
 * 
 * @param nextToken 
 */
void F_ExprAfterLoc(token* nextToken, TreeElement*);

/**
 * @brief This function calls expression commander
 * 
 * @param nextToken 
 * @return expression_block* 
 */
expression_block* F_Expression(token* nextToken);

/**
 * @brief F_SentPar decisions
 * 
 * @param nextToken 
 */
void F_SentPar(token* nextToken, BubbleStack_t *);

/**
 * @brief F_SPar decisions
 * 
 * @param nextToken 
 */
void F_SPar(token* nextToken, BubbleStack_t *);

/**
 * @brief F_Second Expression decisions 
 * 
 * @param nextToken 
 */
void F_SExpr(token* nextToken, BubbleStack_t *);

/**
 * @brief F_Second variable decisions
 * 
 * @param nextToken 
 */
void F_SVar(token* nextToken, TreeElement ***);

/**
 * @brief Main function
 * 
 * @return int 
 */
int mainParseFunction(); 

#endif
/*file end*/