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

int plot_graph(const char* postfixExpression);
int evaluate_postfix_expression(const char* postfixExpression, double x_value,
                                double* result);
Point* gen_points(const char* postfixExpression, int x_min, int x_max,
                  int* num_points);
void graph_draw_cns(const char* postfixExpression, int x_min, int x_max,
                    int y_min, int y_max, int x, int y);

char* replace_x_with_value(const char* str, double value);
char* replace_character(const char* src, char target, const char* replacement,
                        int is_negative);
size_t calculate_new_length(const char* str, int count, int replacement_length,
                            int is_negative);
int format_double(double value, char* buffer, size_t size);
int count_occurrences(const char* str, char target);

#endif  // PLOT_GRAPH_H
