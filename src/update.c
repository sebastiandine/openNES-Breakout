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

    if(ball.pos_y < 111){ //ball is one bg line before first line of bricks

        /* get tile in collision map */
        collision_addr = MAPARRAY_ADR(ball.pos_x, (ball.pos_y - ball.speed)) - 0xC0; /* convert bg tile address to collision map tile address.
                                                                                      * - 0xC0 since the collision map starts
                                                                                       at bg tile 0xE0 */
        collision_tile = ingame_collisionmap[collision_addr];

        if(collision_tile == 0x61){
            ball.dir = DOWN;

            /* update collision map */
            ingame_collisionmap[collision_addr] = 0x00;
            ingame_collisionmap[collision_addr +1] = 0x00;

            /* calculate bg tiles, which need to be rendered with blank tiles */
            brick_hit.tile_left = collision_addr - 0x20; //convert collision map tile address to bg-tile address
            brick_hit.tile_right = brick_hit.tile_left + 1;

            return 1;
        }
        if(collision_tile == 0x62){
            ball.dir = DOWN;

            /* update collision map */
            ingame_collisionmap[collision_addr] = 0x00;
            ingame_collisionmap[collision_addr -1] = 0x00;

            /* calculate bg tiles, which need to be rendered with blank tiles */
            brick_hit.tile_right = collision_addr - 0x20; //convert collision map tile address to bg-tile address
            brick_hit.tile_left = brick_hit.tile_right - 1;

            return 1;
        }

    }
    return 0;

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

    /* calculation of ball movement and speed */
    ball_movement();

    collision_check_ball_playfield();
    collision_check_ball_player();

    flag_brickhit = collision_check_ball_brick();
    flag_miss = collisions_check_ball_bottom();

}