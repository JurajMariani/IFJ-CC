
/**
 * @file Code_Assembly.c
 * @author xmaria03 (xmaria03@stud.fit.vutbr.cz)
 * @brief Code Generation file, transcribing the parser(input) to IFJcode21(output)
 * @version 0.1
 * @date 2021-11-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "../libs/Defs.h"
#include "../libs/BubbleStack.h"
#include "../libs/Code_Assembly.h"
#include <string.h>
#include <stdlib.h>

#define out(s) fprintf(stdout,"%s\n",s);
#define out_partial(s) fprintf(stdout,"%s",s);
#define out_number(number) fprintf(stdout,"%a",number);
#define out_integer(integer) fprintf(stdout,"%d",integer);
#define out_char(char) fprintf(stdout,"%c",char);
#define newline fprintf(stdout,"\n");


/**
 * function to generate adress at the end of expression name 
 * x -> x_adress
 */

char* generate_name(TreeElement* var)
{
    char temp1[101];
    strcpy(temp1, var->name);
    char temp2[14];
    sprintf(temp2, "%p", var);
    unsigned len = strlen(temp1) + strlen(temp2);
    char *out = malloc(sizeof(char) * len);
    unsigned i = 0;
    while (temp1[i] != '\0')
    {
        out[i] = temp1[i];
        i++;
    }
    out[i] = '_';
    i = 0;
    while (strlen(temp2) != i)
    {
        out[strlen(temp1)+i+1] = temp2[i];
        i++;
    }
    return (out);
}


/**
 * function to generate term adress at the end of expression name 
 * x -> T_x_adress
 */
char * generate_term_name(expression_block* term)
{
//T_int_double

    char *out = malloc(sizeof(char));
   unsigned i = 0;
   out[i++] = 'T';
   out[i++] = '_';
   int pom1 = term->_integer;
   double pom2 = term->_double;
   char char_pom1[100];
   char char_pom2[100];
   sprintf(char_pom1, "%d", pom1);//gut??
   sprintf(char_pom2, "%lf", pom2);//gut??
   while (char_pom1[i-2] != '\0')
   {
       out[i] = char_pom1[i-2];
       i++;
   }
   out[i] = '_';
   while (char_pom2[i-3] != '\0')
   {
       out[i] = char_pom2[i-3];
       i++;
   }
   return out;
}


char* generate_param_name(char* name)
{
    int name_len = strlen(name);
    char *out = malloc((name_len + 7)*sizeof(char));
    if (out == NULL)
        return NULL;

    int card = 0;
    int i = 0;
    while(i < name_len)
        card += name[i];

    char num[6];
    sprintf(num, "%d", card);
    
    i = 0;
    while (i < name_len + 6)
    {
        if (i < name_len)
            out[i] = name[i];
        if(i == name_len)
            out[i] = '_';
        if ((i > name_len) && (i < name_len + 6) && (num[i - (name_len + 1)] != '\0'))
        {
            out[i] = num[i - (name_len + 1)];
        }
        else
            out[i] = '\0';
        i++;
    }
    return out;
}




void generate_execute_jump(void)
{
    newline
    newline
    newline
    out_partial("JUMP EXECUTE");
    out_integer(execute_counter);
    execute_counter++;
    newline
    newline
    newline
}


/**
 * @brief 
 */
void generate_header(void)
{

    out(".IFJcode21");
    newline
    out("CREATEFRAME");
    newline
    generate_execute_jump();

}


/**
 * @brief 
 * 
 */
void generate_execute_block(void)
{
    newline
    newline
    newline
    out("LABEL EXECUTE");
    newline
}


//funckia ahoj ako sa mas -> ahoj\032ako\032sa\032mas
/**
 * @brief 
 * 
 * @param string 
 */
void G_string_correct_print(char* string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if (string[i] > 32)
        {
            out_char(string[i]);
        }
        else
        {
            out_partial("\0");
            out_integer(string[i]);
        }
    }
}


