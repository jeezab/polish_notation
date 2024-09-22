#ifndef PLOT_GRAPH_H
#define PLOT_GRAPH_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../polish_calc/polish_calc.h"

#define GRAPH_WIDTH 40
#define GRAPH_HEIGHT 20

void plot_graph(const char* postfixExpression);
void initialize_graphnet(char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH]);
int evaluate_postfix_expression(const char* postfixExpression, double x_value,
                                double* result);
void plot_function(const char* postfixExpression,
                   char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH], int x_min,
                   int x_max, int y_min, int y_max);
void draw_axes(char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH], int x_min, int x_max,
               int y_min, int y_max);
void print_graphnet(const char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH]);

char* replace_x_with_value(const char* str, double value);
char* replace_character(const char* src, char target, const char* replacement,
                        int is_negative);
size_t calculate_new_length(const char* str, int count, int replacement_length,
                            int is_negative);
int format_double(double value, char* buffer, size_t size);
int count_occurrences(const char* str, char target);

#endif  // PLOT_GRAPH_H
