typedef struct Amphipod amphipod_t;
typedef struct Space space_t;
typedef struct Hallway hall_t;
typedef struct Room room_t;
typedef struct Burrow burrow_t;

typedef enum {
    AMBER = 1,
    BRONZE = 10,
    COPPER = 100,
    DESERT = 1000
} amphitype_t;

struct Amphipod {
    amphitype_t type;
    int moves;
    space_t *space;
};

struct Space {
    amphipod_t *occupant;

    space_t *up;
    space_t *down;
    space_t *left;
    space_t *right;
};

struct Hallway {
    space_t spots[11];
};

struct Room {
    amphitype_t type;

    space_t top;
    space_t bottom;
};

struct Burrow {
    int energy;
    amphipod_t amphis[8];
    hall_t hallway;
    room_t rooms[4];
};

char type_char(amphitype_t type) {
    switch (type) {
        case AMBER:
            return 'A';
        case BRONZE:
            return 'B';
        case COPPER:
            return 'C';
        case DESERT:
            return 'D';
    }

    return '?';
}

void print_rooms(burrow_t *burrow) {
    for (int i = 0; i < 4; i++) {
        printf("room %c top: %c, moves=%d\n",
            type_char(burrow->rooms[i].type),
            type_char(burrow->rooms[i].top.occupant->type),
            burrow->rooms[i].top.occupant->moves);
        printf("room %c bottom: %c, moves=%d\n",
            type_char(burrow->rooms[i].type),
            type_char(burrow->rooms[i].bottom.occupant->type),
            burrow->rooms[i].bottom.occupant->moves);
    }
}

void print_burrow(burrow_t *burrow) {
    char out[5][14];
    for (int i = 1; i < 12; i++) {
        out[0][i] = '#';
        out[4][i] = ' ';
        if (burrow->hallway.spots[i - 1].occupant == NULL) {
            out[1][i] = '.';
        } else {
            out[1][i] = type_char(burrow->hallway.spots[i - 1].occupant->type);
        }

        if (burrow->hallway.spots[i - 1].down == NULL) {
            out[2][i] = '#';
            out[3][i] = ' ';
        } else {
            if (burrow->hallway.spots[i - 1].down->occupant == NULL) {
                out[2][i] = '.';
            } else {
                out[2][i] = type_char(burrow->hallway.spots[i - 1].down->occupant->type);
            }

            if (burrow->hallway.spots[i - 1].down->down->occupant == NULL) {
                out[3][i] = '.';
            } else {
                out[3][i] = type_char(burrow->hallway.spots[i - 1].down->down->occupant->type);
            }
            out[3][i - 1] = '#';
            out[4][i - 1] = '#';
            out[4][i] = '#';
        }
    }

    for (int i = 0; i < 5; i++) {
        out[i][13] = '\0';
        if (i < 3) {
            out[i][0] = '#';
            out[i][12] = '#';
        } else {
            out[i][0] = ' ';
            out[i][12] = ' ';
        }
        printf("%s\n", out[i]);
    }
}