/**
 * @file analysis.c
 * @author xhoril01 (xhoril01@stud.fit.vutbr.cz)
 * @brief Lexical analysis, reads characters from stdin(input) and returns token to parser (output)
 * @version 1.0
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */




#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libs/analysis.h"

/* @brief Main function of lexical analysis 
 *        This function starts final state machine and set state of FSM according to character
 *        loaded from standard input
 * 
 * @param tokenOut Pointer to structure token including all information about returning token
 */
void GetNextToken(token *tokenOut)
{
    if(tokenOut->type == _misc)
    {
        if(tokenOut->data.msc == _EOF)                          // In case it is already EOF, function does nothing returns
        {
            free(result.data.str);
            return;
        }
    }

    output = (char*) malloc(STR_SIZE * sizeof(char));           // Array of characters for storing token's characters
    if(output == NULL)                                          // Allocating space for 10 characters and chceking if it was successful
    {
        RaiseError(99);                                         // If malloc is not successful, internal error will be raised
        exit(99);
    }

    output_length = 0;                                          // Length of array initialisated to 0
    lex_err_flag = 0;                                           // Error flag set to 0
    str2write = 0;                                              // This flag determines if string in output variable should be 
                                                                // copied to auxillary token variable
    char state[20];
    strcpy(state, "start");
      
    char input_c = fgetc(stdin);

    // Ignoring whitespaces in stdin
    while WHITESPACES
    {
        if(input_c == '\n') line_cnt++;
        input_c = fgetc(stdin);
    } 

    // Main switch which sets next state of FSM
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
        
        case EOF:                                               // End of file -> end of switch
        {
            result.type = _misc;
            result.data.msc = _EOF;
            break;
        }

        default:                                                // Character which is not from alphabet of actual programming language,
        {                                                       // will raise a lexical error
            RaiseError(1);
            exit(1);
        }
    }
    
    if(!strcmp(state, "start"))                                 // FSM is in state start -> next token will be read from stdin
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

    if(lex_err_flag)                                            // Error flag set to non-zero value -> type of error will be raised according
    {                                                           // to number in error flag
        free(output);

        if(lex_err_flag == 2 || lex_err_flag == -2)
        {
            RaiseError(1);                                      // Lexical error
            exit(1);
        }
        else
        {
            RaiseError(99);                                     // Internal error
            exit(99);
        }
    }

    if(str2write)                                               // Flag set to non-zero value -> memory will be freed, space for string allocated
    {                                                           // and string copied to auxillary structure
        free(result.data.str);
        result.data.str = malloc(strlen(output)*sizeof(char));
        strcpy(result.data.str, output);
    }

    free(output);                                             // This free should be here but it works without it

    tokenOut->type = result.type;                               // Every part of aux structure will be copied to structure tokenOut   
    tokenOut->data.kw = result.data.kw;
    tokenOut->data.msc = result.data.msc;
    tokenOut->data.oper = result.data.oper;
    tokenOut->data.str = result.data.str;
    tokenOut->data.type = result.data.type;
    tokenOut->data._double = result.data._double;
    tokenOut->data._integer = result.data._integer;
    
    return;
}


/* @brief First perimeter of states - continue to second perimeter or returns back with token
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output Pointer to array of characters storing characters of actual token
 * @param output_length Pointer to actual length of output variable
 */

