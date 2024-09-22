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

int check_expression(const char* original_expression,
                     const char* modified_expression);

int process_digit(const char* original_expression,
                  const char* modified_expression, int* i, int* operand_count,
                  int* expect_operand);
int process_operator(const char* original_expression,
                     const char* modified_expression, int* i,
                     int* operator_found, int* expect_operand);
int process_letter(const char* original_expression,
                   const char* modified_expression, int* i, int* operator_found,
                   int* expect_operand, int* operand_count);
int process_parentheses(const char* original_expression,
                        const char* modified_expression, int* i, int* balance,
                        int* expect_operand);

int handle_operator(char token, int* operator_found, int* expect_operand);
int handle_operand(const char* expression, int* i, int* operand_count);
int handle_parentheses(char token, int* balance, int* expect_operand);
int handle_letter(const char* modified_expression,
                  const char* original_expression, int* i, int* operator_found,
                  int* expect_operand, int* operand_count);
int last_check(int balance, int expect_operand, int operand_count,
               const char* original_expression);

int get_function_code(const char* func_name);
int is_function_token(int code);
const char* get_function_name(int code);

void print_error(const char* expression, int error_pos, const char* message);

#endif  // EXPR_LOGIC_CHECK_H
