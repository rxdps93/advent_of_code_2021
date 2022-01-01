#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

int get_x_velocity(int x_min, int x_max) {
    // x = ((-b +- sqrt(b^2 - 4ac)) / 2a)
    int x_vel = 0;
    for (int c = x_min; c < x_max + 1; c++) {
        // printf("trying %d...\n", c);
        int dscrmnt = 1 - (4 * (2 * -c));
        // printf("discriminant is %d...\n", dscrmnt);
        if (dscrmnt >= 0) {
            double root = sqrt(dscrmnt);

            if (floor(root) == root) {
                double pos = (-1 + root) / 2;
                double neg = (-1 - root) / 2;

                printf("pos=%f; neg=%f\n", pos, neg);
                
                if (pos > 0 && neg > 0) {
                    printf("opt a\n");
                    x_vel = pos < neg ? pos : neg;
                    break;
                } else if (pos > 0 && neg <= 0) {
                    printf("opt b\n");
                    x_vel = pos;
                    break;
                } else if (neg > 0 && pos <= 0) {
                    printf("opt c\n");
                    x_vel = neg;
                    break;
                } else {
                    printf("wtf\n");
                }
            }
        }
    }

    return x_vel;
}

int main() {

    FILE *input;

    if ((input = fopen("../test_input.txt", "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    int tx_min, tx_max, ty_min, ty_max;
    fscanf(input, "target area: x=%d..%d, y=%d..%d", &tx_min, &tx_max, &ty_min, &ty_max);

    printf("Target Area: x:(%d -> %d); y:(%d -> %d)\n", tx_min, tx_max, ty_min, ty_max);

    // FOR X: (n * (n + 1)) / 2 >= tx_min but <= tx_max; try for each value starting with tx_min
    // (n * (n + 1)) / 2 = 21
    // (n * (n + 1)) = 42
    // n^2 + n = 42
    // n^2 + n - 42 = 0
    // ax^2 + bx + c = 0 (basic quadratic formula)
    // x = -b +- sqrt(b^2 - 4ac) / 2a
    // x = -1 +- sqrt(1 - (4 * 1 * -42)) / 2
    // x = -1 +- sqrt(1 + 168) / 2
    // x = -1 +- 13 / 2
    // x = (-1 + 13 / 2), (-1 - 13 / 2)
    // x = (12 / 2), (-14 / 2)
    // x = 6, -7
    // x = 6
    const int x_init = get_x_velocity(tx_min, tx_max);
    const int y_init = 9;
    int x_velocity = x_init;
    int y_velocity = y_init;
    int x = 0;
    int y = 0;
    int max_height = INT_MIN;
    int step = 0;
    bool running = true;
    printf("Using initial velocity (x=%d, y=%d)\n", x_velocity, y_velocity);
    while (running) {
        x += x_velocity;
        y += y_velocity;
        printf("After step %d: x=%d, y=%d...", step++, x, y);

        if (y > max_height) {
            max_height = y;
        }
        
        if (x_velocity > 0) {
            x_velocity--;
        } else if (x_velocity < 0) {
            x_velocity++;
        }
        y_velocity--;

        if (x > tx_max && x_velocity >= 0) {
            printf("Too far right; won't make it\n");
            running = false;
        } else if (x < tx_min && x_velocity <= 0) {
            printf("Too far left; won't make it\n");
            running = false;
        } else if (y < ty_min) {
            printf("Too far down; overshot it\n");
            running = false;
        } else if ((x >= tx_min && x <= tx_max) && (y >= ty_min && y <= ty_max)) {
            printf("IN THE TARGET\n");
            running = false;
        } else {
            printf("\n");
        }
    }

    printf("Target achieved with maximum height %d using initial velocity (x=%d, y=%d)\n", max_height, x_init, y_init);

    fclose(input);
    return EXIT_SUCCESS;
}