/**
 * @brief 
 * 
 * @param func 
 * @param params 
 * @param returns 
 * @return int 
 */
int G_CallFunc(TreeElement* func, BubbleStack_t* params, BubbleStack_t* returns)
{
    expression_block* var = NULL;

    while(!BS_IsEmpty(params))
    {
        var = BS_TopStack(params);
        
        out_partial("PUSHS ");
        if (var->dt == _integer)
        {
            out_partial("int@");
            out_integer(var->_integer);
        }
        if (var->dt == _number)
        {
            out_partial("float@");
            out_number(var->_double);
        }
        if (var->dt == _string)
        {
            out_partial("string@");
            G_string_correct_print(var->str);
        }
        if (var->dt == _nan)
        {
            out_partial("nil@nil");
        }
        newline
        
        free(var);
        BS_Pop(params);

    }

    newline
    out_partial("CALL ");
    out_partial(func->name);
    newline
    newline

    expression_block* term = NULL;

    while (!BS_IsEmpty(returns))
    {
        term = BS_TopStack(returns);

        char* term_name = generate_term_name(term);
        if (term_name == NULL)
            return MALLOC_ERR_CODE;
        
        out_partial("DEFVAR TF@");
        out_partial(term_name);
        newline
        out_partial("POPS TF@");
        out_partial(term_name);
        newline

        free(term_name);
        free(term);
        BS_Pop(returns);
    }
        
    return 0;
}


/**
 * @brief 
 * 
 * @param term 
 * @return int 
 */
int G_IfBGN(expression_block* term)
{
    char* term_name = generate_term_name(term);
    if (term_name == NULL)
        return MALLOC_ERR_CODE;
    out_partial("DEFVAR TF@______TMP______");
    out_integer(if_counter);
    newline
    out_partial("TYPE TF@______TMP______ TF@");
    out_partial(term_name);

    out_partial("JUMPIFNEQ ______TMP______");
    out_integer(if_counter);
    out_partial("ELSE TF@______TMP______");
    out_integer(if_counter);
    out_partial(" string@bool");
    newline
    
    out_partial("JUMPIFNEQ ELSE_");
    out_integer(if_counter);
    out_partial(" TF@");
    out_partial(term_name);
    out_partial(" bool@true");
    newline

    out_partial("JUMP IF_");
    out_integer(if_counter);
    newline
    newline

    out_partial("LABEL ______TMP______")
    out_integer(if_counter);
    out_partial("ELSE");
    newline

    out_partial("JUMPIFEQ ELSE_");
    out_integer(if_counter);
    out_partial(" TF@");
    out_partial(term_name);
    out_partial(" nil@nil");
    newline
    newline

    out_partial("LABEL IF_");
    out_integer(if_counter);
    newline

    free(term_name);
    return 0;
}


/**
 * @brief 
 * 
 */
void IfELSE()
{
    out_partial("JUMP END_");
    out_integer(if_counter);
    newline
    newline
    newline
    out_partial("LABEL ELSE_");
    out_integer(if_counter);
    newline
}


/**
 * @brief 
 * 
 */
void IfEND()
{
    newline
    newline
    out_partial("LABEL END_");
    out_integer(if_counter);
    newline
    newline

}


/**
 * @brief 
 * 
 */
void G_WhileBGN()
{
    newline
    newline
    out_partial("LABEL WHILE_LOOP_");
    out_integer(while_counter);
    newline
    newline

    // <statement> <st-list>
    // G_ComparBool / G_CompareNull
}


/**
 * @brief 
 * 
 * @param term 
 * @return int 
 */
int G_CompareBool(expression_block *term)
{
    char* term_name = generate_term_name(term);
    if (term_name == NULL)
        return MALLOC_ERR_CODE;
    
    newline
    newline
    out_partial("JUMPIFNEQ WHILE_LOOP_END_");
    out_integer(while_counter);
    out_partial(" TF@");
    out_partial(term_name);
    out_partial(" bool@true");
    newline
    newline
    newline

    return 0;
    // <statement> <st-list>
    // JMUP WHILE_LOOP_X
}


