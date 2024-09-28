#include "dejkstra.h"

char *infix_to_postfix(char const *const infix) {
    char const *const modified_infix = replace_unary_minus(infix);
    if (!modified_infix)
        return NULL;

    if (!check_expression(infix, modified_infix)) {
        free((void *)modified_infix);
        return NULL;
    }

    char const *const postfix = do_postfix_convert(modified_infix);

    free((void *)modified_infix);
    return (char *const)postfix;
}

char *do_postfix_convert(char const *const modified_infix) {
    Stack_int operator_stack;
    initStack_int(&operator_stack);

    int const length = strlen(modified_infix);
    char *const postfix = (char *)malloc(4 * length * sizeof(char));
    if (!postfix) {
        return NULL;
    }

    int i = 0, j = 0;
    while (modified_infix[i] != '\0') {
        if (isSpace(modified_infix[i])) {
            i++;
            continue;
        }

        if (isDigit(modified_infix[i])) {
            parse_digits(modified_infix, &i, postfix, &j);
        } else if (isLetter(modified_infix[i])) {
            parse_letter(modified_infix, &i, postfix, &j, &operator_stack);
        } else if (isOperator(modified_infix[i]) || modified_infix[i] == '(' ||
                   modified_infix[i] == ')') {
            parse_operator_parentheses(modified_infix, &i, postfix, &j,
                                       &operator_stack);
        }
    }

    pop_until_empty(&operator_stack, postfix, &j);
    postfix[j] = '\0';

    destroyStack_int(&operator_stack);
    return postfix;
}

char *parse_digits(char const *const expression, int *const i,
                   char *const postfix, int *const j) {
    while (isDigit(expression[*i]) || expression[*i] == '.') {
        postfix[(*j)++] = expression[*i];
        (*i)++;
    }
    postfix[(*j)++] = ' ';
    return postfix;
}

char *parse_letter(char const *const modified_infix, int *const i,
                   char *const postfix, int *const j,
                   Stack_int *const operator_stack) {
    char name[10];
    int k = 0;

    while (isLetter(modified_infix[*i]) || isDigit(modified_infix[*i])) {
        if (k < 9) { // prevent buffer overflow
            name[k++] = modified_infix[*i];
        }
        (*i)++;
    }
    name[k] = '\0';

    int const func_code = get_function_code(name);
    if (func_code != -1) {
        push_int(operator_stack, func_code);
    } else {
        // append variable names or single variables to postfix
        strcpy(&postfix[*j], name);
        *j += strlen(name);
        postfix[(*j)++] = ' ';
    }

    return postfix;
}

void parse_operator_parentheses(char const *const modified_infix, int *const i,
                                char *const postfix, int *const j,
                                Stack_int *const operator_stack) {
    handle_operator_and_parentheses(operator_stack, modified_infix[*i], postfix,
                                    j);
    (*i)++;
}

void handle_operator_and_parentheses(Stack_int *const operator_stack,
                                     char const token, char *const postfix,
                                     int *const j) {
    if (token == '(') {
        push_int(operator_stack, '(');
    } else if (token == ')') {
        pop_until_left_paren(operator_stack, postfix, j);
    } else {
        while (!isEmpty_int(operator_stack)) {
            int const top_op = operator_stack->top->data;
            if (is_function_token(top_op) || isOperator((char)top_op)) {
                if ((precedence(top_op) > precedence(token)) ||
                    (precedence(top_op) == precedence(token) &&
                     is_left_associative(top_op))) {
                    int const op = pop_int(operator_stack);
                    append_operator_or_function(op, postfix, j);
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        push_int(operator_stack, token);
    }
}

void pop_until_empty(Stack_int *const operator_stack, char *const postfix,
                     int *const j) {
    while (!isEmpty_int(operator_stack)) {
        int const op = pop_int(operator_stack);
        append_operator_or_function(op, postfix, j);
    }
}

void pop_until_left_paren(Stack_int *const operator_stack, char *const postfix,
                          int *const j) {
    while (!isEmpty_int(operator_stack) && operator_stack->top->data != '(') {
        int const op = pop_int(operator_stack);
        append_operator_or_function(op, postfix, j);
    }
    if (!isEmpty_int(operator_stack)) {
        pop_int(operator_stack); // Pop '('
    }
}

void append_operator_or_function(int const op, char *const postfix,
                                 int *const j) {
    if (is_function_token(op)) {
        char const *const func_name = get_function_name(op);
        if (func_name) {
            strcpy(&postfix[*j], func_name);
            *j += strlen(func_name);
            postfix[(*j)++] = ' ';
        }
    } else {
        postfix[(*j)++] = (char)op;
        postfix[(*j)++] = ' ';
    }
}

char *replace_unary_minus(char const *const expression) {
    int const length = strlen(expression);
    char *const new_expression = (char *)malloc(2 * length * sizeof(char));

    if (!new_expression) {
        fprintf(stderr, "Memory allocation failed! [replaceUnaryMinus()]\n");
        return NULL;
    }

    int i = 0, j = 0;
    int expect_operand = 1;

    while (expression[i] != '\0') {
        if (isSpace(expression[i])) {
            new_expression[j++] = expression[i++];
            continue;
        }

        if (is_unary_minus(expression, i, expect_operand)) {
            new_expression[j++] = OP_UNARY_MINUS; // '~'
            i++;
            expect_operand = 1;
        } else {
            new_expression[j++] = expression[i];
            expect_operand = is_expecting_operand(expression[i]);
            i++;
        }
    }

    new_expression[j] = '\0';
    return new_expression;
}

int is_unary_minus(char const *const expression, int const i,
                   int const expect_operand) {
    return (expression[i] == '-') && expect_operand;
}

int is_expecting_operand(char const token) {
    if (isOperator(token) || token == '(' || token == ',') {
        return 1;
    } else if (isDigit(token) || isLetter(token) || token == 'x' ||
               token == ')') {
        return 0;
    }
    return 1;
}

int precedence(int const op) {
    if (is_function_token(op)) {
        return 5;
    }
    switch (op) {
    case OP_POWER:
        return 4;
    case OP_MULTIPLY:
    case OP_DIVIDE:
        return 3;
    case OP_UNARY_MINUS:
        return 2;
    case OP_PLUS:
    case OP_MINUS:
        return 1;
    default:
        return 0;
    }
}

int is_left_associative(int const op) {
    return (op == OP_POWER || op == OP_UNARY_MINUS) ? 0 : 1;
}
