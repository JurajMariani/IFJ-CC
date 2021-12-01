#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libs/analysis.h"

void GetNextToken(token *tokenOut)
{
    if(tokenOut->type == _misc)
    {
        if(tokenOut->data.msc == _EOF) 
        {
            free(result.data.str);
            //fprintf(stderr,"\nIt's already end of file\n\n");
            return;
        }
    }

    char *output = (char*) malloc(STR_SIZE * sizeof(char));
    if(output == NULL)
    {
        TS_COLLAPSE(&ts);
        RaiseError(99);
        exit(99);
    }

    char state[20];
    strcpy(state, "start");
    lex_err_flag = 0;

    unsigned output_length = 0;
    str2write = 0;
      
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
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '0' ... '9':
        {
            strcpy(state, "num");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '.':
        {
            strcpy(state, "dot");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '+':
        {
            strcpy(state, "add");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }
            
        case '-':
        {
            strcpy(state, "sub");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '*':
        {
            strcpy(state, "mult");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '/':    
        {
            strcpy(state, "div");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '=':
        {
            strcpy(state, "assign");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '<':
        {
            strcpy(state, "less");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '>':
        {
            strcpy(state, "great");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '~':
        {
            strcpy(state, "not");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '#':
        {
            strcpy(state, "len");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }
        
        case '"':
        {
            strcpy(state, "start_str");
            first_perimeter(state, &output, &output_length);
            break;
        }

        case '(' :
        case '[' :
        {
            strcpy(state, "bracket_L");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case ')' :
        case ']' :
        {
            strcpy(state, "bracket_R");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case ':' :
        {
            strcpy(state, "doubleKomma");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }

        case ',' :
        {
            strcpy(state, "komma");
            output[output_length++] = input_c;
            first_perimeter(state, &output, &output_length);
            break;
        }
        
        case EOF:
        {
            result.type = _misc;
            result.data.msc = _EOF;
            break;
        }

        default:
        {
            TS_COLLAPSE(&ts);
            RaiseError(1);
            exit(1);
        }
    }
    
    if(!strcmp(state, "start"))
    {
        if(result.data.msc == _EOF)
        {
            tokenOut->type = _misc;
            tokenOut->data.msc = _EOF;
        }

        free(output);
        GetNextToken(tokenOut);
        return;
    }

    if(lex_err_flag)
    {
        free(output);
        TS_COLLAPSE(&ts);
        RaiseError(99);
        exit(99);
    }

    if(str2write)
    {
        free(result.data.str);
        result.data.str = malloc(strlen(output)*sizeof(char));
        strcpy(result.data.str, output);
    }

    free(output);

    tokenOut->type = result.type;
    tokenOut->data.kw = result.data.kw;
    tokenOut->data.msc = result.data.msc;
    tokenOut->data.oper = result.data.oper;
    tokenOut->data.str = result.data.str;
    tokenOut->data.type = result.data.type;
    tokenOut->data._double = result.data._double;
    tokenOut->data._integer = result.data._integer;
    
    return;
}

void first_perimeter(char *state, char **output, unsigned *output_length)
{
    char input_c;
    if (!strcmp(state, "id"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= 'A') && (input_c <= 'Z')) || ((input_c >= 'a') && (input_c <= 'z')) || ((input_c >= '0') && (input_c <= '9')) || (input_c == '_'))
        {
            (*output)[(*output_length)++] = input_c;

            if(!(*output_length % (unsigned) STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));
                int size = (((*output_length) / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char);

                (*output) = realloc((*output), size);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;  
                }
                else strcpy((*output), tmp);
            }

            input_c = fgetc(stdin);
        }

        ungetc(input_c, stdin);

        (*output)[*output_length] = '\0';

        int tmp = Is_Keyword(output);
        if((tmp >= 0) && (tmp != 8) && (tmp != 13))
        {
            result.type = _keyword;
            result.data.type = _string;
            result.data.kw = tmp;
        }
        else
        {
            result.type = _identifier;
            result.data.type = _string;

            str2write = 1;
        }
         
        return;
    }

    if (!strcmp(state, "num"))
    {
        input_c = fgetc(stdin);
        while((input_c >= '0') && (input_c <= '9'))
        {
            (*output)[(*output_length)++] = input_c;
            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;
                }
                else strcpy((*output), tmp);
            }
            input_c = fgetc(stdin);
        }

        

        if((input_c == 'e') || (input_c == 'E'))
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "int_exp");
            second_perimeter(state, output, output_length);
            return;
        }
        else if(input_c == '.')
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "float");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            (*output)[*output_length] = '\0';

            result.type = _const;
            result.data.type = _integer;
            result.data._integer = atoi((*output));

            return;
        }
    }

    if (!strcmp(state, "dot"))
    {
        input_c = fgetc(stdin);

        if(input_c == '.')
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "conc");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            return;
        }
    }

    if (!strcmp(state, "add"))
    {
        result.type = _operator;
        result.data.oper = _add;
        return;
    }

    if (!strcmp(state, "mult"))
    {
        result.type = _operator;
        result.data.oper = _mul;
        return;
    }

    if (!strcmp(state, "div"))
    {
        input_c = fgetc(stdin);

        if(input_c == '/')
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "fdiv");
            second_perimeter(state, output, output_length);
            return;
        }

        else
        {
            ungetc(input_c, stdin);
            
            result.type = _operator;
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

            result.type = _operator;
            result.data.oper = _sub;
            return;
        }
    }

    if (!strcmp(state, "assign"))
    {
        input_c = fgetc(stdin);
        if(input_c == '=')
        {
            (*output)[(*output_length)++] = input_c;
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
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "leq");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            result.type = _operator;
            result.data.oper = _less;
            return;
        }
    }

    if (!strcmp(state, "great"))
    {
        input_c = fgetc(stdin);

        if(input_c == '=')
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "geq");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            result.type = _operator;
            result.data.oper = _great;
            return;
        }
    }
    
    if (!strcmp(state, "not"))
    {
        input_c = fgetc(stdin);

        if(input_c == '=')
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "neq");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            return;
        }
    }
   
    if (!strcmp(state, "len"))
    {
        result.type = _operator;
        result.data.oper = _length;
        return;
    }

    if (!strcmp(state, "start_str"))
    {   
        input_c = fgetc(stdin);
       
        while(input_c != '"')
        {
            
            if(input_c < 32)
            {
                if(input_c == '\n' || input_c == EOF) 
                {
                    line_cnt++;
                    lex_err_flag = INVALID_TOKEN_CONST_CODE;
                    return;
                }
                lex_err_flag = INVALID_TOKEN_CONST_CODE;
                return;
            }

            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;
                }
                else strcpy((*output), tmp);
            }

            if(input_c == '\\')
            {
                (*output)[(*output_length)++] = input_c;
                strcpy(state,"esc_seq");
                second_perimeter(state, output, output_length);
            }
            else (*output)[(*output_length)++] = input_c;

            input_c = fgetc(stdin);
           
        }

        strcpy(state, "end_str");
        second_perimeter(state, output, output_length);
        return;
    }
   
    if (!strcmp(state, "bracket_L"))
    {
        result.type = _misc;
        result.data.msc = _bracketL;
        return;
    }

    if (!strcmp(state, "bracket_R"))
    {
        result.type = _misc;
        result.data.msc = _bracketR;
        return;
    }

    if (!strcmp(state, "doubleKomma"))
    {
        result.type = _misc;
        result.data.msc = _doubleKomma;
        return;
    }

    if (!strcmp(state, "komma"))
    {
        result.type = _misc;
        result.data.msc = _komma;
        return;
    }
}

