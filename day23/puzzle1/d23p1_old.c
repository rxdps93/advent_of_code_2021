#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TEST 1
#define MIN(a, b) (a < b ? a : b)

typedef enum {
    AMBER = 1,
    BRONZE = 10,
    COPPER = 100,
    DESERT = 1000
} amphitype_t;

typedef struct Amphipod {
    amphitype_t type;
    int moves;
    space_t *space;
    int init;
} amphipod_t;

typedef struct Space {
    amphipod_t *occupant;

    space_t *up;
    space_t *down;
    space_t *left;
    space_t *right;
} space_t;

typedef struct Room {
    amphitype_t type;
    space_t top;
    space_t bottom;
    space_t *hall;
} room_t;

typedef struct Hallway {
    space_t spots[11];
} hall_t;

typedef struct Burrow {
    hall_t hallway;
    room_t rooms[4];
    amphipod_t occupants[8];
} burrow_t;

typedef struct State {
    burrow_t burrow;
    int energy;
} state_t;

int can_move(amphipod_t a) {
    return (a.space->up != NULL && a.space->up->occupant != NULL) &&
        (a.space->down != NULL && a.space->down->occupant != NULL) &&
        (a.space->left != NULL && a.space->left->occupant != NULL) &&
        (a.space->right != NULL && a.space->right->occupant != NULL) &&
        (a.moves > 0);
}

int is_room_available(room_t room) {
    return (room.bottom.occupant == NULL) || (room.bottom.occupant != NULL & room.bottom.occupant->type == room.type);
}

int is_room_finished(room_t room) {
    if (room.top.occupant != NULL && room.bottom.occupant != NULL) {
        return room.type == room.top.occupant->type == room.bottom.occupant->type;
    }

    return 0;
}

int is_hallway_empty(hall_t hallway) {
    for (int i = 0; i < 11; i++) {
        if (hallway.spots[i].occupant != NULL) {
            return 0;
        }
    }
    return 1;
}

int is_organized(burrow_t burrow) {
    for (int i = 0; i < 4; i++) {
        if (!is_room_finished(burrow.rooms[i])) {
            return 0;
        }
    }

    return is_hallway_empty(burrow.hallway);
}

int is_reachable(space_t *dest, space_t *src) {
    if (dest->occupant != NULL || dest == src) {
        return 0;
    }

    // TODO: the rest of it; return number of steps
}

