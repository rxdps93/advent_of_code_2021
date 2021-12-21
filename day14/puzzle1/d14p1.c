#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ARR 1024
#define STEPS 10

typedef struct Element Element;
typedef struct Instruction Instruction;

struct Element {
    char elmnt;
    Element *next;
    Element *q_in;
};

struct Instruction {
    char pair[3];
    char elmnt;
};

void print_list(Element *element) {

    while (element != NULL) {
        printf("%c", element->elmnt);
        element = element->next;
    }
    printf("\n");
}

int list_size(Element *element) {

    int size = 0;
    while (element != NULL) {
        element = element->next;
        size++;
    }
    return size;
}

void free_list(Element *element) {
    
    Element *next;
    while (element != NULL) {
        next = element->next;
        free(element);
        free(element->q_in);
        element = next;
    }
}

void process_queue(Element *element) {
    
    while (element != NULL) {

        if (element->q_in != NULL) {
            element->next = element->q_in;
            element->q_in = NULL;
        }

        element = element->next;
    }
}

void queue_insert(Element *insert_after, char elmnt) {

    if (insert_after != NULL && insert_after->next != NULL) {
        Element *element = malloc(sizeof(Element));
        element->elmnt = elmnt;
        element->next = insert_after->next;
        insert_after->q_in = element;
    }
}

int find_pair(Element *element, char pair[3], Element *pairs[ARR]) {

    int size = 0;
    
    while (element != NULL) {
        if ((element->next != NULL) && (pair[0] == element->elmnt) && (pair[1] == element->next->elmnt)) {
            pairs[size++] = element;
        }

        element = element->next;
    }

    return size;
}

long count_elements(char element_id, Element *element) {

    long size = 0;
    while (element != NULL) {
        if (element->elmnt == element_id) {
            size++;
        }
        element = element->next;
    }
    return size;
}

void log_unique_element(char elmnt, char element_ids[ARR], int *unique_elements) {

    for (int i = 0; i < *unique_elements; i++) {
        if (element_ids[i] == elmnt) {
            return;
        }
    }

    element_ids[(*unique_elements)++] = elmnt;
}

int main() {

    FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

    char element_ids[ARR];
    int unique_elements = 0;

    Element *head = NULL;
    Element *tail = NULL;
    char c;
    fscanf(input, "%c", &c);
    while (c != '\n') {

        if (head == NULL) {
            head = tail = malloc(sizeof(Element));
            tail->elmnt = c;
            tail->next = NULL;
        } else {
            tail->next = malloc(sizeof(Element));
            tail->next->elmnt = c;
            tail = tail->next;
        }

        log_unique_element(c, element_ids, &unique_elements);

        fscanf(input, "%c", &c);
    }

    Instruction iset[ARR];
    int icount = 0;
    char pair[3];
    while (fscanf(input, "%s -> %c\n", pair, &c) == 2) {
        memcpy(iset[icount].pair, pair, sizeof pair);
        iset[icount].elmnt = c;
        icount++;

        log_unique_element(c, element_ids, &unique_elements);
    }

    int size = 0;
    Element *insert_pairs[ARR];
    for (int step = 0; step < STEPS; step++) {
        for (int i = 0; i < icount; i++) {

            size = find_pair(head, iset[i].pair, insert_pairs);
            for (int p = 0; p < size; p++) {
                queue_insert(insert_pairs[p], iset[i].elmnt);
            }
        }
        process_queue(head);
    }

    long count = 0;
    long max_elmnt = INT_MIN;
    long min_elmnt = INT_MAX;
    int max_index = -1;
    int min_index = -1;
    size = 0;
    for (int i = 0; i < unique_elements; i++) {
        count = count_elements(element_ids[i], head);

        if (count > max_elmnt) {
            max_elmnt = count;
            max_index = i;
        }

        if (count < min_elmnt) {
            min_elmnt = count;
            min_index = i;
        }
    }

    printf("Most Common (%c: %ld) minus Least Common (%c: %ld): %ld\n",
        element_ids[max_index], max_elmnt,
        element_ids[min_index], min_elmnt,
        max_elmnt - min_elmnt);

    free_list(head);
    fclose(input);
    return EXIT_SUCCESS;
}