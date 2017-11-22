//
// Created by Sebastian on 20.11.2017.
//
#include "nes_hw_functionalities.h"
#include "nes_hw_functionalities.c"

#include "globals.h"
#include "input.c"
#include "render.c"

#include "menu.h"
#include "ingame.h"

void main(void){

    while(1){

        /* --- DRAW MENU --- */
        ppu_turn_all_off();
        set_bg_palette(bg_palette);
        set_sprite_palette(sprite_palette);
        ppu_load_bg_palette();
        ppu_load_sprite_palette();
        ppu_draw_background(menu, 'a');
        wait_Vblank();
        ppu_turn_all_on();

        /* --- MENU LOOP --- */
        flag_ingame = 0;

        while(!flag_ingame){

            get_controller_input();
            if(gamepad_1 & START){
                if(!(gamepad_1_old & START)){
                    flag_ingame = 1;
                }
            }
        }

        /* --- INIT INGAME VARIABLES --- */
        paddle_x = 125;
        paddle_y = 220;
        flag_pause = 0;


        ppu_turn_all_off();
        ppu_draw_background(ingame, 'a');
        wait_Vblank();
        ppu_turn_all_on();
        while(1){
            gamepad1_input_handling();

            wait_until_nmi();
            mainloop_render();
        }

    }

}
