#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../../common/utils.h"

typedef struct {
    int row;
    int col;
} Pair;

typedef struct {
    int numbers[5][5];
    int marked[5][5];
    int win_num;
    int row_ptr;
    int col_ptr;
    int num_mrk;
} Board;

void board_init(Board *board) {
    board->row_ptr = 0;
    board->col_ptr = 0;
    board->num_mrk = 0;
    board->win_num = 0;
    memset(board->marked, 0, sizeof(board->marked));
}

void pair_init(Pair *pair, int row, int col) {
    pair->row = row;
    pair->col = col;
}

Pair board_add(Board *board, int num) {
    board->numbers[board->row_ptr][board->col_ptr] = num;

    Pair pair;
    pair_init(&pair, board->row_ptr, board->col_ptr);

    board->col_ptr++;
    if (board->col_ptr > 4) {
        board->col_ptr = 0;
        board->row_ptr = (board->row_ptr == 4 ? 0 : board->row_ptr + 1);
    }

    return pair;
}

int fget_board(FILE *file, Board *board) {
    Pair pair;
    int num, status;
    for (int i = 0; i < 25; i++) {
        status = fscanf(file, "%d", &num);
        pair = board_add(board, num);

        if (status != 1)
            break;
    }
    return status;
}

void print_board(Board *board) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (board->marked[row][col]) {
                printf("[%2d] ", board->numbers[row][col]);
            } else {
                printf(" %2d  ", board->numbers[row][col]);
            }
        }
        printf("\n");
    }
}

int mark_board(Board *board, int num) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (board->numbers[row][col] == num) {
                board->marked[row][col] = 1;
                board->num_mrk++;
                return 1;
            }
        }
    }
    return 0;
}

int win_check(Board *board) {
    // Check rows
    int count = 0;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            count += board->marked[row][col];
        }

        if (count == 5) {
            return 1;
        } else {
            count = 0;
        }
    }

    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 5; row++) {
            count += board->marked[row][col];
        }

        if (count ==5) {
            return 1;
        } else {
            count = 0;
        }
    }

    return 0;
}

int moves_to_win(Board *board, IntArray *num_lot) {

    int moves;
    for (moves = 0; moves < (int)num_lot->size; moves++) {
        // Try to mark the board
        // If marked, check how many are marked. If at least 5, check for win
        if (mark_board(board, num_lot->array[moves])) {
            if (board->num_mrk >= 5) {
                if (win_check(board)) {
                    board->win_num = num_lot->array[moves];
                    break;
                }
            }
        }
    }
    return moves;
}

int board_score(Board *board) {
    int score = 0;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if (!board->marked[row][col]) {
                score += board->numbers[row][col];
            }
        }
    }
    return score;
}

int main() {

	FILE *input;
	
	if ((input = fopen("../input.txt", "r")) == NULL) {
		printf("Unable to open file");
		exit(EXIT_FAILURE);
	}

    // Store input line
    int num;
    char c;
    IntArray num_lot;
    init_IntArray(&num_lot, 10);
    do {
        fscanf(input, "%d%c", &num, &c);
        insert_IntArray(&num_lot, num);
    } while (c != '\n');

    // Read in board to struct, determine number of moves to win, if highest, calculate score
    Board current_board;
    Board winning_board;
    board_init(&winning_board);
    int status, moves, score;
    int best_moves = INT_MAX;
    do {
        board_init(&current_board);
        status = fget_board(input, &current_board);
        if (status != -1) {
            moves = moves_to_win(&current_board, &num_lot);

            if (moves < best_moves) {
                best_moves = moves;
                winning_board = current_board;
            }
        }
    } while (status != -1);
    score = board_score(&winning_board);

    printf("\nWINNING BOARD:\n");
    print_board(&winning_board);
    printf("\nWon with %d on move %d\n", winning_board.win_num, best_moves);
    printf("%5d\n%5d\n-----\n%5d\n", score, winning_board.win_num, score * winning_board.win_num);

    free_IntArray(&num_lot);
    fclose(input);
    return EXIT_SUCCESS;
}