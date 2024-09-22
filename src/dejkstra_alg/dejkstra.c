#include "dejkstra.h"

char* infix_to_postfix(const char* infix) {
    char* modified_infix = replace_unary_minus(infix);
    if (!modified_infix) return NULL;

    if (!check_expression(infix, modified_infix)) {
        free(modified_infix);
        return NULL;
    }

    char* postfix = do_postfix_convert(modified_infix);

    free(modified_infix);
    return postfix;
}

char* do_postfix_convert(const char* modified_infix) {
    Stack_int operator_stack;
    initStack_int(&operator_stack);

    int length = strlen(modified_infix);
    char* postfix = (char*)malloc(4 * length * sizeof(char));
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

char* parse_digits(const char* modified_infix, int* i, char* postfix, int* j) {
    handle_digits_and_variables(modified_infix, i, postfix, j);
    return postfix;
}

char* parse_letter(const char* modified_infix, int* i, char* postfix, int* j,
                   Stack_int* operator_stack) {
    char name[10];
    int k = 0;

    while (isLetter(modified_infix[*i]) || isDigit(modified_infix[*i])) {
        if (k < 9) {  // prevent buffer overflow
            name[k++] = modified_infix[*i];
        }
        (*i)++;
    }
    name[k] = '\0';

    int func_code = get_function_code(name);
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

void parse_operator_parentheses(const char* modified_infix, int* i,
                                char* postfix, int* j,
                                Stack_int* operator_stack) {
    handle_operator_and_parentheses(operator_stack, modified_infix[*i], postfix,
                                    j);
    (*i)++;
}

void handle_digits_and_variables(const char* expression, int* i, char* postfix,
                                 int* j) {
    while (isDigit(expression[*i]) || expression[*i] == '.') {
        postfix[(*j)++] = expression[*i];
        (*i)++;
    }
    postfix[(*j)++] = ' ';
}

void handle_operator_and_parentheses(Stack_int* operator_stack, char token,
                                     char* postfix, int* j) {
    if (token == '(') {
        push_int(operator_stack, '(');
    } else if (token == ')') {
        pop_until_left_paren(operator_stack, postfix, j);
    } else {
        while (!isEmpty_int(operator_stack)) {
            int top_op = operator_stack->top->data;
            if (is_function_token(top_op) || isOperator((char)top_op)) {
                if ((precedence(top_op) > precedence(token)) ||
                    (precedence(top_op) == precedence(token) &&
                     is_left_associative(top_op))) {
                    int op = pop_int(operator_stack);
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

void pop_until_empty(Stack_int* operator_stack, char* postfix, int* j) {
    while (!isEmpty_int(operator_stack)) {
        int op = pop_int(operator_stack);
        append_operator_or_function(op, postfix, j);
    }
}

void pop_until_left_paren(Stack_int* operator_stack, char* postfix, int* j) {
    while (!isEmpty_int(operator_stack) && operator_stack->top->data != '(') {
        int op = pop_int(operator_stack);
        append_operator_or_function(op, postfix, j);
    }
    if (!isEmpty_int(operator_stack)) {
        pop_int(operator_stack);  // Pop '('
    }
}

void append_operator_or_function(int op, char* postfix, int* j) {
    if (is_function_token(op)) {
        const char* func_name = get_function_name(op);
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

char* replace_unary_minus(const char* expression) {
    int length = strlen(expression);
    char* new_expression = (char*)malloc(2 * length * sizeof(char));

    if (!new_expression) {
        printf("Memory allocation failed! [replaceUnaryMinus()]\n");
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
            new_expression[j++] = OP_UNARY_MINUS;  // '~'
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

int is_unary_minus(const char* expression, int i, int expect_operand) {
    return (expression[i] == '-') && expect_operand;
}

int is_expecting_operand(char token) {
    if (isOperator(token) || token == '(' || token == ',') {
        return 1;
    } else if (isDigit(token) || isLetter(token) || token == 'x' ||
               token == ')') {
        return 0;
    }
    return 1;
}

int precedence(int op) {
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

int is_left_associative(int op) {
    return (op == OP_POWER || op == OP_UNARY_MINUS) ? 0 : 1;
}
