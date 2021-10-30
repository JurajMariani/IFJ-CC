//#TODO

/**
 * 	CONSTANTS
 */

#define MALLOC_ERR_CODE -1;

/**
 * 	ENUMS
 */
typedef enum {_integer,_number,_string,_nan}dataType;
typedef enum {_identifier,_keyword,_const,_operand,_misc}tokenType;
typedef enum {_bracketL,_bracketR,_komma,_doubleKomma,_assign} misc; //No We wont name it ass
typedef enum {_length,_add,_sub,_mul,_div,_div2,_konk,_less,_lessEq,_great,_greatEq,_Eq,_nEq} operator;
typedef enum {_do,_else,_end,_function,_global,_if,_local,_nil,_read,_require,_return,_then,_while,_write,_k_integer,_k_number,_k_string}keyword;

/*
	STRUCTS
*/

//Ignore this for now
// Variable -- not token stuff
typedef struct variable{
	dataType type;
	int vDefined;
}variable;

typedef struct user_func{
	int vDefined;
	dataType *params;
	dataType *returns;
}user_func;

//TokenStuff
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
