#include "tree.h"
#include "lexer.h"
#include "parser.h"

int yyparse();

int main(int argc, char **argv){
    if (argc != 2){
        printf("wrong number of arguments");
        return 1;
    }
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
    yylex_destroy();
    return 0;
}
