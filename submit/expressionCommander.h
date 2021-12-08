/**
 * Implementace překladače imperativního jazyka IFJ21
 *
 * @file expressionCommander.c
 * @author Tomas Lukac < xlukac16 xlukac16@fit.vutbr.cz >
 * @brief Semantic analysis of expressions
 * @version 1.0
 * @date 2021-12-08
 * 
 * 
 */
#ifndef expressionCommander
#define expressionCommander
#include "Defs.h"
#include "symtable.h"
#include "BubbleStack.h"
#include "Code_Assembly.h"
#include <string.h>

typedef enum {_tab_shift,_tab_equals,_tab_terminalize,_tab_end}actions;

#define INVALID_OPERATION 8
#define IsPushMark (block->blockType == _misc_expr && block->em == _bgnMark)
#define IsEndMark (block->blockType == _misc_expr && block->em == _endMark)
#define IsErrMark (block->blockType == _operand_expr && block->operType == _std_error)
#define IsTerminal (block->blockType!=_operand_expr && block->operType != _not_terminal_oper)

//VL FUNCS
/**
 * @brief Inits array of variables, use in multiple assignments
 * 
 * @return TreeElement** 
 */
TreeElement** VL_INIT();

/**
 * @brief Appends a new variable, use in multiple assignment
 * 
 * @param vl 
 * @param new 
 * @return TreeElement** 
 */
TreeElement** VL_PUSH(TreeElement **vl,TreeElement* new);

/**
 * @brief Disposes of array of vars
 * 
 */
void VL_Dispose (TreeElement **);

/**
 * @brief Checks if function has correct parametres, and converts the ints and floats if needed
 * 
 * @param func 
 * @param stack 
 * @return int 
 */
int ParamCheck(TreeElement* func, BubbleStack_t *stack);

/**
 * @brief In assign, checks if types are compatible
 * 
 * @return int 
 */
int SemanticCheck(TreeElement**,BubbleStack_t *);

/**
 * @brief In return, checks if all returning values are of correct type and adds nil values if needed, also disposess of excess values
 * 
 * @return int 
 */
int ReturnsCheck(TreeElement *, BubbleStack_t *);

/**
 * @brief Helping function that just appends elements of one stack to other
 * 
 */
void MoveStack(BubbleStack_t*, BubbleStack_t*);

/**
 * @brief In assignment of single variable
 * 
 * @return int 
 */
int C_AssignToVar(TreeElement* , expression_block* );

/**
 * @brief Recursive function, that checks if all used functions were defined
 * 
 * @param root 
 */
void TS_Looper(TreeElement* root);

/**
 * @brief Calls TS_Looper, to check if all used functions were defined
 * 
 */
void TS_FunctionCaller();

/**
 * @brief checks if token is one of those that end expression by default
 * 
 * @param nextToken 
 * @return int 
 */
int IsEndSymbol(token *nextToken);

/**
 * @brief converts token to block
 * 
 * @param block 
 * @param nextToken 
 * @return int 
 */
int ConvertToBlock(expression_block *block, token* nextToken);

/**
 * @brief Fills with error marker used to distinguish errors from malloc errors
 * 
 * @return expression_block* 
 */
expression_block* FillErrorMark();

/**
 * @brief Fills begin mark to expression block
 * 
 * @return expression_block* 
 */
expression_block* FillBeginMark();

/**
 * @brief Fills end mark to expression block
 * 
 * @return expression_block* 
 */
expression_block* FillEndMark();

/**
 * @brief Decides the data type of the result of operation
 * 
 * @param operand1 
 * @param operand2 
 * @param sign 
 * @return dataType 
 */
dataType ResultData(expression_block* operand1, expression_block* operand2, expression_block *sign);

/**
 * @brief Checks if its possible to make the operation 
 * 
 * @param operand1 
 * @param sign 
 * @param operand2 
 * @return int 
 */
int TypeCheck(expression_block* operand1,expression_block* sign, expression_block* operand2);

/**
 * @brief Get the Closest Terminal object
 * 
 * @param stack 
 * @param block 
 * @return int 
 */
void GetClosestTerminal(BubbleStack_t *stack, expression_block **block);

/**
 * @brief Makes < operation from tab
 * 
 * @param stack 
 * @param nextToken  
 */
void TAB_Shift(BubbleStack_t *stack,token *nextToken);

/**
 * @brief Makes = operation from tab
 * 
 * @param stack 
 * @param nextToken 
 */
void TAB_Equals(BubbleStack_t *stack, token *nextToken);

/**
 * @brief Makes > operation from tab
 * 
 * @param stack 
 * @param nextToken 
 * @param termNumber 
 * @return int 
 */
void TAB_Terminalize(BubbleStack_t *stack,token *nextToken,int* termNumber);

/**
 * @brief Choose action from the tab 
 * 
 * @param stack 
 * @param nextToken 
 * @return actions 
 */
actions ChooseAction(BubbleStack_t *stack,token* nextToken);

/**
 * @brief Cycle of suffering eternal, main cycle that resolves expression
 * 
 * @param stack 
 * @param nextToken 
 * @param termNumber 
 * @return expression_block 
 */
expression_block* SolveCycle(BubbleStack_t* stack,token *nextToken,int *termNumber);

/**
 * @brief this is main of this program
 * 
 * @param nextToken 
 * @return expression_block
 */
expression_block* CallTheCommander(token *nextToken);
#endif
/*file end*/
