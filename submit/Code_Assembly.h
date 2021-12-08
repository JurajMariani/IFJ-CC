#ifndef __Code_Assembly_h__
#define __Code_Assembly_h__

/**
 * Implementace překladače imperativního jazyka IFJ21
 *
 * @file Code_Assembly.h
 * @author Juraj Mariani < xmaria03 (xmaria03@stud.fit.vutbr.cz) >, Lukas Macejka < xmacej03 (xmacej03@stud.fit.vutbr.cz) >
 * @brief Code Generation file, transcribing the parser(input) to IFJcode21(output)
 * @version 1.0
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"
#include "symtable.h"
#include "BubbleStack.h"
#include "expressionCommander.h"


/**
 * execute_counter - counting the use of JUMP EXECUTEx; x is 0..INT_MAX
 * if_counter - counting the instances of labels used with if statements
 * while_counter - counting the instances of labels used with while loops
 * logic_counter - counting the instances of WRITE instructions, used to write string@nil, if the variable contains nil
 * function_frame_counter - counting the use of JUMP EXECUTEx; x is 0..INT_MAX
 */
int execute_counter;
int if_counter;
int while_counter;
int logic_counter;
int function_frame_counter;

// # ooga booga monkey method
char** variable_names;
int number_of_names;


/**
 * @brief CODE GENERATOR - Generates the IF statement for a loop, compares a given term with a bool value
 * 
 * @param term Pointer to the compared term
 * @return (int) Status, '0' = OK
 */
int G_CompareBool(expression_block *);

/**
 * @brief CODE GENERATOR - Generates the IF statement for a loop, compares a given term with nil
 * 
 * @param term Pointer to the compared term
 * @return (int) Status, '0' = OK
 */
int G_CompareNull(expression_block *);

/**
 * @brief CODE GENERATOR - Generates the IF condition end label
 * 
 */
void G_IfEND(int);

/**
 * @brief CODE GENERATOR - Generates the shift of changed local variable values (TF -> LF), jump to the start of the loop and the end label
 */
void G_WhileEND(int);

/**
 * @brief CODE GENERATOR - Generates the ELSE branch (LABEL)
 */
void G_IfELSE(int);

/**
 * @brief CODE GENERATOR - Generates the WHILE loop label, creates a new frame and copies the values of all local variables inside the frame
 */
void G_WhileBGN();

/**
 * @brief CODE GENERATOR - Generates IF condition
 * 
 * @param term The term variable compared within the condition
 * @return (int) Status, '0' = OK
 */
int G_IfBGN(expression_block*);

/**
 * @brief CODE GENERATOR - Generates assignment TERM -> VARIABLE(local)
 * 
 * @param var Pointer to a local variable (symtable)
 * @param term Pointer to a term variable
 */
void G_AssignToVar(TreeElement* var,expression_block* term);

/**
 * @brief CODE GENERATOR - Generates assignment TERM -> VARIABLE(local) for more variables
 * 
 * @param var Pointer to a local variable stack (symtable)
 * @param stack Pointer to a term variable stack
 */
void G_AssignToVars(TreeElement**,BubbleStack_t*);

/**
 * @brief CODE GENERATOR - Assignment (VAR -> TERM) or (CONST -> TERM)
 * 
 * @param target Pointer to expr_block TERM
 * @param input Pointer to expr_block VAR/CONST
 */
void var_to_term_assign (expression_block* target, expression_block *input);

/**
 * @brief CODE GENERATOR - The # operator
 * 
 * //strlen TF@targetname string@input-str
 * 
 * @param target Pointer to expr_block TERM, DST (int)
 * @param input Pointer to expr_block TERM, SRC (str)
 */
void unary_operator(expression_block* target, expression_block *input);

/**
 * @brief CODE GENERATOR - Performs Arithmetic and Logic instructions
 * 
 * @param target Pointer to expr_block TERM, DST
 * @param operand_one Pointer to expr_block TERM, OPERAND ONE
 * @param operand_two Pointer to expr_block TERM, OPERAND TWO
 * @param sign Pointer to expr_block TERM, OPERATOR
 */
void operation_quick_action(expression_block* target, expression_block *operand1, expression_block *operand2, expression_block *sign);

/**
 * @brief Performs the int to float conversion
 * 
 * @param term Pointer to expr_block TERM
 */
void convert_to_float(expression_block *term);

/**
 * @brief Performs the float to int conversion
 * 
 * @param term Pointer to expr_block TERM
 */
void convert_to_int(expression_block *term);

/**
 * @brief CODE GENERATOR - Generates function label, creates frame and pops parameters
 * 
 * @param func Pointer to a function (symtable)
 * @return (int) Status '0' = OK
 */
int G_function_bgn(TreeElement* func);

/**
 * @brief CODE GENERATOR - Generates function end label and returns
 * 
 * @param func_name Name of the function (for a correct jump)
 */
void G_function_end(char* func);

/**
 * @brief CODE GENERATOR - Pushes function parameters, generates the function call and pops the returns
 * 
 * @param func Pointer to the function element (in symtable)
 * @param params Stack of (expression_block*) terms - Function parameters
 * @param returns Stack of (expression_block*) terms - Functions returns (empty at the beginning, terms generated internally) 
 * @return (int) Status - '0' = OK 
 */
int G_CallFunc(TreeElement* func, BubbleStack_t *params, BubbleStack_t* returns);

/**
 * @brief Jump Generator
 * It is used to jump over function definitions
 */
void generate_execute_jump(void);

/**
 * @brief Generates the code header
 * prints ".IFJcode21" and prints the builtin functions
 */
void generate_header(void);

/**
 * @brief Creates the execute label
 * see generate_execute_jump()
 * 
 */
void generate_execute_block(void);

/**
 * @brief String format converter
 * Converts normal strings into suitable result codes strings\
 * Replaces special characters with escape sequences
 * 
 * EXAMPLE "What is this?", he asked angrily. -> \034What\032is\032this?\034,\032he\032asked\032angrily.
 * 
 * @param string The original string
 */
void G_string_correct_print(char* string);

/**
 * @brief CODE GENERATOR - Pushes the return values to the stack, then jumps to the end of function
 * 
 * @param godzilla Stack containing to be pushed terms
 * @param func_name Name of the function (for a correct jump)
 */
void G_RetrunTerm(BubbleStack_t *godzilla,char* );

/**
 * @brief CODE GENERATOR - Assign nil@nil to TERM
 * 
 * @param term Pointer to expr_block TERM, DST
 * @return (int) Status, '0' = OK 
 */
int G_FillWithNil(expression_block* term);

/**
 * @brief Prints the substr() builtin function
 */
void print_substr();

/**
 * @brief Prints the tointeger() builtin function
 */
void print_tointeger();

/**
 * @brief Prints the ord() builtin function
 */
void print_ord();

/**
 * @brief Prints the chr() builtin function
 */
void print_chr();

/**
 * @brief CODE GENERATOR - Local variable definition
 * 
 * @param new_var Pointer to the variable (in symtable)
 */
void def_var(TreeElement *);

/**
 * @brief Destructor of variable names string array
 */
void dtroy_variable_name_catcher(void);

/**
 * @brief Initialiser of variable names string array
 */
void init_variable_name_catcher(void);

#endif

/**         --------- END OF FILE Code_Assembly.h ----------    */
