#ifndef DEJKSTRA_H
#define DEJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stack/stack.h"
#include "../whotype/whotype.h"
#include "./exprLogicCheck.h"

char* infixToPostfix(const char* infix);

char* doPostfixConvert(const char* modifiedInfix);
char* parseDigits(const char* modifiedInfix, int* i, char* postfix, int* j);
char* parseLetter(const char* modifiedInfix, int* i, char* postfix, int* j,
                  Stack* operatorStack);
void parseOperatorParentheses(const char* modifiedInfix, int* i, char* postfix,
                              int* j, Stack* operatorStack);

void handleDigitsAndVariables(const char* expression, int* i, char* postfix,
                              int* j);
void handleOperatorAndParentheses(Stack* operatorStack, char token,
                                  char* postfix, int* j);
void popUntilEmpty(Stack* operatorStack, char* postfix, int* j);
void popUntilLeftParen(Stack* operatorStack, char* postfix, int* j);
void appendOperatorOrFunction(int op, char* postfix, int* j);

char* replaceUnaryMinus(const char* expression);
int isUnaryMinus(const char* expression, int i, int expectOperand);
int isExpectingOperand(char token);

int getFunctionCode(const char* funcName);
const char* getFunctionName(int code);
int isFunctionToken(int code);
int precedence(int op);
int isLeftAssociative(int op);

#endif /* DEJKSTRA_H */
