#include "./dejkstra_alg/dejkstra.h"
#include "./input/input_expr.h"
#include "./polish_calc/polish_calc.h"

int general();

int main() {
    int err = 0;

    err = general();

    return err;
}

int general() {
    char* str;
    int len = input_expr(&str);

    if (len <= 0) {
        input_err(len);
        return 1;
    }

    len = space_clear(&str, len);

    if (len <= 0) {
        input_err(len);
        return 1;
    }

    len = insert_mul(&str, len);

    if (len <= 0) {
        input_err(len);
        return 1;
    }

    printf("input: %s\n", str);

    printf("\n");

    char* postfixExpression = infixToPostfix(str);

    free(str);

    if (postfixExpression) {
        printf("Postfix: %s\n", postfixExpression);

        double result = 0;
        int err_code = polish_calc(postfixExpression, &result);
        if (err_code == 1) {
            printf("Result: %lf\n", result);
        } else
            print_error_calc(err_code);
        free(postfixExpression);
    }

    return 0;
}
