
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
    char temp2[15];
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
    out[i]='\0';
    return (out);
}


/**
 * function to generate term adress at the end of expression name 
 * x -> T_x_adress
 */
char * generate_term_name(expression_block* term)
{
//T_int_double
    char *out = malloc(sizeof(char)*203);
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
    out[i++] = '_';
    while (char_pom2[i-4] != '\0')
    {
        if (char_pom2[i-4] == '.')
            out[i] = '_';
        else
            out[i] = char_pom2[i-4];
        i++;
    }
    out[i]='\0';
    return out;
}


/*char* generate_param_name(char* name)
{
    int name_len = strlen(name);
    char *out = malloc((name_len + 7)*sizeof(char));
    if (out == NULL)
        return NULL;

    int card = 0;
    int i = 0;
    while(i < name_len)
    {
        card += name[i];
        i++;
    }

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
}*/




void generate_execute_jump(void)
{
    newline
    newline
    newline
    out_partial("JUMP EXECUTE");
    out_integer(execute_counter);
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

    print_substr();
    newline
    newline
    print_tointeger();
    newline
    newline
    print_ord();
    newline
    newline
    print_chr();
    newline
    newline

    generate_execute_block();

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
    out_partial("LABEL EXECUTE");
    out_integer(execute_counter);
    execute_counter++;
    newline
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
            out_partial("\\0");
            out_integer(string[i]);
        }
        i++;
    }
}


/**
 * @brief 
 * 
 * @param returning 
 * @param param_num 
 * @param param_max 
 * @return expression_block* 
 */
expression_block* generate_exp_block(dataType* returning, int param_num, int param_max)
{
    expression_block* ret = (expression_block*) malloc(sizeof(expression_block));
    if (ret == NULL)
        return NULL;
    ret->_double = exprCounter;
    exprCounter++;
    ret->operType = _not_terminal_oper;
    ret->blockType = _operand_expr;
    ret->dt = returning[param_num];
    ret->_integer = 1 - param_max;

    return ret;
}


void add_local_variable_name(char* var_name)
{
    char** temp = (char**) realloc(variable_names, (number_of_names + 1) * sizeof(char*));
    if (temp == NULL)
    {
        RaiseError(99);
        exit(99);
    }
    variable_names = temp;
    variable_names[number_of_names] = var_name;
    number_of_names++;
}



/**
 * @brief 
 * 
 * @param new_var 
 */
