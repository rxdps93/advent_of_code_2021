#include <stdarg.h>
#include <math.h>

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

void mul_insert_IntArray(IntArray *a, int element, size_t mul_factor) {
		if (a->used == a->size) {
		a->size *= mul_factor;
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

int ctoi(char c) {
	return (int)(c - '0');
}

int ctox(char c) {

	int val = ctoi(c);
	if (c > '9') {
		val -= 7;
	}

	return val;
}

long long binary_to_llong_range(int *binary, int index, int number) {

	long long val = 0;
	for (int i = 0; i < number; i++) {
		val += binary[index + i] * (pow(2, (number - 1) - i));
	}

	return val;
}

int binary_to_int_range(int *binary, int index, int number) {

	int val = 0;
	for (int i = 0; i < number; i++) {
		val += binary[index + i] * (pow(2, (number - 1) - i));
	}

	return val;
}

int binary_to_int(int num_bits, ...) {

	va_list valist;
	int val = 0;

	va_start(valist, num_bits);

	for (int i = 0; i < num_bits; i++) {
		val += va_arg(valist, int) * (pow(2, (num_bits - 1) - i) );
	}

	return val;
}