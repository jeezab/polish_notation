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

int space_clear(char** str, int len) {
    int i, j;
    for (i = 0, j = 0; i < len; i++) {
        if ((*str)[i] != ' ') {
            (*str)[j++] = (*str)[i];
        }
    }

    if (j > 0 && (*str)[j - 1] == '=') {
        j--;
    }
    (*str)[j] = '\0';

    char* tmp = realloc(*str, (j + 1) * sizeof(char));
    if (tmp == NULL) {
        return j;
    }
    *str = tmp;

    return j;
}

int insert_mul(char** str, int len) {
    if (len == 0 || *str == NULL || str == NULL) {
        return 0;
    }

    char* new_str = (char*)malloc((len * 2 + 1) * sizeof(char));
    if (new_str == NULL) {
        return -1;
    }

    int i = 0;  // index in source string
    int j = 0;  // index in new string

    char prev_token_type;  // 'n' - number, 'v' - variable, 'f' - function,
                           // ')' - closing parenthesis

    while ((*str)[i] != '\0') {
        char curr = (*str)[i];

        // handle numbers
        if (isDigit(curr) || curr == '.') {
            prev_token_type = 'n';
            while (isDigit((*str)[i]) || (*str)[i] == '.') {
                new_str[j++] = (*str)[i++];
            }
        }
        // handle variables ('x')
        else if (curr == 'x') {
            prev_token_type = 'v';
            new_str[j++] = (*str)[i++];
        }
        // handle funcs
        else if (isLetter(curr)) {
            prev_token_type = 'f';  // Function
            while (isLetter((*str)[i])) {
                new_str[j++] = (*str)[i++];
            }
        }
        // handle parentheses
        else if (curr == '(' || curr == ')') {
            prev_token_type = (curr == ')') ? ')' : curr;
            new_str[j++] = (*str)[i++];
        }
        // handle other symbols
        else {
            prev_token_type = 0;
            new_str[j++] = (*str)[i++];
        }

        // check next char - are we need to insert '*' ?
        if ((*str)[i] != '\0') {
            char next = (*str)[i];
            char next_token_type = 0;

            // determine who is next
            if (isDigit(next) || next == '.') {
                next_token_type = 'n';
            } else if (next == 'x') {
                next_token_type = 'v';
            } else if (isLetter(next)) {
                next_token_type = 'f';
            } else if (next == '(') {
                next_token_type = '(';
            } else {
                next_token_type = 0;
            }

            // Insert '*' between:
            // - Number, variable, closing parenthesis, or function
            // followed by
            // - Number, variable, function, or opening parenthesis
            if ((prev_token_type == 'n' || prev_token_type == 'v' ||
                 prev_token_type == ')' || prev_token_type == 'f') &&
                (next_token_type == 'n' || next_token_type == 'v' ||
                 next_token_type == 'f' || next_token_type == '(')) {
                // do not insert '*' if previous token is a function and next
                // token is
                // '('
                if (!(prev_token_type == 'f' && next_token_type == '(')) {
                    new_str[j++] = '*';
                }
            }
        }
    }

    new_str[j] = '\0';

    char* tmp = realloc(*str, (j + 1) * sizeof(char));
    if (tmp == NULL) {
        free(new_str);
        return -1;
    }
    *str = tmp;
    strcpy(*str, new_str);
    free(new_str);

    return j;  // Return new length
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
