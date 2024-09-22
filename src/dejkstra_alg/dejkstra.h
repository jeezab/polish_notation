#ifndef DEJKSTRA_H
#define DEJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stack/stack.h"
#include "../whotype/whotype.h"
#include "./expr_logic_check.h"

char* infix_to_postfix(const char* infix);

char* do_postfix_convert(const char* modified_infix);
char* parse_digits(const char* expression, int* i, char* postfix, int* j);
char* parse_letter(const char* modified_infix, int* i, char* postfix, int* j,
                   Stack_int* operator_stack);
void parse_operator_parentheses(const char* modified_infix, int* i,
                                char* postfix, int* j,
                                Stack_int* operator_stack);

void handle_operator_and_parentheses(Stack_int* operator_stack, char token,
                                     char* postfix, int* j);
void pop_until_empty(Stack_int* operator_stack, char* postfix, int* j);
void pop_until_left_paren(Stack_int* operator_stack, char* postfix, int* j);
void append_operator_or_function(int op, char* postfix, int* j);

char* replace_unary_minus(const char* expression);
int is_unary_minus(const char* expression, int i, int expect_operand);
int is_expecting_operand(char token);

int get_function_code(const char* func_name);
const char* get_function_name(int code);
int is_function_token(int code);
int precedence(int op);
int is_left_associative(int op);

#endif /* DEJKSTRA_H */