int organize_burrow(state_t state) {

    int energy = 0;
    if (is_organized(state.burrow)) {
        return state.energy;
    }

    for (int a = 0; a < 8; a++) {
        state_t new_state = state;
        int dist;
        if (can_move(state.burrow.occupants[a])) {
            if (state.burrow.occupants[a].moves == 2) { 
                // hasn't left room, must move into hallway or directly to available room
                // we know the hall above the room is clear
                // we must move at least 2 spaces to clear the room
                // find an empty spot in the hallway
                // make sure there is a clear path to it
                // move there
                // TODO: check if room is available for direct finish!
                for (int h = 0; h < 11; h++) {
                    if (new_state.burrow.hallway.spots[h].occupant == NULL && new_state.burrow.hallway.spots[h].down == NULL) {
                        dist = is_reachable(&new_state.burrow.hallway.spots[h], new_state.burrow.occupants[a].space);
                        if (dist > 0) {
                            new_state.burrow.hallway.spots[h].occupant = &new_state.burrow.occupants[a];
                            new_state.burrow.occupants[a].space = &new_state.burrow.hallway.spots[h];
                            new_state.burrow.occupants[a].moves = 1;
                            new_state.energy += (new_state.burrow.occupants[a].type * dist);
                            energy += organize_burrow(new_state);
                        }
                    }
                }
            } else {
                // currently in hallway, must move into the destination room
                // must check if that room can be reached
                // must check if that room contains a wrong type
                for (int r = 0; r < 4; r++) {
                    if (new_state.burrow.rooms[r].type == new_state.burrow.occupants[a].type) {
                        if (is_room_available(new_state.burrow.rooms[r])) {
                            if (new_state.burrow.rooms[r].bottom.occupant == NULL) {
                                // make sure the bottom spot is reachable
                                dist = is_reachable(&new_state.burrow.rooms[r].bottom, new_state.burrow.occupants[a].space);
                                if (dist < 0) {
                                    new_state.burrow.rooms[r].bottom.occupant = &new_state.burrow.occupants[a];
                                    new_state.burrow.occupants[a].space = &new_state.burrow.rooms[r].bottom;
                                    new_state.burrow.occupants[a].moves = 0;
                                    new_state.energy += (new_state.burrow.occupants[a].type * dist);
                                    energy += organize_burrow(new_state);
                                }
                            } else {
                                // make sure the top spot is reachable
                                dist = is_reachable(&new_state.burrow.rooms[r].top, new_state.burrow.occupants[a].space);
                                if (dist < 0) {
                                    new_state.burrow.rooms[r].top.occupant = &new_state.burrow.occupants[a];
                                    new_state.burrow.occupants[a].space = &new_state.burrow.rooms[r].top;
                                    new_state.burrow.occupants[a].moves = 0;
                                    new_state.energy += (new_state.burrow.occupants[a].type * dist);
                                    energy += organize_burrow(new_state);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return energy;
}

space_t *new_hallway(room_t rooms[4]) {
    space_t hallway[11];
    for (int i = 0; i < 11; i++) {
        hallway[i].up = NULL;
        hallway[i].occupant = NULL;

        if (i == 0) {
            hallway[i].left = NULL;
        } else {
            hallway[i].left = &hallway[i - 1];
        }

        if (i == 10) {
            hallway[i].right = NULL;
        } else {
            hallway[i].right = &hallway[i + 1];
        }

        if (i == 2 || i == 4 || i == 6 || i == 8) {
            hallway[i].down = &rooms[i / 2].top;
            rooms[i / 2].top.up = &hallway[i];
            rooms[i / 2].hall = &hallway[i];
        } else {
            hallway[i].down = NULL;
        }
    }
    return hallway;
}

room_t *new_rooms(amphipod_t amphs[8]) {
    room_t rooms[4];
    rooms[0].type = AMBER;
    rooms[1].type = BRONZE;
    rooms[2].type = COPPER;
    rooms[3].type = DESERT;

    for (int i = 0; i < 8; i++) {
        if (amphs[i].init - 4 < 0) {
            rooms[i].top.occupant = &amphs[i];
            amphs[i].space = &rooms[i].top;
        } else {
            rooms[i - 4].bottom.occupant = &amphs[i];
            amphs[i].space = &rooms[i - 4].bottom;
            if (amphs[i].type == rooms[i - 4].type) {
                amphs[i].moves = 0;
            } else {
                amphs[i].moves = 2;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (is_room_finished(rooms[i])) {
            rooms[i].top.occupant->moves = 0;
        } else {
            rooms[i].top.occupant->moves = 2;
        }
    }

    return rooms;
}

amphipod_t *parse_input(FILE *input) {
    char c;
    int count = 0;
    amphipod_t amphs[8];
    c = fgetc(input);
    while (c != EOF) {
        switch (c) {
            case 'A':
                amphs[count].type = AMBER;
                amphs[count].init = count;
                count++;
                break;
            case 'B':
                amphs[count].type = BRONZE;
                amphs[count].init = count;
                count++;
                break;
            case 'C':
                amphs[count].type = COPPER;
                amphs[count].init = count;
                count++;
                break;
            case 'D':
                amphs[count].type = DESERT;
                amphs[count].init = count;
                count++;
                break;
        }
        c = fgetc(input);
    }

    return amphs;
}

int main() {

    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    amphipod_t amphs[8] = parse_input(input);
    fclose(input);
    room_t rooms[4] = new_rooms(amphs);
    space_t spots[11] = new_hallway(&rooms);
    hall_t hallway = { spots };
    burrow_t burrow = { hallway, rooms, amphs };
    state_t state = { burrow, 0 };
    printf("The lowest energy expenditure is %d\n", organize_burrow(state));

    return EXIT_SUCCESS;
}