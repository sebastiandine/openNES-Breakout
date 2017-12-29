/**
 * @file render.c
 * @brief This file contains the rendering functionalities of the game.
 * @author Sebastian Dine
 */

#include "menu.h"
#include "ingame.h"
#include "game_over.h"
#include "credits.h"


/**
 * @brief This function renders the player's paddle to the screen.
 */
void render_paddle(void){

    oam_offset = ppu_load_sprite_to_oam(player.pos_x, PLAYFIELD_BOTTOM, SPR_PADDLE_EDGE, SPRITE_ATTR(0,0,0,0),
                                        oam_offset);
    oam_offset = ppu_load_sprite_to_oam(player.pos_x+8, PLAYFIELD_BOTTOM, SPR_PADDLE_MID, SPRITE_ATTR(0,0,0,0),
                                        oam_offset);
    oam_offset = ppu_load_sprite_to_oam(player.pos_x+16, PLAYFIELD_BOTTOM, SPR_PADDLE_EDGE, SPRITE_ATTR(0,1,0,0),
                                        oam_offset);
}

/**
 * @brief This function renders the ball.
 */
void render_ball(void){
    oam_offset = ppu_load_sprite_to_oam(ball.pos_x, ball.pos_y, SPR_BALL, SPRITE_ATTR(0,0,0,0), oam_offset);
}

/**
 * @brief This function renders the ingame background resp. playfield.
 */
void render_ingame(void){
    ppu_turn_all_off();
	set_bg_palette(bg_palette);
    set_sprite_palette(sprite_palette);
    ppu_load_bg_palette();
    ppu_load_sprite_palette();
    ppu_draw_background(ingame, 'a');
    wait_Vblank();
    ppu_turn_all_on();
    ppu_reset_scroll();
}

/**
 * @brief This function renders the menu background.
 */
void render_menu(void){
	ppu_turn_all_off();
    set_bg_palette(bg_palette);
	ppu_load_bg_palette();
    ppu_draw_background(menu, 'a');
    wait_Vblank();
    ppu_turn_all_on();
    ppu_reset_scroll();

}

/**
 * @brief This function renders the credit screen with a simple fade out effect.
 */
void render_credits(void){
    /* render screen */
    ppu_turn_all_off();
    set_bg_palette(bg_palette);
    ppu_load_bg_palette();
    ppu_draw_background(credits, 'a');
    wait_Vblank();
    ppu_turn_all_on();
    ppu_reset_scroll();

    /* show initial color for 40 frames */
    sleep_loop = 0;
    while(++sleep_loop < 40){
        wait_until_nmi();
    }

    /* fade out effect, darken color every 40 frames */
    while(ppu_bg_palette[3] != COLOR_BLACK){ 

        wait_until_nmi();
        if(ppu_bg_palette[3] == COLOR_DARK_GREY){
            ppu_bg_palette[3] = COLOR_BLACK; 
        }
        if(ppu_bg_palette[3] == COLOR_LIGHT_GREY){
            ppu_bg_palette[3] = COLOR_DARK_GREY; 
        }
        if(ppu_bg_palette[3] == COLOR_WHITE){ 
            ppu_bg_palette[3] = COLOR_LIGHT_GREY;
        }

        ppu_load_bg_palette();
        ppu_reset_scroll();

        sleep_loop = 0;
        while(++sleep_loop < 40) {
            wait_until_nmi();
        }
    }
}

/**
 * @brief This function renders the game over background.
 */
void render_gameover(void) {
    ppu_clear_oam();
    ppu_turn_all_off();
    ppu_draw_background(game_over, 'a');

    PPU_ADDRESS = MSB(0x2191);
    PPU_ADDRESS = LSB(0x2191);
    PPU_DATA = player.score_digit1;
    PPU_DATA = player.score_digit2;         /* since PPU_ADDRESS increases after each PPU_DATA write, we do not need
                                            * to set it again if we want to write to continous bg tiles */
    PPU_DATA = player.score_digit3;

    wait_Vblank();
    ppu_turn_all_on();
    ppu_reset_scroll();
}

/**
 * @brief This function renders the players lives by disabling
 * corresponding heart tiles.
 */
void render_lives(void){
    if(player.lives == 3){
        PPU_ADDRESS = MSB(0x205b);
        PPU_ADDRESS = LSB(0x205b);
        PPU_DATA = BG_VOID;
    }
    if(player.lives == 2){
        PPU_ADDRESS = MSB(0x205c);
        PPU_ADDRESS = LSB(0x205c);
        PPU_DATA = BG_VOID;
    }
    if(player.lives == 1){
        PPU_ADDRESS = MSB(0x205d);
        PPU_ADDRESS = LSB(0x205d);
        PPU_DATA = BG_VOID;
    }
    if(player.lives == 0){
        PPU_ADDRESS = MSB(0x205e);
        PPU_ADDRESS = LSB(0x205e);
        PPU_DATA = BG_VOID;
    }
    ppu_reset_scroll();
}

/**
 * @brief This function disables the brick, that has been git by the ball during the current frame.
 */
void render_brickhit(void){

    PPU_ADDRESS = MSB(0x2100 + brick_hit.tile_left);
    PPU_ADDRESS = LSB(0x2100 + brick_hit.tile_left);
    PPU_DATA = BG_VOID;

    PPU_ADDRESS = MSB(0x2100 + brick_hit.tile_right);
    PPU_ADDRESS = LSB(0x2100 + brick_hit.tile_right);
    PPU_DATA = BG_VOID;

    ppu_reset_scroll();
}

/**
 * @brief This function renders the score of the player.
 */
void render_score(void){

    PPU_ADDRESS = MSB(0x2049);
    PPU_ADDRESS = LSB(0x2049);

    PPU_DATA = player.score_digit1;
    PPU_DATA = player.score_digit2; /* since PPU_ADDRESS increases after each PPU_DATA write, we do not need
                                     * to set it again if we want to write to continous bg tiles */
    PPU_DATA = player.score_digit3;
    ppu_reset_scroll();
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

    if(flag_brickhit){
        render_brickhit();
        render_score();
        flag_brickhit = 0;
    }

    oam_offset = 0;   /* make sure to set oam_offset to 0 before you render sprites */
    render_paddle();
    render_ball();
}