/**
 * @file nes_hw_functionalities.c
 * @brief Implementation of C functions, declared within file @link nes_hw_functionalites.h @endlink.
 * @author Sebastian Dine
 */

#include "nes_hw_functionalities.h"

void wait_until_nmi(void){
	while(nmi_flag == 0);
	nmi_flag = 0;
}

void ppu_turn_all_off(void){
	PPU_CTRL = 0;	/* binary 00000000 - all disabled */
	PPU_MASK = 0; 	/* binary 00000000 - all disabled */
}

void ppu_turn_all_on(void){
	PPU_CTRL = 0x90; /* binary 10010000 - 	bit 7: NMI turned on, 
											bit 4: turn on screen */
											
	PPU_MASK = 0x1e; /* binary 00011110 - 	bit 4: show sprites
											bit 3: show background
											bit 2: show sprites in leftmost 8 pixels on screen
											bit 1: show background in leftmost 8 pixels on screen */
}

void ppu_reset_scroll(void){
	PPU_ADDRESS = 0;	/* set high byte to 0 */
	PPU_ADDRESS = 0;	/* set low byte to 0 */
	PPU_SCROLL = 0;		/* set scroll x_position to 0 */
	PPU_SCROLL = 0;		/* set scroll y_position to 0 */
}

void ppu_load_bg_palette(void) {
    PPU_ADDRESS = MSB(0x3f00);
    PPU_ADDRESS = LSB(0x3f00);

    /* write array bg_pallete to PPU RAM. PPU_ADDRESS will be
       incremented automatically during the write process */
    for (index = 0; index < sizeof(ppu_bg_palette); ++index) {
        PPU_DATA = ppu_bg_palette[index];
    }
}

void ppu_load_sprite_palette(void){

    /* write array bg_pallete to PPU RAM. PPU_ADDRESS will be
       incremented automatically during the write process.
       In order to avoid overwriting the background's universal
       bg color, mirroring addresses 0x3f10, 0x3f14, 0x3f18
       and 0x3f1c are skipped. First color of each sprite palette
       will therefore not be loaded, since it is assumed to be
       transperent.*/
    for(index = 0; index < sizeof(ppu_sprite_palette); ++index){
        if(index == 0){
            PPU_ADDRESS = MSB(0x3f11);
            PPU_ADDRESS = LSB(0x3f11);
            continue;
        }
        if(index == 4){
            PPU_ADDRESS = MSB(0x3f15);
            PPU_ADDRESS = LSB(0x3f15);
            continue;
        }
        if(index == 8){
            PPU_ADDRESS = MSB(0x3f19);
            PPU_ADDRESS = LSB(0x3f19);
            continue;
        }
        if(index == 12){
            PPU_ADDRESS = MSB(0x3f1d);
            PPU_ADDRESS = LSB(0x3f1d);
            continue;
        }

        PPU_DATA = ppu_sprite_palette[index];
    }
}

unsigned char ppu_load_sprite_to_oam(unsigned char x, unsigned char y, unsigned char tile, unsigned char attribute, unsigned char offset){
    ppu_oam[offset] = y;
    ppu_oam[++offset] = tile;
    ppu_oam[++offset] = attribute;
    ppu_oam[++offset] = x;
    return ++offset;
}

void ppu_clear_oam(void){
	for(index = 0; index < oam_offset; ++index){
		ppu_oam[index] = 0x00;
	}
}

void set_bg_palette(const unsigned char *new_bg_palette){
	for(index = 0; index < sizeof(ppu_bg_palette); ++index){
		ppu_bg_palette[index] = new_bg_palette[index];
	}
}

void set_sprite_palette(const unsigned char *new_sprite_palette){
	for(index = 0; index < sizeof(ppu_sprite_palette); ++index){
		ppu_sprite_palette[index] = new_sprite_palette[index];
	}
}

void ppu_draw_background(const unsigned char *bg, unsigned char nametable){
    switch(nametable){
        case 'a' : PPU_ADDRESS = MSB(NAMETABLE_A) ; PPU_ADDRESS = LSB(NAMETABLE_A); break;
        case 'b' : PPU_ADDRESS = MSB(NAMETABLE_B) ; PPU_ADDRESS = LSB(NAMETABLE_B); break;
        case 'c' : PPU_ADDRESS = MSB(NAMETABLE_C) ; PPU_ADDRESS = LSB(NAMETABLE_C); break;
        case 'd' : PPU_ADDRESS = MSB(NAMETABLE_D) ; PPU_ADDRESS = LSB(NAMETABLE_D); break;
        default : PPU_ADDRESS = MSB(NAMETABLE_A) ; PPU_ADDRESS = LSB(NAMETABLE_A);
    }
    unpack_RLE(bg);
}

void load_background_collision_map(unsigned char *col_map){
	memcpy (bg_collision_map, col_map, 256);
}