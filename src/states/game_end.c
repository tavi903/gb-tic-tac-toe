#ifndef GAME_H
#include "./../../include/gameplay/game.h"
#endif
#include <gbdk/font.h>

void printNumber(uint8_t number, uint8_t pos_x, uint8_t pos_y);

uint8_t game_end_counter = 0;
outcome gameOutcome;

const uint8_t NUMBERS[10] = {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19};
const uint8_t YOU_WIN[7] = {0x39,0x2F,0x35,0x00,0x37,0x29,0x2E};
const uint8_t ITS_A_DRAW[11] = {0x29,0x34,0x07,0x33,0x00,0x21,0x00,0x24,0x32,0x21,0x37};
const uint8_t YOU_LOSE[8] = {0x39,0x2F,0x35,0x00,0x2C,0x2F,0x33,0x25};
const uint8_t NUM_WINS[6] = {0x03,0x37,0x29,0x2E,0x33,0x1A};
const uint8_t NUM_DEFEATS[9] = {0x03,0x24,0x25,0x26,0x25,0x21,0x34,0x33,0x1A};
const uint8_t NUM_DRAWS[7] = {0x03,0x24,0x32,0x21,0x37,0x33,0x1A};

void gameEndSetup(game g) {
    HIDE_SPRITES;
    font_init();
    font_load(font_italic);
    gameOutcome = checkIfSomeoneHasVictory(g); 
    if (gameOutcome == PLAYER_VICTORY) {
        set_bkg_tiles(6, 2, 7, 1, YOU_WIN);
        g->wins_counter++;
    } 
    if (gameOutcome == CPU_VICTORY) {
        set_bkg_tiles(6, 2, 8, 1, YOU_LOSE);
        g->defeats_counter++;
    }
    if (gameOutcome == DRAW) {
        set_bkg_tiles(4, 2, 11, 1, ITS_A_DRAW);
        g->draw_counter++;
    }
    set_bkg_tiles(1, 6, 6, 1, NUM_WINS);
    printNumber(g->wins_counter, 12, 6);
    set_bkg_tiles(1, 8, 7, 1, NUM_DRAWS);
    printNumber(g->draw_counter, 12, 8);
    set_bkg_tiles(1, 10, 9, 1, NUM_DEFEATS);
    printNumber(g->defeats_counter, 12, 10);
}

void gameEndUpdate(game g) {
    game_end_counter++;
    if (game_end_counter >= 8) { // aprox 2 seconds
        game_end_counter = 0;
        g->state = CORE_GAME_LOOP;
    }
}

void printNumber(uint8_t number, uint8_t pos_x, uint8_t pos_y) {
    if (number >= 10) {
        uint8_t first_number = number/10;
        uint8_t second_number = number%10;
        printNumber(first_number, pos_x, pos_y);
        printNumber(second_number, pos_x+1, pos_y);
    }
    if (number < 10)
        set_bkg_tiles(pos_x, pos_y, 1, 1, &NUMBERS[number]);
}