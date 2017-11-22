/**
 * @file render.c
 * @brief This file contains the rendering functionalities of the game.
 * @author Sebastian Dine
 */

/**
* @brief Sprite constants
*/
#define SPR_PADDLE_MID  0x02
#define SPR_PADDLE_EDGE 0x03

/**
 * @brief This function renders the player's paddle to the screen.
 */
void render_paddle(void){

    oam_offset = ppu_load_sprite_to_oam(paddle_x, paddle_y, SPR_PADDLE_EDGE, SPRITE_ATTR(0,0,0,0), oam_offset);
    oam_offset = ppu_load_sprite_to_oam(paddle_x+8, paddle_y, SPR_PADDLE_MID, SPRITE_ATTR(0,0,0,0), oam_offset);
    oam_offset = ppu_load_sprite_to_oam(paddle_x+16, paddle_y, SPR_PADDLE_EDGE, SPRITE_ATTR(0,1,0,0), oam_offset);
}

/**
 * @brief This function orchestrates all rendering functions and encapsulates them to the main loop.
 */
void mainloop_render(void){

    /* clear sprites when game is paused */
    if(flag_pause){
        ppu_clear_oam();
        return;
    }

    oam_offset = 0;   /* make sure to set oam_offset to 0 before you render sprites */
    render_paddle();
}