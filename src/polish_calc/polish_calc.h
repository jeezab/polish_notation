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

int polish_calc(char const *const postfix, double *const result);
int apply_operator(int const op, double const operand1, double const operand2,
                   double *const result);
int apply_function(int const func, double const operand, double *const result);

int print_error_calc(int const error_code);

#endif  // POLISH_CALC
