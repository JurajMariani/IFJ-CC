
/**
 * @file Code_Assembly.c
 * @author xmaria03 (xmaria03@stud.fit.vutbr.cz), xmacej03 (xmacej03@stud.fit.vutbr.cz)
 * @brief Code Generation file, transcribing the parser(input) to IFJcode21(output)
 * @version 1.0
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "../libs/Code_Assembly.h"

/**
 * @brief Printing Macros for Instructions and constants
 * 
 */
#define out(s) fprintf(stdout,"%s\n",s);
#define out_partial(s) fprintf(stdout,"%s",s);
#define out_number(number) fprintf(stdout,"%a",number);
#define out_integer(integer) fprintf(stdout,"%d",integer);
#define out_char(char) fprintf(stdout,"%c",char);
#define newline fprintf(stdout,"\n");


/**
 * @brief Local variable name generator
 * Generates name by appending the symtable address to the variable name: x -> x_0xfffffffff
 * 
 * @param var pointer to the variable element in symtable
 * @return (char*) pointer to the generated name 
 */
char* generate_name(TreeElement* var)
{
    char temp1[101];
    strcpy(temp1, var->name);
    char temp2[15];
    sprintf(temp2, "%p", var);
    unsigned len = strlen(temp1) + strlen(temp2) + 5;
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
    out[strlen(temp1) + i + 1]='\0';
    return (out);
}


/**
 * function to generate term adress at the end of expression name 
 * x -> T_x_adress
 */

/**
 * @brief Term variable name generator
 * Generates term names:
 *      T_int_double(trunc)_double(decimal); (int = expression_block->_integer, double = expression_block->_double)
 *      T_1_0_5 => int = 1, double = 0.5
 * @param term 
 * @return char* 
 */
char * generate_term_name(expression_block* term)
{
    char *out = malloc(sizeof(char)*203);
    unsigned i = 0;
    out[i++] = 'T';
    out[i++] = '_';
    int pom1 = term->_integer;
    double pom2 = term->_double;
    char char_pom1[100];
    char char_pom2[100];
    sprintf(char_pom1, "%d", pom1);
    sprintf(char_pom2, "%lf", pom2);
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


/**
 * @brief Jump Generator
 * It is used to jump over function definitions
 */
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
 * @brief Generates the code header
 * prints ".IFJcode21" and prints the builtin functions
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
 * @brief Creates the execute label
 * see generate_execute_jump()
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


/**
 * @brief String format converter
 * Converts normal strings into suitable result codes strings\
 * Replaces special characters with escape sequences
 * 
 * EXAMPLE "What is this?", he asked angrily. -> \034What\032is\032this?\034,\032he\032asked\032angrily.
 * 
 * @param string The original string
 */
void G_string_correct_print(char* string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if (string[i] > 32)
        {
            if (string[i] == '\\')
            {
                out_partial("\\092");
            }
            else
                if (string[i] == '\"')
                {
                    out_partial("\\034");
                }
                else    
                out_char(string[i]);
        }
        else
        {
            if (string[i] > 9)
            {
                out_partial("\\0");
                out_integer(string[i]);
            }
            else
            {
                out_partial("\\00");
                out_integer(string[i]);
            }
        }
        i++;
    }
}


/**
 * @brief New Expression block generator
 * Used to generate (expr_blocks) terms returning from functions
 * 
 * @param returning Array of returning data types
 * @param param_num Index of the returning parameter
 * @param param_max The number of returning parameters
 * @return (expression_block*) Pointer to the newly allocated term
 */
expression_block* generate_exp_block(dataType* returning, int param_num, int param_max)
{
    expression_block* ret = (expression_block*) malloc(sizeof(expression_block));
    if (ret == NULL)
        return NULL;
    ret->_double = ++exprCounter;
    ret->operType = _not_terminal_oper;
    ret->blockType = _operand_expr;
    ret->dt = returning[param_num];
    ret->_integer = param_max - 1;

    return ret;
}


/**
 * @brief Stores the generated variable name pointer to an array (variable_names[global])
 * 
 * @param var_name The pointer to the generated name
 */
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
 * @brief CODE GENERATOR - Local variable definition
 * 
 * @param new_var Pointer to the variable (in symtable)
 */
