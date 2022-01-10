#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TEST 0
#define ROTATIONS 24

enum axis {X, Y, Z};

typedef struct Coords Coords;
typedef struct Matrix Matrix;
typedef struct Scanner Scanner;
typedef enum axis Axis;

struct Coords {
    int x;
    int y;
    int z;
};

struct Matrix {
    Coords a;
    Coords b;
    Coords c;
};

struct Scanner {
    int name;
    int beacon_count;
    size_t capacity;
    Coords *beacons;

    Scanner *aligned_to;
    const Matrix *rotation;
    Coords rel_pos;
};

void free_data(Scanner *scanners, int scanner_count) {
    for (int i = 0; i < scanner_count; i++) {
        free(scanners[i].beacons);
    }
    free(scanners);
}

void create_scanner(Scanner *s, size_t capacity) {
    s->beacon_count = 0;
    s->capacity = capacity;
    s->beacons = malloc(capacity * sizeof(Coords));
    s->aligned_to = NULL;
    s->rotation = NULL;
}

Coords copy_coords(Coords coord) {
    Coords copy;
    copy.x = coord.x;
    copy.y = coord.y;
    copy.z = coord.z;
    return copy;
}

int coords_equal(Coords a, Coords b) {

    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

Coords add_coords(Coords a, Coords b) {
    Coords res;

    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;

    return res;
}

Coords sub_coords(Coords a, Coords b) {
    Coords res;

    res.x = a.x - b.x;
    res.y = a.y - b.y;
    res.z = a.z - b.z;

    return res;
}

Coords rotate(Coords coord, const Matrix *rot) {
    Coords res;

	res.x = coord.x * rot->a.x + coord.y * rot->a.y + coord.z * rot->a.z;
	res.y = coord.x * rot->b.x + coord.y * rot->b.y + coord.z * rot->b.z;
	res.z = coord.x * rot->c.x + coord.y * rot->c.y + coord.z * rot->c.z;

    return res;
}

int overlap(Scanner *base, Scanner *test, const Matrix *rot, Coords coord) {

    int matches = 0;
    for (int s1 = 0; s1 < base->beacon_count; s1++) {
        for (int s2 = 0; s2 < test->beacon_count; s2++) {
            Coords a = base->beacons[s1];
            Coords b = test->beacons[s2];

            b = rotate(b, rot);
            b = add_coords(b, coord);

            if (coords_equal(a, b)) {
                matches++;

                if (matches >= 12) {
                    return 1;
                } else {
                    break;
                }
            }
        }

        if ((s1 - matches) > (base->beacon_count - 12)) {
            return 0;
        }
    }

    return 0;
}

int align_scanners(Scanner *base, Scanner *test, const Matrix rot_mats[ROTATIONS]) {

    for (int rot = 0; rot < ROTATIONS; rot++) {
        for (int s1 = 0; s1 < base->beacon_count; s1++) {
            for (int s2 = 0; s2 < test->beacon_count; s2++) {

                Coords coord = test->beacons[s2];
                coord = rotate(coord, &rot_mats[rot]);
                coord = sub_coords(base->beacons[s1], coord);

                if (overlap(base, test, &rot_mats[rot], coord)) {

                    test->aligned_to = base;
                    test->rotation = &rot_mats[rot];
                    test->rel_pos = copy_coords(coord);

                    // printf("Overlap between scanners %d and %d\n", base->name, test->name);
                    return 1;
                }
            }
        }
    }

    // printf("No overlap between scanners %d and %d\n", base->name, test->name);
    return 0;
}

int main() {

    clock_t start, align, stop;
    start = clock();
    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    Scanner *scanners = malloc((TEST ? 5 : 39) * sizeof(Scanner));
    int scanner_count = 0;
    int beacon_size = 0;
    int x, y, z;
    while (fscanf(input, "--- scanner %d ---\n", &scanner_count) == 1) {
        create_scanner(&scanners[scanner_count], 50);
        while(fscanf(input, "%d, %d, %d\n", &x, &y, &z) == 3) {
            scanners[scanner_count].beacons[scanners[scanner_count].beacon_count].x = x;
            scanners[scanner_count].beacons[scanners[scanner_count].beacon_count].y = y;
            scanners[scanner_count].beacons[scanners[scanner_count].beacon_count].z = z;
            scanners[scanner_count].name = scanner_count;
            scanners[scanner_count].beacon_count++;
        }
        beacon_size += scanners[scanner_count].beacon_count;
        // printf("Scanner %d has detected %d beacons.\n", scanner_count, scanners[scanner_count].beacon_count);
    }
    scanner_count++;

    const Matrix rot_mats[ROTATIONS] = {
        {{ 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }},
        {{ 0, 0, 1 }, { 0, 1, 0 }, {-1, 0, 0 }},
        {{-1, 0, 0 }, { 0, 1, 0 }, { 0, 0,-1 }},
        {{ 0, 0,-1 }, { 0, 1, 0 }, { 1, 0, 0 }},
        {{ 0,-1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }},
        {{ 0, 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 }},
        {{ 0, 1, 0 }, { 1, 0, 0 }, { 0, 0,-1 }},
        {{ 0, 0,-1 }, { 1, 0, 0 }, { 0,-1, 0 }},
        {{ 0, 1, 0 }, {-1, 0, 0 }, { 0, 0, 1 }},
        {{ 0, 0, 1 }, {-1, 0, 0 }, { 0,-1, 0 }},
        {{ 0,-1, 0 }, {-1, 0, 0 }, { 0, 0,-1 }},
        {{ 0, 0,-1 }, {-1, 0, 0 }, { 0, 1, 0 }},
        {{ 1, 0, 0 }, { 0, 0,-1 }, { 0, 1, 0 }},
        {{ 0, 1, 0 }, { 0, 0,-1 }, {-1, 0, 0 }},
        {{-1, 0, 0 }, { 0, 0,-1 }, { 0,-1, 0 }},
        {{ 0,-1, 0 }, { 0, 0,-1 }, { 1, 0, 0 }},
        {{ 1, 0, 0 }, { 0,-1, 0 }, { 0, 0,-1 }},
        {{ 0, 0,-1 }, { 0,-1, 0 }, {-1, 0, 0 }},
        {{-1, 0, 0 }, { 0,-1, 0 }, { 0, 0, 1 }},
        {{ 0, 0, 1 }, { 0,-1, 0 }, { 1, 0, 0 }},
        {{ 1, 0, 0 }, { 0, 0, 1 }, { 0,-1, 0 }},
        {{ 0,-1, 0 }, { 0, 0, 1 }, {-1, 0, 0 }},
        {{-1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }},
        {{ 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }}
    };

    int loop = 1;
    int base_scanner = 0;
    while (loop) {
        for (loop = base_scanner = 0; base_scanner < scanner_count; base_scanner++) {

            if (base_scanner != 0 && scanners[base_scanner].aligned_to == NULL) {
                continue;
            }

            for (int test_scanner = 1; test_scanner < scanner_count; test_scanner++) {

                if (scanners[test_scanner].aligned_to != NULL) {
                    continue;
                }
                
                if (align_scanners(&scanners[base_scanner], &scanners[test_scanner], rot_mats)) {
                    loop = 1;
                }
            }
        }
    }
    align = clock();
    printf("Finished with alignment in %f\n", (double)(align - start) / CLOCKS_PER_SEC);

    Coords unique_beacons[beacon_size];
    int beacon_count = 0;
    for (int scanner = 0; scanner < scanner_count; scanner++) {
        for (int beacon = 0; beacon < scanners[scanner].beacon_count; beacon++) {
            Scanner *s = &scanners[scanner];
            Coords b = s->beacons[beacon];

            while (s->aligned_to != NULL) {
                b = rotate(b, s->rotation);
                b = add_coords(b, s->rel_pos);

                s = s->aligned_to;
            }

            int i;
            for (i = 0; i < beacon_count; i++) {
                if (coords_equal(b, unique_beacons[i])) {
                    break;
                }
            }

            if (i == beacon_count) {
                unique_beacons[beacon_count++] = b;
            }
        }
    }

    printf("There are %d beacons\n", beacon_count);

    stop = clock();
    printf("Found answer in in %f\n", (double)(stop - start) / CLOCKS_PER_SEC);
    printf("The answer took %f since align end\n", (double)(stop - align) / CLOCKS_PER_SEC);
    
    free_data(scanners, scanner_count);
    fclose(input);
    return EXIT_SUCCESS;
}