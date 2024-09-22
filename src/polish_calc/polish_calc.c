#include "./polish_calc.h"

int polish_calc(const char* postfix, double* result) {
    int error_code = ERROR_NONE;
    Stack_double operandStack;
    initStack_double(&operandStack);

    char* expr_copy = (char*)malloc((strlen(postfix) + 1) * sizeof(char));
    strcpy(expr_copy, postfix);
    const char* token = strtok(expr_copy, " ");

    while (token != NULL && error_code == ERROR_NONE) {
        if (isDigit(token[0])) {
            push_double(&operandStack, atof(token));
        } else if (strcmp(token, "~") == 0) {
            double operand = pop_double(&operandStack);
            push_double(&operandStack, -operand);
        } else if (isOperator(token[0])) {
            double operand2 = pop_double(&operandStack);
            double operand1 = pop_double(&operandStack);
            double temp_result;
            error_code =
                apply_operator(token[0], operand1, operand2, &temp_result);
            if (error_code == ERROR_NONE) {
                push_double(&operandStack, temp_result);
            }
        } else {
            int func_code = get_function_code(token);
            if (func_code != -1) {
                double operand = pop_double(&operandStack);
                double temp_result;
                error_code = apply_function(func_code, operand, &temp_result);
                if (error_code == ERROR_NONE) {
                    push_double(&operandStack, temp_result);
                }
            } else {
                error_code = ERROR_UNKNOWN_FUNCTION;
            }
        }
        token = strtok(NULL, " ");
    }

    if (error_code == ERROR_NONE) {
        *result = pop_double(&operandStack);
    }

    free(expr_copy);
    destroyStack_double(&operandStack);

    return error_code;
}

// function to apply binary ops
int apply_operator(int op, double operand1, double operand2, double* result) {
    switch (op) {
        case OP_PLUS:
            *result = operand1 + operand2;
            return ERROR_NONE;
        case OP_MINUS:
            *result = operand1 - operand2;
            return ERROR_NONE;
        case OP_MULTIPLY:
            *result = operand1 * operand2;
            return ERROR_NONE;
        case OP_DIVIDE:
            if (operand2 == 0) {
                return ERROR_DIVISION_BY_ZERO;
            }
            *result = operand1 / operand2;
            return ERROR_NONE;
        case OP_POWER:
            *result = pow(operand1, operand2);
            return ERROR_NONE;
        default:
            return ERROR_UNKNOWN_OPERATOR;
    }
}

// function to apply unary funcs
int apply_function(int func, double operand, double* result) {
    switch (func) {
        case FUNC_SIN:
            *result = sin(operand);
            return ERROR_NONE;
        case FUNC_COS:
            *result = cos(operand);
            return ERROR_NONE;
        case FUNC_TAN:
            *result = tan(operand);
            return ERROR_NONE;
        case FUNC_COT:
            *result = 1.0 / tan(operand);
            return ERROR_NONE;
        case FUNC_SQRT:
            if (operand < 0) {
                return ERROR_NEGATIVE_SQRT;
            }
            *result = sqrt(operand);
            return ERROR_NONE;
        case FUNC_LN:
            if (operand <= 0) {
                return ERROR_NONPOSITIVE_LN;
            }
            *result = log(operand);
            return ERROR_NONE;
        default:
            return ERROR_UNKNOWN_FUNCTION;
    }
}

int print_error_calc(int error_code) {
    switch (error_code) {
        case ERROR_NONE:
            printf("Calculation succeeded.\n");
            break;
        case ERROR_DIVISION_BY_ZERO:
            printf("Error: Division by zero\n");
            break;
        case ERROR_NEGATIVE_SQRT:
            printf("Error: Negative input for sqrt\n");
            break;
        case ERROR_NONPOSITIVE_LN:
            printf("Error: Non-positive input for ln\n");
            break;
        case ERROR_UNKNOWN_OPERATOR:
            printf("Error: Unknown operator\n");
            break;
        case ERROR_UNKNOWN_FUNCTION:
            printf("Error: Unknown function\n");
            break;
        default:
            printf("Error: Unknown error occurred\n");
            break;
    }
    return error_code;
}