void def_var(TreeElement* new_var)
{
    char* name = generate_name(new_var);
    out_partial("DEFVAR TF@");
    out_partial(name);
    newline
    // New variables are automatically nullified
    out_partial("MOVE TF@");
    out_partial(name);
    out_partial(" nil@nil");
    newline
    free(name);
}


/**
 * @brief CODE GENERATOR - Pushes function parameters, generates the function call and pops the returns
 * 
 * @param func Pointer to the function element (in symtable)
 * @param params Stack of (expression_block*) terms - Function parameters
 * @param returns Stack of (expression_block*) terms - Functions returns (empty at the beginning, terms generated internally) 
 * @return (int) Status - '0' = OK 
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
    // Read function variants have no parameters
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
                    out_partial(" int");
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
                // write(param) : if the param is nil, the WRITE instruction is supposed to write string@nil
                out_partial("JUMPIFEQ __WRITE_NIL__");
                out_integer(logic_counter);
                out_partial(" TF@");
                out_partial(term_name);
                out_partial(" nil@nil");
                newline
                
                out_partial("WRITE TF@");
                out_partial(term_name);
                newline
                out_partial("JUMP __AFTER_WRITE_NIL__");
                out_integer(logic_counter);
                newline

                out_partial("LABEL __WRITE_NIL__");
                out_integer(logic_counter);
                newline
                out_partial("WRITE string@nil");
                newline

                out_partial("LABEL __AFTER_WRITE_NIL__");
                out_integer(logic_counter++);
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
            // Even the builtin functions are called here (with returning parameters)
            out_partial("CALL ");
            out_partial(func->name);
            newline
            newline

            expression_block* term = NULL;


            int control = 0;
            while( control < n)
            {
                term = NULL;
                fprintf(stderr, "Co sa sem posiela, hmmm? %d, %d?\n",control, n);fflush(stderr);
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
 * @brief CODE GENERATOR - Generates IF condition
 * 
 * @param term The term variable compared within the condition
 * @return (int) Status, '0' = OK
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
 * @brief CODE GENERATOR - Generates the ELSE branch (LABEL)
 */
void G_IfELSE(int counter)
{
    out_partial("JUMP END_");
    out_integer(counter);
    newline
    newline
    newline
    out_partial("LABEL ELSE_");
    out_integer(counter);
    newline
}


/**
 * @brief CODE GENERATOR - Generates the IF condition end label
 * 
 */
void G_IfEND(int counter)
{
    newline
    newline
    out_partial("LABEL END_");
    out_integer(counter);
    newline
    newline

}


/**
 * @brief CODE GENERATOR - Generates the shift of local variables (LF -> TF) in the while loop
 */
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


/**
 * @brief CODE GENERATOR - Generates the shift of local variables (TF -> LF) in the while loop
 */
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
 * @brief Recursive preorder through the symtable layer tree, stores the generated variable names
 * 
 * @param root Pointer to the root of the tree of current layer
 */
void store_local_var_names(TreeElement* root)
{
    if(root == NULL)
        return;

    if (root->type == w_var)
    {
        char* tmp_name = generate_name(root);

        add_local_variable_name(tmp_name);
    }

    store_local_var_names(root->left);
    store_local_var_names(root->right);
}


/**
 * @brief Recursive go-through the layers of symtable, calling store_local_var_names() over each layer's tree
 * 
 * @param act_layer pointer to the "starting" layer (ts->curLayer is intended)
 */
void extract_local_vars(TreeLayer* act_layer)
{
    if (act_layer == ts->functionLayer)
        return;
    
    store_local_var_names(act_layer->tree->root);

    extract_local_vars(act_layer->prevLayer);
}


/**
 * @brief CODE GENERATOR - Generates the WHILE loop label, creates a new frame and copies the values of all local variables inside the frame
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
    function_frame_counter++;
    newline
    
    extract_local_vars(ts->curLayer);
    get_from_local_frame();
    dtroy_variable_name_catcher();

    newline


    // <statement> <st-list>
    // G_ComparBool / G_CompareNull
}


/**
 * @brief CODE GENERATOR - Generates the IF statement for a loop, compares a given term with a bool value
 * 
 * @param term Pointer to the compared term
 * @return (int) Status, '0' = OK
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
 * @brief CODE GENERATOR - Generates the IF statement for a loop, compares a given term with nil
 * 
 * @param term Pointer to the compared term
 * @return (int) Status, '0' = OK
 */
