#include <stdio.h>
#include <stdlib.h>
#include "../utils/d23_pqueue.h"

#define TEST 1

int is_room_finished(room_t room) {
    if (room.top.status == OCCUPIED && room.bottom.status == OCCUPIED) {
        return room.type == room.top.occupant->type == room.bottom.occupant->type;
    }

    return 0;
}

int is_organized(burrow_t *state) {
    // Check the hallway
    for (int i = 0; i < 11; i++) {
        if (state->hallway.spots[i].status == OCCUPIED) {
            return -1;
        }
    }

    // Check the rooms
    for (int i = 0; i < 4; i++) {
        if (is_room_finished(state->rooms[i])) {
            return 0;
        }
    }

    return 1;
}

int is_visited(burrow_t *visited, int visit_num, burrow_t state) {
    for (int i = 0; i < visit_num; i++) {
        if (burrows_are_equal(&visited[i], &state)) {
            return 1;
        }
    }

    return 0;
}

int can_move(amphipod_t a) {
    return ((a.space->up != NULL && a.space->up->status == EMPTY) ||
        (a.space->down != NULL && a.space->down->status == EMPTY) ||
        (a.space->left != NULL && a.space->left->status == EMPTY) ||
        (a.space->right != NULL && a.space->right->status == EMPTY)) &&
        (a.moves > 0);
}

int is_room_available(room_t room) {
    return (room.bottom.status == EMPTY) ||
        (room.bottom.status == OCCUPIED && room.bottom.occupant->type == room.type);
}

int path_room_to_hall(space_t *dst, int dst_index, space_t *src, int room_num) {
    if (dst->status == OCCUPIED || dst==src || room_num == -1) {
        return 0;
    }

    int i = -1;
    int dist = 0;
    space_t current = *src;
    while (i != dst_index) {
        if (current.up != NULL) {
            current = *current.up;
            dist++;
        } else { // we cannot go up therefore we are in the hall
            if (current.down != NULL && i == -1) {
                // Room 0 leads to hall index 2
                // Room 1 leads to hall index 4
                // Room 2 leads to hall index 6
                // Room 3 leads to hall index 8
                i = (room_num + 1) * 2;
            }

            if (i > dst_index) {
                // move to the left
                current = *current.left;
                i--;
                dist++;
            } else if (i < dst_index) {
                // move to the right
                current = *current.right;
                i++;
                dist++;
            }
        }

        if (current.status == OCCUPIED) {
            dist = 0;
            break;
        }
    }

    return dist;
}

int path_hall_to_room(space_t *dst, int room_num, space_t *src, burrow_t *state, int hall_index) {
    if (dst->status == OCCUPIED || dst == src || hall_index == -1) {
        return 0;
    }

    // path to room entrance
    int dist = 0;
    int i = hall_index;
    while (i != (2 * (room_num + 1))) {
        if (i > (2 * (room_num + 1))) {
            i--;
            dist++;
        } else {
            i++;
            dist++;
        }

        if (state->hallway.spots[i].status == OCCUPIED && i != hall_index) {
            return 0;
        }
    }

    if (&state->rooms[room_num].top == dst) {
        dist++;
    } else if (&state->rooms[room_num].bottom == dst && state->rooms[room_num].top.status != OCCUPIED) {
        dist += 2;
    } else {
        printf("\tsomething went wrong\n");
        return 0;
    }

    return dist;
}

