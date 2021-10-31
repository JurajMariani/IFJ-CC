

#include <stdio.h>
#include <stdlib.h>
#include "../libs/BubbleStack.h"


/**
 * @brief BubbleStack_t structure initialisation function
 *
 * @param stack pointer to the BubbleStack_t stack structure
 */
void BS_Init(BubbleStack_t* stack)
{
    if (stack == NULL)
    {
        err_flag = 1;
        return;
    }

    stack->BS_TopIndex = -1;
}

/**
 * @brief Saves the pointer to Bubble_t element on the top of the stack
 *
 * @param stack pointer to the BubbleStack_t stack structure
 * @param data pointer to the Bubble_t variable, where the pointer to the top of the stack is saved
 */
void BS_TopStack(BubbleStack_t* stack, Bubble_t* data)
{
    if ((stack == NULL) || (data == NULL))
    {
        err_flag = 1;
        return;
    }

    *data = stack->BS_Element[stack->BS_TopIndex];
}

/**
 * @brief The data are inserted on the top of the stack
 *
 * @param stack pointer to the BubbleStack_t stack structure
 * @param data pointer to the Bubble_t variable, where the data pushed on the top of the stack are
 */
void BS_Push(BubbleStack_t* stack, Bubble_t* data)
{
    if ((stack == NULL) || (data == NULL) || (BS_IsFull(stack)))
    {
        err_flag = 1;
        return;
    }

    stack->BS_TopIndex++;
    stack->BS_Element[ stack->BS_TopIndex ] = (*data);
}

/**
 * @brief Decrements the top of the stack, thus forgetting the last top element
 *
 * @param stack pointer to the BubbleStack_t stack structure
 */
void BS_Pop(BubbleStack_t* stack)
{
    if ( BS_IsEmpty(stack) || (stack == NULL) )
    {
        err_flag = 1;
        return;
    }

    stack->BS_TopIndex--;
}

/**
 * @brief Checks, whether the Stack structure contains any elements
 *
 * @param stack pointer to the BubbleStack_t stack structure
 *
 * @return non-zero value when there are elements within the stack, zero when there are none
 */
int BS_IsEmpty(BubbleStack_t* stack)
{

    if (stack == NULL)
    {
        err_flag = 1;
        return 1;
    }

    return ( stack->BS_TopIndex == -1 );

}

/**
 * @brief Checks, whether the Stack structure contains space for new elements
 *
 * @param stack pointer to the BubbleStack_t stack structure
 *
 * @return non-zero value when there is no space available, zero when there is
 */
int BS_IsFull(BubbleStack_t* stack)
{

    if (stack == NULL)
    {
        err_flag = 1;
        return 1;
    }

    return ( stack->BS_TopIndex == max_elements );

}