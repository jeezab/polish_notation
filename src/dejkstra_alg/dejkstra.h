#ifndef DEJKSTRA_H
#define DEJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stack/stack.h"
#include "../whotype/whotype.h"
#include "./expr_logic_check.h"

char *infix_to_postfix(const char *const infix);

char *do_postfix_convert(const char *const modified_infix);
char *parse_digits(const char *const expression, int *const i,
                   char *const postfix, int *const j);
char *parse_letter(const char *const modified_infix, int *const i,
                   char *const postfix, int *const j,
                   Stack_int *const operator_stack);
void parse_operator_parentheses(const char *const modified_infix, int *const i,
                                char *const postfix, int *const j,
                                Stack_int *const operator_stack);

void handle_operator_and_parentheses(Stack_int *const operator_stack,
                                     const char token, char *const postfix,
                                     int *const j);
void pop_until_empty(Stack_int *const operator_stack, char *const postfix,
                     int *const j);
void pop_until_left_paren(Stack_int *const operator_stack, char *const postfix,
                          int *const j);
void append_operator_or_function(const int op, char *const postfix,
                                 int *const j);

char *replace_unary_minus(const char *const expression);
int is_unary_minus(const char *const expression, const int i,
                   const int expect_operand);
int is_expecting_operand(const char token);

char *get_function_name(const int code);
int precedence(const int op);
int is_left_associative(const int op);

#endif /* DEJKSTRA_H */
