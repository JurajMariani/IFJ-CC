/**
 * Implementace překladače imperativního jazyka IFJ21
 *
 * @file BubbleStack.c
 * @author Juraj Mariani < xmaria03 (xmaria03@stud.fit.vutbr.cz) >
 * @brief Header file for Stack ADT operations
 * @date 2021-10-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "BubbleStack.h"


/**
 * @brief BubbleStack_t structure initialisation function
 *
 * @param stack pointer to the BubbleStack_t stack structure
 */
void BS_Init(BubbleStack_t* stack)
{
    if (stack == NULL)
    {
        stack_err_flag = 1;
        return;
    }

    stack->BS_Element = malloc( BS_MAX_ELEMS * sizeof(expression_block*));

    stack->BS_TopIndex = -1;
}

/**
 * @brief Saves the pointer to Bubble_t element on the top of the stack
 *
 * @param stack pointer to the BubbleStack_t stack structure
 * @param data pointer to the Bubble_t variable, where the pointer to the top of the stack is saved
 *
 * @return ...
 */
expression_block* BS_TopStack(BubbleStack_t* stack)
{
    if ( (stack == NULL) )
    {
        stack_err_flag = 1;
        return NULL;
    }

    return(stack->BS_Element[stack->BS_TopIndex]);
}

/**
 * @brief The data are inserted on the top of the stack
 *
 * @param stack pointer to the BubbleStack_t stack structure
 * @param data pointer to the Bubble_t variable, where the data pushed on the top of the stack are
 */
void BS_Push(BubbleStack_t* stack, expression_block* data)
{
    if ((stack == NULL) || (data == NULL) || (BS_IsFull(stack)))
    {
        stack_err_flag = 1;
        return;
    }

    stack->BS_TopIndex++;
    stack->BS_Element[ stack->BS_TopIndex ] = data;
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
        stack_err_flag = 1;
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
        stack_err_flag = 1;
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
        stack_err_flag = 1;
        return 1;
    }

    return ( stack->BS_TopIndex == BS_MAX_ELEMS );

}

/**
 * @brief Dtor. Destroys the contents of the stack
 * 
 * @param stack pointer to the BubbleStack_t structure
 */
void BS_Dispose(BubbleStack_t* stack)
{
    if (stack == NULL)
    {
        stack_err_flag = 1;
        return;
    }

    while(!BS_IsEmpty(stack))
    {
        free(BS_TopStack(stack));
        BS_Pop(stack);
    }
    if(stack->BS_Element!=NULL){
        free(stack->BS_Element);
        stack->BS_Element=NULL;
    }
}


/**------------------  End of File BubbleStack.c --------------------*/


