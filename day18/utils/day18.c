#include <stdio.h>
#include <string.h>
#include "day18.h"

/************
 * FOR NODE *
 ************/
void init_node(Node *node) {
    create_node(node, -1, -1, NULL, NULL, NULL);
}

void create_node(Node *node, int left_val, int right_val, Node *parent, Node *left_child, Node *right_child) {
    node->left_value = left_val;
    node->right_value = right_val;

    node->parent = parent;
    node->left_child = left_child;
    node->right_child = right_child;
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

    to->parent = from->parent;
    to->left_child = from->left_child;
    to->right_child = from->right_child;
}

/*************
 * FOR STACK *
 *************/
void create_stack(Stack *stack, size_t capacity) {
    stack->capacity = capacity;
    stack->top = -1;
    stack->items = (Node *)malloc(stack->capacity * sizeof(Node));
}

void free_stack(Stack *stack) {
    free(stack->items);
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

Node pop(Stack *stack) {

    return stack->items[stack->top--];
}

Node peek(Stack *stack) {

    return stack->items[stack->top];
}