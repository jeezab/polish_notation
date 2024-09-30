#include "polish_calc.h"

int polish_calc(char const *const postfix, double *const result) {
    int error_code = ERROR_NONE;
    Stack_double operandStack;
    initStack_double(&operandStack);

    char *const expr_copy =
        (char *)malloc((strlen(postfix) + 1) * sizeof(char));
    if (!expr_copy) {
        fprintf(stderr, "Memory allocation failed! [polish_calc()]\n");
        destroyStack_double(&operandStack);
        return ERROR_MEMORY_ALLOC;
    }
    strcpy(expr_copy, postfix);
    char const *token = strtok(expr_copy, " ");

    while (token != NULL && error_code == ERROR_NONE) {
        error_code = calc_process_token(&operandStack, token);
        token = strtok(NULL, " ");
    }

    if (error_code == ERROR_NONE) {
        if (!isEmpty_double(&operandStack)) {
            *result = pop_double(&operandStack);
            if (!isEmpty_double(&operandStack)) {
                error_code = ERROR_UNKNOWN_OPERATOR;
            }
        } else {
            error_code = ERROR_UNKNOWN_OPERATOR;
        }
    }

    free(expr_copy);
    destroyStack_double(&operandStack);

    return error_code;
}

int calc_process_operand(Stack_double *operandStack, char const *const token) {
    if (isDigit(token[0]) || (token[0] == '.' && isDigit(token[1]))) {
        push_double(operandStack, atof(token));
        return ERROR_NONE;
    }
    return ERROR_UNKNOWN_OPERATOR;
}

int calc_process_operator(Stack_double *operandStack, char operator) {
    if (operandStack->top == NULL || operandStack->top->next == NULL) {
        return ERROR_UNKNOWN_OPERATOR;
    }

    double operand2 = pop_double(operandStack);
    double operand1 = pop_double(operandStack);
    double temp_result;
    int error_code = apply_operator(operator, operand1, operand2, &temp_result);

    if (error_code == ERROR_NONE) {
        push_double(operandStack, temp_result);
    }

    return error_code;
}

int calc_process_function(Stack_double *operandStack, char const *const token) {
    int func_code = get_function_code(token);

    if (func_code != -1) {
        if (isEmpty_double(operandStack)) {
            return ERROR_UNKNOWN_FUNCTION;
        }
        double operand = pop_double(operandStack);
        double temp_result;
        int error_code = apply_function(func_code, operand, &temp_result);

        if (error_code == ERROR_NONE) {
            push_double(operandStack, temp_result);
        }

        return error_code;
    }

    return ERROR_UNKNOWN_FUNCTION;
}

int calc_process_constant(Stack_double *operandStack, char const *const token) {
    int const_code = get_constant_code(token);
    if (const_code != -1) {
        double const_value = get_constant_value(const_code);
        push_double(operandStack, const_value);
        return ERROR_NONE;
    }
    return ERROR_UNKNOWN_FUNCTION;
}

int calc_process_token(Stack_double *operandStack, char const *const token) {
    if (isDigit(token[0]) || (token[0] == '.' && isDigit(token[1]))) {
        return calc_process_operand(operandStack, token);
    } else if (strcmp(token, "~") == 0) {
        if (isEmpty_double(operandStack)) {
            return ERROR_UNKNOWN_OPERATOR;
        }
        double operand = pop_double(operandStack);
        push_double(operandStack, -operand);
        return ERROR_NONE;
    } else if (isOperator(token[0]) && strlen(token) == 1) {
        return calc_process_operator(operandStack, token[0]);
    } else {
        int error_code = calc_process_function(operandStack, token);
        if (error_code == ERROR_UNKNOWN_FUNCTION) {
            return calc_process_constant(operandStack, token);
        }
        return error_code;
    }
}

int apply_operator(int const op, double const operand1, double const operand2,
                   double *const result) {
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

int apply_function(int const func, double const operand, double *const result) {
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
    case FUNC_EXP:
        *result = exp(operand);
        return ERROR_NONE;
    default:
        return ERROR_UNKNOWN_FUNCTION;
    }
}

int print_error_calc(int const error_code) {
    switch (error_code) {
    case ERROR_NONE:
        fprintf(stderr, "Calculation succeeded.\n");
        break;
    case ERROR_DIVISION_BY_ZERO:
        fprintf(stderr, "Error: Division by zero\n");
        break;
    case ERROR_NEGATIVE_SQRT:
        fprintf(stderr, "Error: Negative input for sqrt\n");
        break;
    case ERROR_NONPOSITIVE_LN:
        fprintf(stderr, "Error: Non-positive input for ln\n");
        break;
    case ERROR_UNKNOWN_OPERATOR:
        fprintf(stderr, "Error: Unknown operator or insufficient operands\n");
        break;
    case ERROR_UNKNOWN_FUNCTION:
        fprintf(stderr, "Error: Unknown function\n");
        break;
    default:
        fprintf(stderr, "Error: Unknown error occurred\n");
        break;
    }
    return error_code;
}

double get_constant_value(int const code) {
    switch (code) {
    case CONST_PI:
        return 3.14159265358979323846;
    default:
        return 0.0;
    }
}
