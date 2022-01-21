#include <stdio.h>
#include <stdlib.h>
#include "../utils/d23_pqueue.h"

#define TEST 1

int is_organized(burrow_t *state) {
    burrow_t done = get_finished_burrow();
    return layouts_equal(state, &done);
}

int is_visited(burrow_t *visited, int visit_num, burrow_t state) {
    for (int i = 0; i < visit_num; i++) {
        if (layouts_equal(&visited[i], &state)) {
            if (visited[i].energy >= state.energy) {
                return 0;
            } else {
                return 1;
            }
        }
    }

    return 0;
}

int room_is_eligible(int from, int room_num, burrow_t state) {
    if (state.layout[ROW_HALL][from] != get_room_type_from_num(room_num)) {
        return 0;
    }

    int col = 2 * (room_num + 1);
    if (state.layout[ROW_RM_TOP][col] == EMPTY) {
        if (state.layout[ROW_RM_BTM][col] == EMPTY) {
            return 1;
        } else {
            return state.layout[ROW_RM_BTM][col] == get_room_type_from_num(room_num);
        }
    }

    return 0;
}

burrow_t execute_move(move_t move, burrow_t state, int dist) {

    burrow_t new_state = state;
    if (move.move_type == ROOM_TO_HALL) {
        new_state.layout[ROW_HALL][move.hall] = new_state.layout[move.room_row][move.room_col];
        new_state.layout[move.room_row][move.room_col] = EMPTY;
        new_state.energy += dist * new_state.layout[ROW_HALL][move.hall];
    } else {
        new_state.layout[move.room_row][move.room_col] = new_state.layout[ROW_HALL][move.hall];
        new_state.layout[ROW_HALL][move.hall] = EMPTY;
        new_state.energy += dist * new_state.layout[move.room_row][move.room_col];
    }
    return new_state;
}

int path_move(move_t move, const burrow_t state) {
    int dist = 0;
    // organize_burrow should make sure the room is good, this just counts distance and makes sure hall is clear
    if (move.move_type == ROOM_TO_HALL) {

        // path to the entrance to the room
        int i = move.hall;
        while (i != move.room_col) {
            if (state.layout[ROW_HALL][i] != EMPTY && i != move.hall) {
                return 0;
            }

            if (i > move.room_col) {
                i--;
                dist++;
            } else {
                i++;
                dist++;
            }
        }

        // now i should == from_col, add 1 to move from top row, 2 to move from bottom row
        dist += move.room_row;

    } else {

        int i = move.hall;
        while (i != move.room_col) {
            if (state.layout[ROW_HALL][i] != EMPTY && i != move.hall) {
                return 0;
            }

            if (i > move.room_col) {
                i--;
                dist++;
            } else {
                i++;
                dist++;
            }
        }

        dist += move.room_row;
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

        print_burrow(&current);

        if (is_organized(&current)) {
            energy = current.energy;
            break;
        }

        // refactored block
        if (!is_visited(visited, visit_num, current)) {

            visited[visit_num++] = current;
            if (visit_num >= visit_cap) {
                visit_cap *= 2;
                visited = (burrow_t *)realloc(visited, visit_cap * sizeof(burrow_t));
            }

            // check for hall to room movements
            for (int h = 0; h < NCOL; h++) {
                if (current.layout[ROW_HALL][h] != EMPTY) {

                }
            }

            // if no h-t-r, then try r-t-h
        }

        // old block
        // possible optimization: add all possible hall-to-room moves to queue.
        // if at least1 is added, don't bother with room-to-hall
        if (!is_visited(visited, visit_num, current)) {

            visited[visit_num++] = current;
            if (visit_num >= visit_cap) {
                visit_cap *= 2;
                visited = (burrow_t *)realloc(visited, visit_cap * sizeof(burrow_t));
            }
            // find all possible moves for the current state
            for (int h = 0; h < NCOL; h++) {
                if (h == COL_ROOM_A || h == COL_ROOM_B || h == COL_ROOM_C || h == COL_ROOM_D) {
                    continue;
                }

                if (current.layout[ROW_HALL][h] == EMPTY) {
                    // see what we can move to this space from a room
                    for (int i = COL_ROOM_A; i <= COL_ROOM_D; i += 2) {
                        // check if room is finished
                        if ((current.layout[ROW_RM_TOP][i] == current.layout[ROW_RM_BTM][i]) && (current.layout[ROW_RM_TOP][i] == get_room_type_from_num((i / 2) - 1))) {
                            continue;
                        }

                        if (current.layout[ROW_RM_TOP][i] != EMPTY) {
                            // try to move from top
                            move_t move = { ROOM_TO_HALL, h, ROW_RM_TOP, i };
                            int dist = path_move(move, current);
                            if (dist > 0) {
                                burrow_t new_state = execute_move(move, current, dist);
                                queue_add(&pq, new_state);
                            }
                        } else if (current.layout[ROW_RM_BTM][i] != EMPTY && current.layout[ROW_RM_BTM][i] != get_room_type_from_num((i / 2) - 1)) {
                            // try to move from bottom
                            move_t move = { ROOM_TO_HALL, h, ROW_RM_BTM, i };
                            int dist = path_move(move, current);
                            if (dist > 0) {
                                burrow_t new_state = execute_move(move, current, dist);
                                queue_add(&pq, new_state);
                            }
                        }
                    }
                } else {
                    // see what we can move from this space to a room
                    for (int i = COL_ROOM_A; i <= COL_ROOM_D; i += 2) {
                        // check if room is eligible
                        // eligible if room type matches amphipod type
                        // plus the room is either fully empty OR
                        // the top spot is empty with the bottom filled by a matching type
                        if (!room_is_eligible(h, (i / 2) - 1, current)) {
                            continue;
                        }

                        if (current.layout[ROW_RM_TOP][i] != EMPTY) {
                            // try to move from top
                            move_t move = { HALL_TO_ROOM, h, ROW_RM_TOP, i };
                            int dist = path_move(move, current);
                            if (dist > 0) {
                                burrow_t new_state = execute_move(move, current, dist);
                                queue_add(&pq, new_state);
                            }
                        } else if (current.layout[ROW_RM_BTM][i] != EMPTY) {
                            // try to move from bottom
                            move_t move = { HALL_TO_ROOM, h, ROW_RM_BTM, i };
                            int dist = path_move(move, current);
                            if (dist > 0) {
                                burrow_t new_state = execute_move(move, current, dist);
                                queue_add(&pq, new_state);
                            }
                        }
                    }
                }
            }
        }
    }

    free(visited);
    queue_free(&pq);
    return energy;
}

