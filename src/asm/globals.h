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