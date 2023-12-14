#include <gb/gb.h>
#include <rand.h>
#ifndef CORE_GAME_LOOP_H
#include "./../../include/states/core_game_loop.h"
#endif
#ifndef COMMON_H
#include "./../../include/utils/common.h"
#endif

#include "./../../include/graphics/tris-board.h"
#include "./../../include/graphics/X_symbol.h"
#include "./../../include/graphics/O_symbol.h"
#include "./../../include/graphics/moving-icon.h"


void elaboratePlayerAction(game g);
void elaborateCpuAction(game g);
void updatePlayerPosition(game g);
void drawMovingIconSprite(void);
void changeGameStateIfGameIsOver(game g);

const uint8_t posX_in_px[3] = {12, 60, 110};
const uint8_t posY_in_px[3] = {14, 61, 108};
const uint8_t posX_symbol_in_tiles[3] = {2, 8, 14};
const uint8_t posY_symbol_in_tiles[3] = {1, 7, 13};

uint8_t core_loop_counter = 0;
uint8_t current_pos_x = 0;
uint8_t current_pos_y = 0;
bool hasPlayerSelect = false;

unsigned char X_symbol_map[16];
unsigned char O_symbol_map[16];

void coreGameLoopSetup(game g) {
    initEmptyBoard(g);
    initrand(DIV_REG); // TODO: control this part

    uint8_t current_offset = 0;
    set_bkg_data(current_offset, tris_board_TILE_COUNT, tris_board_tiles);
    set_bkg_tiles(0, 0, tris_board_WIDTH / tris_board_TILE_W, tris_board_HEIGHT / tris_board_TILE_H, tris_board_map);
    
    current_offset += tris_board_TILE_COUNT;
    set_bkg_data(current_offset, X_TILE_COUNT, X_tiles);
    

    for (uint8_t i = 0; i < 16; i++) {
        X_symbol_map[i] = X_map[i];
        O_symbol_map[i] = O_map[i];
    }

    translateMapToCurrentOffset(X_symbol_map, 16, current_offset);
    current_offset += X_TILE_COUNT;
    set_bkg_data(current_offset, O_TILE_COUNT, O_tiles);
    translateMapToCurrentOffset(O_symbol_map, 16, current_offset);

    // setting tiles inside the VRAM
    set_sprite_data(0, moving_icon_TILE_COUNT, moving_icon_tiles);
    // Setting up OAM
    for (uint8_t i = 0; i < moving_icon_TILE_COUNT; i++) {
        set_sprite_tile(i, i);
    }

    SHOW_SPRITES;
}

void coreGameLoopUpdate(game g) {
    if (checkIfSomeoneHasVictory(g) == EMPTY)
        elaboratePlayerAction(g);
    else {
        changeGameStateIfGameIsOver(g);
        return;
    }
    if (checkIfSomeoneHasVictory(g) == EMPTY)
        elaborateCpuAction(g);
}

void elaboratePlayerAction(game g) {
    uint8_t joypadInput = g->current_joypad != NULL ? g->current_joypad : g->previous_joypad;
    if (joypadInput == J_A && !hasPlayerSelect) {
        if (g->board[current_pos_x][current_pos_y] == EMPTY_CELL) {
            g->board[current_pos_x][current_pos_y] = X;
            set_bkg_tiles(posX_symbol_in_tiles[current_pos_x], posY_symbol_in_tiles[current_pos_y], 4, 4, X_symbol_map);
            hasPlayerSelect = true;
        }
        g->previous_joypad = NULL;
    }
    updatePlayerPosition(g);
    drawMovingIconSprite();
}

void elaborateCpuAction(game g) {
    if (hasPlayerSelect) {
        /*
            For every EMPTY cell check if CPU can win
        */
        for (uint8_t i = 0; i < 3; i++) {
            for (uint8_t j = 0; j < 3; j++) {
                if (g->board[i][j] == EMPTY_CELL) {
                    g->board[i][j] = O;
                    if (checkIfSomeoneHasVictory(g) == CPU_VICTORY) {
                        set_bkg_tiles(posX_symbol_in_tiles[i], posY_symbol_in_tiles[j], 4, 4, O_symbol_map);
                        hasPlayerSelect = false;
                        return;
                    } else {
                        g->board[i][j] = EMPTY_CELL;
                    }
                }
            }
        }
        /*
            For every EMPTY cell check if Player can win
        */
        for (uint8_t i = 0; i < 3; i++) {
            for (uint8_t j = 0; j < 3; j++) {
                if (g->board[i][j] == EMPTY_CELL) {
                    g->board[i][j] = X;
                    if (checkIfSomeoneHasVictory(g) == PLAYER_VICTORY) {
                        g->board[i][j] = O;
                        set_bkg_tiles(posX_symbol_in_tiles[i], posY_symbol_in_tiles[j], 4, 4, O_symbol_map);
                        hasPlayerSelect = false;
                        return;
                    } else {
                        g->board[i][j] = EMPTY_CELL;
                    }
                }
            }
        }
        uint8_t random_pos_x = generateRandomNumber(0, 2);
        uint8_t random_pos_y = generateRandomNumber(0, 2);
        while (g->board[random_pos_x][random_pos_y] != EMPTY_CELL) {
            random_pos_x = generateRandomNumber(0, 2);
            random_pos_y = generateRandomNumber(0, 2);
        };
        g->board[random_pos_x][random_pos_y] = O;
        set_bkg_tiles(posX_symbol_in_tiles[random_pos_x], posY_symbol_in_tiles[random_pos_y], 4, 4, O_symbol_map);
        hasPlayerSelect = false;
    }
}

/**
 * Side Effect: change the values of global variables: current_pos_x and current_pos_y.
*/
void updatePlayerPosition(game g) {
    if (g->current_joypad == g->previous_joypad)
        return;
    uint8_t joypadInput = g->current_joypad != NULL ? g->current_joypad : g->previous_joypad;
    switch (joypadInput) {
        case J_RIGHT:
            if (current_pos_x < 2)
                current_pos_x++;
            break;
        case J_DOWN:
            if (current_pos_y < 2)
                current_pos_y++;
            break;
        case J_LEFT:
            if (current_pos_x > 0)
                current_pos_x--;
            break;
        case J_UP:
            if (current_pos_y > 0)
                current_pos_y--;
            break;
        default:
            break;
    }
    g->previous_joypad = NULL;
    g->current_joypad = NULL;
}

void drawMovingIconSprite(void) {
    for (uint8_t i = 0; i < moving_icon_HEIGHT / 8; i++) {
        for (uint8_t j = 0; j < moving_icon_WIDTH / 8; j++) {
            move_sprite(moving_icon_sprite[i][j], posX_in_px[current_pos_x] + (j+1) * 8, posY_in_px[current_pos_y] + (i+1) * 8);
        }
    }    
}

void changeGameStateIfGameIsOver(game g) {
    core_loop_counter++;
    if (core_loop_counter >= 8) {
        core_loop_counter = 0;
        g->state = GAME_END;
    }
    return;
}