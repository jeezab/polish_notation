#include "./exprLogicCheck.h"

/* function to check the logical correctness of the expression
 * return 1 if correct
 * return 0 if incorrect
 */
int checkExpression(const char* originalExpression,
                    const char* modifiedExpression) {
    int balance = 0, i = 0, operandCount = 0, operatorFound = 0,
        expectOperand = 1;

    while (modifiedExpression[i] != '\0') {
        if (isSpace(modifiedExpression[i])) {
            i++;
            continue;
        }

        if (isDigit(modifiedExpression[i])) {
            if (!processDigit(originalExpression, modifiedExpression, &i,
                              &operandCount, &expectOperand)) {
                return 0;
            }
        } else if (isOperator(modifiedExpression[i]) ||
                   modifiedExpression[i] == OP_UNARY_MINUS) {
            if (!processOperator(originalExpression, modifiedExpression, &i,
                                 &operatorFound, &expectOperand)) {
                return 0;
            }
        } else if (isLetter(modifiedExpression[i])) {
            if (!processLetter(originalExpression, modifiedExpression, &i,
                               &operatorFound, &expectOperand, &operandCount)) {
                return 0;
            }
        } else if (modifiedExpression[i] == '(' ||
                   modifiedExpression[i] == ')') {
            if (!processParentheses(originalExpression, modifiedExpression, &i,
                                    &balance, &expectOperand)) {
                return 0;
            }
        } else {
            printError(originalExpression, i, "Invalid character");
            return 0;
        }
    }

    return lastCheck(balance, expectOperand, operandCount, originalExpression);
}

int processDigit(const char* originalExpression, const char* modifiedExpression,
                 int* i, int* operandCount, int* expectOperand) {
    if (!*expectOperand) {
        printError(originalExpression, *i, "Unexpected operand");
        return 0;
    }
    handleOperand(modifiedExpression, i, operandCount);
    *expectOperand = 0;
    return 1;
}

int processOperator(const char* originalExpression,
                    const char* modifiedExpression, int* i, int* operatorFound,
                    int* expectOperand) {
    if (!handleOperator(modifiedExpression[*i], operatorFound, expectOperand)) {
        printError(originalExpression, *i, "Unexpected operator");
        return 0;
    }
    (*i)++;
    return 1;
}

int processLetter(const char* originalExpression,
                  const char* modifiedExpression, int* i, int* operatorFound,
                  int* expectOperand, int* operandCount) {
    int letter_check = handleLetter(modifiedExpression, originalExpression, i,
                                    operatorFound, expectOperand, operandCount);
    if (!letter_check) {
        return 0;
    }
    return 1;
}

int processParentheses(const char* originalExpression,
                       const char* modifiedExpression, int* i, int* balance,
                       int* expectOperand) {
    if (!handleParentheses(modifiedExpression[*i], balance, expectOperand)) {
        printError(originalExpression, *i, "Mismatched parentheses");
        return 0;
    }
    (*i)++;
    return 1;
}

int handleOperator(char token, int* operatorFound, int* expectOperand) {
    if ((*expectOperand) && token != OP_UNARY_MINUS) {
        return 0;
    }
    *operatorFound = 1;
    *expectOperand = 1;
    return 1;
}

int handleOperand(const char* expression, int* i, int* operandCount) {
    (*operandCount)++;
    while (isDigit(expression[*i]) || expression[*i] == '.') {
        (*i)++;
    }
    return 0;
}

int handleParentheses(char token, int* balance, int* expectOperand) {
    if (token == '(') {
        (*balance)++;
        *expectOperand = 1;
    } else if (token == ')') {
        (*balance)--;
        *expectOperand = 0;
    }
    return (*balance >= 0);
}

int handleLetter(const char* modifiedExpression, const char* originalExpression,
                 int* i, int* operatorFound, int* expectOperand,
                 int* operandCount) {
    char name[10];
    int k = 0;
    int startPos = (*i);
    while (isLetter(modifiedExpression[(*i)]) ||
           isDigit(modifiedExpression[(*i)])) {
        if (k < 9) {
            name[k++] = modifiedExpression[(*i)++];
        } else {
            printError(originalExpression, (*i) - k,
                       "Undefined function or variable");
            return 0;
        }
    }
    name[k] = '\0';

    if (getFunctionCode(name) != -1) {
        if (!(*expectOperand)) {
            printError(originalExpression, startPos, "Unexpected function");
            return 0;
        }
        (*operatorFound) = 1;
        (*expectOperand) = 1;
    } else if (strcmp(name, "x") == 0) {
        if (!(*expectOperand)) {
            printError(originalExpression, startPos, "Unexpected variable");
            return 0;
        }
        (*operandCount)++;
        (*expectOperand) = 0;
    } else {
        printError(originalExpression, startPos,
                   "Undefined function name or variable");
        return 0;
    }

    return 1;
}

int lastCheck(int balance, int expectOperand, int operandCount,
              const char* originalExpression) {
    if (balance != 0) {
        printError(originalExpression, strlen(originalExpression),
                   "Mismatched parentheses");
        return 0;
    }
    if (expectOperand) {
        printError(originalExpression, strlen(originalExpression),
                   "Expression bad end");
        return 0;
    }
    if (operandCount < 1) {
        printError(originalExpression, 0, "No operands in expression");
        return 0;
    }
    return 1;
}

int getFunctionCode(const char* funcName) {
    if (strcmp(funcName, "sin") == 0) return FUNC_SIN;
    if (strcmp(funcName, "cos") == 0) return FUNC_COS;
    if (strcmp(funcName, "tg") == 0 || strcmp(funcName, "tan") == 0)
        return FUNC_TAN;
    if (strcmp(funcName, "ctg") == 0) return FUNC_COT;
    if (strcmp(funcName, "sqrt") == 0) return FUNC_SQRT;
    if (strcmp(funcName, "ln") == 0) return FUNC_LN;
    return -1;  // func name error
}

int isFunctionToken(int code) { return code >= FUNC_SIN && code <= FUNC_LN; }

const char* getFunctionName(int code) {
    switch (code) {
        case FUNC_SIN:
            return "sin";
        case FUNC_COS:
            return "cos";
        case FUNC_TAN:
            return "tan";
        case FUNC_COT:
            return "cot";
        case FUNC_SQRT:
            return "sqrt";
        case FUNC_LN:
            return "ln";
        default:
            return NULL;
    }
}

void printError(const char* expression, int errorPos, const char* message) {
    printf("Error: %s at position %d\n", message, errorPos);
    printf("%s\n", expression);
    for (int pos = 0; pos < errorPos; pos++) {
        printf("-");
    }
    printf("^\n");
}
