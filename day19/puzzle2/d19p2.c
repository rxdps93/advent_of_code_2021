#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TEST 0
#define ROTATIONS 24

typedef struct Coords Coords;
typedef struct Matrix Matrix;
typedef struct Scanner Scanner;

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
    int beacon_count;
    size_t capacity;
    Coords *beacons;

    Scanner *aligned_to;
    const Matrix *rotation;
    Coords rel_pos;
    Coords rel_to_zero;

    int *matches;
    int match_count;
};

void free_data(Scanner *scanners, int scanner_count) {
    for (int i = 0; i < scanner_count; i++) {
        free(scanners[i].beacons);
        free(scanners[i].matches);
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

int manhattan_dist(Coords a, Coords b) {

    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
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

                    return 1;
                }
            }
        }
    }

    return 0;
}

void map_all_alignments(Scanner *scanners, int scanner_count) {

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
    int base = 0;
    while (loop) {
        for (loop = base = 0; base < scanner_count; base++) {

            if (base != 0 && scanners[base].aligned_to == NULL) {
                continue;
            }

            for (int test = 0; test < scanners[base].match_count; test++) {

                if (scanners[scanners[base].matches[test]].aligned_to != NULL) {
                    continue;
                }

                if (align_scanners(&scanners[base], &scanners[scanners[base].matches[test]], rot_mats)) {
                    loop = 1;
                }
            }
        }
    }
}

void map_possible_overlaps(Scanner *scanners, int scanner_count) {

    int ovls[scanner_count];
    int ovl_count;
    for (int s1 = 0; s1 < scanner_count; s1++) {

        ovl_count = 0;

        for (int s2 = 1; s2 < scanner_count; s2++) {

            if (s1 == s2) {
                continue;
            }

            int dist_a = 0;
            int dists_a[(scanners[s1].beacon_count * (scanners[s1].beacon_count - 1)) / 2];
            for (int i = 0; i < scanners[s1].beacon_count; i++) {
                for (int j = i + 1; j < scanners[s1].beacon_count; j++) {
                    
                    dists_a[dist_a++] = manhattan_dist(scanners[s1].beacons[i], scanners[s1].beacons[j]);
                }
            }

            int dist_b = 0;
            int dists_b[(scanners[s2].beacon_count * (scanners[s2].beacon_count - 1)) / 2];
            for (int i = 0; i < scanners[s2].beacon_count; i++) {
                for (int j = i + 1; j < scanners[s2].beacon_count; j++) {
                    dists_b[dist_b++] = manhattan_dist(scanners[s2].beacons[i], scanners[s2].beacons[j]);
                }
            }

            int matched[dist_a];
            int matches = 0;
            for (int i = 0; i < dist_a; i++) {
                for (int j = 0; j < dist_b; j++) {
                    if (dists_a[i] == dists_b[j]) {
                        matched[matches++] = dists_a[i];
                        break;
                    }
                }
            }

            if (matches >= 66) {
                ovls[ovl_count++] = s2;
            }

        }

        scanners[s1].match_count = 0;
        scanners[s1].matches = malloc(ovl_count * sizeof(int));
        for (int i = 0; i < ovl_count; i++) {
            scanners[s1].matches[scanners[s1].match_count++] = ovls[i];
        }

    }
}

void scanners_relative_to_zero(Scanner *scanners, int scanner_count) {

    for (int i = 0; i < scanner_count; i++) {

        Coords to_zero = scanners[i].rel_pos;
        Scanner *s = scanners[i].aligned_to;
        while (s != NULL && s->aligned_to != NULL) {
            to_zero = rotate(to_zero, s->rotation);
            to_zero = add_coords(to_zero, s->rel_pos);

            s = s->aligned_to;
        }

        scanners[i].rel_to_zero = to_zero;
    }
}

Coords largest_scanner_gap(Scanner *scanners, int scanner_count) {

    Coords max_dist = {0, 0, 0};
    for (int i = 0; i < scanner_count - 1; i++) {
        for (int j = i + 1; j < scanner_count; j++) {
            int dist = manhattan_dist(scanners[i].rel_to_zero, scanners[j].rel_to_zero);

            if (dist > max_dist.z) {
                max_dist.x = i;
                max_dist.y = j;
                max_dist.z = dist;
            }
        }
    }

    return max_dist;
}

int main() {

    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    Scanner *scanners = malloc((TEST ? 5 : 39) * sizeof(Scanner));
    int scanner_count = 0;
    int x, y, z;
    while (fscanf(input, "--- scanner %d ---\n", &scanner_count) == 1) {
        create_scanner(&scanners[scanner_count], 50);
        while(fscanf(input, "%d, %d, %d\n", &x, &y, &z) == 3) {
            scanners[scanner_count].beacons[scanners[scanner_count].beacon_count].x = x;
            scanners[scanner_count].beacons[scanners[scanner_count].beacon_count].y = y;
            scanners[scanner_count].beacons[scanners[scanner_count].beacon_count].z = z;
            scanners[scanner_count].beacon_count++;
        }
    }
    scanner_count++;

    map_possible_overlaps(scanners, scanner_count);
    map_all_alignments(scanners, scanner_count);
    scanners_relative_to_zero(scanners, scanner_count);
    Coords max_dist = largest_scanner_gap(scanners, scanner_count);

    printf("The largest gap is between Scanners %d and %d with a distance of %d units.\n", max_dist.x, max_dist.y, max_dist.z);
    
    free_data(scanners, scanner_count);
    fclose(input);
    return EXIT_SUCCESS;
}