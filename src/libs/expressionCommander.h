#ifndef expressionCommander
#define expressionCommander
#include "Defs.h"
#include "TS.h"
#include "BubbleStack.h"

//TODO REMOVE THE LAST TWO IF THEY ARE NOT NEEDED
typedef enum {_tab_shift,_tab_equals,_tab_terminalize,_tab_end,_tab_error}actions;

#define INVALID_OPERATION 8
#define IsPushMark (block->blockType == _misc_expr && block->em == _bgnMark)
#define IsEndMark (block->blockType == _misc_expr && block->em == _endMark)
#define IsTerminal (block->blockType!=_operand_expr && block->operType != _not_terminal_oper)


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
int GetClosestTerminal(BubbleStack_t *stack, expression_block **block);

/**
 * @brief Makes < operation
 * 
 * @param stack 
 * @param nextToken 
 * @return int 
 */
int TAB_Shift(BubbleStack_t *stack,token *nextToken);

/**
 * @brief Makes = operation from tab
 * 
 * @param stack 
 * @param nextToken 
 * @return int 
 */
int TAB_Equals(BubbleStack_t *stack, token *nextToken);

/**
 * @brief Makes > operation 
 * 
 * @param stack 
 * @param nextToken 
 * @param termNumber 
 * @return int 
 */
int TAB_Terminalize(BubbleStack_t *stack,token *nextToken,int* termNumber);

/**
 * @brief Choose action from the tab 
 * 
 * @param stack 
 * @param nextToken 
 * @return actions 
 */
actions ChooseAction(BubbleStack_t *stack,token* nextToken);

/**
 * @brief Cycle of suffering eternal 
 * 
 * @param stack 
 * @param nextToken 
 * @param termNumber 
 * @return int 
 */
int SolveCycle(BubbleStack_t* stack,token *nextToken,int *termNumber);

/**
 * @brief this is main of this program
 * 
 * @param nextToken 
 * @return int 
 */
int CallTheCommander(token *nextToken);
#endif