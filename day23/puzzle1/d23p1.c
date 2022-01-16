#include <stdio.h>
#include <stdlib.h>
#include "../utils/d23_pqueue.h"

#define TEST 1

int is_room_finished(room_t room) {
    if (room.top.occupant != NULL && room.bottom.occupant != NULL) {
        return room.type == room.top.occupant->type == room.bottom.occupant->type;
    }

    return 0;
}

int is_organized(burrow_t *state) {
    // Check the hallway
    for (int i = 0; i < 11; i++) {
        if (state->hallway.spots[i].occupant != NULL) {
            return 0;
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

void organize_burrow(burrow_t state) {

    int visit_num = 0;
    int visit_cap = 1024;
    burrow_t *visited = (burrow_t *)malloc(visit_cap * sizeof(burrow_t));


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
                state.amphis[count].space = &state.rooms[count].top;
                state.rooms[count].top.down = &state.rooms[count].bottom;
            } else {
                // 4->1; 5->2; 6->3; 7->4 == bottom row
                state.amphis[count].moves = (state.amphis[count].type == state.rooms[count - 4].type) ? 0 : 2;
                state.rooms[count - 4].bottom.occupant = &state.amphis[count];
                state.amphis[count].space = &state.rooms[count - 4].bottom;
                state.rooms[count - 4].bottom.up = &state.rooms[count-4].top;
            }

            count++;
        }
    }

    return state;
}

void map_burrow(burrow_t *burrow) {
    
    for (int i = 0; i < 11; i++) {

        burrow->hallway.spots[i].up = NULL;
        burrow->hallway.spots[i].occupant = NULL;

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

int main() {

    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    burrow_t state = parse_input(input);
    map_burrow(&state);

    print_rooms(&state);
    print_burrow(&state);

    fclose(input);
    return EXIT_SUCCESS;
}