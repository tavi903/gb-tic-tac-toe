#ifndef GAME_START_H
#include "./../../include/states/game_start.h"
#endif
#ifndef COMMON_H
#include "./../../include/utils/common.h"
#endif
#include "./../../include/graphics/tic-tac-toe.h"
#include "./../../include/graphics/press-start.h"

unsigned char press_start_map_translated[press_start_TILE_COUNT];
unsigned char press_start_map_blank[press_start_TILE_COUNT];
bool flag_press_start_visible = TRUE;

void gameStartSetup(game g) {
    g->state = GAME_START;

    uint8_t current_offset = 0;
    set_bkg_data(current_offset, tic_tac_toe_TILE_COUNT, tic_tac_toe_tiles);
    set_bkg_tiles(0, 0, tic_tac_toe_WIDTH / tic_tac_toe_TILE_W, tic_tac_toe_HEIGHT / tic_tac_toe_TILE_H, tic_tac_toe_map);
    current_offset = tic_tac_toe_TILE_COUNT;

    SHOW_BKG;
    DISPLAY_ON;

    
    set_bkg_data(current_offset, press_start_TILE_COUNT, press_start_tiles);
    for (uint8_t i = 0; i < press_start_TILE_COUNT; i++) {
        press_start_map_translated[i] = press_start_map[i];
        press_start_map_blank[i] = 0x00;
    }
    translateMapToCurrentOffset(press_start_map_translated, press_start_TILE_COUNT, current_offset);
    set_bkg_tiles(4, 12, press_start_WIDTH / press_start_TILE_W, press_start_HEIGHT / press_start_TILE_H, press_start_map_translated);

}

void gameStartUpdate(game g) {
    uint8_t joypadInput = g->current_joypad != NULL ? g->current_joypad : g->previous_joypad;
    if (g->frame_counter % 30 == 0) {
        if (flag_press_start_visible) {
        set_bkg_tiles(4, 12, press_start_WIDTH / press_start_TILE_W, press_start_HEIGHT / press_start_TILE_H, press_start_map_translated); 
        } else {
        set_bkg_tiles(4, 12, press_start_WIDTH / press_start_TILE_W, press_start_HEIGHT / press_start_TILE_H, press_start_map_blank); 
        }
        flag_press_start_visible = !flag_press_start_visible;
    }    
    if (joypadInput == J_START) {
        g->previous_joypad = NULL;
        g->current_joypad = NULL;
        g->state = CORE_GAME_LOOP;
    }
}