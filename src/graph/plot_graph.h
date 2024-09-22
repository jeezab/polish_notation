#ifndef PLOT_GRAPH_H
#define PLOT_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../polish_calc/polish_calc.h"

#define GRAPH_WIDTH 40
#define GRAPH_HEIGHT 20

void plot_graph(const char* postfixExpression);
char* replace_x_with_value(const char *str, double value);

#endif // PLOT_GRAPH_H