void first_perimeter(char *state, char **output, unsigned *output_length)
{
    char input_c;                                                                   // Loading character

    // SINGLE STATES OF FIRST PERIMETER IN if CONDITIONS

    if (!strcmp(state, "id"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= 'A') && (input_c <= 'Z')) || ((input_c >= 'a') && (input_c <= 'z')) || ((input_c >= '0') && (input_c <= '9')) || (input_c == '_'))
        {
            (*output)[(*output_length)++] = input_c;

            if(!(*output_length % (unsigned) STR_SIZE))                         // If size of actual array is dividable by STR_SIZE, array will be extended to another 10 bytes
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));
                int size = (((*output_length) / STR_SIZE)+1 * (STR_SIZE)) * sizeof(char);

                (*output) = realloc((*output), size + 5);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;                             // Unsuccessful realloc leads to internal error
                    return;  
                }
                else strcpy((*output), tmp);
            }

            input_c = fgetc(stdin);
        }

        // "While cyclus" ends when there's other character than letter, number or underscore

        ungetc(input_c, stdin);                                                 // This character is returned to stdin

        (*output)[*output_length] = '\0';                                       // Correctly ended string

        int tmp = IsKeyword(output);                                            // Check if identifier is not a keyword - returns index of keyword in array, else -1

        // Return string as a keyword
        if((tmp >= 0) && (tmp != 8) && (tmp != 13))
        {
            result.type = _keyword;
            result.data.type = _string;
            result.data.kw = tmp;
        }
        // Return string as an identifier
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
            // String realloc if length is dividable by STR_SIZE (10)
            (*output)[(*output_length)++] = input_c;
            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size + 5);
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
            strcpy(state, "int_exp");                               // Number in exponent form
            second_perimeter(state, output, output_length);
            return;
        }
        else if(input_c == '.')
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "float");                                 // Number in decimal form
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);
            (*output)[*output_length] = '\0';

            result.type = _const;
            result.data.type = _integer;
            result.data._integer = atoi((*output));                 // Convert string to number

            return;
        }
    }

    if (!strcmp(state, "dot"))
    {
        input_c = fgetc(stdin);

        // Change state and move to second perimeter function
        if(input_c == '.')
        {
            (*output)[(*output_length)++] = input_c;
            strcpy(state, "conc");
            second_perimeter(state, output, output_length);
            return;
        }
        else
        {
            ungetc(input_c, stdin);                     // Single dot leads to lexical error
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

        // Return either div operator or move to second perimeter

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

        // Return either sub operator or move to second perimeter

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

        // Return either assign operator or move to second perimeter

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

        // Return either "less than" operator or move to second perimeter

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

        // Return either "greater than" operator or move to second perimeter

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

        // Move to second perimeter or set error flag 

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
            
            // '\n' inside string literal leads to lexical error
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

            // String realloc if length is dividable by STR_SIZE (10)
            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size + 5);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;
                }
                else strcpy((*output), tmp);
            }

            // Start of escape sequence
            if(input_c == '\\')
            {
                (*output)[(*output_length)++] = input_c;
                strcpy(state,"esc_seq");
                second_perimeter(state, output, output_length);
            }
            else (*output)[(*output_length)++] = input_c;

            input_c = fgetc(stdin);
           
        }

        // Move to second perimeter
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

/* @brief Second perimeter of states - continue to next perimeter or returns back with token
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output Pointer to array of characters storing characters of actual token
 * @param output_length Pointer to actual length of output variable
 */

void second_perimeter(char *state, char **output, unsigned *output_length)
{
    char input_c;

    if (!strcmp(state, "float"))
    {
        input_c = fgetc(stdin);

        while((input_c >= '0') && (input_c <= '9'))
        {
            (*output)[(*output_length)++] = input_c;

            // String realloc if length is dividable by STR_SIZE (10)
            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size + 5);
                if((*output) == NULL)
                {
                    lex_err_flag = MALLOC_ERR_CODE;
                    return;
                }
                else strcpy((*output), tmp);
            }

            input_c = fgetc(stdin);
        }

        // Exponent form of decimal number

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
            result.data._double = atof((*output));              // Convert string to float number
            return;
        }
    }

    if (!strcmp(state, "int_exp"))
    {
        input_c = fgetc(stdin);

        while(((input_c >= '0') && (input_c <= '9')) || (input_c == '+') || (input_c == '-'))
        {
            (*output)[(*output_length)++] = input_c;

            // String realloc if length is dividable by STR_SIZE (10)
            if(!(*output_length % STR_SIZE)) 
            {
                char tmp[*output_length];
                strcpy(tmp, (*output));

                int size = (((*output_length / STR_SIZE)+1) * (STR_SIZE)) * sizeof(char);
                (*output) = realloc((*output), size + 5);
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
        result.data._double = atof((*output));              // Convert string to float number
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

        // '[' can be start of block commentary
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

        // Checking for allowed escape sequences
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

            case '0' ... '2':                                       // Escape sequence in form \ddd where ddd is number from 001 to 255
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


/* @brief Other states - last perimeter of states, returns nack with complete token
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output Pointer to array of characters storing characters of actual token
 * @param output_length Pointer to actual length of output variable
 */

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
                (*output) = realloc((*output), size + 5);
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
        result.data._double = atof((*output));                          // Convert string to float number
        return;
    }

}


/* @brief Left bracket can be start of block commentary
 * 
 * @param state Array of characters storin actual state of FSM
 */

int left_bracket(char* state)
{
    char input_c = fgetc(stdin);

    if(input_c == '[')
    {
        strcpy(state, "bl_cmt");
        bl_cmt(state);
        return 1;
    }
    // returns back to line commentary if character is not '['
    else
    {
        strcpy(state, "ln_cmt");
        ungetc(input_c, stdin);
        return 0;
    }
}

/* @brief Start of block commentary
 * 
 * @param state Array of characters storin actual state of FSM
 */

void bl_cmt(char* state)
{
    char input_c = fgetc(stdin);

    while(input_c != ']' && input_c != EOF)
    {
        if(input_c == '\n') line_cnt++;
        input_c = fgetc(stdin);
    }

    // ']' can lead to end of block commentary
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

/* @brief Right bracket can be end of block commentary
 * 
 * @param state Array of characters storin actual state of FSM
 */

int right_bracket(char* state)
{
    char input_c = fgetc(stdin);

    if(input_c == ']')
    {
        strcpy(state, "start");
        return 1;
    }
    // returns back to block commentary if character is not ']'
    else
    {
        strcpy(state, "bl_cmt");
        ungetc(input_c, stdin);
        return 0;
    }
}

/* @brief Check if identifier is from array of keywords
 * 
 * @param output Actual identifier (already complete token)
 */

int IsKeyword(char **output)
{
    // Initiallisation of array of keywords
    KeyArrInit();

    int i = 0;

    // Sequential search in array of keywords
    while((i < ARR_LEN) && (strcmp((*output), arr_keywords[i])))
    {
        i++;
    }
    
    if(i == ARR_LEN) return -1;                                                 // Returns -1 if string is not a keyword
    if (!strcmp((*output), arr_keywords[i])) return i;                          // Returns index of keyword to calling function

    return -2;
}

/* @brief Escpae sequence in \ddd form (ASCII code) - function returns character accroding to its ASCII code 
 * 
 * @param state Array of characters storing actual state of FSM
 */

char esc_num(char *state)
{
    char input_c = fgetc(stdin);
    char output_c[3];
    unsigned int output_len = 0;
    
    // Dividing according to fisrt number - it can be only 0, 1 or 2
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
            // Any other character than sets error flag
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            strcpy(state, "esc_seq");
            return input_c;
        }
    }

    int tmp = atoi(output_c);                   // Converts string to integer
    char result = tmp;                          // Sets result as ASCII number of character
    strcpy(state, "esc_seq");
    return result;
}

