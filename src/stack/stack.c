#include "stack.h"

#define DEFINE_STACK_FUNCTIONS(TYPE, TYPE_SUFFIX)                              \
    void initStack_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *const stack) {           \
        stack->top = NULL;                                                     \
    }                                                                          \
                                                                               \
    int isEmpty_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX const *const stack) {        \
        return stack->top == NULL;                                             \
    }                                                                          \
                                                                               \
    void push_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *const stack,                  \
                            TYPE const value) {                                \
        StackNode_##TYPE_SUFFIX *const newNode =                               \
            (StackNode_##TYPE_SUFFIX *)malloc(                                 \
                sizeof(StackNode_##TYPE_SUFFIX));                              \
        if (!newNode) {                                                        \
            printf("Memory allocation failed [push()]\n");                     \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        newNode->data = value;                                                 \
        newNode->next = stack->top;                                            \
        stack->top = newNode;                                                  \
    }                                                                          \
                                                                               \
    TYPE pop_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *const stack) {                 \
        if (isEmpty_##TYPE_SUFFIX(stack)) {                                    \
            printf("Stack is empty! [pop()]\n");                               \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        StackNode_##TYPE_SUFFIX *const temp = stack->top;                      \
        TYPE const value = temp->data;                                         \
        stack->top = temp->next;                                               \
        free(temp);                                                            \
        return value;                                                          \
    }                                                                          \
                                                                               \
    void destroyStack_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *const stack) {        \
        while (!isEmpty_##TYPE_SUFFIX(stack)) {                                \
            pop_##TYPE_SUFFIX(stack);                                          \
        }                                                                      \
    }

DEFINE_STACK_FUNCTIONS(int, int);
DEFINE_STACK_FUNCTIONS(double, double);
