#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TEST 1

typedef struct Coords Coords;
typedef struct Scanner Scanner;

struct Coords {
    int x;
    int y;
    int z;
};

struct Scanner {
    int x_rot;
    int y_rot;
    int z_rot;
    int beacon_count;
    size_t capacity;
    Coords *beacons;
};

void free_data(Scanner *scanners, int scanner_count) {
    for (int i = 0; i < scanner_count; i++) {
        free(scanners[i].beacons);
    }
    free(scanners);
}

void create_scanner(Scanner *s, size_t capacity) {
    s->x_rot = 0;
    s->y_rot = 0;
    s->z_rot = 0;

    s->beacon_count = 0;
    s->capacity = capacity;
    s->beacons = malloc(capacity * sizeof(Coords));
}

Coords copy_coords(Coords coord) {
    Coords copy;
    copy.x = coord.x;
    copy.y = coord.y;
    copy.z = coord.z;
    return copy;
}

double dist_test(Scanner a, int beacon_a, Scanner b, int beacon_b) {

    Coords a = copy_coords(a.beacons[beacon_a]);
    Coords b = copy_coords(b.beacons[beacon_b]);
}

double distance(Coords a, Coords b) {

    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
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
        printf("Scanner %d has detected %d beacons.\n", scanner_count, scanners[scanner_count].beacon_count);
    }
    scanner_count++;

    printf("no rotations\n");
    for (int i = 0; i < scanners[0].beacon_count; i++) {
        printf("distance from S0B%02d and S1B%02d: %f\n", i, i, distance(scanners[0].beacons[i], scanners[1].beacons[i]));
    }

    printf("\nrotate on x\n");
    for (int i = 0; i < scanners[0].beacon_count; i++) {
        printf("distance from S0B%02d and S1B%02d: %f\n", i, i, distance(scanners[0].beacons[i], scanners[1].beacons[i]));
    }
    
    free_data(scanners, scanner_count);
    fclose(input);
    return EXIT_SUCCESS;
}