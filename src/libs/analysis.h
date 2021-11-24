#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "Defs.h"

#define STR_SIZE 10

#define ARR_LEN 17
char arr_keywords[ARR_LEN][10];

int line_cnt;
token result;

void key_arr_init();
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
