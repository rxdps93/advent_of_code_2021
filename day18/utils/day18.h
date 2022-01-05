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
void init_node(Node *node);

void create_node(Node *node, int left_val, int right_val, Node *parent, Node *left_child, Node *right_child);

void free_node(Node *node);

int magnitude(Node *root);

void copy_node(Node *to, Node *from);

int is_null(Node *node);

/*************
 * FOR STACK *
 *************/
void create_stack(Stack *stack, size_t capacity);

void free_stack(Stack *stack);

int stack_is_full(Stack *stack);

int stack_is_empty(Stack *stack);

void push(Stack *stack, Node item);

Node pop(Stack *stack);

Node peek(Stack *stack);