/**
 * @brief 
 * 
 * @param term 
 * @return int 
 */
int G_CompareNull(expression_block* term)
{
    char* term_name = generate_term_name(term);
    if (term_name == NULL)
        return MALLOC_ERR_CODE;
    
    newline
    newline
    out_partial("JUMPIFNEQ WHILE_LOOP_END_");
    out_integer(while_counter);
    out_partial(" TF@");
    out_partial(term_name);
    out_partial(" nil@nil");
    newline
    newline
    newline

    return 0;

    // <statement> <st-list>
    // JMUP WHILE_LOOP_X
}


/**
 * @brief 
 * 
 */
void G_WhileEND()
{
    newline
    newline
    out_partial("JUMP WHILE_LOOP_");
    out_integer(while_counter);
    newline
    newline
    out_partial("LABEL WHILE_END_LOOP_");
    out_integer(while_counter);
    newline
    newline
}

void G_RetrunTerm(BubbleStack_t *godzilla)
{
expression_block* kingkong = NULL;
while(godzilla != NULL)
{
    BS_TopStack(godzilla);
    BS_Pop(godzilla);
    char* minidzilla = generate_term_name(kingkong);
    out_partial("PUSHS TF@");
    out_partial(minidzilla);
    newline
    free(kingkong);
}
out("RETURN");
}


void G_AssignToVar(TreeElement* var,expression_block* term)
{
    //do var prirad hodnotu z expression
   char* bake_rolls;
   char* boiled_rolls;
   bake_rolls = generate_term_name(term);
   boiled_rolls = generate_name(var);
   out_partial("MOVE TF@");
   out_partial(boiled_rolls);

   out_partial(" TF@");
   out_partial(bake_rolls);
   newline


    free(bake_rolls);
    free(boiled_rolls);
   //asi su tu pojebiky

   // vynegerujes meno pre var a term
   // MOVE TF@var_name TF@term_name
   

}

void G_AssignToVars(TreeElement** var,BubbleStack_t* stack)//<----------------- Ked je NULL,NULL tak len vypopuje returny, funkcia nema priradenie
{
    //aj pomocne funkcie var a stack?
    if ((var == NULL) || (stack == NULL))
    {
        return;
    }
    int i = 0;

    expression_block* temp_expbl;
    TreeElement* temp_treeelem;

    while (!BS_IsEmpty(stack))
    {
        temp_expbl = BS_TopStack(stack);
        temp_treeelem = var[i];

        char* orangutan = generate_term_name(temp_expbl);
        char* pavian = generate_name(temp_treeelem);

        out_partial("MOVE TF@");
        out_partial(pavian);
        out_partial(" TF@");
        out_partial(orangutan);
        newline

        free(pavian);
        free(orangutan);
        free(temp_expbl);

        BS_Pop(stack);

        i++;
    }
}

int G_function_bng(TreeElement* func)
{
    out_partial("LABEL G_");
    out_partial(func->name);
    newline
    out("PUSHFRAME");
    out("CREATEFRAME");

    int i = 0;
    user_func* temp = (user_func*)func->data;
    while( temp->params[i] != _ender)
    {
        char* var_name = generate_param_name(temp->param_names[i]);
        if (var_name == NULL)
            return MALLOC_ERR_CODE;
        
        out_partial("DEFVAR TF@");
        out_partial(var_name);
        newline
        out_partial("POPS TF@");
        out_partial(var_name);
        newline
        newline

        i++;
    }

    return 0;
}


/**
 * @brief 
 * 
 * @param target 
 * @param input 
 */
void var_to_term_assign(expression_block* target, expression_block* input)
{
    char* target_term;
    char* input_variable;
    target_term = generate_term_name(target);
    input_variable = generate_name(input->TSPointer);
    out_partial("DEFVAR TF@");
    out_partial(target_term);
    newline

    out_partial("MOVE TF@");
    out_partial(target_term);
    out_partial(" TF@");
    out_partial(input_variable);
    newline

    free(target_term);
    free(input_variable);
    
}


