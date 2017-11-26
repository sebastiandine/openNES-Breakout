//
// Created by Sebastian on 20.11.2017.
//
#include "nes_hw_functionalities.h"
#include "nes_hw_functionalities.c"

#include "structures.h"
#include "globals.h"
#include "input.c"
#include "update.c"
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
        flag_pause = 0;

        /* init playfield */
        playfield.edge_top = 32;
        playfield.edge_bottom = 200;
        playfield.edge_left = 5;
        playfield.edge_right = 242;

        /* init player */
        player.pos_x = 125;
        player.score = 0;
        player.lives = 4;
        player.speed = 4;

        while(1) {
            /* init ball */
            ball.pos_x = 135;
            ball.pos_y = 180;
            ball.speed = 1;
            ball.dir = UP;
            ball.angle_dir = LEFT;
            ball.angle = MID;

            flag_miss = 0;

            ppu_turn_all_off();
            ppu_draw_background(ingame, 'a');
            wait_Vblank();
            ppu_turn_all_on();

            while (!flag_miss) {
                gamepad1_input_handling();
                mainloop_update();
                wait_until_nmi();
                mainloop_render();
            }
        }

    }

}
