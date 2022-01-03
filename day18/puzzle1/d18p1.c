#include <stdio.h>
#include <stdlib.h>
#include "../../common/stringbuilder.h"

typedef struct Node Node;

struct Node {

    int left_value;
    int right_value;

    Node *parent;

    Node *left_child;
    Node *right_child;
};

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

void print_tree(Node *root) {

    StringBuilder *sb = sb_create();
}

void explode(Node *root) {

}

void split(Node *root) {

}

void add(Node *root, Node *to_add) {

}

int main() {

    FILE *input;

    if ((input = fopen("../test_input_1.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fclose(input);
    return EXIT_SUCCESS;
}