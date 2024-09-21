#include "dejkstra.h"

char* infixToPostfix(const char* infix) {
    char* modifiedInfix = replaceUnaryMinus(infix);
    if (!modifiedInfix) return NULL;

    if (!checkExpression(infix, modifiedInfix)) {
        free(modifiedInfix);
        return NULL;
    }

    char* postfix = doPostfixConvert(modifiedInfix);

    free(modifiedInfix);
    return postfix;
}

char* doPostfixConvert(const char* modifiedInfix) {
    Stack_int operatorStack;
    initStack_int(&operatorStack);

    int length = strlen(modifiedInfix);
    char* postfix = (char*)malloc(4 * length * sizeof(char));
    if (!postfix) {
        return NULL;
    }

    int i = 0, j = 0;
    while (modifiedInfix[i] != '\0') {
        if (isSpace(modifiedInfix[i])) {
            i++;
            continue;
        }

        if (isDigit(modifiedInfix[i])) {
            parseDigits(modifiedInfix, &i, postfix, &j);
        } else if (isLetter(modifiedInfix[i])) {
            parseLetter(modifiedInfix, &i, postfix, &j, &operatorStack);
        } else if (isOperator(modifiedInfix[i]) || modifiedInfix[i] == '(' ||
                   modifiedInfix[i] == ')') {
            parseOperatorParentheses(modifiedInfix, &i, postfix, &j,
                                     &operatorStack);
        }
    }

    popUntilEmpty(&operatorStack, postfix, &j);
    postfix[j] = '\0';

    destroyStack_int(&operatorStack);
    return postfix;
}

char* parseDigits(const char* modifiedInfix, int* i, char* postfix, int* j) {
    handleDigitsAndVariables(modifiedInfix, i, postfix, j);
    return postfix;
}

char* parseLetter(const char* modifiedInfix, int* i, char* postfix, int* j,
                  Stack_int* operatorStack) {
    char name[10];
    int k = 0;

    while (isLetter(modifiedInfix[*i]) || isDigit(modifiedInfix[*i])) {
        if (k < 9) {  // prevent buffer overflow
            name[k++] = modifiedInfix[*i];
        }
        (*i)++;
    }
    name[k] = '\0';

    int funcCode = getFunctionCode(name);
    if (funcCode != -1) {
        push_int(operatorStack, funcCode);
    } else {
        // append variable names or single variables to postfix
        strcpy(&postfix[*j], name);
        *j += strlen(name);
        postfix[(*j)++] = ' ';
    }

    return postfix;
}

void parseOperatorParentheses(const char* modifiedInfix, int* i, char* postfix,
                              int* j, Stack_int* operatorStack) {
    handleOperatorAndParentheses(operatorStack, modifiedInfix[*i], postfix, j);
    (*i)++;
}

void handleDigitsAndVariables(const char* expression, int* i, char* postfix,
                              int* j) {
    while (isDigit(expression[*i]) || expression[*i] == '.') {
        postfix[(*j)++] = expression[*i];
        (*i)++;
    }
    postfix[(*j)++] = ' ';
}

void handleOperatorAndParentheses(Stack_int* operatorStack, char token,
                                  char* postfix, int* j) {
    if (token == '(') {
        push_int(operatorStack, '(');
    } else if (token == ')') {
        popUntilLeftParen(operatorStack, postfix, j);
    } else {
        while (!isEmpty_int(operatorStack)) {
            int topOp = operatorStack->top->data;
            if (isFunctionToken(topOp) || isOperator((char)topOp)) {
                if ((precedence(topOp) > precedence(token)) ||
                    (precedence(topOp) == precedence(token) &&
                     isLeftAssociative(topOp))) {
                    int op = pop_int(operatorStack);
                    appendOperatorOrFunction(op, postfix, j);
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        push_int(operatorStack, token);
    }
}

void popUntilEmpty(Stack_int* operatorStack, char* postfix, int* j) {
    while (!isEmpty_int(operatorStack)) {
        int op = pop_int(operatorStack);
        appendOperatorOrFunction(op, postfix, j);
    }
}

void popUntilLeftParen(Stack_int* operatorStack, char* postfix, int* j) {
    while (!isEmpty_int(operatorStack) && operatorStack->top->data != '(') {
        int op = pop_int(operatorStack);
        appendOperatorOrFunction(op, postfix, j);
    }
    if (!isEmpty_int(operatorStack)) {
        pop_int(operatorStack);  // Pop '('
    }
}

void appendOperatorOrFunction(int op, char* postfix, int* j) {
    if (isFunctionToken(op)) {
        const char* funcName = getFunctionName(op);
        if (funcName) {
            strcpy(&postfix[*j], funcName);
            *j += strlen(funcName);
            postfix[(*j)++] = ' ';
        }
    } else {
        postfix[(*j)++] = (char)op;
        postfix[(*j)++] = ' ';
    }
}

char* replaceUnaryMinus(const char* expression) {
    int length = strlen(expression);
    char* newExpression = (char*)malloc(2 * length * sizeof(char));

    if (!newExpression) {
        printf("Memory allocation failed! [replaceUnaryMinus()]\n");
        return NULL;
    }

    int i = 0, j = 0;
    int expectOperand = 1;

    while (expression[i] != '\0') {
        if (isSpace(expression[i])) {
            newExpression[j++] = expression[i++];
            continue;
        }

        if (isUnaryMinus(expression, i, expectOperand)) {
            newExpression[j++] = OP_UNARY_MINUS;  // '~'
            i++;
            expectOperand = 1;
        } else {
            newExpression[j++] = expression[i];
            expectOperand = isExpectingOperand(expression[i]);
            i++;
        }
    }

    newExpression[j] = '\0';
    return newExpression;
}

int isUnaryMinus(const char* expression, int i, int expectOperand) {
    return (expression[i] == '-') && expectOperand;
}

int isExpectingOperand(char token) {
    if (isOperator(token) || token == '(' || token == ',') {
        return 1;
    } else if (isDigit(token) || isLetter(token) || token == 'x' ||
               token == ')') {
        return 0;
    }
    return 1;
}

int precedence(int op) {
    if (isFunctionToken(op)) {
        return 5;
    }
    switch (op) {
        case OP_UNARY_MINUS:
            return 4;
        case OP_POWER:
            return 3;
        case OP_MULTIPLY:
        case OP_DIVIDE:
            return 2;
        case OP_PLUS:
        case OP_MINUS:
            return 1;
        default:
            return 0;
    }
}

int isLeftAssociative(int op) {
    if (op == OP_POWER || op == OP_UNARY_MINUS) {
        return 0;
    } else {
        return 1;
    }
}
