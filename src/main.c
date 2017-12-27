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

    /* credits loop */
    credits_loop = 0;
    render_credits();
    while(credits_loop < 240){
        wait_until_nmi();
        ++credits_loop;
    }

    while(1){
        /* --- load palettes--- */
        ppu_turn_all_off();
        set_bg_palette(bg_palette);
        set_sprite_palette(sprite_palette);
        ppu_load_bg_palette();
        ppu_load_sprite_palette();
        ppu_turn_all_on();

        /* --- MENU LOOP --- */
        render_menu();

        flag_gameover = 0;
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
            player.lives = 4;
            player.score = 0;
            player.score_digit1 = 0;
            player.score_digit2 = 0;
            player.score_digit3 = 0;
            player.speed = 8;

            count_brickhit = 0;

            ppu_turn_all_off();
            render_ingame();

            memcpy(bg_collision_map, ingame_collisionmap, 192); /* load collision map to special memory section for
                                                                 * collision map */
            ppu_turn_all_on();

            /* --- INGAME LOOP --- */
            while (flag_ingame) {
                /* init ball */
                ball.pos_x = 50;
                ball.pos_y = 120;
                ball.angle = MIN;
                ball.speed = 1;
                ball.dir = DOWN;
                ball.angle_dir = RIGHT;

                player.pos_x = 125;

                flag_miss = 0;
                brick_hit.tile_left = 0;
                brick_hit.tile_right = 0;
                flag_brickhit = 0;

                reset_music(); /* reset music engine */

                while (!flag_miss) {
                    gamepad1_input_handling();
                    mainloop_update();
                    wait_until_nmi();
                    mainloop_render();

                    /* special case: when no brick is left, redraw whole bulk of bricks */
                    if(count_brickhit == 84){
                        count_brickhit = 0;
                        ppu_turn_all_off();
                        render_ingame();

                        memcpy(bg_collision_map, ingame_collisionmap, 192); /* load collision map to special memory section for
                                                                 * collision map */
                        ppu_turn_all_on();
                    }
                }


                /* --- MISS AND GAME OVER HANDLING --- */
                if (player.lives == 0) {
                    reset_music();
                    render_gameover();
                    play_music(0);
                    while(!flag_gameover){
                        update_music();
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