void second_perimeter(char *state, char **output, unsigned *output_length)
{
    char input_c;

    if (!strcmp(state, "float"))
    {
        input_c = fgetc(stdin);

        while((input_c >= '0') && (input_c <= '9'))
        {
            (*output)[(*output_length)++] = input_c;

            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;
                }
                else strcpy((*output), tmp);
            }

            input_c = fgetc(stdin);
        }

        if((input_c == 'e') || (input_c == 'E'))
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "float_exp");
            other_states(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            result.type = _const;
            result.data.type = _number;
            result.data._double = atof((*output));
            return;
        }
    }

    if (!strcmp(state, "int_exp"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= '0') && (input_c <= '9')) || (input_c == '+') || (input_c == '-'))
        {
            (*output)[(*output_length)++] = input_c;
            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;
                }
                else strcpy((*output), tmp);
            }

            input_c = fgetc(stdin);
        }

        ungetc(input_c, stdin);
        result.type = _const;
        result.data.type = _number;
        result.data._double = atof((*output));
        return;
    }

    if (!strcmp(state, "conc"))
    {
        result.type = _operator;
        result.data.oper = _konk;
        return;
    }

    if (!strcmp(state, "fdiv"))
    {
        result.type = _operator;
        result.data.oper = _div2;
        return;
    }

    if (!strcmp(state, "ln_cmt"))
    {
        input_c = fgetc(stdin);
        
        while((input_c != '\n') && (input_c != '[') && (input_c != EOF))
        {
            input_c = fgetc(stdin);
        }

        if(input_c == EOF)
        {
            result.data.msc = _EOF;
            result.type = _misc;
            strcpy(state, "start");
            return;
        }

        if(input_c == '\n') 
        {
            line_cnt++;
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
            else 
            {
                return;
            }
        }
    }

    if (!strcmp(state, "eq"))
    {
        result.type = _operator;
        result.data.oper = _Eq;
        return;
    }

    if (!strcmp(state, "leq"))
    {
        result.type = _operator;
        result.data.oper = _lessEq;
        return;
    }

    if (!strcmp(state, "geq"))
    {
        result.type = _operator;
        result.data.oper = _greatEq;
        return;
    }

    if (!strcmp(state, "neq"))
    {
        result.type = _operator;
        result.data.oper = _nEq;
        return;
    }

    if (!strcmp(state, "end_str"))
    {
        if(lex_err_flag == INVALID_TOKEN_CONST_CODE)
        {
            return;
        }
        else
        {
            (*output)[*output_length] = '\0';
            result.type = _const;
            result.data.type = _string;

            str2write = 1;
            return;
        }
    }

    if (!strcmp(state, "esc_seq"))
    {
        input_c = fgetc(stdin);

        switch(input_c)
        {   
            case 'n':
            {
                (*output)[(*output_length)-1] = 10;
                strcpy(state, "start_str");
                return;
            }

            case 't':
            {
                (*output)[(*output_length)-1] = '\t';
                strcpy(state, "start_str");
                return;
            }

            case '"':
            {
                (*output)[(*output_length)-1] = input_c;
                strcpy(state, "start_str");
                return;
            }

            case '\\':
            {
                strcpy(state, "start_str");
                return;
            }

            case '0' ... '2':
            {
                ungetc(input_c, stdin);
                strcpy(state, "esc_num");
                char tmp = esc_num(state);
                (*output)[(*output_length)-1] = tmp;
                strcpy(state, "start_str");

                return;
            }

            default:
            {
                lex_err_flag = INVALID_TOKEN_CONST_CODE;
                ungetc(input_c, stdin);
                strcpy(state, "start_str");
                return;
            }
        }
    }
}

