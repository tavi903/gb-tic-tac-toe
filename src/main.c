#include <gb/gb.h>
#include "./../include/gameplay/game.h"
#include "./../include/states/game_start.h"
#include "./../include/states/core_game_loop.h"
#include "./../include/states/game_end.h"
#include <rand.h>

struct _game currentGame;
game_state previousState;

void main(void) {

    unsigned char *ptr_div_reg = (unsigned char*) 0xFF04;  // DIV Register
    gameStartSetup(&currentGame);
    initrand((uint16_t) &ptr_div_reg); // Set Random Seed Generator
    
    while (TRUE) {
        (&currentGame)->previous_joypad = (&currentGame)->current_joypad != NULL ? (&currentGame)->current_joypad : (&currentGame)->previous_joypad;
        (&currentGame)->current_joypad = joypad();
        
        // elaborate 4 frame per second
        if ((&currentGame)->frame_counter % 15 == 0) {
            if ((&currentGame)->state == GAME_START) {
                if (previousState != GAME_START)
                    gameStartSetup(&currentGame);
                previousState = (&currentGame)->state;
                gameStartUpdate(&currentGame);
            }
            if ((&currentGame)->state == CORE_GAME_LOOP) {
                if (previousState != CORE_GAME_LOOP)
                    coreGameLoopSetup(&currentGame);
                previousState = (&currentGame)->state;
                coreGameLoopUpdate(&currentGame);
            }
            if ((&currentGame)->state == GAME_END) {
                if (previousState == CORE_GAME_LOOP)
                    gameEndSetup(&currentGame);
                previousState = (&currentGame)->state;
                gameEndUpdate(&currentGame);
            }
        }

        (&currentGame)->frame_counter++;
        if ((&currentGame)->frame_counter > 59)
            (&currentGame)->frame_counter = 0;
        // Wait until the vertical blank phase has completed
        wait_vbl_done();
    }

}