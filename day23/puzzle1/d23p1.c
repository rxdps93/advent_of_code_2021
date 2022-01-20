#include <stdio.h>
#include <stdlib.h>
#include "../utils/d23_pqueue.h"

#define TEST 1

int is_organized(burrow_t *state) {
    for (int h = 0; h < NCOL; h++) {
        if (state->layout[ROW_HALL][h] != EMPTY) {
            return 0;
        }
    }

    return ((state->layout[ROW_RM_TOP][COL_ROOM_A] == state->layout[ROW_RM_BTM][COL_ROOM_A]) && (state->layout[ROW_RM_TOP][COL_ROOM_A] == AMBER)) &&
    ((state->layout[ROW_RM_TOP][COL_ROOM_B] == state->layout[ROW_RM_BTM][COL_ROOM_B]) && (state->layout[ROW_RM_TOP][COL_ROOM_B] == BRONZE)) &&
    ((state->layout[ROW_RM_TOP][COL_ROOM_C] == state->layout[ROW_RM_BTM][COL_ROOM_C]) && (state->layout[ROW_RM_TOP][COL_ROOM_C] == COPPER)) &&
    ((state->layout[ROW_RM_TOP][COL_ROOM_D] == state->layout[ROW_RM_BTM][COL_ROOM_D]) && (state->layout[ROW_RM_TOP][COL_ROOM_D] == DESERT));
}

int is_visited(burrow_t *visited, int visit_num, burrow_t state) {
    for (int i = 0; i < visit_num; i++) {
        if (layouts_equal(&visited[i], &state)) {
            return 1;
        }
    }

    return 0;
}

int room_is_eligible(int from, int room_num, burrow_t state) {
    if (state.layout[ROW_HALL][from] != get_room_type(room_num)) {
        return 0;
    }

    int col = 2 * (room_num + 1);
    if (state.layout[ROW_RM_TOP][col] == EMPTY) {
        if (state.layout[ROW_RM_BTM][col] == EMPTY) {
            return 1;
        } else {
            return state.layout[ROW_RM_BTM][col] == get_room_type(room_num);
        }
    }

    return 0;
}

int path_move(move_t move, const burrow_t state) {
    int dist = 0;
    // organize_burrow should make sure the room is good, this just counts distance and makes sure hall is clear
    if (move.move_type == ROOM_TO_HALL) {

        const int hall_index = move.to;
        const int from_col = move.from % 10;
        const int from_row = move.from / 10;

        // path to the entrance to the room
        int i = hall_index;
        while (i != from_col) {
            if (state.layout[ROW_HALL][i] != EMPTY) {
                return 0;
            }

            if (i > from_col) {
                i--;
                dist++;
            } else {
                i++;
                dist++;
            }
        }

        // now i should == from_col, add 1 to move from top row, 2 to move from bottom row
        dist += from_row;

    } else {

        const int from_hall = move.from;
        const int to_col = move.to % 10;
        const int to_row = move.to / 10;

        int i = from_hall;
        while (i != to_col) {
            if (state.layout[ROW_HALL][i] != EMPTY) {
                return 0;
            }

            if (i > to_col) {
                i--;
                dist++;
            } else {
                i++;
                dist++;
            }
        }

        dist += to_row;
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

        if (is_organized(&current)) {
            energy = current.energy;
            break;
        }

        if (!is_visited(visited, visit_num, current)) {
            int move_num = 0;
            move_t moves[1024];
            // find all possible moves for the current state
            for (int h = 0; h < NCOL; h++) {
                if (h == COL_ROOM_A || h == COL_ROOM_B || h == COL_ROOM_C || h == COL_ROOM_D) {
                    continue;
                }

                if (state.layout[ROW_HALL][h] == EMPTY) {
                    // see what we can move to this space from a room
                    for (int i = COL_ROOM_A; i <= COL_ROOM_D; i += 2) {
                        // check if room is finished
                        if ((state.layout[ROW_RM_TOP][i] == state.layout[ROW_RM_BTM][i]) && (state.layout[ROW_RM_TOP][i] == get_room_type((i / 2) - 1))) {
                            continue;
                        }

                        if (state.layout[ROW_RM_TOP][i] != EMPTY) {
                            // try to move from top
                            move_t move = { ROOM_TO_HALL, (ROW_RM_TOP * 10) + i, h };
                            path_move(move, state);
                        } else if (state.layout[ROW_RM_BTM][i] != EMPTY && state.layout[ROW_RM_BTM][i] != get_room_type((i / 2) - 1)) {
                            // try to move from bottom
                        }
                    }
                } else {
                    // see what we can move from this space to a room
                    for (int i = COL_ROOM_A; i <= COL_ROOM_D; i += 2) {
                        // check if room is eligible
                        // eligible if room type matches amphipod type
                        // plus the room is either fully empty OR
                        // the top spot is empty with the bottom filled by a matching type
                        if (!room_is_eligible(h, (i / 2) - 1, state)) {
                            continue;
                        }

                        if (state.layout[ROW_RM_TOP][i] != EMPTY) {
                            // try to move from top
                        } else if (state.layout[ROW_RM_BTM][i] != EMPTY) {
                            // try to move from bottom
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
    print_burrow(&state);


    // pathing tests
    int test_dist;
    for (int h = 0; h < NCOL; h++) {
        if (h == COL_ROOM_A || h == COL_ROOM_B || h == COL_ROOM_C || h == COL_ROOM_D) {
            continue;
        }
        for (int r = COL_ROOM_A; r <= COL_ROOM_D; r += 2) {
            for (int s = ROW_RM_TOP; s <= ROW_RM_BTM; s++) {
                // move_t test_move = { ROOM_TO_HALL, (s * 10) + r, h };
                // test_dist = path_move(test_move, state);
                // printf("Moving from the %s of room %d to hallway spot %d: %d\n", s == 1 ? "top" : "bottom",  (r / 2) - 1, h, test_dist);

                move_t test_move = { HALL_TO_ROOM, h, (s * 10) + r };
                test_dist = path_move(test_move, state);
                printf("Moving from hallway spot %d to the %s of room %d: %d\n", h, s == 1 ? "top" : "bottom", (r / 2) - 1, test_dist);
            }
        }
    }

    return EXIT_SUCCESS;
}