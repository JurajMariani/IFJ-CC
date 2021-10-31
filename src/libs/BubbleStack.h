//TODO
//ifndef atd

/**
 *  @brief Structure Bubble_t contains elements of the Derivative tree. As of right now there is only one integer value inside, this is a PLACEHOLDER
 * TODO
 */
typedef struct stack{
    
    int value;

}Bubble_t;


/**
 * @brief Structure BubbleStack_t is a stack structure containing Bubble_t elements inside
 *
 * ...
 */
typedef struct bstack{

    Bubble_t* BS_Element;

    int BS_TopIndex;

}BubbleStack_t;


/**
 * @brief Global flag, when an error occures, this variable will be set to a non-zero value
 */
int err_flag = 0;

/**
 * @brief Defines the maximal amount of elements in the stack
 */
unsigned int max_elements;


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
 */
void BS_TopStack( BubbleStack_t* , Bubble_t* );


/**
 * @brief The data are inserted on the top of the stack
 *
 * @param stack pointer to the BubbleStack_t stack structure
 * @param data pointer to the Bubble_t variable, where the data pushed on the top of the stack are
 */
void BS_Push( BubbleStack_t* , Bubble_t* );


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
