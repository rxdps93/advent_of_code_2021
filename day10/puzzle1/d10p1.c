#include <stdio.h>
#include <stdlib.h>

#define OPEN_PRTH '('
#define OPEN_BRKT '['
#define OPEN_BRCE '{'
#define OPEN_ANGL '<'

#define CLOSE_PRTH ')'
#define CLOSE_BRKT ']'
#define CLOSE_BRCE '}'
#define CLOSE_ANGL '>'

#define LINE_SIZE 1024

typedef struct {
	int top;
	size_t capacity;
	char *items;
} Stack;

void initialize_stack(Stack *stack, size_t capacity) {
	stack->top = -1;
	stack->capacity = capacity;
	stack->items = malloc(capacity * sizeof(char));
}

int is_stack_full(Stack *stack) {
	return stack->top == (int)(stack->capacity - 1);
}

int is_stack_empty(Stack *stack) {
	return stack->top == -1;
}

void reset_stack(Stack *stack) {
	stack->top = -1;
}

void push(Stack *stack, char item) {
	if (is_stack_full(stack)) {
		printf("Push aborted; stack is full.\n");
	} else {
		stack->items[++stack->top] = item;
	}
}

char pop(Stack *stack) {
	if (is_stack_empty(stack)) {
		printf("Pop aborted; stack is empty\n");
		return EOF;
	} else {
		return stack->items[stack->top--];
	}
}

char peek(Stack *stack) {
	if (is_stack_empty(stack)) {
		printf("Peek aborted; stack is empty\n");
		return EOF;
	} else {
		return stack->items[stack->top];
	}
}

int is_open(char c) {
	return (c == OPEN_PRTH) || (c == OPEN_ANGL) || (c == OPEN_BRCE) || (c == OPEN_BRKT);
}

int is_close(char c) {
	return (c == CLOSE_PRTH) || (c == CLOSE_ANGL) || (c == CLOSE_BRCE) || (c == CLOSE_BRKT);
}

int is_match(char open, char close) {
	if (open == OPEN_PRTH) {

		return close == CLOSE_PRTH;

	} else if (open == OPEN_ANGL) {

		return close == CLOSE_ANGL;

	} else if (open == OPEN_BRCE) {

		return close == CLOSE_BRCE;

	} else if (open == OPEN_BRKT) {

		return close == CLOSE_BRKT;

	} else {

		return -1;
	}
}

int syntax_score(char c) {
	int score;
	switch(c) {
		case CLOSE_PRTH:
			score = 3;
			break;
		case CLOSE_BRKT:
			score = 57;
			break;
		case CLOSE_BRCE:
			score = 1197;
			break;
		case CLOSE_ANGL:
			score = 25137;
			break;
		default: 
			score = 0;
	}

	return score;
}

char get_opposite(char c) {
	char opposite;
	switch(c) {
		case OPEN_PRTH:
			opposite = CLOSE_PRTH;
			break;
		case CLOSE_PRTH:
			opposite = OPEN_PRTH;
			break;
		case OPEN_BRKT:
			opposite = CLOSE_BRKT;
			break;
		case CLOSE_BRKT:
			opposite = OPEN_BRKT;
			break;
		case OPEN_BRCE:
			opposite = CLOSE_BRCE;
			break;
		case CLOSE_BRCE:
			opposite = OPEN_BRCE;
			break;
		case OPEN_ANGL:
			opposite = CLOSE_ANGL;
			break;
		case CLOSE_ANGL:
			opposite = OPEN_ANGL;
			break;
		default: 
			opposite = EOF;
	}
	return opposite;
}

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file\n");
		exit(EXIT_FAILURE);
	}

	Stack open_chars;
	initialize_stack(&open_chars, LINE_SIZE);
	char c;
	int score = 0;
	int corrupted = 0;
	while (fscanf(input, "%c", &c) == 1) {
		if (!corrupted) {
			if (c == '\n') {
				printf("Incomplete line...\n");
				reset_stack(&open_chars);
			} else {
				if (is_open(c)) {
					push(&open_chars, c);
				} else if (is_close(c)) {
					if (is_match(peek(&open_chars), c)) {
						pop(&open_chars);
					} else {
						score += syntax_score(c);
						corrupted = 1;
						printf("Expected %c, but found %c instead.\n", get_opposite(peek(&open_chars)), c);
					}
				} else {
					printf("uh oh\n");
				}
			}
		} else if (corrupted) {
			if (c == '\n') {
				corrupted = 0;
				reset_stack(&open_chars);
			}
		}
	}

	printf("Score: %d\n", score);
	fclose(input);
	return EXIT_SUCCESS;
}