burrow_t parse_input(FILE *input) {

    burrow_t state;
    state.energy = 0;

    char str[50];
    fscanf(input, "%[^\n]s", str);
    fgetc(input);
    for (int row = 0; row < NROW; row++) {
        fgetc(input);
        for (int col = 0; col < NCOL; col++) {
            state.layout[row][col] = char_to_type(fgetc(input));
        }
        fscanf(input, "%[^\n]s", str);
        fgetc(input);
    }

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
    fclose(input);

    int dist = organize_burrow(state);
    printf("Energy: %d\n", dist);
    
    // testing pathing and moving
    // burrow_t new_state;
    // move_t rth = { ROOM_TO_HALL, 5, ROW_RM_TOP, COL_ROOM_A };
    // int dist = path_move(rth, state);
    // if (dist > 0) {
    //     printf("MOVE: top room A to hall 5\n");
    //     new_state = execute_move(rth, state, dist);
    //     state = new_state;
    // } else {
    //     printf("Unable to move!\n");
    // }
    // print_burrow(&state);
    // printf("energy spent thus far: %d\n", state.energy);

    // rth.hall = 9;
    // rth.room_row = ROW_RM_TOP;
    // rth.room_col = COL_ROOM_B;
    // dist = path_move(rth, state);
    // if (dist > 0) {
    //     printf("MOVE: top room B to hall 9\n");
    //     new_state = execute_move(rth, state, dist);
    //     state = new_state;
    // } else {
    //     printf("Unable to move!\n");
    // }
    // print_burrow(&state);
    // printf("energy spent thus far: %d\n", state.energy);

    // rth.hall = 0;
    // rth.room_row = ROW_RM_TOP;
    // rth.room_col = COL_ROOM_B;
    // dist = path_move(rth, state);
    // if (dist > 0) {
    //     printf("MOVE: top room B to hall 0\n");
    //     new_state = execute_move(rth, state, dist);
    //     state = new_state;
    // } else {
    //     printf("Unable to move!\n");
    // }
    // print_burrow(&state);
    // printf("energy spent thus far: %d\n", state.energy);

    // printf("modifying state\n");
    // state.layout[ROW_RM_BTM][COL_ROOM_B] = BRONZE;
    // state.layout[ROW_RM_TOP][COL_ROOM_C] = DESERT;
    // print_burrow(&state);
    // printf("energy spent thus far: %d\n", state.energy);

    // move_t htr = { HALL_TO_ROOM, 5, ROW_RM_TOP, COL_ROOM_B };
    // dist = path_move(rth, state);
    // if (dist > 0) {
    //     printf("MOVE: hall 5 to top room B\n");
    //     new_state = execute_move(htr, state, dist);
    //     state = new_state;
    // } else {
    //     printf("Unable to move!\n");
    // }
    // print_burrow(&state);
    // printf("energy spent thus far: %d\n", state.energy);

    // printf("total energy expended: %d\n", state.energy);

    return EXIT_SUCCESS;
}