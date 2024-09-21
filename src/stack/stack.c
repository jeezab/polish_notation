#include "stack.h"

void initStack(Stack *stack) { stack->top = NULL; }

int isEmpty(const Stack *stack) { return stack->top == NULL; }

void push(Stack *stack, int value) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed [push()]\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = stack->top;
    stack->top = newNode;
}

int pop(Stack *stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack is empty! [pop()]\n");
        exit(EXIT_FAILURE);
    }
    StackNode *temp = stack->top;
    int value = temp->data;
    stack->top = temp->next;
    free(temp);
    return value;
}

void destroyStack(Stack *stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}
