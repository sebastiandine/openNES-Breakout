/**
 * @file globals.h
 * @brief This file contains all game specific globals.
 * @author Sebastian Dine
 */

#pragma bss-name(push, "ZEROPAGE")
/**
 * @brief This flag which indicates, if the game is in an ingame loop.
 * 0 indicates, that the game is not in an ingame loop, 1 indicates,
 * that the game is in aan ingame loop.
 */
unsigned char flag_ingame;

/**
 * @brief This variable indicates whether the game is pause or not.
 * 0 indicates, that the games is not paused, 1, that the game is paused.
 */
unsigned char flag_pause;

/**
 * @brief This variable indicates whether the game is over or not.
 * 0 indicates, that the games is not over, 1, that the game is over.
 */
unsigned flag_gameover;

/**
 * @brief This variable stores the collision map tile, which the ball collided with during
 * the current frame.
 */
unsigned char collision_tile;

/**
 * @brief This variable stores the collision map tile address, resp. the bg tile address of the tile,
 * which the ball collided with during the last frame.
 */
unsigned char collision_addr;
/**
 * @brief  This variable indicates, if the player missed the ball and therefore
 * loses one live.
 */
unsigned char flag_miss;

/**
 * @brief This flag indicated, if the ball hit a brick during the current frame.
 */
unsigned char flag_brickhit;

/**
 * @brief This variable counts, how many bricks have been hit. It is used to
 * determine, when no brick is left and the bulk of bricks needs to be redrawn.
 */
unsigned char count_brickhit;

/**
 * @brief This variable controls, how long the credits screen will be displayed.
 */
unsigned char credits_loop;

/**
 * @brief This variable represents the games ball with its parameters.
 */
static struct ball_struct ball;

/**
 * @brief This variable represents the player.
 */
static struct player_struct player;

/**
 * @brief This variable stores data about the last brick, the player hit.
 * Data will be used to render a blank tile to the place of the brick, which got hit.
 */
static struct brickhit_struct brick_hit;

#pragma bss-name(pop)

/**
 * @brief Background palette
 */
const unsigned char bg_palette[16] =
		{ 	0x0f,0x00,0x10,0x30,
			0x0f,0x01,0x21,0x31,
			0x0f,0x06,0x16,0x26,
			0x0f,0x09,0x19,0x29 };


/**
 * @brief Sprite palette
 */
const unsigned char sprite_palette[16]  =
        {0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30 };