void other_states(char *state, char **output, unsigned *output_length)
{
    char input_c;

    if (!strcmp(state, "float_exp"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= '0') && (input_c <= '9')) || (input_c == '+') || (input_c == '-'))
        {
            (*output)[(*output_length)++] = input_c;

            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;
                }
                else strcpy((*output), tmp);
            }

            input_c = fgetc(stdin);
        }

        ungetc(input_c, stdin);
        result.type = _const;
        result.data.type = _number;
        result.data._double = atof((*output));
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

    while(input_c != ']' && input_c != EOF)
    {
        if(input_c == '\n') line_cnt++;
        input_c = fgetc(stdin);
    }
          
    if(input_c == ']')
    {
        strcpy(state, "right_bracket");
        if(!right_bracket(state))
        {
            bl_cmt(state);
        }
        else 
        {
            return;
        }
    }
    else
    {
        lex_err_flag = UNEND_COMMENT;
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

int Is_Keyword(char **output)
{
    KeyArrInit();

    int i = 0;
    while((i < ARR_LEN) && (strcmp((*output), arr_keywords[i])))
    {

        i++;
    }
    
    if(i == ARR_LEN-1) return -1;
    if (!strcmp((*output), arr_keywords[i])) return i;

    return -2;
}

char esc_num(char *state)
{
    char input_c = fgetc(stdin);
    char output_c[3];
    unsigned int output_len = 0;
    
    switch(input_c)
    {
        case '0': 
        {
            output_c[output_len++] = input_c;
            strcpy(state, "zero");
            zero(output_c, &output_len, state);
            break;
        }

        case '1':
        {
            output_c[output_len++] = input_c;
            strcpy(state, "one");
            one(output_c, &output_len, state);
            break;
        }

        case '2':
        {
            output_c[output_len++] = input_c;
            strcpy(state, "two");
            two(output_c, &output_len, state);
            break;
        }

        default:
        {
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            strcpy(state, "esc_seq");
            return input_c;
        }
    }

    int tmp = atoi(output_c);
    char result = tmp;
    strcpy(state, "esc_seq");
    return result;
}

void zero(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);

    switch(input_c)
    {
        case '0':
        {
            output_c[(*output_len)++] = input_c;
            strcpy(state, "zero_zero");
            zero_zero(output_c, output_len, state);
            return;   
        }

        case '1' ... '9':
        {
            output_c[(*output_len)++] = input_c;
            strcpy(state, "others");
            others(output_c, output_len, state);
            return;   
        }

        default:
        {
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            strcpy(state, "esc_num");
            ungetc(input_c, stdin);
            return;
        }
    }
}

