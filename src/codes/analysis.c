#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Defs.h"
#include "lex_analyz.h"

token get_next_token()
{
    char *output = (char*) malloc(STR_SIZE * sizeof(char));
    if(output == NULL)
    {
        //TODO
    }

    char state[20];
    strcpy(state, "start");

    unsigned output_length = 0;
      
    char input_c = fgetc(stdin);

    while(input_c == 32 || input_c == '\t' || input_c == '\r' || input_c == '\n' || input_c == '\v' || input_c == '\f')
    {
        if(input_c == '\n') line_cnt++;
        input_c = fgetc(stdin);
    } 

    switch(input_c)
    {
        case 'a' ... 'z' :
        case 'A' ... 'Z' :
        case '_' :
        {
            strcpy(state, "id");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '0' ... '9':
        {
            strcpy(state, "num");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '.':
        {
            strcpy(state, "dot");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '+':
        {
            strcpy(state, "add");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }
            
        case '-':
        {
            strcpy(state, "sub");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '*':
        {
            strcpy(state, "mult");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '/':    
        {
            strcpy(state, "div");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '=':
        {
            strcpy(state, "assign");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '<':
        {
            strcpy(state, "less");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '>':
        {
            strcpy(state, "great");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '~':
        {
            strcpy(state, "not");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case '#':
        {
            strcpy(state, "len");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }
        
        case '"':
        {
            strcpy(state, "start_str");
            first_perimeter(state, output, &output_length);
            break;
        }

        case '(' :
        case '[' :
        {
            strcpy(state, "bracket_L");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case ')' :
        case ']' :
        {
            strcpy(state, "bracket_R");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case ':' :
        {
            strcpy(state, "doubleKomma");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }

        case ',' :
        {
            strcpy(state, "komma");
            output[output_length++] = input_c;
            first_perimeter(state, output, &output_length);
            break;
        }
        

        default: break;
    }

    return result;
}

void first_perimeter(char *state, char *output, unsigned *output_length)
{
    char input_c;
    
    if (!strcmp(state, "id"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= 'A') && (input_c <= 'Z')) || ((input_c >= 'a') && (input_c <= 'z')) || ((input_c >= 0) && (input_c <= 9)) || (input_c == '_'))
        {
            output[(*output_length)++] = input_c;

            if(!(*output_length % (unsigned) STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, output);

                output = realloc(output, (((*output_length) / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char));
                if(output == NULL)
                {
                    strcpy(state, "id");
                    strcpy(output, tmp);
                    err_flag = MALLOC_ERR_CODE;
                    return;
                    //TODO navratova hodnota tokenu
                }
            }

            input_c = fgetc(stdin);
        }

        ungetc(input_c, stdin);

        output[*output_length] = '\0';

        int tmp = Is_Keyword(output);
        if(tmp > 0)
        {
            result.type = _keyword;
            result.data.type = _string;
            result.data.kw = tmp;
        }
        else
        {
            result.type = _identifier;
            result.data.type = _string;
            strcpy(result.data.str, output);
        }

        return;
    }

    if (!strcmp(state, "num "))
    {
        input_c = fgetc(stdin);

        while((input_c >= 0) && (input_c <= 9))
        {
            output[(*output_length)++] = input_c;
            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, output);

                output = realloc(output, ((*output_length / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char));
                if(output == NULL)
                {
                    strcpy(state, "int");
                    strcpy(output, tmp);
                    err_flag = MALLOC_ERR_CODE;
                    return;
                    //TODO navratova hodnota tokenu
                }
            }
            input_c = fgetc(stdin);
        }

        if((input_c == 'e') || (input_c == 'E'))
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "int_exp");
            second_perimeter(state, output, output_length);
            return;
        }
        else if(input_c == '.')
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "float");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            output[*output_length] = '\0';

            result.type = _const;
            result.data.type = _integer;
            result.data._integer = atoi(output);

            return;
        }
    }

    if (!strcmp(state, "dot"))
    {
        input_c = fgetc(stdin);

        if(input_c == '.')
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "conc");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            //TODO nespravny operand
            ungetc(input_c, stdin);
            output[*output_length] = '\0';
            err_flag = INVALID_TOKEN;
            return;
        }
    }

    if (!strcmp(state, "add"))
    {
        result.type = _operand;
        result.data.oper = _add;
        return;
    }

    if (!strcmp(state, "mult"))
    {
        result.type = _operand;
        result.data.oper = _mul;
        return;
    }

    if (!strcmp(state, "div"))
    {
        input_c = fgetc(stdin);

        if(input_c == '/')
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "fdiv");
            second_perimeter(state, output, output_length);
            return;
        }

        else
        {
            ungetc(input_c, stdin);
            
            result.type = _operand;
            result.data.oper = _div;
            return;
        }
    }

    if (!strcmp(state, "sub"))
    {
        input_c = fgetc(stdin);

        if(input_c == '-')
        {
            strcpy(state, "ln_cmt");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);

            result.type = _operand;
            result.data.oper = _sub;
            return;
        }
    }

    if (!strcmp(state, "assign"))
    {
        input_c = fgetc(stdin);
        if(input_c == '=')
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "eq");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            
            result.type = _misc;
            result.data.msc = _assign;

            return;
        }
    }

    if (!strcmp(state, "less"))
    {
        input_c = fgetc(stdin);

        if(input_c == '=')
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "leq");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            result.type = _operand;
            result.data.oper = _less;
            return;
        }
    }

    if (!strcmp(state, "great"))
    {
        input_c = fgetc(stdin);

        if(input_c == '=')
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "geq");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            result.type = _operand;
            result.data.oper = _great;
            return;
        }
    }
    
    if (!strcmp(state, "not"))
    {
        input_c = fgetc(stdin);

        if(input_c == '=')
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "neq");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            err_flag = INVALID_TOKEN;
            //TODO
            return;
        }
    }
   
    if (!strcmp(state, "len"))
    {
        result.type = _operand;
        result.data.oper = _length;
        return;
    }

    if (!strcmp(state, "start_str"))
    {
        input_c = fgetc(stdin);
        while(input_c != '"')
        {
            output[(*output_length)++] = input_c;

            if(input_c == 10) 
            {
                line_cnt++;
                err_flag = INVALID_TOKEN;
                return;
                // TODO navratovy token
            }

            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, output);

                output = realloc(output, ((*output_length / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char));
                if(output == NULL)
                {
                    strcpy(state, "id");
                    strcpy(output, tmp);
                    err_flag = MALLOC_ERR_CODE;
                    //TODO navratova hodnota tokenu
                    return;
                }
            }

            if(input_c == '\\')
            {
                strcpy(state,"esc_seq");
                output[(*output_length)++] = input_c;
                second_perimeter(state, output, output_length);
                
                strcpy(state, "start_str");
                first_perimeter(state, output, output_length);
                return;
            }
        }
        strcpy(state, "end_str");
        second_perimeter(state, output, output_length);
        return;
    }
   
    if (!strcmp(state, "bracket_L"))
    {
        result.type = _misc;
        result.data.oper = _bracketL;
        return;
    }

    if (!strcmp(state, "bracket_R"))
    {
        result.type = _misc;
        result.data.oper = _bracketR;
        return;
    }

    if (!strcmp(state, "doubleKomma"))
    {
        result.type = _misc;
        result.data.oper = _bracketL;
        return;
    }

    if (!strcmp(state, "komma"))
    {
        result.type = _misc;
        result.data.oper = _komma;
        return;
    }
}

