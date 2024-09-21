#ifndef INPUT_EXPR_H
#define INPUT_EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../whotype/whotype.h"

typedef enum {
    TOKEN_NONE,
    TOKEN_NUMBER,
    TOKEN_VARIABLE,
    TOKEN_FUNCTION,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OTHER
} TokenType;

int input_expr(char** str);

int space_clear(char** str, int len);
int copy_characters(char** str, char* new_str);
int should_remove_space(char prev, char current, char next);

int insert_mul(char** str, int len);
int process_tokens(char** str, char* new_str, TokenType* prev_token_type);
TokenType get_token_type(char c);
void copy_token(char** src, int* i, char* dest, int* j, TokenType token_type);
int should_insert_mul(TokenType prev, TokenType next);

void input_err(int err_code);

#endif
