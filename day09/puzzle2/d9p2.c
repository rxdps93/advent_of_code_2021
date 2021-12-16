#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../common/utils.h"

#define LINE_SIZE 100

typedef struct HeightRating HeightRating;

struct HeightRating {
	int height;
	bool is_low_point;

	bool is_mapped;
	HeightRating *up;
	HeightRating *down;
	HeightRating *left;
	HeightRating *right;
};

typedef struct {
	HeightRating *items[LINE_SIZE * LINE_SIZE];
	int size;
	int front_ptr;
	int rear_ptr;
} Queue;

int comp_int_desc(const void *a, const void *b) {
	return *(int*)b - *(int*)a;
}

void initialize_queue(Queue *queue, int qsize) {
	queue->size = qsize;
	queue->front_ptr = -1;
	queue->rear_ptr = -1;
}

int is_full(Queue *queue) {
	return queue->rear_ptr == queue->size - 1;
}

int is_empty(Queue *queue) {
	return queue->rear_ptr == -1;
}

void enqueue(Queue *queue, HeightRating *item) {

	if (is_full(queue)) {
		printf("Enqueue aborted; queue is full.\n");
	} else {
		if (queue->front_ptr == -1) {
			queue->front_ptr = 0;
		}

		queue->rear_ptr++;
		queue->items[queue->rear_ptr] = item;
	}
}

HeightRating *dequeue(Queue *queue) {
	
	HeightRating *item;
	if (is_empty(queue)) {
		printf("Dequeue aborted; queue is full.\n");
		item = NULL;
	} else {
		item = queue->items[queue->front_ptr];
		queue->front_ptr++;
		
		if (queue->front_ptr > queue->rear_ptr) {
			queue->front_ptr = -1;
			queue->rear_ptr = -1;
		}
	}

	return item;
}

void print_queue(Queue *queue) {

	if (is_empty(queue)) {
		printf("Queue is empty\n");
	} else {
		printf("queue (f=%d, r=%d): ", queue->front_ptr, queue->rear_ptr + 1);
		for (int i = queue->front_ptr; i < queue->rear_ptr + 1; i++) {
			printf("[%d] ", queue->items[i]->height);
		}
		printf("\n");
	}
}

void build_graph(HeightRating ratings[LINE_SIZE][LINE_SIZE], int row_ptr) {

	for (int row = 0; row < row_ptr; row++) {
		for (int col = 0; col < LINE_SIZE; col++) {
			// Assign up
			if (row != 0) {
				ratings[row][col].up = &ratings[row - 1][col];
			}

			// Assign down
			if (row < row_ptr - 1) {
				ratings[row][col].down = &ratings[row + 1][col];
			}

			// Assign left
			if (col != 0) {
				ratings[row][col].left = &ratings[row][col - 1];
			}

			// Assign right
			if (col < LINE_SIZE - 1) {
				ratings[row][col].right = &ratings[row][col + 1];
			}
		}
	}
}

int find_low_points(HeightRating ratings[LINE_SIZE][LINE_SIZE], int row_ptr) {
	int low_point_count = 0;
	for (int row = 0; row < row_ptr; row++) {

		for (int col = 0; col < LINE_SIZE; col++) {

			// Check up
			if (row != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row - 1][col].height;
			}

			// Check down
			if (ratings[row][col].is_low_point && row < row_ptr - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row + 1][col].height;
			}

			// Check left
			if (ratings[row][col].is_low_point && col != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col - 1].height;
			}

			// Check right
			if (ratings[row][col].is_low_point && col < LINE_SIZE - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col + 1].height;
			}

			if (ratings[row][col].is_low_point) {
				low_point_count++;
			}
		}
	}

	return low_point_count;
}

int map_basin(HeightRating *node, int queue_size) {

	int size = 0;
	Queue queue;
	initialize_queue(&queue, queue_size);
	enqueue(&queue, node);

	while (!is_empty(&queue)) {

		HeightRating *current_node = dequeue(&queue);
		if (current_node->height != 9 && !current_node->is_mapped) {
			size++;
		}
		current_node->is_mapped = true;

		if (current_node->up) {
			if (!current_node->up->is_mapped && current_node->up->height != 9) {
				enqueue(&queue, current_node->up);
			}
		}

		if (current_node->down) {
			if (!current_node->down->is_mapped && current_node->down->height != 9) {
				enqueue(&queue, current_node->down);
			}
		}

		if (current_node->left) {
			if (!current_node->left->is_mapped && current_node->left->height != 9) {
				enqueue(&queue, current_node->left);
			}
		}

		if (current_node->right) {
			if (!current_node->right->is_mapped && current_node->right->height != 9) {
				enqueue(&queue, current_node->right);
			}
		}
	}

	return size;
}

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

	HeightRating ratings[LINE_SIZE][LINE_SIZE];
	HeightRating rating;
	char height;
	int row_ptr = 0;
	int col_ptr = 0;
	while (fscanf(input, "%c", &height) == 1) {

		if (height != '\n') {
			rating.height = ctoi(height);
			rating.is_low_point = true;
			rating.is_mapped = false;
			rating.up = NULL;
			rating.down = NULL;
			rating.left = NULL;
			rating.right = NULL;
			ratings[row_ptr][col_ptr++] = rating;
		} else {
			row_ptr++;
			col_ptr = 0;
		}
	}

	int basin_sizes[find_low_points(ratings, row_ptr)];
	build_graph(ratings, row_ptr);
	int basin = 0;
	for (int row = 0; row < row_ptr; row++) {
		for (int col = 0; col < LINE_SIZE; col++) {
			if (ratings[row][col].is_low_point) {
				basin_sizes[basin++] = map_basin(&ratings[row][col], LINE_SIZE * row_ptr);
			}
		}
	}

	qsort(basin_sizes, basin, sizeof(int), comp_int_desc);

	printf("Three largest basins: %d, %d, %d\n", basin_sizes[0], basin_sizes[1], basin_sizes[2]);
	printf("The answer is: %lu\n", (long)basin_sizes[0] * (long)basin_sizes[1] * (long)basin_sizes[2]);
	fclose(input);
	return EXIT_SUCCESS;
}