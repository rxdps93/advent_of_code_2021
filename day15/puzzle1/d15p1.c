#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "../../common/utils.h"

#define SIZE 100

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

    int distances[SIZE][SIZE];
    memset(distances, -1, sizeof(int[SIZE][SIZE]));

    int distance = create_distance_map(risk_map, distances, row_count, col_count);
    printf("Risk level: %d\n", distance);

    fclose(input);
    return EXIT_SUCCESS;
}