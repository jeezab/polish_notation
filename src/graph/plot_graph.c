#include "plot_graph.h"

void plot_graph(const char* postfixExpression) {
    int x_min = -20, x_max = 20;
    int y_min = -10, y_max = 10;

    double total_x_range = x_max - x_min;
    double total_y_range = y_max - y_min;

    double x_step = total_x_range / GRAPH_WIDTH;
    double y_step = total_y_range / GRAPH_HEIGHT;

    char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH];

    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            graphnet[i][j] = ' ';
        }
    }

    for (int x_axis = 0; x_axis < GRAPH_WIDTH; x_axis++) {
        double x_value = x_min + x_axis * x_step;
        char* x_replace_postExpr = replace_x_with_value(postfixExpression, x_value);
        double result = 0;

        if (polish_calc(x_replace_postExpr, &result) == ERROR_NONE) {
            double relative_y = (y_max - result) / y_step;
            int y_axis = (int)(relative_y + 0.5);

            if (y_axis >= 0 && y_axis < GRAPH_HEIGHT) {
                graphnet[y_axis][x_axis] = '*';
            }
        }
        free(x_replace_postExpr);
    }

    int y_axis_pos = GRAPH_WIDTH / 2;
    int x_axis_pos = GRAPH_HEIGHT / 2;

    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            if (graphnet[i][j] != '*') {
                if (i == x_axis_pos && j == y_axis_pos) {
                    graphnet[i][j] = '+';
                } else if (i == x_axis_pos) {
                    graphnet[i][j] = '-';
                } else if (j == y_axis_pos) {
                    graphnet[i][j] = '|';
                }
            }
        }
    }

    for (int y_axis = 0; y_axis < GRAPH_HEIGHT; y_axis++) {
        for (int x_axis = 0; x_axis < GRAPH_WIDTH; x_axis++) {
            printf("%c", graphnet[y_axis][x_axis]);
        }
        printf("\n");
    }
}

char* replace_x_with_value(const char *str, double value) {
    if (str == NULL) {
        return NULL;
    }

    int is_negative = value < 0;

    char value_str[32];
    int len_value_str = snprintf(value_str, sizeof(value_str), "%.2f", fabs(value));

    if (len_value_str < 0) {
        return NULL;
    }

    int count_x = 0;
    for (const char *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == 'x') {
            count_x++;
        }
    }

    if (count_x == 0) {
        char *copy_str = (char *)malloc(strlen(str) + 1);
        if (!copy_str) {
            return NULL;
        }
        strcpy(copy_str, str);
        return copy_str;
    }

    size_t original_len = strlen(str);
    size_t new_len = original_len + count_x * (len_value_str - 1);

    if (is_negative) {
        new_len += count_x * 2;
    }

    char *new_str = (char *)malloc(new_len + 1);
    if (!new_str) {
        return NULL;
    }

    const char *src = str;
    char *dest = new_str;

    while (*src) {
        if (*src == 'x') {
            memcpy(dest, value_str, len_value_str);
            dest += len_value_str;
            src++;

            if (is_negative) {
                *dest++ = ' ';
                *dest++ = '~';
            }
        } else {
            *dest++ = *src++;
        }
    }

    *dest = '\0';

    return new_str;
}
