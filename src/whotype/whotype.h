#ifndef WHOTYPE_H
#define WHOTYPE_H

#include <stdlib.h>
#include <string.h>

#define OP_PLUS '+'
#define OP_MINUS '-'
#define OP_MULTIPLY '*'
#define OP_DIVIDE '/'
#define OP_POWER '^'
#define OP_UNARY_MINUS '~'

#define FUNC_SIN 300
#define FUNC_COS 301
#define FUNC_TAN 302
#define FUNC_COT 303
#define FUNC_SQRT 304
#define FUNC_LN 305
#define FUNC_EXP 306

#define CONST_PI 400

int isDigit(char const c);
int isAlpha(char const c);
int isLetter(char const c);
int isSpace(char const c);
int isOperator(char const c);

char *get_function_name(int const code);
int get_function_code(char const *const func_name);
int is_function_token(int const code);

int get_constant_code(char const *const const_name);
int is_constant_token(int const code);

#endif // WHOTYPE_H
