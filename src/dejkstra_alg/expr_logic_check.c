#include "./expr_logic_check.h"

/* Функция для проверки логической корректности выражения
 * Возвращает 1, если корректно
 * Возвращает 0, если некорректно
 */
int check_expression(char const *const original_expression,
                     char const *const modified_expression) {
    int balance = 0;
    int i = 0;
    int operand_count = 0;
    int operator_found = 0;
    int expect_operand = 1;

    while (modified_expression[i] != '\0') {
        if (isSpace(modified_expression[i])) {
            i++;
            continue;
        }

        if (isDigit(modified_expression[i])) {
            if (!process_digit(original_expression, modified_expression, &i,
                               &operand_count, &expect_operand)) {
                return 0;
            }
        } else if (isOperator(modified_expression[i]) ||
                   modified_expression[i] == OP_UNARY_MINUS) {
            if (!process_operator(original_expression, modified_expression, &i,
                                  &operator_found, &expect_operand)) {
                return 0;
            }
        } else if (isLetter(modified_expression[i])) {
            if (!process_letter(original_expression, modified_expression, &i,
                                &operator_found, &expect_operand,
                                &operand_count)) {
                return 0;
            }
        } else if (modified_expression[i] == '(' ||
                   modified_expression[i] == ')') {
            if (!process_parentheses(original_expression, modified_expression,
                                     &i, &balance, &expect_operand)) {
                return 0;
            }
        } else {
            print_error(original_expression, i, "Invalid character");
            return 0;
        }
    }

    return last_check(balance, expect_operand, operand_count,
                      original_expression);
}

int process_digit(char const *const original_expression,
                  char const *const modified_expression, int *const i,
                  int *const operand_count, int *const expect_operand) {
    if (!*expect_operand) {
        print_error(original_expression, *i, "Unexpected operand");
        return 0;
    }
    handle_operand(modified_expression, i, operand_count);
    *expect_operand = 0;
    return 1;
}

int process_operator(char const *const original_expression,
                     char const *const modified_expression, int *const i,
                     int *const operator_found, int *const expect_operand) {
    if (!handle_operator(modified_expression[*i], operator_found,
                         expect_operand)) {
        print_error(original_expression, *i, "Unexpected operator");
        return 0;
    }
    (*i)++;
    return 1;
}

int process_letter(char const *const original_expression,
                   char const *const modified_expression, int *const i,
                   int *const operator_found, int *const expect_operand,
                   int *const operand_count) {
    int letter_check =
        handle_letter(modified_expression, original_expression, i,
                      operator_found, expect_operand, operand_count);
    if (!letter_check) {
        return 0;
    }
    return 1;
}

int process_parentheses(char const *const original_expression,
                        char const *const modified_expression, int *const i,
                        int *const balance, int *const expect_operand) {
    if (!handle_parentheses(modified_expression[*i], balance, expect_operand)) {
        print_error(original_expression, *i, "Mismatched parentheses");
        return 0;
    }
    (*i)++;
    return 1;
}

int handle_operator(char const token, int *const operator_found,
                    int *const expect_operand) {
    if ((*expect_operand) && token != OP_UNARY_MINUS) {
        return 0;
    }
    *operator_found = 1;
    *expect_operand = 1;
    return 1;
}

int handle_operand(char const *const expression, int *const i,
                   int *const operand_count) {
    (*operand_count)++;
    while (isDigit(expression[*i]) || expression[*i] == '.') {
        (*i)++;
    }
    return 0;
}

int handle_parentheses(char const token, int *const balance,
                       int *const expect_operand) {
    if (token == '(') {
        (*balance)++;
        *expect_operand = 1;
    } else if (token == ')') {
        (*balance)--;
        *expect_operand = 0;
    }
    return (*balance >= 0);
}

int handle_letter(char const *const modified_expression,
                  char const *const original_expression, int *const i,
                  int *const operator_found, int *const expect_operand,
                  int *const operand_count) {
    char name[10];
    int k = 0;
    int start_pos = (*i);
    while (isLetter(modified_expression[(*i)]) ||
           isDigit(modified_expression[(*i)])) {
        if (k < 9) {
            name[k++] = modified_expression[(*i)++];
        } else {
            print_error(original_expression, (*i) - k,
                        "Undefined function or variable");
            return 0;
        }
    }
    name[k] = '\0';

    if (get_function_code(name) != -1) {
        if (!(*expect_operand)) {
            print_error(original_expression, start_pos, "Unexpected function");
            return 0;
        }
        (*operator_found) = 1;
        (*expect_operand) = 1;
    } else if (strcmp(name, "x") == 0) {
        if (!(*expect_operand)) {
            print_error(original_expression, start_pos, "Unexpected variable");
            return 0;
        }
        (*operand_count)++;
        (*expect_operand) = 0;
    } else if (strcmp(name, "PI") == 0) {
        if (!(*expect_operand)) {
            print_error(original_expression, start_pos, "Unexpected constant");
            return 0;
        }
        (*operand_count)++;
        (*expect_operand) = 0;
    } else {
        print_error(original_expression, start_pos,
                    "Undefined function name or variable");
        return 0;
    }

    return 1;
}

int last_check(int const balance, int const expect_operand,
               int const operand_count, char const *const original_expression) {
    if (balance != 0) {
        print_error(original_expression, strlen(original_expression),
                    "Mismatched parentheses");
        return 0;
    }
    if (expect_operand) {
        print_error(original_expression, strlen(original_expression),
                    "Expression bad end");
        return 0;
    }
    if (operand_count < 1) {
        print_error(original_expression, 0, "No operands in expression");
        return 0;
    }
    return 1;
}

void print_error(char const *const expression, int const error_pos,
                 char const *const message) {
    fprintf(stderr, "Error: %s at position %d\n", message, error_pos);
    fprintf(stderr, "%s\n", expression);
    for (int pos = 0; pos < error_pos; pos++) {
        fprintf(stderr, "-");
    }
    fprintf(stderr, "^\n");
}
