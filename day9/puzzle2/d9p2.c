#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../common/utils.h"

#define LINE_SIZE 10

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
	HeightRating *items;
	int size;
	int front_ptr;
	int rear_ptr;
} Queue;

void initialize_queue(Queue *queue, int qsize) {
	HeightRating *temp = malloc(qsize * sizeof(HeightRating));
	if (!temp) {
		exit(EXIT_FAILURE);
	}
	queue->items = temp;
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

void enqueue(Queue *queue, HeightRating item) {

	if (is_full(queue)) {
		printf("Enqueue aborted; queue is full.\n");
	} else {
		if (queue->front_ptr == -1) {
			queue->front_ptr = 0;
		}

		queue->rear_ptr++;
		queue->items[queue->rear_ptr] = item;
		printf("Added: %d\n", queue->items[queue->rear_ptr].height);
	}
}

HeightRating *dequeue(Queue *queue) {
	
	HeightRating *item;
	if (is_empty(queue)) {
		printf("Dequeue aborted; queue is full.\n");
		item = NULL;
	} else {
		item = &queue->items[queue->front_ptr];
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
			printf("[%d] ", queue->items[i].height);
		}
		printf("\n");
	}
}

int find_low_points(HeightRating ratings[LINE_SIZE][LINE_SIZE], int row_ptr) {
	int low_point_count = 0;
	for (int row = 0; row < row_ptr; row++) {

		for (int col = 0; col < LINE_SIZE; col++) {

			// Check up
			if (row != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row - 1][col].height;
				ratings[row][col].up = &ratings[row - 1][col];
			}

			// Check down
			if (ratings[row][col].is_low_point && row < row_ptr - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row + 1][col].height;
				ratings[row][col].down = &ratings[row + 1][col];
			}

			// Check left
			if (ratings[row][col].is_low_point && col != 0) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col - 1].height;
				ratings[row][col].left = &ratings[row][col - 1];
			}

			// Check right
			if (ratings[row][col].is_low_point && col < LINE_SIZE - 1) {
				ratings[row][col].is_low_point = ratings[row][col].height < ratings[row][col + 1].height;
				ratings[row][col].right = &ratings[row][col + 1];
			}

			if (ratings[row][col].is_low_point) {
				low_point_count++;
			}
		}
	}

	return low_point_count;
}

int map_basin(HeightRating node, int queue_size) {

	int size = 1;
	Queue queue;
	initialize_queue(&queue, queue_size);
	enqueue(&queue, node);

	while (!is_empty(&queue)) {
		print_queue(&queue);

		HeightRating current_node = *dequeue(&queue);
		printf("Visited %d\n", current_node.height);
		if (current_node.height != 9 && !current_node.is_mapped) {
			current_node.is_mapped = true;
			size++;
		}

		if (current_node.up) {
			if (!current_node.up->is_mapped && current_node.up->height != 9) {
				enqueue(&queue, *current_node.up);
			} else {
				printf("up not valid!\n");
			}
		} else {
			printf("up doesn't exist!\n");
		}

		if (current_node.down) {
			if (!current_node.down->is_mapped && current_node.down->height != 9) {
				enqueue(&queue, *current_node.down);
			} else {
				printf("down not valid!\n");
			}
		} else {
			printf("down doesn't exist!\n");
		}

		if (current_node.left) {
			if (!current_node.left->is_mapped && current_node.left->height != 9) {
				enqueue(&queue, *current_node.left);
			} else {
				printf("left not valid!\n");
			}
		} else {
			printf("left doesn't exist!\n");
		}

		if (current_node.right) {
			if (!current_node.right->is_mapped && current_node.right->height != 9) {
				enqueue(&queue, *current_node.right);
			} else {
				printf("right not valid!\n");
			}
		} else {
			printf("right doesn't exist!\n");
		}

		print_queue(&queue);
		printf("holding...press enter to go on\n");
		getchar();
	}

	printf("basin mapped; size = %d\n", size);

	return size;
}

int main() {

	FILE *input;
	
	if ((input = fopen("../test_input.txt", "r")) == NULL) {
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
	int basin = 0;
	for (int row = 0; row < row_ptr; row++) {
		for (int col = 0; col < LINE_SIZE; col++) {
			if (ratings[row][col].is_low_point) {
				basin_sizes[basin++] = map_basin(ratings[row][col], LINE_SIZE * row_ptr);
			}
		}
	}

	fclose(input);
	return EXIT_SUCCESS;
}