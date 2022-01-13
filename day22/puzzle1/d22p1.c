#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define TEST 1

typedef struct Vector {
    int x;
    int y;
    int z;
} vector_t;

typedef struct Cube {
    int state;
    vector_t pos;

    int change[500];
    int ins_cnt;
} cube_t;

typedef struct Instruction {
    int state;

    int x_min;
    int x_max;
    
    int y_min;
    int y_max;

    int z_min;
    int z_max;
} ins_t;

int vectors_match(vector_t a, vector_t b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

int find_cube(cube_t *cubes, int size, vector_t pos) {
    for (int i = 0; i < size; i++) {
        if (vectors_match(cubes[i].pos, pos) && cubes[i].ins_cnt != -1) {
            return i;
        }
    }

    return -1;
}

int read_input(ins_t ins_set[500], FILE *input) {
    int ins_cnt = -1;
    char c;
    do {
        fgetc(input);
        c = fgetc(input);
        ins_cnt++;
        if (c == 'n') {
            ins_set[ins_cnt].state = 1;
            fgetc(input);
        } else {
            ins_set[ins_cnt].state = 0;
            fseek(input, 2, SEEK_CUR);
        }

    } while(fscanf(input, "x=%d..%d,y=%d..%d,z=%d..%d\n",
            &ins_set[ins_cnt].x_min, &ins_set[ins_cnt].x_max,
            &ins_set[ins_cnt].y_min, &ins_set[ins_cnt].y_max,
            &ins_set[ins_cnt].z_min, &ins_set[ins_cnt].z_max) == 6);

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

        // x min
        if (ins_set[i].x_min < min.x) {
            min.x = ins_set[i].x_min;
        }

        // x max
        if (ins_set[i].x_max > max.x) {
            max.x = ins_set[i].x_max;
        }

        // y min
        if (ins_set[i].y_min < min.y) {
            min.y = ins_set[i].y_min;
        }

        // y max
        if (ins_set[i].y_max > max.y) {
            max.y = ins_set[i].y_max;
        }

        // z min
        if (ins_set[i].z_min < min.z) {
            min.z = ins_set[i].z_min;
        }

        // z max
        if (ins_set[i].z_max > max.z) {
            max.z = ins_set[i].z_max;
        }
    }
    int num_cubes = (abs(max.x - min.x) + 1) * (abs(max.y - min.y) + 1) * (abs(max.z - min.z) + 1);

    cube_t cubes[num_cubes];
    for (int i = 0; i < num_cubes; i++) {
        cubes[i].ins_cnt = -1;
    }

    int c;
    vector_t pos;
    for (int i = 0; i < ins_cnt; i++) {
        for (int x = ins_set[i].x_min; x <= ins_set[i].x_max; x++) {
            for (int y = ins_set[i].y_min; y <= ins_set[i].y_max; y++) {
                for (int z = ins_set[i].z_min; z <= ins_set[i].z_max; z++) {
                    pos.x = x;
                    pos.y = y;
                    pos.z = z;
                    int c = find_cube(cubes, num_cubes, pos);

                    if (c == -1) {

                    }

                    
                }
            }
        }
    }

    return EXIT_SUCCESS;
}