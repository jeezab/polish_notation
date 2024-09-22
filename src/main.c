#include <stdio.h>
#include <stdlib.h>

#include "./dejkstra_alg/dejkstra.h"
#include "./graph/plot_graph.h"
#include "./input/input_expr.h"
#include "./polish_calc/polish_calc.h"

int general();
char* read_and_preprocess_input(int* out_len);
char* convert_expression_to_postfix(const char* str);
void plot_postfix(const char* postfixExpression);

int main() {
    int err = 0;

    err = general();

    return err;
}

int general() {
    char* input_str = read_and_preprocess_input(NULL);
    if (input_str == NULL) {
        return 1;
    }

    char* postfixExpression = convert_expression_to_postfix(input_str);
    printf("Postfix: %s\n", postfixExpression);
    free(input_str);

    if (postfixExpression == NULL) {
        return 1;
    }

    plot_postfix(postfixExpression);

    free(postfixExpression);
    return 0;
}

char* read_and_preprocess_input(int* out_len) {
    char* str = NULL;
    int len = input_expr(&str);

    if (len <= 0) {
        input_err(len);
        return NULL;
    }

    len = space_clear(&str, len);
    if (len <= 0) {
        input_err(len);
        free(str);
        return NULL;
    }

    len = insert_mul(&str, len);
    if (len <= 0) {
        input_err(len);
        free(str);
        return NULL;
    }

    printf("Input: %s\n", str);

    if (out_len) {
        *out_len = len;
    }

    return str;
}

char* convert_expression_to_postfix(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    char* postfixExpression = infixToPostfix(str);
    if (postfixExpression == NULL) {
        fprintf(stderr,
                "Error: Failed to convert infix expression to postfix.\n");
        return NULL;
    }

    return postfixExpression;
}

void plot_postfix(const char* postfixExpression) {
    if (postfixExpression == NULL) {
        return;
    }

    plot_graph(postfixExpression);
}
