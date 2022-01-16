typedef struct Amphipod amphipod_t;
typedef struct Space space_t;
typedef struct Hallway hall_t;
typedef struct Room room_t;
typedef struct Burrow burrow_t;

typedef enum {
    AMBER = 1,
    BRONZE = 10,
    COPPER = 100,
    DESERT = 1000
} amphitype_t;

struct Amphipod {
    amphitype_t type;
    int moves;
    space_t *space;
};

struct Space {
    amphipod_t *occupant;

    space_t *up;
    space_t *down;
    space_t *left;
    space_t *right;
};

struct Hallway {
    space_t spots[11];
};

struct Room {
    amphitype_t type;
    space_t *hall_left;
    space_t *hall_right;

    space_t spots[2];
};

struct Burrow {
    int energy;
    amphipod_t amphis[8];
    hall_t hallway;
    room_t rooms[4];
};