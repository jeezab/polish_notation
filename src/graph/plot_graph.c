#include "plot_graph.h"

int plot_graph(char const *const postfixExpression) {
    int const x_min = -20, x_max = 20;
    int const y_min = -10, y_max = 10;

    int num_points = 0;
    Point *const points =
        gen_points(postfixExpression, x_min, x_max, &num_points);

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

int evaluate_postfix_expression(char const *const postfixExpression,
                                double const x_value, double *const result) {
    char *const x_replaced_expr =
        replace_x_with_value(postfixExpression, x_value);
    if (!x_replaced_expr) {
        return -1;
    }

    int const status = polish_calc(x_replaced_expr, result);
    free(x_replaced_expr);
    return status;
}

Point *gen_points(char const *const postfixExpression, int const x_min,
                  int const x_max, int *const num_points) {
    double const total_x_range = x_max - x_min;

    double const x_step = total_x_range / GRAPH_WIDTH;
    Point *const points = (Point *)malloc(GRAPH_WIDTH * sizeof(Point));

    *num_points = 0;

    for (int x_axis = 0; x_axis < GRAPH_WIDTH; x_axis++) {
        double const x_value = x_min + x_axis * x_step;
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

void graph_draw_cns(char const *const postfixExpression, int const x_min,
                    int const x_max, int const y_min, int const y_max,
                    int const x, int const y) {
    if (x_max == x_min || y_max == y_min) {
        printf(" ");
        return;
    }

    double const x_scale = (double)(x_max - x_min) / GRAPH_WIDTH;
    double const y_scale = (double)(y_max - y_min) / GRAPH_HEIGHT;

    double const x_value = x_min + x * x_scale;
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
        double const y_pos = (double)(y_max - result) / y_scale;
        int const y_axis = (int)round(y_pos);

        if (fabs((double)y_axis - y) <= DELTA) {
            ch = '*';
        }
    }

    printf("%c", ch);
}

char *replace_x_with_value(char const *const str, double const value) {
    if (str == NULL) {
        return NULL;
    }

    int const is_negative = value < 0;

    char value_str[32];
    int const len_value_str =
        format_double(value, value_str, sizeof(value_str));

    if (len_value_str < 0) {
        return NULL;
    }

    char *const replaced_str =
        replace_character(str, 'x', value_str, is_negative);
    return replaced_str;
}

// replace 'target' characters with 'replacement' only if they are standalone
char *replace_character(char const *const src, char const target,
                        char const *const replacement, int const is_negative) {
    if (src == NULL || replacement == NULL) {
        return NULL;
    }

    // count only the 'x's that should be replaced
    int const count = count_occurrences(src, target);
    if (count == 0) {
        char *const copy_str = (char *)malloc(strlen(src) + 1);
        if (!copy_str) {
            return NULL;
        }
        strcpy(copy_str, src);
        return copy_str;
    }

    int const is_neg = is_negative ? 1 : 0;
    size_t const replacement_length = strlen(replacement);
    size_t const new_length =
        calculate_new_length(src, count, replacement_length, is_neg);

    char *const new_str = (char *)malloc(new_length + 1);
    if (!new_str) {
        return NULL;
    }

    char const *s = src;
    char *d = new_str;
    char prev_char = 0;

    while (*s) {
        if (*s == target) {
            char next_char = *(s + 1);

            if (!isLetter(prev_char) && !isLetter(next_char)) {
                memcpy(d, replacement, replacement_length);
                d += replacement_length;
                s++;

                if (is_neg) {
                    memcpy(d, " ~", 2);
                    d += 2;
                }

                prev_char = 0;
                continue;
            }
        }
        *d++ = *s++;
        prev_char = *(s - 1);
    }

    *d = '\0';
    return new_str;
}

size_t calculate_new_length(char const *const str, int const count,
                            int const replacement_length,
                            int const is_negative) {
    if (str == NULL) {
        return 0;
    }

    size_t const original_len = strlen(str);
    size_t const new_len = original_len + count * (replacement_length - 1);

    if (is_negative) {
        return new_len + count * 2;
    }

    return new_len;
}

int format_double(double const value, char *const buffer, size_t const size) {
    if (buffer == NULL) {
        return -1;
    }

    int const len = snprintf(buffer, size, "%.2f", fabs(value));
    return len;
}

int count_occurrences(char const *str, char const target) {
    if (str == NULL) {
        return 0;
    }

    int count = 0;
    char prev = 0;
    char current;
    while ((current = *str) != '\0') {
        if (current == target) {
            char next = *(str + 1);
            // check if 'x' is standalone
            if (!isLetter(prev) && !isLetter(next)) {
                count++;
            }
        }
        prev = current; // update previous character
        str++;
    }
    return count;
}
