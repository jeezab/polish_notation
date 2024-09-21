#ifndef INPUT_EXPR_H
#define INPUT_EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../whotype/whotype.h"

int input_expr(char** str);
int space_clear(char** str, int len);
int insert_mul(char** str, int len);

void input_err(int err_code);

#endif
