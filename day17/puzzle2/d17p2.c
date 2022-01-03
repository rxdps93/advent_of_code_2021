#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

typedef struct Vector {
    int x;
    int y;
} Vector;

int get_y_velocity(int y_min, int y_max) {
    
    int max_h = INT_MIN;
    int max_n = INT_MIN;
    for (int i = y_min; i < y_max; i++) {
        double n = abs(i) - 1;
        double h = (n * (n + 1)) / 2;
        if ((int)h > max_h) {
            max_h = (int)h;
            max_n = (int)n;
        }
    }
    return max_n;
}

int get_x_velocity(int x_min, int x_max) {
    int x_vel = 0;
    for (int c = x_min; c < x_max + 1; c++) {

        int dscrmnt = 1 - (4 * (2 * -c));
        if (dscrmnt >= 0) {
            double root = sqrt(dscrmnt);

            if (floor(root) == root) {
                double pos = (-1 + root) / 2;
                double neg = (-1 - root) / 2;
                
                if (pos > 0 && neg > 0) {
                    x_vel = pos < neg ? pos : neg;
                    break;
                } else if (pos > 0 && neg <= 0) {
                    x_vel = pos;
                    break;
                } else if (neg > 0 && pos <= 0) {
                    x_vel = neg;
                    break;
                }
            }
        }
    }

    return x_vel;
}

bool simulate_probe(int x_vel, int y_vel, int tx_min, int tx_max, int ty_min, int ty_max) {

    bool running = true;
    bool valid = false;
    int x = 0;
    int y = 0;
    while (running) {
        x += x_vel;
        y += y_vel;

        x_vel += x_vel < 0 ? 1 : x_vel > 0 ? -1 : 0;
        y_vel--;

        if (x > tx_max && x_vel >= 0) {
            // Too far right
            running = false;
            valid = false;
        } else if (x < tx_min && x_vel <= 0) {
            // Too far left
            running = false;
            valid = false;
        } else if (y < ty_min) {
            // Overshot
            running = false;
            valid = false;
        } else if ((x >= tx_min && x <= tx_max) && (y >= ty_min && y <= ty_max)) {
            // On target
            running = false;
            valid = true;
        }
    }

    return valid;
}

int main() {

    FILE *input;

    if ((input = fopen("../input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    int tx_min, tx_max, ty_min, ty_max;
    fscanf(input, "target area: x=%d..%d, y=%d..%d", &tx_min, &tx_max, &ty_min, &ty_max);

    const int x_low = get_x_velocity(tx_min, tx_max);
    const int x_high = tx_max;

    const int y_low = ty_min;
    const int y_high = get_y_velocity(ty_min, ty_max);

    int valid_coords = 0;
    Vector coords[((x_high - x_low) + 1) * ((y_high - y_low) + 1)];
    for (int x_velocity = x_low; x_velocity <= x_high; x_velocity++) {
        for (int y_velocity = y_low; y_velocity <= y_high; y_velocity++) {
            if (simulate_probe(x_velocity, y_velocity, tx_min, tx_max, ty_min, ty_max)) {
                coords[valid_coords].x = x_velocity;
                coords[valid_coords].y = y_velocity;
                valid_coords++;
            }
        }
    }

    printf("Found %d valid coordinates\n", valid_coords);

    fclose(input);
    return EXIT_SUCCESS;
}