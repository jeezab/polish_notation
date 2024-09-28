#ifndef PLOT_GRAPH_H
#define PLOT_GRAPH_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../polish_calc/polish_calc.h"

#define GRAPH_WIDTH 80
#define GRAPH_HEIGHT 20

#define DELTA 0.5

typedef struct {
    double x_value;
    double y_value;
} Point;

int plot_graph(char const *const postfixExpression);
int evaluate_postfix_expression(char const *const postfixExpression,
                                double const x_value, double *const result);
Point *gen_points(char const *const postfixExpression, int const x_min,
                  int const x_max, int *const num_points);
void graph_draw_cns(char const *const postfixExpression, int const x_min,
                    int const x_max, int const y_min, int const y_max,
                    int const x, int const y);

char *replace_x_with_value(char const *const str, double const value);
char *replace_character(char const *const src, char const target,
                        char const *const replacement, int const is_negative);
size_t calculate_new_length(char const *const str, int const count,
                            int const replacement_length,
                            int const is_negative);
int format_double(double const value, char *const buffer, size_t const size);
int count_occurrences(char const *const str, char const target);

#endif // PLOT_GRAPH_H