/*                                                                                                                                   
                                                                                      ....                                                            
                                                                                  .;cloxkOkxxdl:,.                                                    
                                                                                .,;,'..':okKKK00K0xl,.                                                
                                                                             .,,..  .......,;;''cOXXXOl.                                              
                                                                          .';:.     .............,lododd;                                             
                                                                         ,dx:.      .............',,'..':,                                            
                                                                       .lOo,.    ....''........,;;;,,''.',.                                           
                                                                      ;oc...    ....,,;;.....';:::;,,'''';'                                           
                                                                    .ld' ..     .''o0kl:'...,;:ccc:;'.'',:,                                           
                                                                   ,xd. .,c:;,'..;:ldo:,..;c;::,',;;,''',:;.                                          
                                                                  :Od.  .;oodkkxocc:::;,;:lo:;,.:Okc;,,;;cc.                                          
                                                                .oKx.    .:ooxOXXKOkxxxxdooodl;:oxxlc:;;;od.                                          
                                                               ,kXO,      ..;ldk0XXXXXXXXXKK0kdoolllcc:::ol.                                          
                                                             .:0X0:.      . ..;ldk0Oxk0KXXXXXXK0Okkxdolc:c:.                                          
                                                            .dKXKl.          ...,lddl:coxOOKXNNNNXKK0Oko:lc.                                          
                                                           :OXXXd.              ..;oddlloxO0KK0000OOkdlc:oc.                                          
                                                         'dKXXXk'             .....,cdxkkkkkxxxddddolcc::xl                                           
                                                      ..l0XXXXO,                ...'';:looollcccccllcccclOl                                           
                                            ..;:clodxxk0XXXXXk,                   ...';clllllllllllllllcdKl                                           
                                       .';ldOKXXXX0kkXXXXXXKo.                      ...,:loooooooooollllkXl                                           
                                    .:ok0XXXXXXOd:.'xXXXXKx;.                         ..';coooooooooollo0No                                           
                                .'cx0XXXXXXKkl;.  .xXXX0x;.                             ..,:loooooooolldKNd.                                          
                              'lkKXXXXNX0dc'     'k0ko:'.                                ...;clooooolllxXNk.                                          
                           'ckKXXXXNXOo;.       'cc'.                                      ..':clllolllkXNK,                                          
                        .:dKXXXXNKxl,.         ',.                                        .....,:clllllxXNNl                                          
                      .lOKXXXXOd:.            ''.                                          .....';clllldKNN0,                                         
                    .cOXXXXkl;.             .'.                                            .......,:clloONNN0d;.                                      
                   ,xKXXOl,.                '.                                           ..........,:cllxKNNXXN0d;.                                   
                 .c0XKx;.                  .. .                                         ............,;cloOXNdcONNXOl.                                 
                .dX0o'                    ..   .                                           ..........';:coOXd.,ONXNX0l.                               
               .x0o'                     ...                                             .............',:clkx. ,ONXNXXO:.                             
              'dd'                       .. ..                                         .................,:cll'  'kXNNXNXx,                            
              ''                        .....  .                                      ..................',;cc.   .dXNNNXXKl.                          
                                        .... ....                                 ... ...................',:c,    .cKNXXXXXx'                         
                                        ...........                               ......................''',c,      ,ONNXXXXO;                        
                                       .............                             .....................'''',,:;.      .dXNXNXX0c.                      
                                       .....................        ..   .............................''',,;:;.       .c0NXXXXKo.                     
                                       .........................    ..............................'''''',,;;c;.         ,kXNNXXXd.                    
                                       ........................................................''''''',,,;:cc,           .oKNNXXXd.                   
                                       .......................................................'''''',,,;;:ccc'             ;ONNXXXo.                  
                                       ....................................................'''''',,,,;;::ccc:.              .dXNXXKc                  
                                       ..................................................'''''',,,,;;::ccccc,                 cKNXXO'                 
                               ';.     ..............................................''''''',,,,;;:::cccccc:.                  ;OXXXl.                
                              .:do,    ..........................................''''''''',,,;;;::cccccccc:.                    ,OXXk.                
                           .,'.cxdo,   ...........................'''''''''''''''''''',,,,;;;::cccccccccc:.                      ;OXO'                
                           .cdcoxddl,'............''''''''''''''''''''''''''''''',,,,,,;;;:::ccccccccccc:.                        cK0,                
                            :xdxxxxdddo,........''''''''''''',,,,,,,,,,,,,,,,,,,,,;;;;::::ccccccccccccc;.                         .o0;                
                            ,dxxxxxxxdxo:,.....'',,;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;:::::cccccccccccccccc,.                           .l'                
                            .cxxxxkkxxxxdlc;.',,;;::::::::c::::::::::::::::::::ccccccccccccccccccccc:'                                                
                             .cxkkkkkxxxxddd:;;:::ccccccccccccccccccccccccccccccccccccccccccccccccc,.                                                 
                              .:xkkkkkkkkkxxoc::ccccccccccccccccccccccccccccccccccccccccccccccccc;.                                                   
                                'lxOkkkkkOkkdl:ccccccccccccccccccclllllllcccccccccccccccccccccc;.                                                     
                                  'lxOOOkl'.......'',,;;:cclllllllllllllllllllllllllllcccclc;'.                                                       
                                    .;cc'                ......';cooooololllllllllllllllc;,.                                                          
                                                                 .:oooooooolloollloll:;'.                                                             
                                                                  .lxdooooooooool:,...                                                                
                                                                 .:xOkxxxxolc:,..                                                                     
                                                                ;dOOOOOOk:..                                                                          
                                                               cO0OOOOO0ko:,'.                                                                        
                                                              .o0OOOOO000000Oxoc:,..                                                                  
                                                               'xO00O00O00000OOOOxoc'                                                                 
                                                                .,:llo:',;:::ccll:. 
*/