void second_perimeter(char *state, char *output, unsigned *output_length)
{
    char input_c;

    if (!strcmp(state, "float"))
    {
        input_c = fgetc(stdin);

        while((input_c >= 0) && (input_c <= 9))
        {
            output[(*output_length)++] = input_c;

            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, output);

                output = realloc(output, ((*output_length / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char));
                if(output == NULL)
                {
                    strcpy(state, "dec");
                    strcpy(output, tmp);
                    err_flag = MALLOC_ERR_CODE;
                    return;
                    //TODO NAVRATOVA HODNOTA
                }
            }

            input_c = fgetc(stdin);
        }

        if((input_c == 'e') || (input_c == 'E'))
        {
            output[(*output_length)++] = input_c;
            strcpy(state, "float_exp");
            other_states(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            result.type = _const;
            result.data.type = _number;
            result.data._double = atof(output);
            return;
        }
    }

    if (!strcmp(state, "int_exp"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= 0) && (input_c <= 9)) || (input_c == '+') || (input_c == '-'))
        {
            output[(*output_length)++] = input_c;

            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, output);

                output = realloc(output, ((*output_length / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char));
                if(output == NULL)
                {
                    strcpy(state, "dec");
                    strcpy(output, tmp);
                    err_flag = MALLOC_ERR_CODE;
                    return;
                    //TODO NAVRATOVA HODNOTA
                }
            }

            input_c = fgetc(stdin);
        }

        ungetc(input_c, stdin);
        result.type = _const;
        result.data.type = _number;
        result.data._double = atof(output);
        return;
    }

    if (!strcmp(state, "conc"))
    {
        result.type = _operand;
        result.data.oper = _konk;
        return;
    }

    if (!strcmp(state, "fdiv"))
    {
        result.type = _operand;
        result.data.oper = _div2;
        return;
    }

    if (!strcmp(state, "ln_cmt"))
    {
        input_c = fgetc(stdin);

        while((input_c != '\0') || (input_c != '['))
        {
            input_c = fgetc(stdin);
        }

        if(input_c == '\0') 
        {
            strcpy(state, "start");
            return;
        }

        if(input_c == '[')
        {
            strcpy(state, "left_bracket");
            if(!left_bracket(state))
            {
                second_perimeter(state, output, output_length);
            }
            else return;
        }
    }

    if (!strcmp(state, "eq"))
    {
        result.type = _operand;
        result.data.oper = _Eq;
        return;
    }

    if (!strcmp(state, "leq"))
    {
        result.type = _operand;
        result.data.oper = _lessEq;
        return;
    }

    if (!strcmp(state, "geq"))
    {
        result.type = _operand;
        result.data.oper = _greatEq;
        return;
    }

    if (!strcmp(state, "neq"))
    {
        result.type = _operand;
        result.data.oper = _nEq;
        return;
    }

    if (!strcmp(state, "end_str"))
    {
        output[*output_length] = '\0';
        result.type = _const;
        result.data.type = _string;
        strcpy(result.data.str, output);
        return;
    }

    if (!strcmp(state, "esc_seq"))
    {
        input_c = fgetc(stdin);

        switch(input_c)
        {   
            case 'n':
            case 't':
            {
                output[(*output_length)++] = input_c;
                return;
            }

            case '"':
            {
                output[(*output_length)-1] = input_c;
                return;
            }

            case '\\':
            {
                return;
            }

            case '0' ... '2':
            {
                ungetc(input_c, stdin);
                char tmp = esc_num();
                if(tmp != '9')
                {
                    output[(*output_length)++] = tmp;
                    return;
                }
                else return;
                
            }

            default:
            {
                err_flag = INVALID_TOKEN;
                ungetc(input_c, stdin);
                return;
            }
        }
    }
}

