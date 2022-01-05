#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "../../common/utils.h"
#include "../../common/stringbuilder.h"
#include "../utils/day18.h"

#define LEFT_VAL "[0-9]+,"
#define RIGHT_VAL ",[0-9]+"

char *node_to_string(Node *root) {

    StringBuilder *sb = sb_create();
    sb_append_char(sb, '[');

    if (root->left_child != NULL) {
        sb_append_str(sb, node_to_string(root->left_child), 0);
    } else {
        sb_append_int(sb, root->left_value);
    }

    sb_append_char(sb, ',');

    if (root->right_child != NULL) {
        sb_append_str(sb, node_to_string(root->right_child), 0);
    } else {
        sb_append_int(sb, root->right_value);
    }

    sb_append_char(sb, ']');
    char *str = sb_get(sb);
    sb_free(sb);
    return str;
}

int str_match(char *to_check, char *regex_str) {
    int status;
    regex_t regex;

    status = regcomp(&regex, regex_str, REG_EXTENDED);
    if (status) {
        printf("Could not establish regex\n");
        return REG_NOMATCH;
    }

    status = regexec(&regex, to_check, 0, NULL, 0);

    regfree(&regex);
    return status;
}

Node parse_input(char str[100]) {

    if (str == NULL || strlen(str) == 0) {
        Node ret;
        init_node(&ret);
        return ret;
    }

    Stack stack;
    create_stack(&stack, 1024);
    Node current;
    init_node(&current);
    push(&stack, current);

    printf("Checking: %s\n", str);
    for (int i = 0; i < (int)strlen(str); i++) {
        
        if (str[i] == '[') {
            printf("\tNew node; push to stack\n");
            current = pop(&stack);
            Node new_node;
            create_node(&new_node, -1, -1, &current, NULL, NULL);
            push(&stack, new_node);
        } else if (str[i] ==']') {
            printf("\tEnd node; pop from stack\n");
            Node child = pop(&stack);

            printf("child: %d, %d; current: %d, %d\n", child.left_value, child.right_value, current.left_value, current.right_value);

            if (stack_is_empty(&stack)) {
                copy_node(&current, &child);
                break;
            }

            current = peek(&stack);

            if (current.left_value == -1 && current.left_child == NULL) {
                copy_node(current.left_child, &child);
            } else if (current.right_value == -1 && current.right_child == NULL) {
                copy_node(current.right_child, &child);
            }
        } else {
            char sub_str[3];
            strncpy(sub_str, &str[i], 2);
            sub_str[2] = '\0';

            if (str_match(sub_str, LEFT_VAL) == 0) {
                printf("\tLeft Value: %d\n", ctoi(str[i]));
                current = pop(&stack);
                current.left_value = ctoi(str[i]);
                push(&stack, current);
            } else if (str_match(sub_str, RIGHT_VAL) == 0) { 
                printf("\tRight Value: %d\n", ctoi(str[i + 1]));
                current = pop(&stack);
                current.right_value = ctoi(str[i + 1]);
                push(&stack, current);
            }
        }
    }

    free_stack(&stack);
    return current;
}

Node add(Node a, Node b) {

    printf("%s\n", node_to_string(&a));
    printf("%s\n", node_to_string(&b));

    if ((a.left_value == -1 && a.right_value == -1)) {
        printf("first\n");
        return b;
    }

    if ((b.left_value == -1 && b.right_value == -1)) {
        printf("second\n");
        return a;
    }

    Node node;
    init_node(&node);

    node.left_child = &a;
    node.right_child = &b;
    a.parent = &node;
    b.parent = &node;

    printf("third\n");
    return node;
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
    };

    Node root;
    init_node(&root);
    for (int i = 0; i < count; i++) {
        Node node = parse_input(input_str[i]);
        printf("%s\n", node_to_string(&root));
        printf("%s\n", node_to_string(&node));
        root = add(root, node);
        printf("%s\n", node_to_string(&root));
    }
    printf("%s\n", node_to_string(&root));

    fclose(input);
    return EXIT_SUCCESS;
}