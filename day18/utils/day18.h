#include <stddef.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct Stack Stack;

struct Node {
    int left_value;
    int right_value;

    Node *parent;

    Node *left_child;
    Node *right_child;
};

struct Stack {
    Node *items;
    size_t capacity;
    int top;
};

/************
 * FOR NODE *
 ************/
Node *init_node(void);

Node *create_node(int left_val, int right_val, Node *parent, Node *left_child, Node *right_child);

void free_node(Node *node);

int magnitude(Node *root);

void copy_node(Node *to, Node *from);

/*************
 * FOR STACK *
 *************/
Stack *create_stack(size_t capacity);

void free_stack(Stack *stack);

int stack_is_full(Stack *stack);

int stack_is_empty(Stack *stack);

void push(Stack *stack, Node item);

void pop(Stack *stack, Node *item);

void peek(Stack *stack, Node *item);