/**
 * @brief 
 * 
 * @param target 
 * @param input 
 */
void unary_operator(expression_block* target, expression_block* input)
{
    char* pom_target;
    char* pom_input;
    out_partial("STRLEN ");
    out_partial("TF@");
    //strlen_TF@targetname_string@input-str
    pom_target = generate_term_name(target);
    out_partial(pom_target);
    out_partial(" TF@");
    pom_input = generate_term_name(input);
    out_partial(pom_input);
    newline

    free(pom_target);
    free(pom_input);
}



/**
 * @brief Get the hash object
 * 
 * function copied from: https://stackoverflow.com/questions/13325125/lightweight-8-byte-hash-function-algorithm
 * 
 * @param string 
 * @return unsigned int 
 */
unsigned int get_hash(char* string)
{
    unsigned int P1 = 7;
    unsigned int P2 = 31;

    unsigned int hash = P1;
    for (const char* p = string; *p != 0; p++) {
        hash = hash * P2 + *p;
    }
    return hash;
}


/**
 * @brief 
 * 
 * @param target 
 * @param operand_one 
 * @param operand_two 
 * @param sign 
 */
void operation_quick_action(expression_block* target, expression_block* operand_one, expression_block* operand_two, expression_block* sign)
{
    char* operand_one_name = generate_term_name(operand_one);
    char* operand_two_name = generate_term_name(operand_two);
    char* target_name = generate_term_name(target);


    out_partial("DEFVAR TF@");
    out_partial(target_name);
    newline

    unsigned int hash;

    switch(sign->oper)
    {
        case _add:
            out_partial("ADD TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _sub:
            out_partial("SUB TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _mul:
            out_partial("MUL TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _div:
            out_partial("DIV TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _div2:
            out_partial("IDIV TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _konk:
            out_partial("CONCAT TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _less:
            out_partial("LT TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _lessEq:
            out_partial("LT TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            
            hash = get_hash(operand_one_name) + get_hash(operand_two_name) - get_hash(target_name) + logic_counter;
            out_partial("DEFVAR TF@__TMP__");
            out_integer(hash);
            newline
            
            out_partial("EQ TF@__TMP__");
            out_integer(hash);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline

            out_partial("AND TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(target_name);
            out_partial(" TF@__TMP__");
            out_integer(hash);
            newline

            logic_counter++;
            break;

        case _great:
            out_partial("GT TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _greatEq:
            out_partial("GT TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            
            hash = get_hash(operand_one_name) + get_hash(operand_two_name) - get_hash(target_name) + logic_counter;
            out_partial("DEFVAR TF@__TMP__");
            out_integer(hash);
            newline
            
            out_partial("EQ TF@__TMP__");
            out_integer(hash);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline

            out_partial("AND TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(target_name);
            out_partial(" TF@__TMP__");
            out_integer(hash);
            newline

            logic_counter++;
            break;

        case _Eq:
            out_partial("EQ TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            break;

        case _nEq:
            out_partial("EQ TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline
            out_partial("NOT TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(target_name);
            newline
            break;

        default:
            break;

    }

    free(operand_one_name);
    free(operand_two_name);
    free(target_name);
}

/**
 * @brief 
 * 
 * @param term 
 */
void convert_to_float(expression_block* term)
{
    newline
    out_partial("INT2FLOAT ");
    out_partial("TF@");
    char* term_name = generate_term_name(term);
    out_partial(term_name);
    out_partial(" TF@");
    out_partial(term_name);
    newline

    term->dt = _number;

}


/**
 * @brief 
 * 
 * @param term 
 */
void convert_to_int(expression_block* term)
{
    newline
    out_partial("FLOAT2INT ");
    out_partial("TF@");
    char* term_name = generate_term_name(term);
    out_partial(term_name);
    out_partial(" TF@");
    out_partial(term_name);
    newline

    term->dt = _integer;
}


//ic21int prg.code < prg.in > prg.out
