#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define STR_SIZE 10

#define ARR_LEN 17
char arr_keywords[ARR_LEN][10] = 
{
    "do",
    "else",
    "end",
    "function",
    "global",
    "if",
    "local",
    "nil",
    "read",
    "require",
    "return",
    "then",
    "while",
    "write",
    "integer",
    "number",
    "string"            
};

int line_cnt = 1;
token result;

void get_next_token( token *);
void first_perimeter( char * , char ** , unsigned * );
void second_perimeter( char * , char ** , unsigned * );
void other_states( char * , char ** , unsigned * );

int Is_Keyword(char **);

int left_bracket(char *);
int right_bracket(char *);
void bl_cmt(char *);

char esc_num(void);
void zero(char *, unsigned *);
void one(char *, unsigned *);
void two(char *, unsigned *);
void zero_zero(char *, unsigned *);
void two_five(char *, unsigned *);
void others(char *, unsigned *);
