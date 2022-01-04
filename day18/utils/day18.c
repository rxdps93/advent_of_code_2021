#include <stdio.h>
#include <string.h>
#include "day18.h"

/************
 * FOR NODE *
 ************/
Node *init_node(void) {
    return create_node(-1, -1, NULL, NULL, NULL);
}

Node *create_node(int left_val, int right_val, Node *parent, Node *left_child, Node *right_child) {
    Node *node = malloc(sizeof(Node));

    node->left_value = left_val;
    node->right_value = right_val;

    node->parent = parent;
    node->left_child = left_child;
    node->right_child = right_child;
    // if (parent != NULL) {
    //     node->parent = parent;
    // } else {
    //     node->parent = malloc(sizeof(Node));
    // }

    // if (left_child != NULL) {
    //     node->left_child = left_child;
    // } else {
    //     node->left_child = malloc(sizeof(Node));
    // }

    // if (right_child != NULL) {
    //     node->right_child = right_child;
    // } else {
    //     node->right_child = malloc(sizeof(Node));
    // }

    return node;
}

void free_node(Node *node) {
    free(node);
}

int magnitude(Node *root) {
    if (root->left_child != NULL && root->right_child != NULL) {
        return (3 * magnitude(root->left_child)) + (2 * magnitude(root->left_child));
    } else if (root->left_child != NULL) {
        return (3 * magnitude(root->left_child)) + (2 * root->right_value);
    } else if (root->right_child != NULL) {
        return (3 * root->left_value) + (2 * magnitude(root->right_child));
    } else {
        return (3 * root->left_value) + (2 * root->right_value);
    }
}

void copy_node(Node *to, Node *from) {

    to->left_value = from->left_value;
    to->right_value = from->right_value;

    // memcpy(to->parent, from->parent, sizeof(Node));
    // memcpy(to->left_child, from->left_child, sizeof(Node));
    // memcpy(to->right_child, from->right_child, sizeof(Node));
    to->parent = from->parent;
    to->left_child = from->left_child;
    to->right_child = from->right_child;
}

/*************
 * FOR STACK *
 *************/
Stack *create_stack(size_t capacity) {
    Stack *stack = malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->items = malloc(stack->capacity * sizeof(Node));
    return stack;
}

void free_stack(Stack *stack) {
    free(stack->items);
    free(stack);
}

int stack_is_full(Stack *stack) {
    return stack->top == (int)(stack->capacity - 1);
}

int stack_is_empty(Stack *stack) {
    return stack->top == -1;
}

void push(Stack *stack, Node item) {
    if (!stack_is_full(stack)) {
        stack->items[++stack->top] = item;
    }
}

void pop(Stack *stack, Node *item) {
    if (stack_is_empty(stack)) {
        item = NULL;
    } else {
        item = &stack->items[stack->top--];
    }
}

void peek(Stack *stack, Node *item) {
    if (stack_is_empty(stack)) {
        item = NULL;
    } else {
        item = &stack->items[stack->top];
    }
}