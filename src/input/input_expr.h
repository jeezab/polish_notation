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

int input_expr(char **const str);

int space_clear(char **const str, int const len);
int copy_characters(char const **const str, char *const new_str);
int should_remove_space(char const prev, char const current, char const next);

int insert_mul(char **const str, int const len);
int process_tokens(char const **const str, char *const new_str,
                   TokenType *const prev_token_type);
TokenType get_token_type(char const c);
void copy_token(char const **const src, int *const i, char *const dest,
                int *const j, TokenType const token_type);
int should_insert_mul(TokenType const prev, TokenType const next);

void input_err(int const err_code);

#endif // INPUT_EXPR_H
