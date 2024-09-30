#include "whotype.h"

int isDigit(char const c) { return c >= '0' && c <= '9'; }

int isLetter(char const c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isSpace(char const c) { return c == ' ' || c == '\t' || c == '\n'; }

int isOperator(char const c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '~';
}

int get_function_code(char const *const func_name) {
    if (strcmp(func_name, "sin") == 0)
        return FUNC_SIN;
    if (strcmp(func_name, "cos") == 0)
        return FUNC_COS;
    if (strcmp(func_name, "tg") == 0 || strcmp(func_name, "tan") == 0)
        return FUNC_TAN;
    if (strcmp(func_name, "ctg") == 0)
        return FUNC_COT;
    if (strcmp(func_name, "sqrt") == 0)
        return FUNC_SQRT;
    if (strcmp(func_name, "ln") == 0)
        return FUNC_LN;
    if (strcmp(func_name, "exp") == 0)
        return FUNC_EXP;
    return -1; // func name error
}

int is_function_token(int const code) {
    return code >= FUNC_SIN && code <= FUNC_EXP;
}

int get_constant_code(char const *const const_name) {
    if (strcmp(const_name, "PI") == 0)
        return CONST_PI;
    return -1; // const name error
}

int is_constant_token(int const code) {
    return code >= CONST_PI && code <= CONST_PI;
}

char *get_function_name(int const code) {
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
