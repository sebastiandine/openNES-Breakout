/**
 * @file update.c
 * @brief This file contains the actual game logic and calculations which will be executed during the update phase of
 * a frame.
 * @author Sebastian Dine
 */

#include "ingame_collisionmap.h"

/**
 * @brief This function calculates the movement of the ball for the next frame.
 */
void ball_movement(void){

    if (ball.dir == UP) {
        ball.pos_y -= ball.speed;

        if (ball.angle_dir == LEFT) {
            ball.pos_x -= ball.angle;
        }
        if (ball.angle_dir == RIGHT) {
            ball.pos_x += ball.angle;
        }

        return;
    }

    if (ball.dir == DOWN) {
        ball.pos_y += ball.speed;

        if (ball.angle_dir == LEFT) {
            ball.pos_x -= ball.angle;
        }
        if (ball.angle_dir == RIGHT) {
            ball.pos_x += ball.angle;
        }

        return;
    }
}

/**
 * @brief This function checks the collision of the ball with the playfield and calculates the
 * resulting movement of the ball in case of a collision.
 */
void collision_check_ball_playfield(void){

    if(ball.pos_x < playfield.edge_left){
        if(ball.angle_dir == LEFT){
            ball.angle_dir = RIGHT;
            return;
        }
        else {
            ball.angle_dir = LEFT;
            return;
        }
    }

    if(ball.pos_x > playfield.edge_right){
        if(ball.angle_dir == LEFT){
            ball.angle_dir = RIGHT;
            return;
        }
        else {
            ball.angle_dir = LEFT;
            return;
        }
    }

    if(ball.pos_y < playfield.edge_top) {
        ball.dir = DOWN;
        return;
    }
}

/**
 * @brief this function calculates the collision between the ball and the bricks.
 * @return 1, if the ball hit a brick during this frame, 0 if not.
 */
unsigned char collision_check_ball_brick(void){

    if(ball.pos_y < 111 && ball.pos_y > 48){ //ball is one bg line before first line of bricks

        /* get tile in collision map */
        collision_addr = MAPARRAY_ADR(ball.pos_x, (ball.pos_y - ball.speed)) - 0xC0; /* convert bg tile address to collision map tile address.
                                                                                      * - 0xC0 since the collision map starts
                                                                                       at bg tile 0xE0 */
        collision_tile = bg_collision_map[collision_addr];

        if(collision_tile == 0x61){
            ++player.score;
            ball.dir = DOWN;

            /* update collision map */
            bg_collision_map[collision_addr] = 0x00;
            bg_collision_map[collision_addr +1] = 0x00;

            /* calculate bg tiles, which need to be rendered with blank tiles */
            brick_hit.tile_left = collision_addr - 0x20; //convert collision map tile address to bg-tile address
            brick_hit.tile_right = brick_hit.tile_left + 1;

            return 1;
        }
        if(collision_tile == 0x62){
            ball.dir = DOWN;
            ++player.score;

            /* update collision map */
            bg_collision_map[collision_addr] = 0x00;
            bg_collision_map[collision_addr -1] = 0x00;

            /* calculate bg tiles, which need to be rendered with blank tiles */
            brick_hit.tile_right = collision_addr - 0x20; //convert collision map tile address to bg-tile address
            brick_hit.tile_left = brick_hit.tile_right - 1;

            return 1;
        }

    }
    return 0;

}

/**
 * @brief This function calculates the digits of the players score.
 * Since calculating digits (reason modulo opp) is quite time consuming, it needs to be put out of
 * the render routine.
 */
void calc_score_digits(void){

    player.score_digit1 = 0x10 + (player.score >> 10);
    player.score_digit2 = 0x10 + ((player.score / 10)%10);
    player.score_digit3 = 0x10 + (player.score %10);
}

/**
 * @brief This function calculates the speed of the balld and the score of the player, corresponding
 * to the brick, the player hit during the current frame.
 */
