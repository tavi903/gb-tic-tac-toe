#define GAME_H

#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum  { X, O, EMPTY_CELL } cell_value;
typedef enum  { PLAYER_VICTORY, CPU_VICTORY, DRAW, EMPTY } outcome;
typedef enum  { GAME_START, CORE_GAME_LOOP, GAME_END } game_state;

struct _game {
    uint8_t frame_counter;
    uint8_t previous_joypad;
    uint8_t current_joypad;
    game_state state;
    cell_value board[3][3];
    uint8_t wins_counter;
    uint8_t defeats_counter;
    uint8_t draw_counter;
};

typedef struct _game* game;

void initEmptyBoard(game g);
outcome checkIfSomeoneHasVictory(game g);