int G_CompareNull(expression_block* term)
{
    char* term_name = generate_term_name(term);
    if (term_name == NULL)
        return MALLOC_ERR_CODE;
    
    newline
    newline
    out_partial("JUMPIFEQ WHILE_LOOP_END_");
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
 * @brief CODE GENERATOR - Generates the shift of changed local variable values (TF -> LF), jump to the start of the loop and the end label
 */
void G_WhileEND(int counter)
{
    newline
    
    extract_local_vars(ts->curLayer);
    get_from_temp_frame();
    dtroy_variable_name_catcher();

    newline
    out("POPFRAME");
    out_partial("JUMP WHILE_LOOP_");
    out_integer(counter);
    newline
    newline
    out_partial("LABEL WHILE_LOOP_END_");
    out_integer(counter);
    newline
    newline
}


/**
 * @brief CODE GENERATOR - Pushes the return values to the stack, then jumps to the end of function
 * 
 * @param godzilla Stack containing to be pushed terms
 * @param func_name Name of the function (for a correct jump)
 */
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
    int iter = 0;
    while(iter < function_frame_counter)
    {
        out("POPFRAME");
        iter++;
    }

    out_partial("JUMP __END_OF_");
    out_partial(func_name);
    out_partial("__");
    newline
    newline
}


/**
 * @brief CODE GENERATOR - Generates assignment TERM -> VARIABLE(local)
 * 
 * @param var Pointer to a local variable (symtable)
 * @param term Pointer to a term variable
 */
void G_AssignToVar(TreeElement* var,expression_block* term)
{
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
}


/**
 * @brief CODE GENERATOR - Generates assignment TERM -> VARIABLE(local) for more variables
 * 
 * @param var Pointer to a local variable stack (symtable)
 * @param stack Pointer to a term variable stack
 */
void G_AssignToVars(TreeElement** var,BubbleStack_t* stack)//<----------------- Ked je NULL,NULL tak len vypopuje returny, funkcia nema priradenie
{
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

        variable* varPtr = temp_treeelem->data;

        char* orangutan = generate_term_name(temp_expbl);
        char* pavian = generate_name(temp_treeelem);

        out_partial("MOVE TF@");
        out_partial(pavian);
        out_partial(" TF@");
        out_partial(orangutan);
        newline

        varPtr->vDefined = 1;
        if(temp_expbl->dt == _nan)
            varPtr->isNil = 1;
        else
            varPtr->isNil = 0;

        free(pavian);
        free(orangutan);
        free(temp_expbl);

        BS_Pop(stack);

        i++;
    }
}


/**
 * @brief Initialiser of variable names string array
 */
void init_variable_name_catcher()
{
    variable_names = NULL;
    number_of_names = 0;
}


/**
 * @brief Destructor of variable names string array
 */
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


/**
 * @brief CODE GENERATOR - Generates function label, creates frame and pops parameters
 * 
 * @param func Pointer to a function (symtable)
 * @return (int) Status '0' = OK
 */
int G_function_bgn(TreeElement* func)
{
    init_variable_name_catcher();
    function_frame_counter = 0;

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

        out_partial("DEFVAR TF@");
        out_partial(var_name);
        newline
        out_partial("POPS TF@");
        out_partial(var_name);
        newline
        newline

        add_local_variable_name(var_name);
        i++;
    }

    i = 0;
    while( temp->returns[i] != _ender)
    {
        out("PUSHS nil@nil");
        i++;
    }

    return 0;
}


/**
 * @brief CODE GENERATOR - Generates function end label and returns
 * 
 * @param func_name Name of the function (for a correct jump)
 */
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
 * @brief CODE GENERATOR - Assignment (VAR -> TERM) or (CONST -> TERM)
 * 
 * @param target Pointer to expr_block TERM
 * @param input Pointer to expr_block VAR/CONST
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
    if(input->operType!=_variable_oper)
    {
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
    }else
    {
        input_variable=generate_name(input->TSPointer);
        out_partial(" TF@");
        out_partial(input_variable);
        free(input_variable);
    }
    newline

    free(target_term);
}


/**
 * @brief CODE GENERATOR - The # operator
 * 
 * //strlen TF@targetname string@input-str
 * 
 * @param target Pointer to expr_block TERM, DST (int)
 * @param input Pointer to expr_block TERM, SRC (str)
 */
