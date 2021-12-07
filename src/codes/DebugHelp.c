#include "../libs/Defs.h"

void DebbugPrintToken(token *nextToken){
	if (nextToken->type==_operator){
		switch (nextToken->data.oper)
		{
		case _less: fprintf(stderr," < ");break;
		case _lessEq: fprintf(stderr," <= ");break;
		case _great: fprintf(stderr," > ");break;
		case _greatEq: fprintf(stderr," >= ");break;
		case _Eq: fprintf(stderr," == ");break;
		case _nEq: fprintf(stderr," != ");break;
		case _add: fprintf(stderr," + ");break;
		case _sub: fprintf(stderr," - ");break;
		case _div: fprintf(stderr," / ");break;
		case _div2: fprintf(stderr," // ");break;
		case _mul: fprintf(stderr," * ");break;
		case _length: fprintf(stderr," # ");break;
		case _konk: fprintf(stderr," .. ");break;
		default:break;
		}
	}else if(nextToken->type==_misc){
		switch (nextToken->data.msc)
		{
		case _bracketL: fprintf(stderr," ( "); break;
		case _bracketR: fprintf(stderr," ) ");break;
		case _EOF: fprintf(stderr," EOF ");break;
		case _assign: fprintf(stderr," = ");break;
		case _komma: fprintf(stderr," , ");break;
		case _doubleKomma: fprintf(stderr," : ");break;
		default:
			break;
		}
	}else if(nextToken->type==_identifier){
		fprintf(stderr," %s ",nextToken->data.str);
	}else if(nextToken->type==_const){
		if (nextToken->data.type==_string) fprintf(stderr," \"%s\" ",nextToken->data.str);
		if (nextToken->data.type==_integer) fprintf(stderr," %d ",nextToken->data._integer);
		if (nextToken->data.type==_number) fprintf(stderr," %f ",nextToken->data._double);
	}else if(nextToken->type==_keyword){
		switch (nextToken->data.kw)
		{
		case _if:fprintf (stderr," if ");break;
		case _end:fprintf(stderr," end ");break;
		case _while: fprintf(stderr," while ");break;
		case _function: fprintf(stderr," function ");break;
		case _require: fprintf(stderr," require ");break;
		case _return: fprintf (stderr," return ");break;
		case _global: fprintf (stderr," global ");break;
		case _do : fprintf(stderr," do ");break;
		case _local: fprintf(stderr," local ");break;
		case _else: fprintf(stderr," else ");break;
		case _k_integer: fprintf(stderr," integer ");break;
		case _k_number: fprintf(stderr," number ");break;
		case _k_string: fprintf(stderr," string ");break;
		case _nil: fprintf(stderr," nil ");break;
		case _then: fprintf(stderr," then ");break;
		default:break;
		}
	}
	fflush(stderr);
}

void DebbugPrintExpress(expression_block *block){
	if (block->blockType==_misc_expr){
		switch (block->em)
		{
		case _lbr: fprintf(stderr," ( "); break;
		case _rbr: fprintf(stderr," ) ");break;
		case _bgnMark: fprintf(stderr," M< ");break;
		case _endMark: fprintf(stderr," $ ");break;
		default:fprintf(stderr," Invalid ");break;
		}
	}else 
	if (block->blockType==_operator_expr){
		switch (block->oper)
		{
		case _less: fprintf(stderr," < ");break;
		case _lessEq: fprintf(stderr," <= ");break;
		case _great: fprintf(stderr," > ");break;
		case _greatEq: fprintf(stderr," >= ");break;
		case _Eq: fprintf(stderr," == ");break;
		case _nEq: fprintf(stderr," != ");break;
		case _add: fprintf(stderr," + ");break;
		case _sub: fprintf(stderr," - ");break;
		case _div: fprintf(stderr," / ");break;
		case _div2: fprintf(stderr," // ");break;
		case _mul: fprintf(stderr," * ");break;
		case _length: fprintf(stderr," # ");break;
		case _konk: fprintf(stderr," .. ");break;
		default:fprintf(stderr," Invalid ");break;
		}
	}else 
	if (block->blockType==_operand_expr){
		if(block->operType==_constant_oper){
			if(block->dt==_number){fprintf(stderr," CfloatC ");}
			if(block->dt==_integer){fprintf(stderr," CintegerC ");}
			if(block->dt==_string){fprintf(stderr," CstringC ");}
			if(block->dt==_nan)fprintf(stderr," nil ");
		}else
		if (block->operType==_variable_oper){
			if(block->dt==_number)fprintf(stderr," Vfloat %sV ",block->str);
			if(block->dt==_integer)fprintf(stderr," Vinteger %sV ",block->str);
			if(block->dt==_string)fprintf(stderr," Vstring %sV  ",block->str);
		}else
		if(block->operType==_not_terminal_oper){
			if(block->dt==_integer)fprintf(stderr," <integer %d %f> ",block->_integer, block->_double);
			if(block->dt==_string)fprintf(stderr," <string %d %f> ", block->_integer, block->_double);
			if(block->dt==_number)fprintf(stderr," <number %d %f> ", block->_integer, block->_double);
			if(block->dt==_bool)fprintf(stderr," <bool %d %f> ", block->_integer, block->_double);
			if(block->dt==_nan)fprintf(stderr," <nil %d %f> ", block->_integer, block->_double);
		}
		else printf(" Invalid ");
	}
	else printf(" Invalid ");
	fflush(stdout);
}

void DebbugPrintStack(BubbleStack_t *stack){
	fprintf(stderr,"Line %d: Stack bgn",GetLineNumber());
	for (int i=0;i<=stack->BS_TopIndex;i++){
		DebbugPrintExpress((stack->BS_Element)[i]);
		fprintf(stderr,"\n");
	}
	fprintf(stderr,"Stack end\n");
	fflush(stderr);
}