void other_states(char *state, char *output, unsigned *output_length)
{
    char input_c;

    if (!strcmp(state, "float_exp"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= 0) && (input_c <= 9)) || (input_c == '+') || (input_c == '-'))
        {
            output[(*output_length)++] = input_c;

            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, output);

                output = realloc(output, ((*output_length / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char));
                if(output == NULL)
                {
                    strcpy(output, tmp);
                    err_flag = MALLOC_ERR_CODE;
                    return;
                    //TODO navratova hpdnota
                }
            }

            input_c = fgetc(stdin);
        }

        ungetc(input_c, stdin);
        result.type = _const;
        result.data.type = _number;
        result.data._double = atof(output);
        return;
    }

}

int left_bracket(char* state)
{
    char input_c = fgetc(stdin);

    if(input_c == '[')
    {
        strcpy(state, "bl_cmt");
        bl_cmt(state);
        return 1;
    }
    else
    {
        strcpy(state, "ln_cmt");
        ungetc(input_c, stdin);
        return 0;
    }
}

void bl_cmt(char* state)
{
    char input_c = fgetc(stdin);

    while(input_c != ']' || input_c != EOF)
    {
        if(input_c == '\n') line_cnt++;
        input_c = fgetc(stdin);
    }
          
    if(input_c == ']')
    {
        if(!right_bracket(state))
        {
            bl_cmt(state);
        }
        else return;
    }
    else
    {
        //TODO
        //error_flag = -2; 
        return;
    }
}

