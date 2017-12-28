/**
 * @file input.c
 * @brief This file contains the gamepad input handling of the game.
 * @author Sebastian Dine
 */

/**
 * @brief This function provides the input handling of gamepad 1.
 */
void gamepad1_input_handling(void) {

    get_controller_input();

    if (gamepad_1 & START) {
        if (!(gamepad_1_old & START)) {            /* make sure, that pause mode does not switch off an on again, when
                                                     player holds START longer than one frame */
            flag_pause = !flag_pause;
        }
    }

    if (!flag_pause) {                             /* make sure that position cannot be changed during pause */
        if (gamepad_1 & DIR_LEFT) {
            if ((player.pos_x - player.speed) > PLAYFIELD_LEFT) {  /* consider speed to avoid rendering the player into the
                                                                           wall in cases, where the distance between player and
                                                                           wall is smaller than the speed per frame */
                player.pos_x -= player.speed;
            }
        }
        if (gamepad_1 & DIR_RIGHT) {
            if ((player.pos_x + player.speed + 16) < PLAYFIELD_RIGHT) { /* add 16 because the right end of the paddle is x+16 */
                player.pos_x += 4;
            }
        }
    }
}
