/**
 * @file BubbleStack.h
 * @author xmaria03 (xmaria03@stud.fit.vutbr.cz)
 * @brief Header file for Stack ADT operations
 * @date 2021-10-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __BubbleStack__
#define __BubbleStack__
#include "Defs.h"

#define STACK_ERROR -2
#define CHECK_FLAG if(stack_err_flag != 0)              \
                    return STACK_ERROR
#define BS_MAX_ELEMS 200 - 1


// STACK FUNCTIONS:

/**
 * @brief BubbleStack_t structure initialisation function
 *
 * @param stack pointer to the BubbleStack_t stack structure
 */
void BS_Init( BubbleStack_t* );


/**
 * @brief Saves the pointer to Bubble_t element on the top of the stack
 *
 * @param stack pointer to the BubbleStack_t stack structure
 * @param data pointer to the Bubble_t variable, where the pointer to the top of the stack is saved
 *
 * @return ...
 */
expression_block* BS_TopStack( BubbleStack_t* );


/**
 * @brief The data are inserted on the top of the stack
 *
 * @param stack pointer to the BubbleStack_t stack structure
 * @param data pointer to the Bubble_t variable, where the data pushed on the top of the stack are
 */
void BS_Push( BubbleStack_t* , expression_block* );


/**
 * @brief Decrements the top of the stack, thus forgetting the last top element
 *
 * @param stack pointer to the BubbleStack_t stack structure
 */
void BS_Pop( BubbleStack_t* );


/**
 * @brief Checks, whether the Stack structure contains any elements
 *
 * @param stack pointer to the BubbleStack_t stack structure
 *
 * @return non-zero value when there are elements within the stack, zero when there are none
 */
int BS_IsEmpty( BubbleStack_t* );


/**
 * @brief Checks, whether the Stack structure contains space for new elements
 *
 * @param stack pointer to the BubbleStack_t stack structure
 *
 * @return non-zero value when there is no space available, zero when there is
 */
int BS_IsFull( BubbleStack_t* );


/**
 * @brief Dtor. Destroys the contents of the stack
 * 
 * @param stack pointer to the BubbleStack_t structure
 */
void BS_Dispose( BubbleStack_t* );

#endif

/** ---- End of File BubbleStack.h ---*/
