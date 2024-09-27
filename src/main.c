#include <stdio.h>
#include <stdlib.h>

#include "./dejkstra_alg/dejkstra.h"
#include "./graph/plot_graph.h"
#include "./input/input_expr.h"
#include "./polish_calc/polish_calc.h"

typedef enum {
    SUCCESS = 0,
    ERR_INPUT = 1,
    ERR_PREPROCESS = 2,
    ERR_CONVERSION = 3,
    ERR_PLOTTING = 4
} ErrorCode;

ErrorCode process_expression();
char* read_and_preprocess_input();
char* convert_expression_to_postfix(const char* str);
ErrorCode plot_postfix(const char* postfixExpression);

int main() {
    ErrorCode status = process_expression();

    if (status != SUCCESS) {
        fprintf(stderr, "Program terminated with error code: %d\n", status);
    }

    return status;
}

ErrorCode process_expression() {
    char* input_str = read_and_preprocess_input();
    if (input_str == NULL) {
        return ERR_INPUT;
    }

    char* postfixExpression = convert_expression_to_postfix(input_str);
    if (postfixExpression == NULL) {
        free(input_str);
        return ERR_CONVERSION;
    }

    printf("Postfix: %s\n", postfixExpression);
    free(input_str);

    ErrorCode plot_status = plot_postfix(postfixExpression);
    free(postfixExpression);

    if (plot_status != SUCCESS) {
        return plot_status;
    }

    return SUCCESS;
}

char* read_and_preprocess_input() {
    char* str = NULL;
    int len = input_expr(&str);

    len = space_clear(&str, len);

    len = insert_mul(&str, len);
    if (len <= 0) {
        input_err(len);
        free(str);
        return NULL;
    }

    printf("Input: %s\n", str);
    return str;
}

char* convert_expression_to_postfix(const char* str) {
    if (str == NULL) {
        fprintf(
            stderr,
            "Error: NULL string provided to convert_expression_to_postfix.\n");
        return NULL;
    }

    char* postfixExpression = infix_to_postfix(str);
    if (postfixExpression == NULL) {
        fprintf(stderr,
                "Error: Failed to convert infix expression to postfix.\n");
    }

    return postfixExpression;
}

ErrorCode plot_postfix(const char* postfixExpression) {
    if (postfixExpression == NULL) {
        fprintf(stderr,
                "Error: NULL postfix expression provided to plot_postfix.\n");
        return ERR_PLOTTING;
    }

    if (!plot_graph(postfixExpression)) {
        fprintf(stderr, "Error: Failed to plot the postfix expression.\n");
        return ERR_PLOTTING;
    }

    return SUCCESS;
}