void one(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);
    if(input_c >= '0' && input_c <= '9')
    {
        output_c[(*output_len)++] = input_c;
        strcpy(state, "others");
        others(output_c, output_len, state);
        return;
    }
    else
    {
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c,stdin);
        strcpy(state, "esc_num");
        return;
    }
}

void two(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);
    switch(input_c)
    {
        case '0' ... '4':
        {
            output_c[(*output_len)++] = input_c;
            strcpy(state, "others");
            others(output_c, output_len, state);
            return;
        }

        case '5':
        {
            output_c[(*output_len)++] = input_c;
            strcpy(state, "two_five");
            two_five(output_c, output_len, state);
            return;
        }

        default:
        {
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            ungetc(input_c, stdin);
            return;
        }
    }
}

void zero_zero(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);
    if(input_c >= '1' && input_c <= '9')
    {
        output_c[*output_len] = input_c;
        strcpy(state, "esc_num");
        return;
    }
    else
    {
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c, stdin);
        strcpy(state, "esc_num");
        return;
    }
}

void two_five(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);
    if(input_c >= '0' && input_c <= '5')
    {
        output_c[*output_len] = input_c;
        strcpy(state, "esc_num");
        return;
    }
    else
    {
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c, stdin);
        strcpy(state, "esc_num");
        return;
    }
}

void others(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);
    if(input_c >= '0' && input_c <= '9')
    {
        output_c[*output_len] = input_c;
        strcpy(state, "esc_num");
        return;
    }
    else
    {
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c, stdin);
        strcpy(state, "esc_num");
        return;
    }
}

int GetLineNumber()
{
    return line_cnt;
}

void KeyArrInit()
{
    strcpy(arr_keywords[0],"do");
    strcpy(arr_keywords[1],"else");
    strcpy(arr_keywords[2],"end");
    strcpy(arr_keywords[3],"function");
    strcpy(arr_keywords[4],"global");
    strcpy(arr_keywords[5],"if");
    strcpy(arr_keywords[6],"local");
    strcpy(arr_keywords[7],"nil");
    strcpy(arr_keywords[8],"read");
    strcpy(arr_keywords[9],"require");
    strcpy(arr_keywords[10],"return");
    strcpy(arr_keywords[11],"then");
    strcpy(arr_keywords[12],"while");
    strcpy(arr_keywords[13],"write");
    strcpy(arr_keywords[14],"integer");
    strcpy(arr_keywords[15],"number");
    strcpy(arr_keywords[16],"string");            
}