/* @brief Number escape sequence starts with 0
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output_c Array of characters storing characters of escape sequence
 * @param output_len Pointer to actual length of output variable
 */

void zero(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);

    // Changing states according to next number
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
            // Any other character than sets error flag
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            strcpy(state, "esc_num");
            ungetc(input_c, stdin);
            return;
        }
    }
}

/* @brief Number escape sequence starts with 1
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output_c Array of characters storing characters of escape sequence
 * @param output_len Pointer to actual length of output variable
 */

void one(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);

    // Next number can be any number from 0 to 9
    if(input_c >= '0' && input_c <= '9')
    {
        output_c[(*output_len)++] = input_c;
        strcpy(state, "others");
        others(output_c, output_len, state);
        return;
    }
    else
    {
        // Any other character than sets error flag
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c,stdin);
        strcpy(state, "esc_num");
        return;
    }
}

/* @brief Number escape sequence starts with 2
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output_c Array of characters storing characters of escape sequence
 * @param output_len Pointer to actual length of output variable
 */

void two(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);

    // Changing states according to next number
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
            // Any other character than sets error flag
            lex_err_flag = INVALID_TOKEN_CONST_CODE;
            ungetc(input_c, stdin);
            return;
        }
    }
}


/* @brief Number escape sequence starts with 00
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output_c Array of characters storing characters of escape sequence
 * @param output_len Pointer to actual length of output variable
 */

void zero_zero(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);

    // Last number can be just from 1 to 9
    if(input_c >= '1' && input_c <= '9')
    {
        output_c[*output_len] = input_c;
        strcpy(state, "esc_num");
        return;
    }
    else
    {
        // Any other character than sets error flag
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c, stdin);
        strcpy(state, "esc_num");
        return;
    }
}

/* @brief Number escape sequence starts with 25
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output_c Array of characters storing characters of escape sequence
 * @param output_len Pointer to actual length of output variable
 */

void two_five(char *output_c, unsigned *output_len, char *state)
{
    char input_c = fgetc(stdin);

    // Last number can be just from 0 to 5
    if(input_c >= '0' && input_c <= '5')
    {
        output_c[*output_len] = input_c;
        strcpy(state, "esc_num");
        return;
    }
    else
    {
        // Any other character than sets error flag
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c, stdin);
        strcpy(state, "esc_num");
        return;
    }
}

/* @brief Last number can be any number from 0 to 9
 * 
 * @param state Array of characters storin actual state of FSM
 * @param output_c Array of characters storing characters of escape sequence
 * @param output_len Pointer to actual length of output variable
 */

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
        // Any other character than sets error flag
        lex_err_flag = INVALID_TOKEN_CONST_CODE;
        ungetc(input_c, stdin);
        strcpy(state, "esc_num");
        return;
    }
}

/* @brief Returns actual line number in stdin
 * 
 */

int GetLineNumber()
{
    return line_cnt;
}

/* @brief Array of keywords
 * 
 */

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


/**      ----------------       END OF ANALYSIS.C      -------------------     **/