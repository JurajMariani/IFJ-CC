#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#define STR_SIZE 10
#define WHITESPACES ((input_c == 32 || input_c == '\t' || input_c == '\r' || input_c == '\n' || input_c == '\v' || input_c == '\f'))

#define ARR_LEN 17
char arr_keywords[ARR_LEN][10];

char *output;
unsigned output_length;
int lex_err_flag;
int line_cnt;
token result;
int str2write;

void first_perimeter( char * , char ** , unsigned * );
void second_perimeter( char * , char ** , unsigned * );
void other_states( char * , char ** , unsigned * );

int IsKeyword(char **);

int left_bracket(char *);
int right_bracket(char *);
void bl_cmt(char *);

char esc_num(char *);
void zero(char *, unsigned *, char *);
void one(char *, unsigned *, char *);
void two(char *, unsigned *, char *);
void zero_zero(char *, unsigned *, char *);
void two_five(char *, unsigned *, char *);
void others(char *, unsigned *, char *);

void KeyArrInit();

#endif
