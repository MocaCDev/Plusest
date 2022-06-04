#include <stdio.h>
#include "lang/read.h"
#include "lang/lexer.h"
#include "lang/parser.h"
#include "lang/util.h"

int main(int argc, char *argv[]) {
    char *f_content = read_file("main.m");
    _Lexer *lexer = init_lexer(f_content);
    _Parser *p = run_parser(lexer);
    return 0;
}