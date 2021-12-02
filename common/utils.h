typedef struct {
	int *array;
	size_t used;
	size_t size;
} IntArray;

void init_IntArray(IntArray *a, size_t initialSize) {
	if (initialSize <= 0) {
		initialSize = 1;
	}
	int *temp = malloc(initialSize * sizeof(int));
	if (!temp) {
		exit(EXIT_FAILURE);
	}
	a->array = temp;
	a->used = 0;
	a->size = initialSize;
}

void insert_IntArray(IntArray *a, int element) {
	if (a->used == a->size) {
		a->size += 1;
		int *temp = realloc(a->array, a->size * sizeof(int));
		if (!temp) {
			exit(EXIT_FAILURE);
		}
		a->array = temp;
	}
	a->array[a->used++] = element;
}

void free_IntArray(IntArray *a) {
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}