void unary_operator(expression_block* target, expression_block* input)
{
    char* pom_target;
    char* pom_input;
    pom_target = generate_term_name(target);
    pom_input = generate_term_name(input);
    out_partial("DEFVAR TF@");
    out_partial(pom_target);
    newline
    out_partial("STRLEN ");
    out_partial("TF@");
    
    out_partial(pom_target);
    out_partial(" TF@");
    out_partial(pom_input);
    newline

    free(pom_target);
    free(pom_input);
}



/**
 * @brief CODE GENERATOR - Assign nil@nil to TERM
 * 
 * @param term Pointer to expr_block TERM, DST
 * @return (int) Status, '0' = OK 
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
 * @brief CODE GENERATOR - Performs Arithmetic and Logic instructions
 * 
 * @param target Pointer to expr_block TERM, DST
 * @param operand_one Pointer to expr_block TERM, OPERAND ONE
 * @param operand_two Pointer to expr_block TERM, OPERAND TWO
 * @param sign Pointer to expr_block TERM, OPERATOR
 */
void operation_quick_action(expression_block* target, expression_block* operand_one, expression_block* operand_two, expression_block* sign)
{
    char* operand_one_name = generate_term_name(operand_one);
    char* operand_two_name = generate_term_name(operand_two);
    char* target_name = generate_term_name(target);


    out_partial("DEFVAR TF@");
    out_partial(target_name);
    newline

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
            out_partial("GT TF@");
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
            out_partial("LT TF@");
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(operand_one_name);
            out_partial(" TF@");
            out_partial(operand_two_name);
            newline

            out_partial("NOT TF@")
            out_partial(target_name);
            out_partial(" TF@");
            out_partial(target_name);
            newline
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
 * @brief Performs the int to float conversion
 * 
 * @param term Pointer to expr_block TERM
 */
void convert_to_float(expression_block* term)
{
    char* term_name = generate_term_name(term);
    newline
    
    out_partial("JUMPIFEQ __SKIP_INT2FLOAT__");
    out_integer(logic_counter);
    out_partial(" TF@");
    out_partial(term_name);
    out_partial(" nil@nil");
    newline;

    out_partial("INT2FLOAT ");
    out_partial("TF@");
    out_partial(term_name);
    out_partial(" TF@");
    out_partial(term_name);
    newline

    out_partial("LABEL __SKIP_INT2FLOAT__");
    out_integer(logic_counter++);
    newline

    term->dt = _number;
}


/**
 * @brief Performs the float to int conversion
 * 
 * @param term Pointer to expr_block TERM
 */
void convert_to_int(expression_block* term)
{
    char* term_name = generate_term_name(term);
    
    out_partial("JUMPIFEQ __SKIP_FLOAT2INT__");
    out_integer(logic_counter);
    out_partial(" TF@");
    out_partial(term_name);
    out_partial(" nil@nil");
    newline

    newline
    out_partial("FLOAT2INT ");
    out_partial("TF@");
    out_partial(term_name);
    out_partial(" TF@");
    out_partial(term_name);
    newline

    out_partial("LABEL __SKIP_FLOAT2INT__");
    out_integer(logic_counter++);
    newline

    term->dt = _integer;
}


/**
 * @brief Prints the substr() builtin function
 */
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
    out("JUMPIFNEQ __SUBSTR_RET_EMPTY__ TF@if_stmnt_1 bool@true");      // JUMP LABEL __SUBSTR_RET_EMPTY__
    newline
    out("GT TF@if_stmnt_1 TF@j_number int@0");
    out("LT TF@if_stmnt_2 TF@j_number TF@str_len");
    newline
    out("AND TF@if_stmnt_1 TF@if_stmnt_1 TF@if_stmnt_2");
    out("JUMPIFNEQ __SUBSTR_RET_EMPTY__ TF@if_stmnt_1 bool@true");      // JUMP LABEL __SUBSTR_RET_EMPTY__
    newline
    out("LT TF@if_stmnt_1 TF@j_number TF@i_number");
    out("JUMPIFNEQ __SUBSTR_RET_EMPTY__ TF@if_stmnt_1 bool@false");     // JUMP LABEL __SUBSTR_RET_EMPTY__
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
    out("SUB TF@i_number TF@i_number int@1");                           // subtracting one from input params
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


/**
 * @brief Prints the tointeger() builtin function
 */
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


/**
 * @brief Prints the ord() builtin function
 */
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


/**
 * @brief Prints the chr() builtin function
 */
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
