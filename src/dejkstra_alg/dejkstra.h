#ifndef DEJKSTRA_H
#define DEJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stack/stack.h"
#include "../whotype/whotype.h"
#include "./expr_logic_check.h"

char *infix_to_postfix(char const *const infix);

char *do_postfix_convert(char const *const modified_infix);
char *parse_digits(char const *const expression, int *const i,
                   char *const postfix, int *const j);
char *parse_letter(char const *const modified_infix, int *const i,
                   char *const postfix, int *const j,
                   Stack_int *const operator_stack);
void parse_operator_parentheses(char const *const modified_infix, int *const i,
                                char *const postfix, int *const j,
                                Stack_int *const operator_stack);

void handle_operator_and_parentheses(Stack_int *const operator_stack,
                                     char const token, char *const postfix,
                                     int *const j);
void pop_until_empty(Stack_int *const operator_stack, char *const postfix,
                     int *const j);
void pop_until_left_paren(Stack_int *const operator_stack, char *const postfix,
                          int *const j);
void append_operator_or_function(int const op, char *const postfix,
                                 int *const j);

char *replace_unary_minus(char const *const expression);
int is_unary_minus(char const *const expression, int const i,
                   int const expect_operand);
int is_expecting_operand(char const token);

char *get_function_name(int const code);
int precedence(int const op);
int is_left_associative(int const op);

#endif /* DEJKSTRA_H */
