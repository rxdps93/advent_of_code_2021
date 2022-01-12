#include <stdio.h>
#include <stdlib.h>

#define TEST 0

typedef struct Player {
    int player;
    int position;
    int score;
} player_t;

typedef struct Dice {
    int times_rolled;
    int last_roll;
} dice_t;

dice_t new_dice(void) {
    dice_t dice;
    dice.times_rolled = 0;
    dice.last_roll = 1;
    return dice;
}

int roll_dice(dice_t *dice, int num_rolls) {
    int roll = 0;
    for (int i = 0; i < num_rolls; i++) {
        if (dice->last_roll > 100) {
            dice->last_roll = 1;
        }

        roll += dice->last_roll++;
        dice->times_rolled++;
    }
    return roll;
}

int main() {
    FILE *input;
    char *file_name = TEST ? "../test_input.txt" : "../input.txt";

    if ((input = fopen(file_name, "r")) == NULL) {
        printf("Unable to open file");
        exit(EXIT_FAILURE);
    }

    player_t players[2];
    fscanf(input, "Player %d starting position: %d\n", &players[0].player, &players[0].position);
    fscanf(input, "Player %d starting position: %d\n", &players[1].player, &players[1].position);
    players[0].score = 0;
    players[1].score = 0;

    dice_t dice = new_dice();
    int active_player = 1;
    do {
        active_player = 1 - active_player;
        players[active_player].position += roll_dice(&dice, 3);

        while (players[active_player].position > 10) {
            players[active_player].position -= 10;
        }

        players[active_player].score += players[active_player].position;
    } while (players[active_player].score < 1000);

    printf("Player %d has won with a score of %d.\n", active_player, players[active_player].score);
    printf("Player %d has lost with a score of %d.\n", 1 - active_player, players[1 - active_player].score);
    printf("The dice was rolled %d times.\n", dice.times_rolled);
    
    long ans = players[1 - active_player].score * dice.times_rolled;
    printf("The answer is %ld.\n", ans);

    fclose(input);
    return EXIT_SUCCESS;
}