#include "../libs/Defs.h"

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
		default:printf(" Invalid ");break;
		}
	}else 
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
		default:printf(" Invalid ");break;
		}
	}else 
	if (block->blockType==_operand_expr){
		if(block->operType==_constant_oper){
			if(block->dt==_number){printf(" CfloatC ");}
			if(block->dt==_integer){printf(" CintegerC ");}
			if(block->dt==_string){printf(" CstringC ");}
			if(block->dt==_nan)printf(" nil ");
		}else
		if (block->operType==_variable_oper){
			if(block->dt==_number)printf(" Vfloat %sV ",block->str);
			if(block->dt==_integer)printf(" Vinteger %sV ",block->str);
			if(block->dt==_string)printf(" Vstring %sV  ",block->str);
		}else
		if(block->operType==_not_terminal_oper){
			if(block->dt==_integer)printf(" <integer %d %f> ",block->_integer, block->_double);
			if(block->dt==_string)printf(" <string %d %f> ", block->_integer, block->_double);
			if(block->dt==_number)printf(" <number %d %f> ", block->_integer, block->_double);
			if(block->dt==_bool)printf(" <bool %d %f> ", block->_integer, block->_double);
			if(block->dt==_nan)printf(" <nil %d %f> ", block->_integer, block->_double);
		}
		else printf(" Invalid ");
	}
	else printf(" Invalid ");
	fflush(stdout);
}

void DebbugPrintStack(BubbleStack_t *stack){
	for (int i=0;i<=stack->BS_TopIndex;i++){
		DebbugPrintExpress((stack->BS_Element)[i]);
	}
	printf("\n");
}