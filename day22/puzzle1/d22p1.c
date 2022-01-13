#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define TEST 0

typedef struct Vector {
    int x;
    int y;
    int z;
} vector_t;

typedef struct Instruction {
    int state;
    int skip;

    vector_t min;
    vector_t max;
} ins_t;

int read_input(ins_t ins_set[500], FILE *input) {
    int ins_cnt = -1;
    char c;
    do {
        fgetc(input);
        c = fgetc(input);
        ins_cnt++;
        ins_set[ins_cnt].skip = 0;
        if (c == 'n') {
            ins_set[ins_cnt].state = 1;
            fgetc(input);
        } else {
            ins_set[ins_cnt].state = 0;
            fseek(input, 2, SEEK_CUR);
        }

    } while(fscanf(input, "x=%d..%d,y=%d..%d,z=%d..%d\n",
            &ins_set[ins_cnt].min.x, &ins_set[ins_cnt].max.x,
            &ins_set[ins_cnt].min.y, &ins_set[ins_cnt].max.y,
            &ins_set[ins_cnt].min.z, &ins_set[ins_cnt].max.z) == 6);

    return ins_cnt;
}

int main() {
    FILE *input;
    char *file_name = TEST == 1 ? "../test_input_small.txt" : TEST == 2 ? "../test_input_large.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    ins_t ins_set[500];
    int ins_cnt = read_input(ins_set, input);
    fclose(input);

    vector_t min = { INT_MAX, INT_MAX, INT_MAX };
    vector_t max = { INT_MIN, INT_MIN, INT_MIN };
    for (int i = 0; i < ins_cnt; i++) {

        if ((ins_set[i].max.x < -50 || ins_set[i].min.x > 50) ||
            (ins_set[i].max.y < -50 || ins_set[i].min.y > 50) ||
            (ins_set[i].max.z < -50 || ins_set[i].min.z > 50)) {

                ins_set[i].skip = 1;
                continue;
        }

        // x min
        if (ins_set[i].min.x < min.x) {
            min.x = ins_set[i].min.x;
        }

        // x max
        if (ins_set[i].max.x > max.x) {
            max.x = ins_set[i].max.x;
        }

        // y min
        if (ins_set[i].min.y < min.y) {
            min.y = ins_set[i].min.y;
        }

        // y max
        if (ins_set[i].max.y > max.y) {
            max.y = ins_set[i].max.y;
        }

        // z min
        if (ins_set[i].min.z < min.z) {
            min.z = ins_set[i].min.z;
        }

        // z max
        if (ins_set[i].max.z > max.z) {
            max.z = ins_set[i].max.z;
        }
    }

    int set[(abs(max.x - min.x) + 1)][(abs(max.y - min.y) + 1)][(abs(max.z - min.z) + 1)];
    for (int x = 0; x < (abs(max.x - min.x) + 1); x++) {
        for (int y = 0; y < (abs(max.y - min.y) + 1); y++) {
            for (int z = 0; z < (abs(max.z - min.z) + 1); z++) {
                set[x][y][z] = 0;
            }   
        }
    }

    int cube_count = 0;
    for (int i = 0; i < ins_cnt; i++) {
        printf("instruction #%d will flip cubes to %d...", i, ins_set[i].state);
        if (ins_set[i].skip) {
            printf("skipping\n");
            continue;
        }
        printf("\n");
        for (int x = ins_set[i].min.x; x <= ins_set[i].max.x; x++) {
            if (x < -50 || x > 50) {
                continue;
            }
            for (int y = ins_set[i].min.y; y <= ins_set[i].max.y; y++) {
                if (y < -50 || y > 50) {
                    continue;
                }
                for (int z = ins_set[i].min.z; z <= ins_set[i].max.z; z++) {
                    if (z < -50 || z > 50) {
                        continue;
                    }

                    cube_count += (ins_set[i].state) - set[abs(x - min.x)][abs(y - min.y)][abs(z - min.z)];
                    set[abs(x - min.x)][abs(y - min.y)][abs(z - min.z)] = ins_set[i].state;
                }
            }
        }
    }

    printf("There are %d cubes on\n", cube_count);

    return EXIT_SUCCESS;
}