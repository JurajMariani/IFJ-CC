
#ifndef __DefsH__
#define __DefsH__
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Global flag, when an error occures, this variable will be set to a non-zero value
 */
int stack_err_flag;
int lex_err_flag;

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

    expression_block** BS_Element;

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
void GetNextToken(token*);
int GetLineNumber();

#define NEXT GetNextToken(nextToken)

//Debug functions
void DebbugPrintToken(token *nextToken);

void DebbugPrintExpress(expression_block *block);

#endif