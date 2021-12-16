#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ID_LEN 8
#define SIZE 1024

typedef struct Cave Cave;
typedef struct Stack Stack;

struct Cave {
	char id[ID_LEN];
	bool visited;
	bool is_big_cave;
	int adj_count;
	Cave *adj[SIZE];
};

struct Stack {
	int top;
	size_t capacity;
	Cave **items;
};

void init_stack(Stack *stack, size_t capacity) {
	stack->top = -1;
	stack->capacity = capacity;
	stack->items = malloc(capacity * sizeof(Cave*));
}

bool is_stack_full(Stack *stack) {
	return stack->top == (int)(stack->capacity - 1);
}

bool is_stack_empty(Stack *stack) {
	return stack->top == -1;
}

void push(Stack *stack, Cave *item) {
	if (!is_stack_full(stack)) {
		stack->items[++stack->top] = item;
	}
}

Cave *pop(Stack *stack) {
	if (is_stack_empty(stack)) {
		return NULL;
	} else {
		return stack->items[stack->top--];
	}
}

Cave *peek(Stack *stack) {
	if (is_stack_empty(stack)) {
		return NULL;
	} else {
		return stack->items[stack->top];
	}
}

int cave_type(char id[ID_LEN]) {
	return ((id[0] >= 'a') && (id[0] <= 'z'));
}

int contains(Cave *caves, char id[ID_LEN], int cave_count) {
	for (int i = 0; i < cave_count; i++) {
		if (strcmp(caves[i].id, id) == 0) {
			return 1;
		}
	}
	return 0;
}

void add_connection(Cave *caves, char id_a[ID_LEN], char id_b[ID_LEN], int cave_count) {

	Cave *a, *b;
	for (int i = 0; i < cave_count; i++) {
		if (strcmp(caves[i].id, id_a) == 0) {
			a = &caves[i];
		}

		if (strcmp(caves[i].id, id_b) == 0) {
			b = &caves[i];
		}
	}

	a->adj[a->adj_count++] = b;
	b->adj[b->adj_count++] = a;
	printf("Connections added between %s and %s.\n", id_a, id_b);
}

int main() {

	FILE *input;
	
	if ((input = fopen("../small_input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	char first_id[ID_LEN];
	char second_id[ID_LEN];
	Cave *caves = malloc(SIZE * sizeof(Cave));
	Cave cave;
	Cave start;
	Cave end;
	int cave_count = 0;
	while (fscanf(input, "%[^-]-%[^\n]\n", first_id, second_id) == 2) {
		// printf("%s -> %s\n", first_id, second_id);

		// if it is a new cave, add it to the list
		if (!contains(caves, first_id, cave_count)) {
			memcpy(cave.id, first_id, sizeof first_id);
			cave.visited = false;
			cave.is_big_cave = cave_type(cave.id);
			cave.adj_count = 0;
			caves[cave_count++] = cave;

			if (strcmp(cave.id, "start") == 0) {
				start = cave;
			} else if (strcmp(cave.id, "end") == 0) {
				end = cave;
			}

			printf("Added new %s cave %s.\n", cave.is_big_cave ? "big" : "small", cave.id);
		}

		if (!contains(caves, second_id, cave_count)) {
			memcpy(cave.id, second_id, sizeof second_id);
			cave.visited = false;
			cave.is_big_cave = cave_type(cave.id);
			cave.adj_count = 0;
			caves[cave_count++] = cave;

			if (strcmp(cave.id, "start") == 0) {
				start = cave;
			} else if (strcmp(cave.id, "end") == 0) {
				end = cave;
			}

			printf("Added new %s cave %s.\n", cave.is_big_cave ? "big" : "small", cave.id);
		}

		add_connection(caves, first_id, second_id, cave_count);
	}

	caves = realloc(caves, cave_count * sizeof(Cave));

	Stack stack;
	init_stack(&stack, cave_count);

	// for (int i = 0; i < cave_count; i++) {
	// 	printf("Cave %s which connects to: ", caves[i].id);
	// 	for (int adj = 0; adj < caves[i].adj_count; adj++) {
	// 		printf("%s ", caves[i].adj[adj]->id);
	// 	}
	// 	printf("\n");
	// }

	free(stack.items);
	free(caves);
	fclose(input);
	return EXIT_SUCCESS;
}