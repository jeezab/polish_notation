#include "plot_graph.h"

int plot_graph(const char *postfixExpression) {
    int x_min = -20, x_max = 20;
    int y_min = -10, y_max = 10;

    int num_points = 0;
    Point *points = gen_points(postfixExpression, x_min, x_max, &num_points);

    for (int y = 0; y < GRAPH_HEIGHT; y++) {
        for (int x = 0; x < GRAPH_WIDTH; x++) {
            graph_draw_cns(postfixExpression, x_min, x_max, y_min, y_max, x, y);
        }
        printf("\n");
    }

    printf("x: [%d; %d]\n", x_min, x_max);
    printf("y: [%d; %d]\n", y_min, y_max);

    free(points);

    return 1;
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

Point *gen_points(const char *postfixExpression, int x_min, int x_max,
                  int *num_points) {
    double total_x_range = x_max - x_min;

    double x_step = total_x_range / GRAPH_WIDTH;
    Point *points = (Point *)malloc(GRAPH_WIDTH * sizeof(Point));

    *num_points = 0;

    for (int x_axis = 0; x_axis < GRAPH_WIDTH; x_axis++) {
        double x_value = x_min + x_axis * x_step;
        double result = 0.0;

        if (evaluate_postfix_expression(postfixExpression, x_value, &result) ==
            ERROR_NONE) {
            points[*num_points].x_value = x_value;
            points[*num_points].y_value = result;
            (*num_points)++;
        }
    }

    return points;
}

void graph_draw_cns(const char *postfixExpression, int x_min, int x_max,
                    int y_min, int y_max, int x, int y) {
    if (x_max == x_min || y_max == y_min) {
        printf(" ");
        return;
    }

    double x_scale = (double)(x_max - x_min) / GRAPH_WIDTH;
    double y_scale = (double)(y_max - y_min) / GRAPH_HEIGHT;

    double x_value = x_min + x * x_scale;
    double result = 0.0;

    int y_zero = -1;
    if (y_min <= 0 && y_max >= 0) {
        y_zero = (int)round(((double)(y_max) / (y_max - y_min)) * GRAPH_HEIGHT);
    }

    int x_zero = -1;
    if (x_min <= 0 && x_max >= 0) {
        x_zero = (int)round(((double)(-x_min) / (x_max - x_min)) * GRAPH_WIDTH);
    }

    char ch = ' ';

    if (x_zero != -1 && x == x_zero && y_zero != -1 && y == y_zero) {
        ch = '+';
    } else if (x_zero != -1 && x == x_zero) {
        ch = '|';
    } else if (y_zero != -1 && y == y_zero) {
        ch = '-';
    }

    if (evaluate_postfix_expression(postfixExpression, x_value, &result) ==
        ERROR_NONE) {
        double y_pos = (double)(y_max - result) / y_scale;
        int y_axis = (int)round(y_pos);

        if (fabs((double)y_axis - y) <= DELTA) {
            ch = '*';
        }
    }

    printf("%c", ch);
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