void def_var(TreeElement* new_var)
{
    char* name = generate_name(new_var);
    out_partial("DEFVAR TF@");
    out_partial(name);
    newline
    add_local_variable_name(name);
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
    char* term_name = NULL;

    dataType* rets = ((user_func*)(func->data))->returns;

    int n = 0;
    int i = 0;
    while(rets[n] != _ender)
    {
        n++;
    }

    if ((strcmp(func->name,"reads") == 0) || (strcmp(func->name,"readn") == 0) || (strcmp(func->name,"readi") == 0))
    {
        var = NULL;
        var = generate_exp_block(rets, i, n);
        i++;
        if (var == NULL)
            return MALLOC_ERR_CODE;

        term_name = generate_term_name(var);
        if (term_name == NULL)
            return MALLOC_ERR_CODE;

        out_partial("DEFVAR TF@");
        out_partial(term_name);
        newline

        out_partial("READ TF@");
        out_partial(term_name);
        if (strcmp(func->name,"reads") == 0)
        {
            out_partial(" string");
        }
        else
        {
            if (strcmp(func->name,"readn") == 0)
            {
                out_partial(" float");
            }
            else
            {
                if (strcmp(func->name,"readi") == 0)
                    out_partial(" integer");
            }
        }
        newline

        free(term_name);
        BS_Push(returns, var);
        
    }
    else
    {
        if (strcmp(func->name,"write") == 0)
        {
            while(!BS_IsEmpty(params))
            {
                var = BS_TopStack(params);
                term_name = generate_term_name(var);

                out_partial("WRITE TF@");
                out_partial(term_name);
                newline

                free(term_name);
                free(var);
                BS_Pop(params);
            }
        }
        else
        {
            while(!BS_IsEmpty(params))
            {
                var = BS_TopStack(params);
                term_name = generate_term_name(var);
                if (term_name == NULL)
                    return MALLOC_ERR_CODE;
                        
                out_partial("PUSHS TF@");
                out_partial(term_name);
                newline
                        
                free(var);
                term_name = NULL;
                BS_Pop(params);

            }

            newline
            out_partial("CALL ");
            out_partial(func->name);
            newline
            newline

            expression_block* term = NULL;


            int control = 0;
            while( control < n)
            {
                term = NULL;
                term = generate_exp_block(rets, control, n);
                if (term == NULL)
                    return MALLOC_ERR_CODE;
                
                term_name = generate_term_name(term);
                if (term_name == NULL)
                    return MALLOC_ERR_CODE;
                
                out_partial("DEFVAR TF@");
                out_partial(term_name);
                newline
                out_partial("POPS TF@");
                out_partial(term_name);
                newline
                control++;
                free(term_name);
                BS_Push(returns, term);
            }
        }
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
    out_partial("TYPE TF@______TMP______");
    out_integer(if_counter);
    out_partial(" TF@");
    out_partial(term_name);
    newline

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
void G_IfELSE()
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
void G_IfEND()
{
    newline
    newline
    out_partial("LABEL END_");
    out_integer(if_counter);
    newline
    newline

}


void get_from_local_frame()
{
    int index = 0;
    while (index < number_of_names)
    {
        out_partial("DEFVAR TF@");
        out_partial(variable_names[index]);
        newline
        out_partial("MOVE TF@");
        out_partial(variable_names[index]);
        out_partial(" LF@");
        out_partial(variable_names[index]);
        newline

        index++;
    }
}


void get_from_temp_frame()
{
    int index = 0;
    while (index < number_of_names)
    {
        out_partial("MOVE LF@");
        out_partial(variable_names[index]);
        out_partial(" TF@");
        out_partial(variable_names[index]);
        newline

        index++;
    }
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
    out("PUSHFRAME");
    out("CREATEFRAME");
    newline
    get_from_local_frame();
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
    get_from_temp_frame();
    newline
    out("POPFRAME");
    out_partial("JUMP WHILE_LOOP_");
    out_integer(while_counter);
    newline
    newline
    out_partial("LABEL WHILE_LOOP_END_");
    out_integer(while_counter);
    newline
    newline
}

void G_RetrunTerm(BubbleStack_t *godzilla, char* func_name)
{
    expression_block* kingkong = NULL;
    while(!BS_IsEmpty(godzilla))
    {
        kingkong = BS_TopStack(godzilla);
        BS_Pop(godzilla);
        char* minidzilla = generate_term_name(kingkong);
        out_partial("PUSHS TF@");
        out_partial(minidzilla);
        newline
        free(kingkong);
        free(minidzilla);
    }
    out_partial("JUMP __END_OF_");
    out_partial(func_name);
    out_partial("__");
    newline
    newline
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
    //fprintf(stderr,"sssssssssssssssssssssss%p %p",var,stack);
    //aj pomocne funkcie var a stack?
    if ((var == NULL) || (stack == NULL))
    {
        return;
    }
    int i = 0;

    expression_block* temp_expbl;
    TreeElement* temp_treeelem;

    //fprintf(stderr,"%p",*var);
    //fprintf(stderr,"%p\n",BS_TopStack(stack));

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


/**
 * @brief 
 * 
 */
void init_variable_name_catcher()
{
    variable_names = NULL;
    number_of_names = 0;
}


void dtroy_variable_name_catcher()
{
    int i = 0;
    if (variable_names == NULL)
    {
    	return;
    }

    while (i < number_of_names)
    {
    	free(variable_names[i]);
    	i++;
    }
    free(variable_names);
    
    init_variable_name_catcher();
    return;
}


int G_function_bgn(TreeElement* func)
{
    init_variable_name_catcher();

    generate_execute_jump();


    out_partial("LABEL ");
    out_partial(func->name);
    newline
    out("PUSHFRAME");
    out("CREATEFRAME");

    TreeElement* local_var;

    int i = 0;
    user_func* temp = (user_func*)func->data;
    while( temp->params[i] != _ender)
    {
        local_var = TS_LookVariable(ts, temp->param_names[i]);
        char* var_name = generate_name(local_var);
        add_local_variable_name(var_name);

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


void G_function_end(char* func_name)
{
    newline
    newline
    out_partial("LABEL __END_OF_");
    out_partial(func_name);
    out_partial("__");
    newline
    out("POPFRAME");
    out("RETURN");
    newline
    newline
    generate_execute_block();
    
    dtroy_variable_name_catcher();
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
    out_partial("DEFVAR TF@");
    out_partial(target_term);
    newline

    out_partial("MOVE TF@");
    out_partial(target_term);
    if(input->operType!=_variable_oper){
        switch(input->dt)
        {
            case _string:
                out_partial(" string@");
                G_string_correct_print(input->str);
                break;
            case _integer:
                out_partial(" int@");
                out_integer(input->_integer);
                break;
            case _number:
                out_partial(" float@");
                out_number(input->_double);
                break;
            case _nan:
                out_partial(" nil@nil");
                break;
            default:
                break;
        }
    }else{
        //fprintf(stderr,"vec"); fflush(stderr);
        input_variable=generate_name(input->TSPointer);
        out_partial(" TF@");
        out_partial(input_variable);
    }
    newline

    free(target_term);
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
 * @brief 
 * 
 * @param term 
 * @return int 
 */
int G_FillWithNil(expression_block* term)
{
    char* term_name = generate_term_name(term);
    if (term_name == NULL)
        return MALLOC_ERR_CODE;

    out_partial("MOVE TF@");
    out_partial(term_name);
    out(" nil@nil");

    free(term_name);
    return 0;
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


void print_substr()
{
    out("LABEL substr");
    newline
    out("PUSHFRAME");
    out("CREATEFRAME");
    out("DEFVAR TF@s_string");
    out("POPS TF@s_string");
    newline
    out("DEFVAR TF@i_number");
    out("POPS TF@i_number");
    newline
    out("DEFVAR TF@j_number");
    out("POPS TF@j_number");
    newline
    out("JUMPIFEQ __SUBSTR_DPRINT_NIL__ TF@s_string nil@nil");          // JUMP LABEL __SUBSTR_DPRINT_NIL__
    out("JUMPIFEQ __SUBSTR_DPRINT_NIL__ TF@i_number nil@nil");          // JUMP LABEL __SUBSTR_DPRINT_NIL__
    out("JUMPIFEQ __SUBSTR_DPRINT_NIL__ TF@j_number nil@nil");          // JUMP LABEL __SUBSTR_DPRINT_NIL__
    newline
    out("DEFVAR TF@str_len");
    out("STRLEN TF@str_len TF@s_string");
    out("FLOAT2INT TF@i_number TF@i_number");
    out("FLOAT2INT TF@j_number TF@j_number");
    newline
    out("DEFVAR TF@if_stmnt_1");
    out("DEFVAR TF@if_stmnt_2");
    newline
    out("ADD TF@str_len TF@str_len int@1");
    out("GT TF@if_stmnt_1 TF@i_number int@0");
    out("LT TF@if_stmnt_2 TF@i_number TF@str_len");
    newline
    out("DEFVAR TF@out_string");                                        // TF@out_string <-- outup string
    out("MOVE TF@out_string string@");
    newline
    out("AND TF@if_stmnt_1 TF@if_stmnt_1 TF@if_stmnt_2");
    out("JUMPIFNEQ __SUBSTR_RET_EMPTY__ TF@if_stmnt_1 bool@true");        // JUMP LABEL __SUBSTR_RET_EMPTY__
    newline
    out("GT TF@if_stmnt_1 TF@j_number int@0");
    out("LT TF@if_stmnt_2 TF@j_number TF@str_len");
    newline
    out("AND TF@if_stmnt_1 TF@if_stmnt_1 TF@if_stmnt_2");
    out("JUMPIFNEQ __SUBSTR_RET_EMPTY__ TF@if_stmnt_1 bool@true");        // JUMP LABEL __SUBSTR_RET_EMPTY__
    newline
    out("LT TF@if_stmnt_1 TF@j_number TF@i_number");
    out("JUMPIFNEQ __SUBSTR_RET_EMPTY__ TF@if_stmnt_1 bool@false");       // JUMP LABEL __SUBSTR_RET_EMPTY__
    newline
    out("DEFVAR TF@new_len");
    out("MOVE TF@new_len TF@j_number");
    out("SUB TF@new_len TF@new_len TF@i_number");
    out("ADD TF@new_len TF@new_len int@1");                             // TF@new_len    <-- the output length
    newline
    out("DEFVAR TF@tmp_len");                                           // TF@tmp_len    <-- temporary current length (*reusable*)
    out("MOVE TF@out_string string@a");
    newline
    out("LABEL __SUBSTR_WHILE__");                                      // LABEL __SUBSTR_WHILE__
    out("STRLEN TF@tmp_len TF@out_string");
    out("JUMPIFEQ __SUBSTR_END_WHILE__ TF@tmp_len TF@new_len");         // JUMP LABEL __SUBSTR_END_WHILE__
    out("CONCAT TF@out_string TF@out_string string@a");
    out("JUMP __SUBSTR_WHILE__");                                       // JUMP LABEL __SUBSTR_WHILE__
    newline
    newline
    out("LABEL __SUBSTR_END_WHILE__")
    newline
    newline
    out("SUB TF@i_number TF@i_number int@1");                // subtracting one from input params
    out("SUB TF@j_number TF@j_number int@1");
    out("DEFVAR TF@getter");
    out("DEFVAR TF@displacement");
    out("MOVE TF@tmp_len TF@i_number");
    newline
    out("LABEL __SUBSTR_WHILE_2__");                                    // LABEL __SUBSTR_WHILE_2__
    out("GT TF@if_stmnt_1 TF@tmp_len TF@j_number");
    out("JUMPIFEQ __SUBSTR_END_WHILE_2__ TF@if_stmnt_1 bool@true");     // JUMP LABEL __SUBSTR_END_WHILE_2__
    out("GETCHAR TF@getter TF@s_string TF@tmp_len");
    out("SUB TF@displacement TF@tmp_len TF@i_number");
    out("SETCHAR TF@out_string TF@displacement TF@getter");
    out("ADD TF@tmp_len TF@tmp_len int@1");
    out("JUMP __SUBSTR_WHILE_2__");
    newline
    out("LABEL __SUBSTR_END_WHILE_2__");
    newline
    out("PUSHS TF@out_string");
    newline
    out("JUMP __END_OF_SUBSTR__");
    newline
    newline
    out("LABEL __SUBSTR_DPRINT_NIL__");
    out("DPRINT string@ERROR\\032\\0358:\\032At\\032least\\032one\\032parameter\\032is\\032of\\032type\\032nil");
    out("EXIT int@8");
    newline
    newline
    out("LABEL __SUBSTR_RET_EMPTY__");
    out("PUSHS string@");
    newline
    newline
    out("LABEL __END_OF_SUBSTR__");
    out("POPFRAME");
    out("RETURN");
    newline
    newline
}


void print_tointeger()
{
    newline
    out("LABEL tointeger");
    newline
    out("PUSHFRAME");
    out("CREATEFRAME");
    newline
    out("DEFVAR TF@f_number");
    out("POPS TF@f_number");
    newline
    out("JUMPIFEQ __TOINTEGER_RET_NIL__ TF@f_number nil@nil");
    newline
    out("FLOAT2INT TF@f_number TF@f_number");
    out("PUSHS TF@f_number");
    out("JUMP __END_OF_TOINTEGER__");
    newline
    newline
    out("LABEL __TOINTEGER_RET_NIL__");
    out("PUSHS nil@nil");
    newline
    out("LABEL __END_OF_TOINTEGER__");
    out("POPFRAME");
    out("RETURN");
    newline
    newline
}


void print_ord()
{
    newline
    out("LABEL ord");
    newline
    out("PUSHFRAME");
    out("CREATEFRAME");
    newline
    out("DEFVAR TF@s_string");
    out("DEFVAR TF@i_integer");
    newline
    out("POPS TF@s_string");
    out("POPS TF@i_integer");
    out("JUMPIFEQ __ORD_DPRINT_ERR__ TF@s_string nil@nil");
    out("JUMPIFEQ __ORD_DPRINT_ERR__ TF@i_integer nil@nil");
    newline
    out("DEFVAR TF@str_len");
    out("DEFVAR TF@if_stmnt");
    out("STRLEN TF@str_len TF@s_string");
    out("GT TF@if_stmnt TF@i_integer TF@str_len");
    out("JUMPIFEQ __ORD_RET_NIL__ TF@if_stmnt bool@true");
    out("LT TF@if_stmnt TF@i_integer int@1");
    out("JUMPIFEQ __ORD_RET_NIL__ TF@if_stmnt bool@true");
    newline
    out("SUB TF@i_integer TF@i_integer int@1");
    out("DEFVAR TF@out_char");
    out("STRI2INT TF@out_char TF@s_string TF@i_integer");
    out("PUSHS TF@out_char");
    out("JUMP __END_OF_ORD__");
    newline
    out("LABEL __ORD_RET_NIL__");
    out("PUSHS nil@nil");
    out("JUMP __END_OF_ORD__");
    newline
    out("LABEL __ORD_DPRINT_ERR__");
    out("DPRINT string@ERROR\\032\\0358:\\032At\\032least\\032one\\032parameter\\032is\\032of\\032type\\032nil");
    out("EXIT int@8");
    newline
    out("LABEL __END_OF_ORD__");
    out("POPFRAME");
    out("RETURN");
    newline
    newline
}


void print_chr()
{
    newline
    out("LABEL chr");
    newline
    out("PUSHFRAME");
    out("CREATEFRAME");
    newline
    out("DEFVAR TF@i_integer");
    out("POPS TF@i_integer");
    newline
    out("JUMPIFEQ __CHR_DPRINT_ERR__ TF@i_integer nil@nil");
    newline
    out("DEFVAR TF@if_stmnt");
    out("LT TF@if_stmnt TF@i_integer int@0");
    out("JUMPIFEQ __CHR_RET_NIL__ TF@if_stmnt bool@true")
    out("GT TF@if_stmnt TF@i_integer int@255");
    out("JUMPIFEQ __CHR_RET_NIL__ TF@if_stmnt bool@true");
    newline
    out("INT2CHAR TF@i_integer TF@i_integer");
    out("PUSHS TF@i_integer");
    out("JUMP __END_OF_CHR__");
    newline
    newline
    out("LABEL __CHR_RET_NIL__");
    out("PUSHS nil@nil");
    out("JUMP __END_OF_CHR__");
    newline
    out("LABEL __CHR_DPRINT_ERR__");
    out("DPRINT string@ERROR\\032\\0358:\\032At\\032least\\032one\\032parameter\\032is\\032of\\032type\\032nil");
    out("EXIT int@8");
    newline
    out("LABEL __END_OF_CHR__");
    out("POPFRAME");
    out("RETURN");
    newline
    newline
}


/**      ----------------       END OF CODE_ASSEMBLY.C      -------------------     */
