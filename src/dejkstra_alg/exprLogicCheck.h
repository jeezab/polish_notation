#ifndef EXPRLOGICCHECK_H
#define EXPRLOGICCHECK_H

#include <stdio.h>
#include <string.h>

#include "../whotype/whotype.h"

#define OP_PLUS '+'
#define OP_MINUS '-'
#define OP_MULTIPLY '*'
#define OP_DIVIDE '/'
#define OP_POWER '^'
#define OP_UNARY_MINUS '~'

#define FUNC_SIN 256
#define FUNC_COS 257
#define FUNC_TAN 258
#define FUNC_COT 259
#define FUNC_SQRT 260
#define FUNC_LN 261

int checkExpression(const char* originalExpression,
                    const char* modifiedExpression);

int processDigit(const char* originalExpression, const char* modifiedExpression,
                 int* i, int* operandCount, int* expectOperand);
int processOperator(const char* originalExpression,
                    const char* modifiedExpression, int* i, int* operatorFound,
                    int* expectOperand);
int processLetter(const char* originalExpression,
                  const char* modifiedExpression, int* i, int* operatorFound,
                  int* expectOperand, int* operandCount);
int processParentheses(const char* originalExpression,
                       const char* modifiedExpression, int* i, int* balance,
                       int* expectOperand);

int handleOperator(char token, int* operatorFound, int* expectOperand);
int handleOperand(const char* expression, int* i, int* operandCount);
int handleParentheses(char token, int* balance, int* expectOperand);
int handleLetter(const char* modifiedExpression, const char* originalExpression,
                 int* i, int* operatorFound, int* expectOperand,
                 int* operandCount);
int lastCheck(int balance, int expectOperand, int operandCount,
              const char* originalExpression);

int getFunctionCode(const char* funcName);
int isFunctionToken(int code);
const char* getFunctionName(int code);

void printError(const char* expression, int errorPos, const char* message);

#endif  // EXPRLOGICCHECK_H
