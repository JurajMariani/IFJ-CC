#include <stdio.h>
#include <stdlib.h>
#include "Defs.h"
#include "TS.h"
#include "BubbleStack.h"
#include "expressionCommander.h"


/**
 * @brief global counter, counting the use of JUMP EXECUTEx; x is 0..INT_MAX
 * to add the 'x' at the back
 */
int execute_counter;
int if_counter;
int while_counter;
int logic_counter;


//asi su nejake includes zbytocne, ptm skontroluj
int G_CompareBool(expression_block *);
int G_CompareNull(expression_block *);
void G_IfEND();
void G_WhileEND(); 
void G_IfELSE(); 
void G_WhileBGN();
int G_IfBGN(expression_block*);
//void G_RetrunTerm(BubbleStack_t *); vola sa ked najde return, a na zasobniku su pushnute vysledky vyrazov v return, Tomas to chce zmazat ci?
void G_AssignToVar(TreeElement* var,expression_block* term);
void G_AssignToVars(TreeElement**,BubbleStack_t*);//<----------------- Ked je NULL,NULL tak len vypopuje returny, funkcia nema priradenie
void var_to_term_assign (expression_block* target, expression_block *input);
void unary_operator(expression_block* target, expression_block *input); //<---- You just assume that unary operator is the # sign
void operation_quick_action(expression_block* target, expression_block *operand1, expression_block *operand2, expression_block *sign);
void convert_to_float(expression_block *term);
void convert_to_int(expression_block *term);
int G_function_bng(TreeElement* func);
int G_CallFunc(TreeElement* func, BubbleStack_t *params, BubbleStack_t* returns);
void generate_execute_jump(void);
void generate_header(void);
void generate_execute_block(void);
void G_string_correct_print(char* string);
void G_RetrunTerm(BubbleStack_t *godzilla);
void print_substr();
void print_tointeger();
void print_ord();
void print_chr();
