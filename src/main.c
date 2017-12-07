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

void main(void){

    while(1){


        /* --- load palettes--- */
        ppu_turn_all_off();
        set_bg_palette(bg_palette);
        set_sprite_palette(sprite_palette);
        ppu_load_bg_palette();
        ppu_load_sprite_palette();
        ppu_turn_all_on();

        flag_gameover = 0;

        /* --- MENU LOOP --- */
        render_menu();
        while(!flag_gameover) {
            flag_ingame = 0;

            while (!flag_ingame) {
                get_controller_input();
                if (gamepad_1 & START) {
                    if (!(gamepad_1_old & START)) {
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

            render_ingame();

            /* --- INGAME LOOP --- */
            while (flag_ingame) {
                /* init ball */
                ball.pos_x = 135;
                ball.pos_y = 150;
                ball.speed = 1;
                ball.dir = DOWN;
                ball.angle_dir = LEFT;
                ball.angle = HORZ;

                flag_miss = 0;

                brick_hit.tile_left = 0;
                brick_hit.tile_right = 0;
                flag_brickhit = 0;

                while (!flag_miss) {
                    gamepad1_input_handling();
                    mainloop_update();
                    wait_until_nmi();
                    mainloop_render();
                }


                /* --- MISS AND GAME OVER HANDLING --- */
                if (player.lives == 0) {
                    render_gameover();
                    while(!flag_gameover){
                        get_controller_input();
                        if (gamepad_1 & START) {
                            if (!(gamepad_1_old & START)) {
                                flag_gameover = 1;
                                flag_ingame = 0;
                            }
                        }
                    }
                }
                else {
                    --player.lives;
                    wait_until_nmi();
                    render_lives();
                }

            }
        }
    }

}
