
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

#define out(s) fprintf(stdout,"%s\n",s);
#define out_partial(s) fprintf(stdout,"%s",s);
#define out_number(number) fprintf(stdout,"%a",number);
#define out_integer(integer) fprintf(stdout,"%d",integer);
#define newline fprintf(stdout,"\n");
#define function_out(d, s) for(xxx_help_xxx = 0;xxx_help_xxx < d; xxx_help_xxx++)   \
                           {                                                        \
                               fprintf(stdout,"\t");                                \
                           }                                                        \
                           out(s);



/**
 * @brief global variable that helps print n * '\t' so that functions are segmented nicely :)
 *        macro : function_out()
 */
int xxx_help_xxx;

/**
 * @brief global counter, counting the use of JUMP EXECUTEx; x is 0..INT_MAX
 * to add the 'x' at the back
 */
int execute_counter = 0;


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

    newline
    out(".IFJcode21");
    generate_execute_jump();

}


void generate_execute_block(void)
{
    newline
    newline
    newline
    out("LABEL EXECUTE");
    newline
}


/**
 * @brief 
 * 
 * @param target 
 * @param input 
 */
void var_to_term_assign(expression_block* target, expression_block* input)
{
    (void)target;
    (void)input;
}


/**
 * @brief 
 * 
 * @param target 
 * @param input 
 */
void unary_operator(expression_block* target, expression_block* input)
{
    (void)target;
    out_partial("STRLEN "); 
    //out_partial(); //TODO get_name(target->TSPointer->name); GET VARIABLE NAME
    out_partial(" ");
    if (input->str == NULL)
    {
        out_partial((char*)input->TSPointer->data);
    }
    else
    {
        out_partial(input->str);
    }
    newline

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
    (void)target;
    (void)operand_two;
    (void)operand_one;
    (void)sign;
}


/**
 * @brief 
 * 
 * @param term 
 */
void assign_to_end_block(expression_block* term)
{
    (void)term;
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
    //out_partial(); // TODO get_name(term->TSPointer->name); GET VARIABLE NAME
    out_partial(" ");
    out_integer(*((int*)(term->TSPointer->data)));
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
    //out_partial(); // TODO get_name(term->TSPointer->name); GET VARIABLE NAME
    out_partial(" ");
    out_number(*((float*)(term->TSPointer->data)));
    newline

    term->dt = _integer;
}