#include <stdio.h>
#include <stdlib.h>
#include "d23_structs.h"

typedef struct BurrowQueue {
    burrow_t *items;
    int size;
    size_t capacity;
    int head;
    int tail;
} burrow_queue_t;

void queue_init(burrow_queue_t *q, size_t capacity) {
    q->items = (burrow_t *)malloc(capacity * sizeof(burrow_t));
    q->capacity = capacity;
    q->size = 0;
    q->head = -1;
    q->tail = -1;
}

void queue_free(burrow_queue_t *q) {
    free(q->items);
}

int queue_is_full(burrow_queue_t *q) {
    return q->tail == (int)(q->capacity - 1);
}

int queue_is_empty(burrow_queue_t *q) {
    return q->tail == -1;
}

void queue_add(burrow_queue_t *q, burrow_t add) {
    if (queue_is_full(q)) {
        q->capacity *= 2;
        q->items = (burrow_t *)realloc(q->items, q->capacity * sizeof(burrow_t));
    }

    if (queue_is_empty(q)) {
        q->tail = q->head = 0;
        q->items[0] = add;
    } else {
        int i = q->tail;

        while (add.energy < q->items[i].energy) {
            q->items[(i + 1) % (int)q->capacity] = q->items[i];
            i = (i - 1 + (int)q->capacity) % (int)q->capacity;
            
            if ((i + 1) % (int)q->capacity == q->head) {
                break;
            }
        }

        i = (i + 1) % (int)q->capacity;
        q->items[i] = add;

        q->tail = (q->tail + 1) % (int)q->capacity;
    }
}

int queue_remove(burrow_queue_t *q, burrow_t *dst) {

    if (queue_is_empty(q)) {
        printf("Queue empty; cannot remove\n");
        dst = NULL;
        return 0;
    } else {
        burrow_t tmp = q->items[q->head];

        if (q->tail == q->head) {
            q->head = -1;
            q->tail = -1;
        } else {
            q->head = (q->head + 1) % (int)q->capacity;
        }

        *dst = tmp;
        return 1;
    }
}

void print_queue(burrow_queue_t *q) {
    if (queue_is_empty(q)) {
        printf("Queue is empty\n");
    } else {
        int i = q->head;
        burrow_t b;
        while (i != q->tail) {
            b = q->items[i];
            printf("%d\n", b.energy);
            i = (i + 1) % (int)q->capacity;
        }

        b = q->items[i];
        printf("%d\n", b.energy);
    }
}