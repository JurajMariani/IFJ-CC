#include "Defs.h"

/**
 * Implementace překladače imperativního jazyka IFJ21
 *
 * @file err_one_func.c
 * @author Lukas Macejka < xmacej03 xmacej03@fit.vutbr.cz >
 * @brief Error Message Print
 * @version 1.0
 * @date 2021-12-08
 * 
 * 
 */

void RaiseError(int err_flag)
{
    fprintf(stderr,"%d ",GetLineNumber());
    if (err_flag == 1)
    {
        fprintf(stderr,"LEXICAL_ANALYSIS_ERR1\n");
    }

    else if (err_flag == 2)
    {
        fprintf(stderr,"SYNTAX_ANALYSIS_ERR2\n");
    }


    else if ( err_flag == 3)
    {
        fprintf(stderr,"SEMANTIC_PROGRAM_UNDEF_ERR3\n");
    }

    else if ( err_flag == 4)
    {
        fprintf(stderr,"ASSIGN_ERR4\n");
    }

    else if ( err_flag == 5)
    {
        fprintf(stderr,"SEMANTIC_WRONG_NUMBER_OF_PARAMS_ERR5\n");
    }

    else if ( err_flag == 6)
    {
        fprintf(stderr,"SEMANTIC_TYPE_COMPATIBILITY_ERR6\n");
    }

    else if ( err_flag == 7)
    {
        fprintf(stderr,"OTHER_SEMANTIC__ERR7\n");
    }

    else if ( err_flag == 8)
    {
        fprintf(stderr,"UNEXPECTED_NIL_ERR8\n");
    }

    else if ( err_flag == 9)
    {
        fprintf(stderr,"DIVIDE_BY_NULL_ERR9\n");
    }

    else if ( err_flag == 99)
    {
        fprintf(stderr,"INTERNAL_ERR99\n");
    }

//TODO EXIT HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
/*
1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
• 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu).
• 3 - sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefi-
nici proměnné, atp.
• 4 - sémantická chyba v příkazu přiřazení (typová nekompatibilita).
• 5 - sémantická chyba v programu – špatný počet/typ parametrů či návratových hodnot
u volání funkce či návratu z funkce.
• 6 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních
výrazech.
• 7 - ostatní sémantické chyby.
• 8 - běhová chyba při práci s neočekávanou hodnotou nil.
• 9 - běhová chyba celočíselného dělení nulovou konstantou.
• 99 - interní chyba překladač */