int right_bracket(char* state)
{
    char input_c = fgetc(stdin);
    if(input_c == ']')
    {
        strcpy(state, "start");
        return 1;
    }
    else
    {
        strcpy(state, "bl_cmt");
        ungetc(input_c, stdin);
        return 0;
    }
}

int Is_Keyword(char *output)
{
    int i = 0;
    while((i != ARR_LEN) || !(strcmp(output, arr_keywords[i])))
    {
        i++;
    }

    if(i == ARR_LEN) return 0;
    if (!strcmp(output, arr_keywords[i])) return i;

    return -1;
}

char esc_num(void)
{
    char input_c = fgetc(stdin);
    char output_c[3];
    unsigned int output_len = 0;
    
    switch(input_c)
    {
        case '0': 
        {
            output_c[output_len++] = input_c;
            zero(output_c, &output_len);
            break;
        }

        case '1':
        {
            output_c[output_len++] = input_c;
            one(output_c, &output_len);
            break;
        }

        case '2':
        {
            output_c[output_len++] = input_c;
            two(output_c, &output_len);
            break;
        }

        default:
        {
            err_flag = INVALID_TOKEN;
            ungetc(input_c, stdin);
            return '9';
        }
    }

    int tmp = atoi(output_c);
    char result = tmp;
    return result;
}

void zero(char *output_c, unsigned *output_len)
{
    char input_c = fgetc(stdin);

    switch(input_c)
    {
        case '0':
        {
            output_c[(*output_len)++] = input_c;
            zero_zero(output_c, output_len);
            return;   
        }

        case '1' ... '9':
        {
            output_c[(*output_len)++] = input_c;
            others(output_c, output_len);
            return;   
        }

        default:
        {
            err_flag = INVALID_TOKEN;
            ungetc(input_c, stdin);
            return;
        }
    }
}

void one(char *output_c, unsigned *output_len)
{
    char input_c = fgetc(stdin);
    if(input_c >= '0' && input_c <= '9')
    {
        output_c[(*output_len)++] = input_c;
        others(output_c, output_len);
        return;
    }
    else
    {
        err_flag = INVALID_TOKEN;
        ungetc(input_c,stdin);
        return;
    }
}

void two(char *output_c, unsigned *output_len)
{
    char input_c = fgetc(stdin);
    switch(input_c)
    {
        case '0' ... '4':
        {
            output_c[(*output_len)++] = input_c;
            others(output_c, output_len);
            return;
        }

        case '5':
        {
            output_c[(*output_len)++] = input_c;
            two_five(output_c, output_len);
            return;
        }

        default:
        {
            err_flag = INVALID_TOKEN;
            ungetc(input_c, stdin);
            return;
        }
    }
}

void zero_zero(char *output_c, unsigned *output_len)
{
    char input_c = fgetc(stdin);
    if(input_c >= '1' && input_c <= '9')
    {
        output_c[*output_len] = input_c;
        return;
    }
    else
    {
        err_flag = INVALID_TOKEN;
        ungetc(input_c, stdin);
        return;
    }
}

void two_five(char *output_c, unsigned *output_len)
{
    char input_c = fgetc(stdin);
    if(input_c >= '0' && input_c <= '5')
    {
        output_c[*output_len] = input_c;
        return;
    }
    else
    {
        err_flag = INVALID_TOKEN;
        ungetc(input_c, stdin);
        return;
    }
}

void others(char *output_c, unsigned *output_len)
{
    char input_c = fgetc(stdin);
    if(input_c >= '0' && input_c <= '9')
    {
        output_c[*output_len] = input_c;
        return;
    }
    else
    {
        err_flag = INVALID_TOKEN;
        ungetc(input_c, stdin);
        return;
    }
}

int get_line_number()
{
    return line_cnt;
}