//#TODO JURO Dorobi IFNDEF, a vsetky ERR KODY
//Juro to nedorobil

#ifndef Defs
#define Defs
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Global flag, when an error occures, this variable will be set to a non-zero value
 */
int stack_err_flag = 0;
int lex_err_flag = 0;

/**
 * 	CONSTANTS
 */

#define MALLOC_ERR_CODE -1
#define REDECLARATION_OF_FUNCTION 1
#define INVALID_TOKEN 2
#define UNKNOWN_IDENTIF 3
#define INVALID_EXPRESSION 4
#define FLOAT_INT_COMPARISM 5
#define UNEND_COMMENT -2

/**
 * 	ENUMS
 */
typedef enum {_integer,_number,_string,_bool,_nan}dataType;
typedef enum {_identifier,_keyword,_const,_operand,_misc,_operator}tokenType;
typedef enum {_bracketL,_bracketR,_komma,_doubleKomma,_assign,_EOF} misc; //No We wont name it ass
typedef enum {_length,_add,_sub,_mul,_div,_div2,_konk,_less,_lessEq,_great,_greatEq,_Eq,_nEq,_func} operator;
typedef enum {_do,_else,_end,_function,_global,_if,_local,_nil,_read,_require,_return,_then,_while,_write,_k_integer,_k_number,_k_string}keyword;

typedef enum {_operand_expr,_operator_expr,_misc_expr}exprBlockType;
typedef enum {_variable_oper,_constant_oper, _not_terminal_oper}operandType;
typedef enum {_lbr, _rbr,_bgnMark,_endMark}exprMisc;

typedef enum {_while_begin,_while_end,_if_begin,_if_else,_if_end,_assign_expression,_assign_variable}codeBlockTypes;
/*
	STRUCTS
*/

/**
 * These sctructs are used to save data in TS
 */
typedef struct variable{
	dataType type;
	int vDefined;
}variable;

typedef struct user_func{
	int vDefined;
	dataType *params;
	dataType *returns;
}user_func;

typedef enum {w_var,w_func}treeElementType;

/**
 * This struct is one element in TS
 */
typedef struct TreeElement{
	char *name;
	treeElementType type;
	void *data;
	struct TreeElement *left;
	struct TreeElement *right;
}TreeElement;

typedef struct{
	TreeElement *root;	
}Tree;


//TreeSupprot -- Tabulka Symbolov

typedef struct TreeLayer{
	struct TreeLayer *prevLayer;
	Tree *tree;
}TreeLayer;

typedef struct TreeSupport{
	TreeLayer *curLayer;
	TreeLayer *functionLayer;
	TreeLayer *bounceLayer;
}TreeSupport;





/**
 * This is a block in an expression
 */
typedef struct expression_block{
	exprBlockType blockType;
	exprMisc em;
	operandType operType;
	dataType dt;
	operator oper;
	TreeElement *TSPointer;
	char *str;
	double _double;
	int _integer;
}expression_block;

/**
 * This is a stack of expression blocks
 */
typedef struct bstack{

    expression_block* BS_Element;

    int BS_TopIndex;

}BubbleStack_t;

/**
 * This is a code block, sent to generate code
 */
typedef struct code_block{
	codeBlockTypes type;
	BubbleStack_t* exprStack;
}code_block;


/**
 * These are tokens
 */
typedef struct tokenData{
	char *str;               //ALWAYS COPY THE STRING BY VALUE
	double _double;
	int _integer;
	keyword kw;
	misc msc;
	operator oper;
	dataType type;
}tokenData;

typedef struct token{
	tokenType type;
	tokenData data;
}token;

//Delete this later
int GetNextToken(token*);
#define NEXT GetNextToken(nextToken)

void DebbugPrintToken(token *nextToken){
	if (nextToken->type==_operator){
		switch (nextToken->data.oper)
		{
		case _less: printf(" < ");break;
		case _lessEq: printf(" <= ");break;
		case _great: printf(" > ");break;
		case _greatEq: printf(" >= ");break;
		case _Eq: printf(" == ");break;
		case _nEq: printf(" != ");break;
		case _add: printf(" + ");break;
		case _sub: printf(" - ");break;
		case _div: printf(" / ");break;
		case _div2: printf(" // ");break;
		case _mul: printf(" * ");break;
		case _length: printf(" # ");break;
		case _konk: printf(" .. ");break;
		default:break;
		}
	}else if(nextToken->type==_misc){
		switch (nextToken->data.msc)
		{
		case _bracketL: printf(" ( "); break;
		case _bracketR: printf(" ) ");break;
		case _EOF: printf(" EOF ");break;
		case _assign: printf(" = ");break;
		case _komma: printf(" , ");break;
		case _doubleKomma: printf(" : ");break;
		default:
			break;
		}
	}else if(nextToken->type==_identifier){
		printf(" %s ",nextToken->data.str);
	}else if(nextToken->type==_const){
		if (nextToken->data.type==_string) printf(" \"%s\" ",nextToken->data.str);
		if (nextToken->data.type==_integer) printf(" %d ",nextToken->data._integer);
		if (nextToken->data.type==_number) printf(" %f ",nextToken->data._double);
	}else if(nextToken->type==_keyword){
		switch (nextToken->data.kw)
		{
		case _if:printf (" if ");break;
		case _end:printf(" end ");break;
		case _while: printf(" while ");break;
		case _function: printf(" function ");break;
		case _require: printf(" require ");break;
		case _return: printf (" return ");break;
		case _global: printf (" global ");break;
		case _do : printf(" do ");break;
		case _local: printf(" local ");break;
		case _else: printf(" else ");break;
		case _k_integer: printf(" integer ");break;
		case _k_number: printf(" number ");break;
		case _k_string: printf(" string ");break;
		case _nil: printf(" nil ");break;
		case _then: printf(" then ");break;
		default:break;
		}
	}
}

void DebbugPrintExpress(expression_block *block){
	if (block->blockType==_misc_expr){
		switch (block->em)
		{
		case _lbr: printf(" ( "); break;
		case _rbr: printf(" ) ");break;
		case _bgnMark: printf(" M< ");break;
		case _endMark: printf(" $ ");break;
		default:break;
		}
	}
	if (block->blockType==_operator_expr){
		switch (block->oper)
		{
		case _less: printf(" < ");break;
		case _lessEq: printf(" <= ");break;
		case _great: printf(" > ");break;
		case _greatEq: printf(" >= ");break;
		case _Eq: printf(" == ");break;
		case _nEq: printf(" != ");break;
		case _add: printf(" + ");break;
		case _sub: printf(" - ");break;
		case _div: printf(" / ");break;
		case _div2: printf(" // ");break;
		case _mul: printf(" * ");break;
		case _length: printf(" # ");break;
		case _konk: printf(" .. ");break;
		default:break;
		}
	}
	if (block->blockType==_operand_expr){
		if(block->operType==_constant_oper){
			if(block->dt==_number){printf(" Cfloat %fC ",block->_double);}
			if(block->dt==_integer){printf(" Cinteger %dC ",block->_integer);}
			if(block->dt==_string){printf(" Cstring %sC ",block->str);}
		}
		if (block->operType==_variable_oper){
			if(block->dt==_number)printf(" Vfloat %sV ",block->str);
			if(block->dt==_integer)printf(" Vinteger %sV ",block->str);
			if(block->dt==_string)printf(" Vstring %sV ",block->str);
		}
		if(block->operType==_not_terminal_oper){
			if(block->dt==_integer)printf(" <integer>");
			if(block->dt==_string)printf(" <string> ");
			if(block->dt==_number)printf(" <number> ");
		}
	}
}

#endif