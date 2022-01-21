#include <string.h>

#define COL_ROOM_A  2
#define COL_ROOM_B  4
#define COL_ROOM_C  6
#define COL_ROOM_D  8
#define ROW_HALL    0
#define ROW_RM_TOP  1
#define ROW_RM_BTM  2

#define ROOM_TO_HALL 0
#define HALL_TO_ROOM 1

#define NROW 3
#define NCOL 11

typedef struct burrow burrow_t;
typedef struct move move_t;

typedef enum {
    UNKNOWN = -1,
    BLOCKED = -1,
    EMPTY = 0,
    AMBER = 1,
    BRONZE = 10,
    COPPER = 100,
    DESERT = 1000
} type_t;

struct burrow {
    int energy;
    type_t layout[NROW][NCOL];
};

struct move {
    int move_type;
    int hall;
    int room_row;
    int room_col;
};

type_t get_room_type_from_num(int room_num) {
    type_t rooms[4] = { AMBER, BRONZE, COPPER, DESERT };
    if (room_num < 0 || room_num >= 4) {
        return UNKNOWN;
    } else {
        return rooms[room_num];
    }
}

type_t get_room_type_from_col(int room_col) {
    int num = (room_col / 2) - 1;
    if (2 * (num + 1) == room_col) {
        return get_room_type_from_num(num);
    } else {
        return UNKNOWN;
    }
}

int get_room_from_type(type_t type) {
    switch (type) {
        case AMBER:
            return 0;
        case BRONZE:
            return 1;
        case COPPER:
            return 2;
        case DESERT:
            return 3;
    }

    return -1;
}

burrow_t get_finished_burrow(void) {
    burrow_t b;
    b.energy = -1;

    for (int i = 0; i < NCOL; i++) {
        b.layout[ROW_HALL][i] = EMPTY;
    }

    for (int r = ROW_RM_TOP; r <= ROW_RM_BTM; r++) {
        for (int c = 0; c < NCOL; c++) {
            b.layout[r][c] = get_room_type_from_col(c);
        }
    }

    return b;
}

type_t char_to_type(char c) {
    switch (c) {
        case '.':
            return EMPTY;
        case 'A':
            return AMBER;
        case 'B':
            return BRONZE;
        case 'C':
            return COPPER;
        case 'D':
            return DESERT;
        case '#':
        case ' ':
            return BLOCKED;
    }

    return UNKNOWN;
}

char type_char(type_t type) {
    switch (type) {
        case EMPTY:
            return '.';
        case AMBER:
            return 'A';
        case BRONZE:
            return 'B';
        case COPPER:
            return 'C';
        case DESERT:
            return 'D';
        case BLOCKED:
            return ' ';
    }

    return '?';
}

void burrow_to_string(burrow_t *b, char out[NROW][NCOL + 1]) {

    for (int row = 0; row < NROW; row++) {
        for (int col = 0; col < NCOL; col++) {
            out[row][col] = type_char(b->layout[row][col]);
        }
        out[row][NCOL] = '\0';
    }
}

void print_burrow(burrow_t *b) {

    char out[NROW][NCOL + 1];
    burrow_to_string(b, out);
    for (int row = 0; row < NROW; row++) {
        printf("%s\n", out[row]);
    }
}

int layouts_equal(burrow_t *a, burrow_t *b) {
    char ba[NROW][NCOL + 1];
    char bb[NROW][NCOL + 1];
    burrow_to_string(a, ba);
    burrow_to_string(b, bb);

    for (int row = 0; row < NROW; row++) {
        if (strcmp(ba[row], bb[row]) != 0) {
            return 0;
        }
    }

    return 1;
}

int barrows_equal(burrow_t *a, burrow_t *b) {

    return layouts_equal(a, b) && (a->energy == b->energy);
}