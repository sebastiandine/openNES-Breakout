/**
 * @file update.c
 * @brief This file contains the actual game logic and calculations which will be executed during the update phase of
 * a frame.
 * @author Sebastian Dine
 */

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
 * @brief This function checks the collision of the ball with the player and calculates the
 * resulting movement of the ball in case of a collision.
 */
void collision_check_ball_player(void){
    if((ball.pos_y + ball.speed) > (playfield.edge_bottom - 8)) {   /* consider speed to avoid rendering the ball into the player */
                                                                    /* since the paddle is 8 pixels high, the check needs to be done
                                                                       with paddle_y - 8*/
        if (ball.pos_x > (player.pos_x + 8) && ball.pos_x < (player.pos_x + 24)) { /* range of paddle is [paddle_x-8; paddle_x + 24] */
            ball.dir = UP;

            /* implement different angles here */
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

    flag_miss = collisions_check_ball_bottom();

}