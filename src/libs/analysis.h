#include<stdio.h>
#include<stdlib.h>

#define STR_SIZE 10

#define ARR_LEN 15
char arr_keywords[15][10] = 
{
    "do",
    "else",
    "end",
    "function",
    "global",
    "if",
    "integer",
    "local",
    "nil",
    "number",
    "require",
    "return",
    "string",
    "then",
    "while"             
};

int line_cnt = 0;
token result;

token get_next_token( );
void first_perimeter( char * , char * , unsigned * );
void second_perimeter( char * , char * , unsigned * );
void other_states( char * , char * , unsigned * );

int Is_Keyword(char *);
int get_line_number();

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