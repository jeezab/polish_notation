#ifndef EXPR_LOGIC_CHECK_H
#define EXPR_LOGIC_CHECK_H

#include <stdio.h>
#include <string.h>

#include "../whotype/whotype.h"

#define OP_PLUS '+'
#define OP_MINUS '-'
#define OP_MULTIPLY '*'
#define OP_DIVIDE '/'
#define OP_POWER '^'
#define OP_UNARY_MINUS '~'

#define FUNC_SIN 256
#define FUNC_COS 257
#define FUNC_TAN 258
#define FUNC_COT 259
#define FUNC_SQRT 260
#define FUNC_LN 261
#define FUNC_EXP 262

int check_expression(char const *const original_expression,
                     char const *const modified_expression);

int process_digit(char const *const original_expression,
                  char const *const modified_expression, int *const i,
                  int *const operand_count, int *const expect_operand);
int process_operator(char const *const original_expression,
                     char const *const modified_expression, int *const i,
                     int *const operator_found, int *const expect_operand);
int process_letter(char const *const original_expression,
                   char const *const modified_expression, int *const i,
                   int *const operator_found, int *const expect_operand,
                   int *const operand_count);
int process_parentheses(char const *const original_expression,
                        char const *const modified_expression, int *const i,
                        int *const balance, int *const expect_operand);

int handle_operator(char const token, int *const operator_found,
                    int *const expect_operand);
int handle_operand(char const *const expression, int *const i,
                   int *const operand_count);
int handle_parentheses(char const token, int *const balance,
                       int *const expect_operand);
int handle_letter(char const *const modified_expression,
                  char const *const original_expression, int *const i,
                  int *const operator_found, int *const expect_operand,
                  int *const operand_count);
int last_check(int const balance, int const expect_operand,
               int const operand_count, char const *const original_expression);

int get_function_code(char const *const func_name);
int is_function_token(int const code);

void print_error(char const *const expression, int const error_pos,
                 char const *const message);

#endif // EXPR_LOGIC_CHECK_H
