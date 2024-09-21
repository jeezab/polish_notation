#include "input_expr.h"

/*
 * funcs below work with type char*
 * if everything is ok - return length of char*
 * else - return error code:
 * 0 - empty string
 * -1 - memory alloc/realloc failed
 */

int input_expr(char** str) {
    *str = (char*)malloc(2 * sizeof(char));
    if (*str == NULL) {
        return -1;  // memory alloc/realloc error
    }

    int s_len = 0;
    (*str)[s_len] = '\0';

    char inp = getchar();
    while (inp != '\n') {
        s_len++;
        char* tmp = realloc(*str, (s_len + 1) * sizeof(char));
        if (tmp == NULL) {
            free(*str);
            return -1;  // memory alloc/realloc error
        }
        *str = tmp;
        (*str)[s_len - 1] = inp;
        (*str)[s_len] = '\0';
        inp = getchar();
    }

    return s_len;
}

/* func for delete spaces between digits and symbols
 * example: 2 sin(x)
 * out: 2sin(x)
 */
int space_clear(char** str, int len) {
    if (len == 0 || *str == NULL || str == NULL) {
        return 0;
    }

    char* new_str = malloc((len + 1) * sizeof(char));
    if (new_str == NULL) {
        return -1;
    }

    int new_len = copy_characters(str, new_str);

    char* tmp = realloc(*str, (new_len + 1) * sizeof(char));
    if (tmp == NULL) {
        free(new_str);
        return -1;
    }
    *str = tmp;

    strcpy(*str, new_str);
    free(new_str);

    return new_len;
}

int copy_characters(char** str, char* new_str) {
    int i = 0;
    int j = 0;
    char prev_char = '\0';

    while ((*str)[i] != '\0') {
        char current_char = (*str)[i];
        char next_char = (*str)[i + 1];

        if (current_char != ' ') {
            new_str[j++] = current_char;
        } else {
            if (!should_remove_space(prev_char, current_char, next_char)) {
                if (!(prev_char == ' ')) {
                    new_str[j++] = ' ';
                }
            }
        }
        prev_char = current_char;
        i++;
    }

    if (j > 0 && new_str[j - 1] == ' ') {
        j--;
    }

    new_str[j] = '\0';
    return j;
}

int should_remove_space(char prev, char current, char next) {
    if (current == ' ') {
        if (((isDigit(prev) || prev == '.') && isLetter(next)) ||
            (isLetter(prev) && (isDigit(next) || next == '.'))) {
            return 1;
        }
        if (isSpace(prev)) {
            return 1;
        }
    }
    return 0;
}

/* func for insert multiply between digits and symbols
 * example: 2sin(4x)
 * out: 2*sin(4*x)
 */
int insert_mul(char** str, int len) {
    if (len == 0 || *str == NULL || str == NULL) {
        return 0;
    }

    char* new_str = malloc((len * 2 + 1) * sizeof(char));
    if (new_str == NULL) {
        return -1;
    }

    TokenType prev_token_type = TOKEN_NONE;
    int new_len = process_tokens(str, new_str, &prev_token_type);

    char* tmp = realloc(*str, (new_len + 1) * sizeof(char));
    if (tmp == NULL) {
        free(new_str);
        return -1;
    }
    *str = tmp;
    strcpy(*str, new_str);
    free(new_str);

    return new_len;
}

int process_tokens(char** str, char* new_str, TokenType* prev_token_type) {
    int i = 0;
    int j = 0;
    while ((*str)[i] != '\0') {
        char curr = (*str)[i];
        TokenType current_type = get_token_type(curr);
        copy_token(str, &i, new_str, &j, current_type);
        *prev_token_type = current_type;
        if ((*str)[i] != '\0') {
            char next = (*str)[i];
            TokenType next_type = get_token_type(next);
            if (should_insert_mul(*prev_token_type, next_type)) {
                new_str[j++] = '*';
            }
        }
    }
    new_str[j] = '\0';
    return j;
}

TokenType get_token_type(char c) {
    if (isDigit(c) || c == '.') {
        return TOKEN_NUMBER;
    } else if (c == 'x') {
        return TOKEN_VARIABLE;
    } else if (isLetter(c)) {
        return TOKEN_FUNCTION;
    } else if (c == '(') {
        return TOKEN_OPEN_PAREN;
    } else if (c == ')') {
        return TOKEN_CLOSE_PAREN;
    } else {
        return TOKEN_OTHER;
    }
}

void copy_token(char** src, int* i, char* dest, int* j, TokenType token_type) {
    if (token_type == TOKEN_NUMBER) {
        while (isDigit((*src)[*i]) || (*src)[*i] == '.') {
            dest[(*j)++] = (*src)[(*i)++];
        }
    } else if (token_type == TOKEN_VARIABLE) {
        dest[(*j)++] = (*src)[(*i)++];
    } else if (token_type == TOKEN_FUNCTION) {
        while (isLetter((*src)[*i])) {
            dest[(*j)++] = (*src)[(*i)++];
        }
    } else if (token_type == TOKEN_OPEN_PAREN ||
               token_type == TOKEN_CLOSE_PAREN) {
        dest[(*j)++] = (*src)[(*i)++];
    } else {
        dest[(*j)++] = (*src)[(*i)++];
    }
}

int should_insert_mul(TokenType prev, TokenType next) {
    // Insert '*' between:
    // - Number, variable, closing parenthesis, or function
    // followed by
    // - Number, variable, function, or opening parenthesis
    if ((prev == TOKEN_NUMBER || prev == TOKEN_VARIABLE ||
         prev == TOKEN_CLOSE_PAREN || prev == TOKEN_FUNCTION) &&
        (next == TOKEN_NUMBER || next == TOKEN_VARIABLE ||
         next == TOKEN_FUNCTION || next == TOKEN_OPEN_PAREN)) {
        // Do not insert '*' if previous token is a function and next token is
        // '('
        if (!(prev == TOKEN_FUNCTION && next == TOKEN_OPEN_PAREN)) {
            return 1;
        }
    }
    return 0;
}

void input_err(int err_code) {
    switch (err_code) {
        case 0:
            printf("ERROR!\nError code: 0 [String is empty]\n");
            break;
        case -1:
            printf("ERROR!\nError code: -1 [Memory alloc/realloc failed]\n");
            break;
        default:
            printf("ERROR!\nError code: unknown");
            break;
    }
}
