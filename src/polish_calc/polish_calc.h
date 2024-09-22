#ifndef POLISH_CALC
#define POLISH_CALC

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../dejkstra_alg//expr_logic_check.h"
#include "../stack/stack.h"

#define ERROR_NONE 1
#define ERROR_DIVISION_BY_ZERO -1
#define ERROR_NEGATIVE_SQRT -2
#define ERROR_NONPOSITIVE_LN -3
#define ERROR_UNKNOWN_OPERATOR -4
#define ERROR_UNKNOWN_FUNCTION -5

int polish_calc(const char* postfix, double* result);
int apply_operator(int op, double operand1, double operand2, double* result);
int apply_function(int func, double operand, double* result);

int print_error_calc(int error_code);

#endif  // POLISH_CALC
