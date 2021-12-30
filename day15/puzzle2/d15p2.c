#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../common/utils.h"

#define SIZE 500

bool adjacent_distances(int risk_map[SIZE][SIZE], int distances[SIZE][SIZE], int rows, int cols, int row, int col) {

    bool distance_updates = false;
    if (distances[row][col] != -1) {
        // Up
        if (row > 0) {
            int new_dist = distances[row][col] + risk_map[row - 1][col];
            if (new_dist < distances[row - 1][col] || distances[row - 1][col] == -1) {
                distances[row - 1][col] = new_dist;
                distance_updates = true;
            }
        }

        // Down
        if (row < rows - 1) {
            int new_dist = distances[row][col] + risk_map[row + 1][col];
            if (new_dist < distances[row + 1][col] || distances[row + 1][col] == -1) {
                distances[row + 1][col] = new_dist;
                distance_updates = true;
            }
        }

        // Left
        if (col > 0) {
            int new_dist = distances[row][col] + risk_map[row][col - 1];
            if (new_dist < distances[row][col - 1] || distances[row][col - 1] == -1) {
                distances[row][col - 1] = new_dist;
                distance_updates = true;
            }
        }

        // Right
        if (col < cols - 1) {
            int new_dist = distances[row][col] + risk_map[row][col + 1];
            if (new_dist < distances[row][col + 1] || distances[row][col + 1] == -1) {
                distances[row][col + 1] = new_dist;
                distance_updates = true;
            }
        }
    }

    return distance_updates;
}

int create_distance_map(int risk_map[SIZE][SIZE], int distances[SIZE][SIZE], int rows, int cols) {

    distances[0][0] = risk_map[0][0];

    bool fully_mapped = false;
    while(!fully_mapped) {
        fully_mapped = true;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (adjacent_distances(risk_map, distances, rows, cols, row, col)) {
                    fully_mapped = false;
                }
            }
        }
    }

    return distances[rows - 1][cols - 1] - distances[0][0];
}

void create_full_map(int risk_map[SIZE][SIZE], int initial_rows, int initial_cols) {

    // Along the top
    for (int i = 1; i < 5; i++) {
        for (int row = 0; row < initial_rows; row++) {
            for (int col = 0; col < initial_cols; col++) {
                int new_col = col + (initial_cols * i);
                risk_map[row][new_col] = risk_map[row][new_col - initial_cols];
                if (risk_map[row][new_col] == 9) {
                    risk_map[row][new_col] = 1;
                } else {
                    risk_map[row][new_col]++;
                }
            }
        }
    }

    // Going down
    for (int i = 1; i < 5; i++) {
        for (int row = 0; row < initial_rows; row++) {
            for (int col = 0; col < SIZE; col++) {
                int new_row = row + (initial_rows * i);
                risk_map[new_row][col] = risk_map[new_row - initial_rows][col];
                if (risk_map[new_row][col] == 9) {
                    risk_map[new_row][col] = 1;
                } else {
                    risk_map[new_row][col]++;
                }
            }
        }
    }
}

int main() {

    FILE *input;

    if ((input = fopen("../input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    char c;
    int count = 0;
    int row_count = 0;
    int col_count = 0;
    int risk_map[SIZE][SIZE];
    while(fscanf(input, "%c", &c) == 1) {
        if (c == '\n') {
            col_count = 0;
            row_count++;
        } else {
            risk_map[row_count][col_count] = ctoi(c);
            count++;
            col_count++;
        }
    }
    col_count = (count / row_count);

    create_full_map(risk_map, row_count, col_count);

    int distances[SIZE][SIZE];
    memset(distances, -1, sizeof(int[SIZE][SIZE]));

    int distance = create_distance_map(risk_map, distances, SIZE, SIZE);
    printf("Risk level: %d\n", distance);

    fclose(input);
    return EXIT_SUCCESS;
}