//
// Created by Sebastian on 20.11.2017.
//
#include "nes_hw_functionalities.h"
#include "nes_hw_functionalities.c"

#include "structures.h"
#include "macros.h"
#include "globals.h"
#include "input.c"
#include "update.c"
#include "render.c"

void main(void){

    /* CREDITS */
    render_credits();
/*-------------------------------------------------------------------------------------*/
    while(1){
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

/*-------------------------------------------------------------------------------------*/
            /* --- INIT INGAME VARIABLES --- */
            flag_pause = 0;
            count_brickhit = 0;

            /* init player */
            player.lives = 4;
            player.score = 0;
            player.speed = 8;

            /* init score digits */
            player.score_digit1 = 0;
            player.score_digit2 = 0;
            player.score_digit3 = 0;
            calc_score_digits();

			/* render ingame */
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
						
						ball.pos_x = 50;
						ball.pos_y = 120;
						ball.angle = MIN;
						ball.speed = 1;
						ball.dir = DOWN;
						ball.angle_dir = RIGHT;
						
                        ppu_turn_all_off();
                        render_ingame();

                        memcpy(bg_collision_map, ingame_collisionmap, 192); /* load collision map to special memory section for
                                                                 * collision map */
                        ppu_turn_all_on();
                    }
                }

/*-------------------------------------------------------------------------------------*/
                /* --- MISS AND GAME OVER HANDLING --- */
                if (player.lives == 0) {
                    /*--- game over handling */
                    render_gameover();

                    sleep_loop = 0;
                    play_soundeffect(5);
                    while(++sleep_loop < 60){  /* wait 60 frames to play the whole game over soundeffect */
                        update_music();
                        wait_until_nmi();
                    }
                    reset_music();

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
                    /* simple miss handling */
                else {
                    --player.lives;
                    wait_until_nmi();
                    render_lives();
                }

            }
        }
    }

}
