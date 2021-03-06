#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TEST 0
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

typedef struct Vector {
    int x;
    int y;
    int z;
} vector_t;

typedef struct Cuboid {
    int state;
    vector_t min;
    vector_t max;
} cuboid_t;

typedef struct Instruction {
    int state;

    vector_t min;
    vector_t max;
} ins_t;

u_int64_t vector_volume(vector_t min, vector_t max) {
    return (u_int64_t)((max.x - min.x) + 1) * ((max.y - min.y) + 1) * ((max.z - min.z) + 1);
}

u_int64_t cuboid_volume(cuboid_t c) {
    return vector_volume(c.min, c.max);
}

u_int64_t instruction_volume(ins_t i) {
    return vector_volume(i.min, i.max);
}

int cuboid_intersect(cuboid_t *i, cuboid_t c1, cuboid_t c2) {
    cuboid_t tmp = { 0, { MAX(c1.min.x, c2.min.x), MAX(c1.min.y, c2.min.y), MAX(c1.min.z, c2.min.z) },
                     { MIN(c1.max.x, c2.max.x), MIN(c1.max.y, c2.max.y), MIN(c1.max.z, c2.max.z) } };

    if ((tmp.min.x > tmp.max.x) || (tmp.min.y > tmp.max.y) || (tmp.min.z > tmp.max.z)) {
        i = NULL;
        return 0;
    } else {
        *i = tmp;
        return 1;
    }
}

int read_input(ins_t ins_set[500], FILE *input) {
    int num = 0;
    char ins[4];
    int x_min, y_min, z_min, x_max, y_max, z_max;
    while (fscanf(input, "%[^ ] x=%d..%d,y=%d..%d,z=%d..%d\n", ins, &x_min, &x_max, &y_min, &y_max, &z_min, &z_max) == 7) {

        ins_set[num].state = (ins[1] == 'n');

        ins_set[num].min.x = x_min;
        ins_set[num].min.y = y_min;
        ins_set[num].min.z = z_min;
        
        ins_set[num].max.x = x_max;
        ins_set[num].max.y = y_max;
        ins_set[num].max.z = z_max;
        num++;
    }

    return num;
}

int main() {
    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    ins_t ins_set[500];
    int ins_cnt = read_input(ins_set, input);
    fclose(input);

    uint64_t reactor_size = 0;
    uint64_t reactor_capacity = 1024;
    cuboid_t *reactor = calloc(reactor_capacity, sizeof(cuboid_t));

    uint64_t add_size = 0;
    uint64_t add_cap = 1024;
    cuboid_t *to_add = calloc(add_cap, sizeof(cuboid_t));
    cuboid_t *tmp;
    for (int i = 0; i < ins_cnt; i++) {
        cuboid_t c = { ins_set[i].state, ins_set[i].min, ins_set[i].max };
        if (add_size > 0) {
            add_size = 0;
            add_cap = 1024;
            tmp = (cuboid_t *)realloc(to_add, add_cap * sizeof(cuboid_t));

            if (!tmp) {
                free(reactor);
                free(to_add);
                printf("Failed realloc!\n");
                exit(EXIT_FAILURE);
            }

            to_add = tmp;
        }

        if (ins_set[i].state) {
            to_add[add_size++] = c;
        }

        for (uint64_t rc = 0; rc < reactor_size; rc++) {
            cuboid_t intersect;
            int inter = cuboid_intersect(&intersect, c, reactor[rc]);
            if (inter) {
                intersect.state = -reactor[rc].state;
                to_add[add_size++] = intersect;

                if (add_size == add_cap) {
                    add_cap *= 2;
                    tmp = (cuboid_t *)realloc(to_add, add_cap * sizeof(cuboid_t));
                    if (!tmp) {
                        free(reactor);
                        free(to_add);
                        printf("Failed realloc\n");
                        exit(EXIT_FAILURE);
                    }

                    to_add = tmp;
                }
            }
        }

        for (uint64_t a = 0; a < add_size; a++) {
            reactor[reactor_size++] = to_add[a];

            if (reactor_size == reactor_capacity) {
                reactor_capacity *= 2;
                tmp = (cuboid_t *)realloc(reactor, reactor_capacity * sizeof(cuboid_t));
                if (!tmp) {
                    printf("Reactor realloc failure\n");
                    free(reactor);
                    free(to_add);
                    exit(EXIT_FAILURE);
                }
                reactor = tmp;
            }
        }
    }
    free(to_add);

    uint64_t cubes = 0;
    for (uint64_t i = 0; i < reactor_size; i++) {
        cubes += reactor[i].state * cuboid_volume(reactor[i]);
    }

    printf("There are %llu cubes on.\n", cubes);

    free(reactor);
    return EXIT_SUCCESS;
}