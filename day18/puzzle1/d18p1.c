#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../common/utils.h"

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

Node *copy_nodes(Node *from) {
    Node *to = malloc(sizeof(Node));
    if (to == NULL) {
        printf("Malloc failed in func: copy_node");
        return NULL;
    }

    to->value = from->value;
    if (from->left_child != NULL) {
        to->left_child = copy_nodes(from->left_child);
    } else {
        to->left_child = NULL;
    }

    if (from->right_child != NULL) {
        to->right_child = copy_nodes(from->right_child);
    } else {
        to->right_child = NULL;
    }

    return to;
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

int is_number(char c) {
    int i = ctoi(c);
    return i >= 0 && i <= 9;
}

void parse_input(char str[100], int index, Node *node) {

    for (int i = index; i < (int)strlen(str); i++) {
        if (is_number(str[i])) {
            node->value = ctoi(str[i]);
            node->left_child = NULL;
            node->right_child = NULL;
            break;
        } else if (str[i] == '[') {
            node->left_child = malloc(sizeof(Node));
            node->right_child = malloc(sizeof(Node));
            i++;

            parse_input(str, i, node->left_child);
            i++;

            parse_input(str, i, node->right_child);
            i++;
        } else {
            printf("???\n");
            break;
        }
    }
}

int main() {
    FILE *input;

    if ((input = fopen("../test_inputs/test_input_1.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char input_str[100][100];
    int count = 0;
    while (fscanf(input, "%s\n", input_str[count]) == 1) {
        count++;
    }

    Node nodes[count];
    for (int i = 0; i < count; i++) {
        parse_input(input_str[i], 0, &nodes[i]);
    }

    fclose(input);
    return EXIT_SUCCESS;
}