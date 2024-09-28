#include "whotype.h"

int isDigit(char const c) { return c >= '0' && c <= '9'; }

int isLetter(char const c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isSpace(char const c) { return c == ' ' || c == '\t' || c == '\n'; }

int isOperator(char const c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '~';
}
