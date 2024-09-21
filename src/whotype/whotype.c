#include "whotype.h"

int isDigit(char c) { return c >= '0' && c <= '9'; }

int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '~';
}
