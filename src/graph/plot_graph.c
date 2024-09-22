#include "plot_graph.h"

void plot_graph(const char *postfixExpression) {
    int x_min = -20, x_max = 20;
    int y_min = -10, y_max = 10;

    char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH];

    initialize_graphnet(graphnet);

    plot_function(postfixExpression, graphnet, x_min, x_max, y_min, y_max);

    draw_axes(graphnet, x_min, x_max, y_min, y_max);

    print_graphnet(graphnet);

    printf("x: [%d; %d]\n", x_min, x_max);
    printf("y: [%d; %d]\n", y_min, y_max);
}

void initialize_graphnet(char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH]) {
    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            graphnet[i][j] = ' ';
        }
    }
}

int evaluate_postfix_expression(const char *postfixExpression, double x_value,
                                double *result) {
    char *x_replaced_expr = replace_x_with_value(postfixExpression, x_value);
    if (!x_replaced_expr) {
        return -1;
    }

    int status = polish_calc(x_replaced_expr, result);
    free(x_replaced_expr);
    return status;
}

void plot_function(const char *postfixExpression,
                   char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH], int x_min,
                   int x_max, int y_min, int y_max) {
    double total_x_range = x_max - x_min;
    double total_y_range = y_max - y_min;

    double x_step = total_x_range / GRAPH_WIDTH;
    double y_step = total_y_range / GRAPH_HEIGHT;

    for (int x_axis = 0; x_axis < GRAPH_WIDTH; x_axis++) {
        double x_value = x_min + x_axis * x_step;
        double result = 0.0;

        if (evaluate_postfix_expression(postfixExpression, x_value, &result) ==
            ERROR_NONE) {
            double relative_y = (y_max - result) / y_step;
            int y_axis = (int)(relative_y + 0.5);

            if (y_axis > 0 && y_axis < GRAPH_HEIGHT) {
                graphnet[y_axis][x_axis] = '*';
            }
        }
    }
}

void draw_axes(char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH], int x_min, int x_max,
               int y_min, int y_max) {
    int x_axis_pos = (int)((double)(y_max) / (y_max - y_min) * GRAPH_HEIGHT);
    int y_axis_pos = (int)((double)(-x_min) / (x_max - x_min) * GRAPH_WIDTH);

    if (x_axis_pos < 0) x_axis_pos = 0;
    if (x_axis_pos >= GRAPH_HEIGHT) x_axis_pos = GRAPH_HEIGHT / 2;
    if (y_axis_pos < 0) y_axis_pos = GRAPH_WIDTH / 2;
    if (y_axis_pos >= GRAPH_WIDTH) y_axis_pos = GRAPH_WIDTH / 2;

    for (int i = 0; i < GRAPH_HEIGHT; i++) {
        for (int j = 0; j < GRAPH_WIDTH; j++) {
            if (graphnet[i][j] != '*') {
                if (i == x_axis_pos && j == y_axis_pos) {
                    graphnet[i][j] = '+';
                } else if ((i == 0) && (j == GRAPH_WIDTH / 2)) {
                    graphnet[i][j] = 'y';
                } else if ((i == GRAPH_HEIGHT / 2) && (j == GRAPH_WIDTH - 1)) {
                    graphnet[i][j] = 'x';
                } else if (i == x_axis_pos) {
                    graphnet[i][j] = '-';
                } else if (j == y_axis_pos) {
                    graphnet[i][j] = '|';
                }
            }
        }
    }
}

void print_graphnet(const char graphnet[GRAPH_HEIGHT][GRAPH_WIDTH]) {
    for (int y = 0; y < GRAPH_HEIGHT; y++) {
        for (int x = 0; x < GRAPH_WIDTH; x++) {
            printf("%c", graphnet[y][x]);
        }
        printf("\n");
    }
}

char *replace_x_with_value(const char *str, double value) {
    if (str == NULL) {
        return NULL;
    }

    int is_negative = value < 0;

    char value_str[32];
    int len_value_str = format_double(value, value_str, sizeof(value_str));

    if (len_value_str < 0) {
        return NULL;
    }

    char *replaced_str = replace_character(str, 'x', value_str, is_negative);
    return replaced_str;
}

char *replace_character(const char *src, char target, const char *replacement,
                        int is_negative) {
    if (src == NULL || replacement == NULL) {
        return NULL;
    }

    int count = count_occurrences(src, target);
    if (count == 0) {
        char *copy_str = (char *)malloc(strlen(src) + 1);
        if (!copy_str) {
            return NULL;
        }
        strcpy(copy_str, src);
        return copy_str;
    }

    int is_neg = is_negative ? 1 : 0;
    size_t replacement_length = strlen(replacement);
    size_t new_length =
        calculate_new_length(src, count, replacement_length, is_neg);

    char *new_str = (char *)malloc(new_length + 1);
    if (!new_str) {
        return NULL;
    }

    const char *s = src;
    char *d = new_str;

    while (*s) {
        if (*s == target) {
            memcpy(d, replacement, replacement_length);
            d += replacement_length;
            s++;

            if (is_neg) {
                memcpy(d, " ~", 2);
                d += 2;
            }
        } else {
            *d++ = *s++;
        }
    }

    *d = '\0';
    return new_str;
}

int count_occurrences(const char *str, char target) {
    if (str == NULL) {
        return 0;
    }

    int count = 0;
    for (const char *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == target) {
            count++;
        }
    }
    return count;
}

int format_double(double value, char *buffer, size_t size) {
    if (buffer == NULL) {
        return -1;
    }
    // Format the absolute value with two decimal places
    int len = snprintf(buffer, size, "%.2f", fabs(value));
    return len;
}

size_t calculate_new_length(const char *str, int count, int replacement_length,
                            int is_negative) {
    if (str == NULL) {
        return 0;
    }

    size_t original_len = strlen(str);
    size_t new_len = original_len + count * (replacement_length - 1);

    if (is_negative) {
        new_len += count * 2;
    }

    return new_len;
}
