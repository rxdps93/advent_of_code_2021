#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ID_LEN 8
#define SIZE 1024

typedef struct Cave Cave;
typedef struct Queue Queue;

struct Cave {
	char id[ID_LEN];
	bool visited;
	bool is_big_cave;
	int adj_count;
	Cave *adj[SIZE];
};

struct Queue {
	int front_ptr;
	int rear_ptr;
	size_t capacity;
	Cave *items[SIZE][SIZE];
};

void init_queue(Queue *q, size_t capacity) {
	q->capacity = capacity;
	q->front_ptr = -1;
	q->rear_ptr = -1;
}

bool is_queue_full(Queue *q) {
	return q->rear_ptr == (int)(q->capacity - 1);
}

bool is_queue_empty(Queue *q) {
	return q->front_ptr == -1;
}

void enqueue(Queue *q, Cave **item) {
	if (is_queue_full(q)) {
		printf("Enqueue aborted; queue is full.\n");
	} else {
		if (q->front_ptr == -1) {
			q->front_ptr = 0;
		}

		q->rear_ptr++;
		memcpy(q->items[q->rear_ptr], item, sizeof(Cave *));
	}
}

Cave **dequeue(Queue *q) {
	Cave **item;
	if (is_queue_empty(q)) {
		printf("Dequeue aborted; queue is full.\n");
		item = NULL;
	} else {
		item = q->items[q->front_ptr];
		q->front_ptr++;
		
		if (q->front_ptr > q->rear_ptr) {
			q->front_ptr = -1;
			q->rear_ptr = -1;
		}
	}

	return item;
}

Cave **queue_peek(Queue *q) {
	if (is_queue_empty(q)) {
		return NULL;
	} else {
		return q->items[q->front_ptr];
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

void reset_cave(Cave *caves, int size) {
	for (int i = 0; i < size; i++) {
		caves[i].visited = false;
	}
}

void print_path(Cave **path, int size) {
	for (int i = 0; i < size; i++) {
		printf("%s,", path[i]->id);
	}
	printf("\n");
}

int path_search(Cave *graph, Cave *start, Cave *end, int size) {
	
	Queue q;
	init_queue(&q, size);
	printf("here\n");

	int path_size = 0;
	Cave **path = malloc(size * sizeof(Cave));
	path[path_size++] = start;

	enqueue(&q, path);
	
	Cave **new_path;
	Cave *current;
	int num_paths = 0;
	while (!is_queue_empty(&q)) {
		printf("start\n");
		path = dequeue(&q);
		current = path[path_size - 1];

		if (!current->is_big_cave) {
			current->visited = true;
			printf("visited\n");
		}

		if (strcmp(current->id, end->id) == 0) {
			printf("done\n");
			num_paths++;
			print_path(path, path_size);
		}

		for (int i = 0; i < current->adj_count; i++) {
			if (!current->adj[i]->visited) {
				printf("new\n");
				new_path = malloc(size * sizeof(Cave*));
				new_path = path;
				new_path[path_size++] = current->adj[i];
			}
		}
	}

	return num_paths;
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

	int num_paths = path_search(caves, &start, &end, cave_count);
	printf("There are %d paths\n", num_paths);

	// for (int i = 0; i < cave_count; i++) {
	// 	printf("Cave %s which connects to: ", caves[i].id);
	// 	for (int adj = 0; adj < caves[i].adj_count; adj++) {
	// 		printf("%s ", caves[i].adj[adj]->id);
	// 	}
	// 	printf("\n");
	// }

	free(caves);
	fclose(input);
	return EXIT_SUCCESS;
}