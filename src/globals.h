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
 * @brief tbd
 */
unsigned flag_gameover;

/**
 * @brief  This variable indicates, if the player missed the ball and therefore
 * loses one live.
 */
unsigned char flag_miss;

/**
 * @brief This variable represents the games ball with its parameters.
 */
static struct ball_struct ball;

/**
 * @brief This variable represents the volume of the playfield.
 */
static struct playfield_struct playfield;

/**
 * @brief This variable represents the player.
 */
static struct player_struct player;

#pragma bss-name(pop)

/**
 * @brief Background palette
 */
const unsigned char bg_palette[16]  =
        {0x0f, 0x00, 0x10, 0x30,    //black, grey, light grey, white
         0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30 };

/**
 * @brief Sprite palette
 */
const unsigned char sprite_palette[16]  =
        {0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30,
         0x0f, 0x00, 0x10, 0x30 };