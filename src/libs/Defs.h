//#TODO

/**
 * 	CONSTANTS
 */

#define MALLOC_ERR_CODE -1;

/**
 * 	ENUMS
 */
typedef enum {_bool,_int,_float,_string}dataType;
typedef enum {_identifier,_keyword,_integer,_decimal}tokenType;
typedef enum {_if,_else,_end}keywords;

/*
	STRUCTS
*/

//var
//name ? *name

typedef struct variable{
	char name[100];
	dataType type;
}variable;

//TokenStuff
//var ? *var

typedef struct tokenData{
	variable *var;
	char *str;
	double _double;
	int _integer;
	keyword kw;
}tokenData;

typedef struct token{
	tokenType type;
	tokenData data;
}token;
