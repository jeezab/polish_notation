#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#define DECLARE_STACK_TYPE(TYPE, TYPE_SUFFIX)                        \
    typedef struct StackNode_##TYPE_SUFFIX {                         \
        TYPE data;                                                   \
        struct StackNode_##TYPE_SUFFIX *next;                        \
    } StackNode_##TYPE_SUFFIX;                                       \
                                                                     \
    typedef struct Stack_##TYPE_SUFFIX {                             \
        StackNode_##TYPE_SUFFIX *top;                                \
    } Stack_##TYPE_SUFFIX;                                           \
                                                                     \
    void initStack_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *stack);        \
    int isEmpty_##TYPE_SUFFIX(const Stack_##TYPE_SUFFIX *stack);     \
    void push_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *stack, TYPE value); \
    TYPE pop_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *stack);              \
    void destroyStack_##TYPE_SUFFIX(Stack_##TYPE_SUFFIX *stack);

DECLARE_STACK_TYPE(int, int)
DECLARE_STACK_TYPE(double, double)

#endif  // STACK_H