void calc_score_and_speed(void){

    if(collision_addr > 1 && collision_addr < 30){
        ball.speed = 4;
        player.score += 4;
        calc_score_digits();
        return;
    }
    if(collision_addr > 33 && collision_addr < 62){
        ball.speed = 3;
        player.score += 3;
        calc_score_digits();
        return;
    }
    if(collision_addr > 65 && collision_addr < 94){
        ball.speed = 3;
        player.score += 3;
        calc_score_digits();
        return;
    }
    if(collision_addr > 97 && collision_addr < 126){
        ball.speed = 2;
        player.score += 2;
        calc_score_digits();
        return;
    }
    if(collision_addr > 129 && collision_addr < 158){
        ball.speed = 2;
        player.score += 2;
        calc_score_digits();
        return;
    }
    if(collision_addr > 161 && collision_addr < 190){
        ball.speed = 1;
        player.score += 1;
        calc_score_digits();
        return;
    }


}


/**
 * @brief This function checks the collision of the ball with the player and calculates the
 * resulting movement of the ball in case of a collision.
 */
void collision_check_ball_player(void){
    if((ball.pos_y + ball.speed) > (playfield.edge_bottom - 8)) {   /* consider speed to avoid rendering the ball into the player */
                                                                    /* since the paddle is 8 pixels high, the check needs to be done
                                                                       with player_y - 8*/
        if (ball.pos_x > (player.pos_x - 8) && ball.pos_x < (player.pos_x + 24)) { /* range of paddle is ]player_x-8; player_x + 24[ */
            ball.dir = UP;


            /* Each paddle is divided into 7 sections according to the angle the ball bounces back, when it hits the specific
             * section of the paddle. Those sections do not have the same size, therefore it is more likely to hit several
             * sections than other. Therefore in the following the collision check is ordered by the likeliness of the section
             * in terms of its size:
             *
             * ]player_x - 2; player_x - 6[ && ]player_x + 10; player_x + 18[    : 14 px section -> ball bounces back in minimum angle
             * ]player_x - 6; player_x - 1[ && ]player_x + 17; player_x + 22[    : 8 px section -> ball bounces back in medium angle
             * ]player_x + 5; player_x + 11[                                     : 4 px section -> ball bounces back horizontally
             * ]player_x - 8; player_x - 5[ && ]player_x + 21; player_x + 24[    : 4 px section -> ball bounces back in maximum angle and inverses direction
             *
             */

            /* minimum angle */
            if(ball.pos_x < (player.pos_x + 6) && ball.pos_x > (player.pos_x - 2)){
                ball.angle = MIN;
                return;
            }
            if(ball.pos_x > (player.pos_x + 10) && ball.pos_x < (player.pos_x + 18)){
                ball.angle = MIN;
                return;
            }

            /* medium angle */
            if(ball.pos_x < (player.pos_x - 1) && ball.pos_x > (player.pos_x - 6)){
                ball.angle = MID;
                return;
            }
            if(ball.pos_x > (player.pos_x + 17) && ball.pos_x < (player.pos_x + 22)){
                ball.angle = MID;
                return;
            }

            /* horizontal */
            if(ball.pos_x > (player.pos_x + 5) && ball.pos_x < (player.pos_x + 11)){
                ball.angle = HORZ;
                return;
            }

            /* maximum angle and direction inversion */
            if(ball.pos_x < (player.pos_x - 5)){
                ball.angle = MAX;
                if(ball.angle_dir == LEFT){
                    ball.angle_dir == RIGHT;
                }
                else{
                    ball.angle_dir == LEFT;
                }
                return;
            }
            if(ball.pos_x > (player.pos_x + 21)){
                ball.angle_dir = MAX;
                if(ball.angle_dir == LEFT){
                    ball.angle_dir == RIGHT;
                }
                else{
                    ball.angle_dir == LEFT;
                }
                return;
            }
        }
    }
}

/**
 * @brief This function checks, if the ball moved below the bottom of the playfield.
 * @return 1 if the ball left the playfield, 0 if not.
 */
unsigned char collisions_check_ball_bottom(void){
    if(ball.pos_y > playfield.edge_bottom){
        return 1;
    }
    else{
        return 0;
    }
}


/**
 * @brief This function orchestrates all update functions and encapsulates them to the main loop.
 */
void mainloop_update(void){

    if(!flag_pause) {
        /* calculation of ball movement and speed */
        ball_movement();

        collision_check_ball_playfield();
        collision_check_ball_player();

        flag_brickhit = collision_check_ball_brick();
        if (flag_brickhit) {
            calc_score_and_speed();
        }

        flag_miss = collisions_check_ball_bottom();
    }

}