#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ID_LEN 8
#define SIZE 1024

typedef struct Cave Cave;
typedef struct Queue Queue;
typedef struct Path Path;

struct Cave {
	char id[ID_LEN];
	bool is_big_cave;
	int adj_count;
	Cave *adj[SIZE];
};

struct Path {
	Cave **path;
	int path_size;
	bool double_visit;
	size_t capacity;
};

struct Queue {
	int front_ptr;
	int rear_ptr;
	int size;
	size_t capacity;
	Path *items;
};

void init_queue(Queue *q, size_t capacity) {
	q->items = malloc(capacity * sizeof(Path));
	q->capacity = capacity;
	q->size = 0;
	q->front_ptr = -1;
	q->rear_ptr = -1;
}

void init_path(Path *p, size_t capacity) {
	p->path = malloc(capacity * sizeof(Cave *));
	p->path_size = 0;
	p->double_visit = false;
	p->capacity = capacity;
}

bool queue_is_full(Queue *q) {
	return q->rear_ptr == (int)(q->capacity - 1);
}

bool queue_is_empty(Queue *q) {
	return q->front_ptr == -1;
}

void path_add(Path *p, Cave *c) {

	if (p->path_size == (int)p->capacity) {

		p->capacity *= 2;
		p->path = realloc(p->path, p->capacity * sizeof(Cave *));
	}
	p->path[p->path_size++] = c;
}

Cave *path_get_last(Path p) {
	if (p.path_size == 0) {
		return NULL;
	}
	return p.path[p.path_size - 1];
}

void print_path(Path path) {
	for (int i = 0; i < path.path_size; i++) {
		printf("%s,", path.path[i]->id);
	}
	printf("\n");
}

void enqueue(Queue *q, Path item) {
	if (queue_is_full(q)) {
		q->capacity *= 2;
		q->items = realloc(q->items, q->capacity * sizeof(Path));
	}

	if (q->front_ptr == -1) {
		q->front_ptr = 0;
	}

	q->rear_ptr++;
	q->items[q->rear_ptr] = item;
	q->size++;
}

Path dequeue(Queue *q) {
	Path item = q->items[q->front_ptr];
	q->front_ptr++;
	q->size--;
	
	if (q->front_ptr > q->rear_ptr) {
		q->front_ptr = -1;
		q->rear_ptr = -1;
	}

	return item;
}

Path queue_peek(Queue *q) {
	return q->items[q->front_ptr];
}

bool path_contains(Path *path, Cave *cave) {

	if (!cave->is_big_cave) {

		if (strcmp(cave->id, "start") == 0) {
			return true;
		}

		for (int i = 0; i < path->path_size; i++) {

			if (strcmp(path->path[i]->id, cave->id) == 0) {

				if (path->double_visit || strcmp(cave->id, "end") == 0) {
					return true;
				} else {
					return false;
				}
			}
		}
	}

	return false;
}

bool has_double_visit(Path *path) {
	for (int a = 0; a < path->path_size; a++) {

		for (int b = 0; b < path->path_size; b++) {
			
			if (a != b && (!path->path[a]->is_big_cave && !path->path[b]->is_big_cave) && strcmp(path->path[a]->id, path->path[b]->id) == 0) {
				return true;
			}
		}
	}
	return false;
}

void path_copy(Path *old_path, Path *new_path) {
	memcpy(new_path->path, old_path->path, old_path->capacity * sizeof old_path->path);
	new_path->capacity = old_path->capacity;
	new_path->double_visit = has_double_visit(old_path);
	new_path->path_size = old_path->path_size;
}

int cave_type(char id[ID_LEN]) {
	return !((id[0] >= 'a') && (id[0] <= 'z'));
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
}

void print_queue(Queue *queue) {

	if (!queue_is_empty(queue)) {
		printf("START QUEUE (%d, %d, size: %d)\n", queue->front_ptr, queue->rear_ptr, queue->size);

		print_path(queue_peek(queue));

		for (int i = queue->front_ptr; i < queue->rear_ptr; i++) {
			print_path(queue->items[i]);
		}

		printf("END QUEUE\n");
	} else {
		printf("Cannot print queue: it is empty\n");
	}
}

int path_search(Cave *start, Cave *end, int size) {
	
	Queue queue;
	init_queue(&queue, SIZE);

	Path path;
	init_path(&path, size);

	path_add(&path, start);
	enqueue(&queue, path);

	Cave *cave;
	int path_count = 0;
	while(!queue_is_empty(&queue)) {
		path = dequeue(&queue);
		path.double_visit = has_double_visit(&path);
		cave = path_get_last(path);

		if (strcmp(cave->id, end->id) == 0) {
			path_count++;
		} else {

			for (int i = 0; i < cave->adj_count; i++) {

				if (!path_contains(&path, cave->adj[i])) {

					Path new_path;
					init_path(&new_path, path.capacity);
					path_copy(&path, &new_path);

					path_add(&new_path, cave->adj[i]);
					new_path.double_visit = has_double_visit(&new_path);
					enqueue(&queue, new_path);
				}
			}
		}
	}

	return path_count;
}

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
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

		// if it is a new cave, add it to the list
		if (!contains(caves, first_id, cave_count)) {
			memcpy(cave.id, first_id, sizeof first_id);
			cave.is_big_cave = cave_type(cave.id);
			cave.adj_count = 0;
			caves[cave_count++] = cave;

			if (strcmp(cave.id, "start") == 0) {
				start = cave;
			} else if (strcmp(cave.id, "end") == 0) {
				end = cave;
			}
		}

		if (!contains(caves, second_id, cave_count)) {
			memcpy(cave.id, second_id, sizeof second_id);
			cave.is_big_cave = cave_type(cave.id);
			cave.adj_count = 0;
			caves[cave_count++] = cave;
		}

		add_connection(caves, first_id, second_id, cave_count);
	}

	caves = realloc(caves, cave_count * sizeof(Cave));

	for (int i = 0; i < cave_count; i++) {
		if (strcmp(caves[i].id, "start") == 0) {
			start = caves[i];
		} else if (strcmp(caves[i].id, "end") == 0) {
			end = caves[i];
		}
	}

	int num_paths = path_search(&start, &end, cave_count);
	printf("There are %d paths\n", num_paths);

	free(caves);
	fclose(input);
	return EXIT_SUCCESS;
}