#include <stdio.h>
#include <stdlib.h>

#define TEST 0

typedef struct Player player_t;
typedef struct Game game_t;
typedef struct Score wins_t;
typedef struct Status status_t;

struct Player {
    int position;
    int score;
};

struct Game {
    player_t players[2];
    int active_player;
};

struct Score {
    u_int64_t p1;
    u_int64_t p2;
};

struct Status {
    game_t state;
    wins_t wins;
    status_t *next;
};

static status_t *head = NULL;

int compare_state(game_t a, game_t b) {
    return (a.players[0].position == b.players[0].position) && (a.players[1].position == b.players[1].position) &&
        (a.players[0].score == b.players[0].score) && (a.players[1].score == b.players[1].score) &&
        (a.active_player == b.active_player);
}

int load_status(game_t state, wins_t *wins) {
    if (head == NULL) {
        return 0;
    }

    status_t *current = head;
    while (!compare_state(current->state, state)) {
        if (current->next == NULL) {
            return 0;
        } else {
            current = current->next;
        }
    }

    wins->p1 = current->wins.p1;
    wins->p2 = current->wins.p2;
    return 1;
}

void save_status(game_t state, wins_t wins) {
    status_t *status = malloc(sizeof(status_t));
    status->state = state;
    status->wins = wins;

    status->next = head;
    head = status;
}

wins_t play_game(game_t state) {

    wins_t wins;
    wins.p1 = 0;
    wins.p2 = 0;
    if (state.players[0].score >= 21) {
        wins.p1 = 1;
        return wins;
    } else if (state.players[1].score >= 21) {
        wins.p2 = 1;
        return wins;
    }

    if (load_status(state, &wins)) {
        return wins;
    }

    for (int r1 = 1; r1 < 4; r1++) {
        for (int r2 = 1; r2 < 4; r2++) {
            for (int r3 = 1; r3 < 4; r3++) {

                game_t new_state = state;
                new_state.players[new_state.active_player].position += (r1 + r2 + r3);

                while (new_state.players[new_state.active_player].position > 10) {
                    new_state.players[new_state.active_player].position -= 10;
                }

                new_state.players[new_state.active_player].score += new_state.players[new_state.active_player].position;
                new_state.active_player = 1 - new_state.active_player;

                wins_t result = play_game(new_state);
                wins.p1 += result.p1;
                wins.p2 += result.p2;
            }
        }
    }
    save_status(state, wins);
    return wins;
}

int main() {
    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    game_t state;
    fscanf(input, "Player 1 starting position: %d\n", &state.players[0].position);
    fscanf(input, "Player 2 starting position: %d\n", &state.players[1].position);
    fclose(input);

    state.players[0].score = 0;
    state.players[1].score = 0;
    state.active_player = 0;

    wins_t wins = play_game(state);
    printf("Player 1 has won %llu times.\nPlayer 2 has won %llu times.\n", wins.p1, wins.p2);
    printf("The greater of the two is %llu.\n", wins.p1 > wins.p2 ? wins.p1 : wins.p2);

    return EXIT_SUCCESS;
}