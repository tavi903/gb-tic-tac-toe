#ifndef GAME_H
#include "./../../include/gameplay/game.h"
#endif
#include <stdbool.h>

bool allCellAreSelected(game g);
outcome computeOutcome(cell_value x, cell_value y, cell_value z);

void initEmptyBoard(game g) {
    g->frame_counter = 0;
    g->previous_joypad = NULL;
    g->current_joypad = NULL;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            g->board[i][j] = EMPTY_CELL;
        }
    }
}

/**
 * Returns the game outcome, EMPTY if the game is not finished yet.
*/
outcome checkIfSomeoneHasVictory(game g) {
    outcome result;
    // check row victory conditions
    for (uint8_t i = 0; i < 3; i++) {
        result = computeOutcome(g->board[i][0], g->board[i][1], g->board[i][2]);
        if (result != EMPTY)
            return result;
    }
    // check column victory conditions
    for (uint8_t j = 0; j < 3; j++) {
        result = computeOutcome(g->board[0][j], g->board[1][j], g->board[2][j]);
        if (result != EMPTY)
            return result;
    }
    // check diagonal victory conditions
    result = computeOutcome(g->board[0][0], g->board[1][1], g->board[2][2]);
    if (result != EMPTY)
        return result;
    result = computeOutcome(g->board[0][2], g->board[1][1], g->board[2][0]);
    if (result != EMPTY)
        return result;
    if (allCellAreSelected(g))
        return DRAW;
    return EMPTY;
}

bool allCellAreSelected(game g) {
    return g->board[0][0] != EMPTY_CELL && g->board[0][1] != EMPTY_CELL && g->board[0][2] != EMPTY_CELL
        && g->board[1][0] != EMPTY_CELL && g->board[1][1] != EMPTY_CELL && g->board[1][2] != EMPTY_CELL
        && g->board[2][0] != EMPTY_CELL && g->board[2][1] != EMPTY_CELL && g->board[2][2] != EMPTY_CELL;
}

/**
 * Check if Player or CPU has won, otherwise returns EMPTY.
*/
outcome computeOutcome(cell_value x, cell_value y, cell_value z) {
    if (x == X && y == X && z == X)
        return PLAYER_VICTORY;
    if (x == O && y == O && z == O)
        return CPU_VICTORY;
    return EMPTY;
}
