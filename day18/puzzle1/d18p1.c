#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../common/utils.h"

#define true 1
#define false 0

typedef struct Node Node;

struct Node {
    int value;

    Node *left_child;
    Node *right_child;
};

void free_nodes(Node *root) {
    if (root->left_child != NULL) {
        free_nodes(root->left_child);
        free(root->left_child);
    }

    if (root->right_child != NULL) {
        free_nodes(root->right_child);
        free(root->right_child);
    }
}

void print_nodes(Node *root) {
    if (root->left_child == NULL && root->right_child == NULL) {
        printf("%d", root->value);
    } else {
        printf("[");
        print_nodes(root->left_child);
        printf(",");
        print_nodes(root->right_child);
        printf("]");
    }
}

void print_nodes_newline(Node *root) {
    print_nodes(root);
    printf("\n");
}

int is_number(char c) {
    int i = ctoi(c);
    return i >= 0 && i <= 9;
}

void parse_input(char str[100], int *index, Node *node) {

    if (is_number(str[*index])) {
        node->value = ctoi(str[*index]);
        node->left_child = NULL;
        node->right_child = NULL;
        (*index)++;
    } else if (str[*index] == '[') {
        node->left_child = malloc(sizeof(Node));
        node->right_child = malloc(sizeof(Node));
        (*index)++;

        parse_input(str, index, node->left_child);
        (*index)++;

        parse_input(str, index, node->right_child);
        (*index)++;
    } else {
        printf("??? -> %c\n", str[*index]);
    }
}

Node *add(Node *a, Node *b) {
    Node *node = malloc(sizeof(Node));
    node->left_child = a;
    node->right_child = b;
    return node;
}

long magnitude(Node *node) {
    if (node->left_child == NULL && node->right_child == NULL) {
        return (long)node->value;
    }

    long left_val = magnitude(node->left_child);
    long right_val = magnitude(node->right_child);
    return (3 * left_val) + (2 * right_val);
}

int get_left_node(Node *current, Node *explode, Node **prev, int *left) {
    if (current == explode) {
        *left = true;
    }

    if (*left) {
        return true;
    }

    if (current->left_child == NULL && current->right_child == NULL) {
        *prev = current;
    }

    if (current->left_child != NULL) {

        if (get_left_node(current->left_child, explode, prev, left)) {
            return true;
        }

        if (get_left_node(current->right_child, explode, prev, left)) {
            return true;
        }
    }

    return false;
}

int get_right_node(Node *current, Node *explode, int *right) {

    if (current == explode) {
        *right = true;
    } else if (*right && current->left_child == NULL) {
        current->value += explode->value;
        return true;
    }

    if (current->left_child != NULL) {
        if (get_right_node(current->left_child, explode, right)) {
            return true;
        }

        if (get_right_node(current->right_child, explode, right)) {
            return true;
        }
    }

    return false;
}

int explode(Node *node, int depth) {
    
    static Node *root;
    if (depth == 0) {
        root = node;
    }

    if (node->left_child != NULL) {
        if (depth >= 4) {

            int found = false;
            Node *prev = NULL;
            get_left_node(root, node, &prev, &found);
            if (prev != NULL) {
                prev->value += node->left_child->value;
            }

            found = false;
            get_right_node(root, node->right_child, &found);
            node->left_child = NULL;
            node->right_child = NULL;
            node->value = 0;
            return true;
        } else {

            if (explode(node->left_child, depth + 1)) {
                return true;
            }

            if (explode(node->right_child, depth + 1)) {
                return true;
            }
        }
    }
    
    return false;
}

int split(Node *node) {

    if (node->left_child == NULL) {
        
        if (node->value >= 10) {

            node->left_child = malloc(sizeof(Node));
            node->right_child = malloc(sizeof(Node));
            
            node->left_child->left_child = NULL;
            node->left_child->right_child = NULL;
            node->left_child->value = node->value / 2;

            node->right_child->left_child = NULL;
            node->right_child->right_child = NULL;
            node->right_child->value = (node->value / 2) + (node->value % 2);
            return true;
        }
    } else {

        if (split(node->left_child)) {
            return true;
        }
        if (split(node->right_child)) {
            return true;
        }
    }

    return false;
}

void reduce(Node *node) {

    int to_reduce = true;
    while (to_reduce == true) {
        int to_explode = true;
        while (to_explode) {
            to_explode = explode(node, 0);
        }

        to_reduce = split(node);
    }
}

int main() {
    FILE *input;

    // test_input_1         PASS
    // test_input_2         PASS
    // test_input_3         PASS
    // test_input_4         PASS
    // final_test_input     PASS
    if ((input = fopen("../input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char input_str[100][100];
    int count = 0;
    while (fscanf(input, "%s\n", input_str[count]) == 1) {
        count++;
    }

    Node nodes[count];
    int index;
    for (int i = 0; i < count; i++) {
        index = 0;
        parse_input(input_str[i], &index, &nodes[i]);
    }

    Node *result = add(&nodes[0], &nodes[1]);
    reduce(result);
    for (int i = 2; i < count; i++) {
        result = add(result, &nodes[i]);
        reduce(result);
    }

    print_nodes_newline(result);
    printf("Magnitude is: %ld\n", magnitude(result));

    // free_nodes(result);
    fclose(input);
    return EXIT_SUCCESS;
}