int organize_burrow(burrow_t state) {

    int visit_num = 0;
    int visit_cap = 1024;
    burrow_t *visited = (burrow_t *)malloc(visit_cap * sizeof(burrow_t));

    burrow_queue_t pq;
    queue_init(&pq, 1024);
    queue_add(&pq, state);

    int energy = -1;
    while (!queue_is_empty(&pq)) {
        burrow_t current;
        queue_remove(&pq, &current);
        printf("current state energy expenditure -> %d\n", current.energy);
        print_burrow(&current);

        int test = is_organized(&current);
        printf("reason -> %d\n", test);
        // if (is_organized(&current)) {
        if (test == 1) {
            printf("found end\n");
            energy = current.energy;
            break;
        }

        if (!is_visited(visited, visit_num, current)) {

            visited[visit_num++] = current;
            if (visit_num == visit_cap) {
                visit_cap *= 2;
                visited = (burrow_t *)realloc(visited, visit_cap * sizeof(burrow_t));
            }

            for (int a = 0; a < 8; a++) {
                burrow_t new_state = state;
                print_burrow(&new_state);
                int dist;
                // printf("Checking amphi %d of type %c\n", a, type_char(state.amphis[a].type));
                if (can_move(new_state.amphis[a])) {
                    // printf("\tis able to move\n");
                    if (new_state.amphis[a].moves == 2) {
                        // printf("\tis in room, must try to move to hallway\n");
                        // move from room to hallway
                        for (int h = 0; h < 11; h++) {
                            // printf("\t\tattempting hallway index %d\n", h);
                            if (new_state.hallway.spots[h].status == EMPTY && new_state.hallway.spots[h].down == NULL) {
                                // printf("\t\ttarget is empty and not a hallway entrance\n");
                                dist = path_room_to_hall(&new_state.hallway.spots[h], h, new_state.amphis[a].space, new_state.amphis[a].room_num);
                                if (dist > 0) {
                                    // printf("\t\texecuting move\n");
                                    new_state.hallway.spots[h].occupant = &new_state.amphis[a];
                                    new_state.amphis[a].space->status = EMPTY;
                                    new_state.amphis[a].space = &new_state.hallway.spots[h];
                                    new_state.amphis[a].moves = 1;
                                    new_state.hallway.spots[h].status = OCCUPIED;
                                    new_state.amphis[a].hall_spot = h;
                                    new_state.amphis[a].room_num = -1;
                                    new_state.amphis[a].room_loc = -1;
                                    new_state.energy += (new_state.amphis[a].type * dist);
                                    queue_add(&pq, new_state);
                                }
                            }
                        }
                    } else if (new_state.amphis[a].moves == 1) {
                        // printf("\tis in hallway, must try to move to room\n");
                        // move from hallway to room
                        for (int r = 0; r < 4; r++) {
                            if (new_state.rooms[r].type == new_state.amphis[a].type) {
                                if (is_room_available(new_state.rooms[r])) {
                                    if (new_state.rooms[r].bottom.status == EMPTY) {
                                        dist = path_hall_to_room(&new_state.rooms[r].bottom, r, new_state.amphis[a].space, &new_state, new_state.amphis[a].hall_spot);
                                        if (dist > 0) {
                                            new_state.rooms[r].bottom.occupant = &new_state.amphis[a];
                                            new_state.rooms[r].bottom.status = OCCUPIED;
                                            new_state.amphis[a].space->status = EMPTY;
                                            new_state.amphis[a].space = &new_state.rooms[r].bottom;
                                            new_state.amphis[a].moves = 0;
                                            new_state.amphis[a].hall_spot = -1;
                                            new_state.amphis[a].room_num = r;
                                            new_state.amphis[a].room_loc = 1;
                                            new_state.energy += (new_state.amphis[a].type * dist);
                                            queue_add(&pq, new_state);
                                        }
                                    } else {
                                        dist = path_hall_to_room(&new_state.rooms[r].top, r, new_state.amphis[a].space, &new_state, new_state.amphis[a].hall_spot);
                                        if (dist > 0) {
                                            new_state.rooms[r].top.occupant = &new_state.amphis[a];
                                            new_state.rooms[r].top.status = OCCUPIED;
                                            new_state.amphis[a].space->status = EMPTY;
                                            new_state.amphis[a].space = &new_state.rooms[r].top;
                                            new_state.amphis[a].moves = 0;
                                            new_state.amphis[a].hall_spot = -1;
                                            new_state.amphis[a].room_num = r;
                                            new_state.amphis[a].room_loc = 0;
                                            new_state.energy += (new_state.amphis[a].type * dist);
                                            queue_add(&pq, new_state);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    free(visited);
    return energy;
}

void map_burrow(burrow_t *burrow) {
    
    for (int i = 0; i < 11; i++) {

        burrow->hallway.spots[i].up = NULL;
        burrow->hallway.spots[i].occupant = NULL;
        burrow->hallway.spots[i].status = EMPTY;

        if (i == 0) {
            burrow->hallway.spots[i].left = NULL;
        } else {
            burrow->hallway.spots[i].left = &burrow->hallway.spots[i - 1];
        }

        if (i == 10) {
            burrow->hallway.spots->right = NULL;
        } else {
            burrow->hallway.spots[i].right = &burrow->hallway.spots[i + 1];
        }

        if (i == 2 || i == 4 || i == 6 || i == 8) {
            burrow->hallway.spots[i].down = &burrow->rooms[(i / 2) - 1].top;
            burrow->rooms[(i / 2) - 1].top.up = &burrow->hallway.spots[i];
        } else {
            burrow->hallway.spots[i].down = NULL;
        }
    }
}

burrow_t parse_input(FILE *input) {

    burrow_t state;
    state.energy = 0;
    state.rooms[0].type = AMBER;
    state.rooms[1].type = BRONZE;
    state.rooms[2].type = COPPER;
    state.rooms[3].type = DESERT;

    char c;
    char str[50];
    int count = 0;
    while (fscanf(input, "%[^A,B,C,D]s", str) == 1) {
        c = fgetc(input);

        switch (c) {
            case 'A':
                state.amphis[count].type = AMBER;
                break;
            case 'B':
                state.amphis[count].type = BRONZE;
                break;
            case 'C':
                state.amphis[count].type = COPPER;
                break;
            case 'D':
                state.amphis[count].type = DESERT;
                break;
        }

        if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {
            // 0->-4; 1->-3; 2->-2, 3->-1 == top row
            if (count - 4 < 0) {
                state.amphis[count].moves = 2;
                state.rooms[count].top.occupant = &state.amphis[count];
                state.rooms[count].top.status = OCCUPIED;
                state.amphis[count].space = &state.rooms[count].top;
                state.rooms[count].top.down = &state.rooms[count].bottom;
                state.amphis[count].room_num = count;
                state.amphis[count].room_loc = 0;
            } else {
                // 4->1; 5->2; 6->3; 7->4 == bottom row
                state.amphis[count].moves = (state.amphis[count].type == state.rooms[count - 4].type) ? 0 : 2;
                state.rooms[count - 4].bottom.occupant = &state.amphis[count];
                state.rooms[count - 4].bottom.status = OCCUPIED;
                state.amphis[count].space = &state.rooms[count - 4].bottom;
                state.rooms[count - 4].bottom.up = &state.rooms[count-4].top;
                state.amphis[count].room_num = count - 4;
                state.amphis[count].room_loc = 1;
            }
            state.amphis[count].hall_spot = -1;
            count++;
        }
    }

    map_burrow(&state);
    return state;
}

int main() {

    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    burrow_t state = parse_input(input);

    printf("The lowest energy expenditure is %d\n", organize_burrow(state));

    fclose(input);
    return EXIT_SUCCESS;
}