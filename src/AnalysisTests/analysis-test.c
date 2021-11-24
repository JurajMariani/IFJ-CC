#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../libs/Defs.h"
#include <unistd.h>
#include <fcntl.h>

void EmptyToken(token* token2empty)
{
    token2empty->type = -1;
    token2empty->data.kw = -1;
    token2empty->data.msc = -1;
    token2empty->data.oper = -1;
    token2empty->data.str = NULL;
    token2empty->data._double = 0;
    token2empty->data._integer = 0;
    token2empty->data.type = -1;

    return;
}

int main()
{
    token write_token;

    FILE* fout = fopen("analysis-output.out","w");
    if(!fout)
    {
        fprintf(stderr, "Cannot open output file\n");
        exit(0);
    }

    int line_cnt = 0;
    GetNextToken(&write_token);
    fprintf(fout, "LINE | TOKEN \n");
    fprintf(fout, "-----------------------------\n");
    while(write_token.data.msc != _EOF)
    {
        line_cnt = GetLineNumber() + 1;
        switch(write_token.type)
        {
            case 0:
            {
               fprintf(fout, "%d    | id: %s\n", line_cnt, write_token.data.str);
                break;
            }

            case 1:
            {
               fprintf(fout, "%d    | kw (enum): %d\n",line_cnt, write_token.data.kw);
                break;
            }

            case 2:
            {
                if(write_token.data._double)
                {
                   fprintf(fout, "%d    | double: %6.6f\n",line_cnt, write_token.data._double);
                }
                
                if(write_token.data._integer)
                {
                   fprintf(fout, "%d    | integer: %d\n",line_cnt, write_token.data._integer);
                }
                
                if(write_token.data.str)
                {
                   fprintf(fout, "%d    | literal: %s\n", line_cnt, write_token.data.str);
                }

                else fprintf(stderr, "ERROR: Invalid Token\n");
                break;
            }

            case 4:
            {
               fprintf(fout, "%d    | misc (enum): %d\n",line_cnt, write_token.data.msc);
                break;
            }

            case 5:
            {
               fprintf(fout, "%d    | operator (enum): %d\n",line_cnt, write_token.data.oper);
                break;
            }
            default: break;
        }
        EmptyToken(&write_token);
        GetNextToken(&write_token);
    }

    fclose(fout);
    exit(0);
}
