/**
 * @file nes_hw_functionalities.h
 * @brief This file contains hardware related macros, globals and function declarations.
 * @author Sebastian Dine
 */


/** @name PPU Register addresses.
 *  See http://wiki.nesdev.com/w/index.php/PPU_registers for detail.
 * @{
 */
#define PPU_CTRL		*((unsigned char*)0x2000)
#define PPU_MASK		*((unsigned char*)0x2001)
#define PPU_STATUS		*((unsigned char*)0x2002)
#define OAM_ADDRESS		*((unsigned char*)0x2003)
#define PPU_SCROLL		*((unsigned char*)0x2005)
#define PPU_ADDRESS		*((unsigned char*)0x2006)
#define PPU_DATA		*((unsigned char*)0x2007)
#define OAM_DMA			*((unsigned char*)0x4014)
/**
 * @}
 */

/**
 * @name Controller button masks
 * Use global variable @link gamepad_1 @endlink or @link gamepad_2 @endlink to get controller
 * input information. Example: @code if(gamepad_1&DIR_UP)@endcode .
 * @{
 */
#define DIR_RIGHT	0x01
#define DIR_LEFT	0x02
#define DIR_DOWN	0x04
#define DIR_UP		0x08
#define START		0x10
#define SELECT		0x20
#define B_BUTTON	0x40
#define A_BUTTON	0x80
/**
 * @}
 */

/**
 * @name Nametable starting addresses
 * @{
 */
#define NAMETABLE_A 0x2000
#define NAMETABLE_B 0x2400
#define NAMETABLE_C 0x2800
#define NAMETABLE_D 0x2c00
/**
 * @}
 */


/** @brief Macro for calculating in which tile of the 32*30 tiles (256x240px) the given position is placed.
   Optimized with bitshifting, arithmetic pendant is (((y/8)*32)+(x/8)). x and y are assumed to be pixel based coordinates.
*/
#define MAPARRAY_ADR(x,y)	((y<<2)|(x>>3))

/**
 * @brief Macro for calculating the most significant bit of a given byte.
 */
#define MSB(x)			(((x)>>8))
/**
 * @brief Macro for calculating the least significant bit of a given byte.
 */
#define LSB(x)			(((x)&0xff))

/**
 * @brief Macro for calculating the attribute byte of a sprite.
 * See http://wiki.nesdev.com/w/index.php/PPU_OAM for detail.
 */
 /*
 * The bits of this byte store data as followed (taken from NerdyNights tutorial):
 * 76543210
 * |||   ||
 * |||   ++- Color Palette of sprite.  Choose which set of 4 from the 16 colors to use
 * |||
 * ||+------ Priority (0: in front of background; 1: behind background)
 * |+------- Flip sprite horizontally
 * +-------- Flip sprite vertically
 */
#define SPRITE_ATTR(flip_horz,flip_vert,prio,pal) ((flip_horz<<7) + (flip_vert<<6) + (prio<<5) + pal)


/* Globals */
#pragma bss-name(push, "ZEROPAGE")
/**
 * @brief Indicator, if VBlank is reached resp. NMI interrupt is triggered.
 */
unsigned char nmi_flag;
/**
 * @brief Frame counter, this will be increased once per frame.
 */
unsigned char frm_count;
/**
 * @brief General variable for any loop purpose.
 */
unsigned char index;

/**
 * @brief OAM offset for next sprite to render. See @link ppu_load_sprite_to_oam @endlink for instructions
 * on how to use this global.
 */
unsigned char oam_offset;

/**
 * @name Gamepad 1 variables.
 * In the games C code, in most cases only @link gamepad_1 @endlink is required.
 * All other variables are mostly used by the assembly code of function
 * @link get_controller_input(void) @endlink.
 * @{
 */
unsigned char gamepad_1;
unsigned char gamepad_1_old;
unsigned char gamepad_1_test;
/**
 * @}
 */
/**
 * @name Gamepad 2 variables.
 * In the games C code, in most cases only @link gamepad_2 @endlink is required.
 * All other variables are mostly used by the assembly code of function
 * @link get_controller_input(void) @endlink.
 * @{
 */
unsigned char gamepad_2;
unsigned char gamepad_2_old;
unsigned char gamepad_2_test;
/**
 * @}
 */
#pragma bss-name(push, "OAM")
/**
 * @brief PPU OAM storage for sprites. See file 'config/nes.cfg' for definition of 'OAM' space.
 */
unsigned char ppu_oam[256];				/* OAM equals ram addresses 200-2ff */

#pragma bss-name(push, "MAP")
/**
 * @brief Collision map for collision dectection with background. See See file 'config/nes.cfg' for definition of
 * 'MAP' space.
 */
unsigned char bg_collision_map[256];	/* Collision map, ram adresses 300-3ff */

#pragma bss-name(push, "BSS")
/**
 * @brief Background color palette. Each four entries represent one palette.
 */
unsigned char ppu_bg_palette[16];		/* BG color palette at $3f00-3f0f */
/**
 * @brief Sprite color palette. Each four entries represent one palette.
 */
unsigned char ppu_sprite_palette[16]; 	/* Sprite color palette at 3f10-3f1f */
#pragma bss-name (pop)





/* C-implemented functions */
/**
 * @brief This function waits until NMI interrupt is triggered during VBlank (see aöso .NMI section in reset.s).
 * The difference to function @link wait_Vblank() @endlink is, that this function
 * checks, if especially NMI is reached, not VBlank.
 * Therefore this function should be used to frequently update graphics when the
 * screen is on.
 *
 * Use @link wait_Vblank() @endlink when you return from an disabled screen using
 * @link ppu_turn_all_on() @endlink .
 *
 */
void wait_until_nmi(void);

/**
 * @brief This function disables background and sprite rendering.
 */
void ppu_turn_all_off(void);

/**
 * @brief This function enables background and sprite rendering.
 */
void ppu_turn_all_on(void);

/**
 * @brief This functions resets scrolling. This needs to be done after every render routine.
 */
void ppu_reset_scroll(void);

/**
 * @brief this function loads the background color palette at @link ppu_bg_palette @endlink into
 * PPU RAM $3f00-§f0f. Use function @link set_bg_palette @endlink first in order to write to
 * @link ppu_bg_palette @endlink.
 */
void ppu_load_bg_palette(void);

/**
 * @brief this function loads the sprite color palette at @link ppu_sprite_palette @endlink into
 * PPU RAM $3f10-§f1f. Use function @link set_sprite_palette @endlink first in order to write to
 * @link ppu_sprite_palette @endlink.
 */
void ppu_load_sprite_palette(void);

/**
 * @brief This function loads sprite information to the OAM section of PPU RAM.
 * Internally this function loads sprite data into global array @link ppu_oam @endlink
 * which will be transferred to OAM once per frame.
 *
 * @param x X coordination (pixel based)
 * @param y Y coordination (pixel based)
 * @param tile Graphical tile, which should be send to OAM
 * @param attribute Attribute set for the sprite. Use macro @link SPRITE_ATTR @endlink to calculate attribute bytes for sprites
 * @param offset Offset from OAM address $200 to which the 4 byte per sprite should be stored
 *
 * @return Next free offset from OAM address $200
 */
unsigned char ppu_load_sprite_to_oam(unsigned char x, unsigned char y, unsigned char tile, unsigned char attribute,
									 unsigned char offset);
									
/**
 * @brief This function clear the whole OAM section of the PPU RAM by writing 0x00 to all spaces.
 */
void ppu_clear_oam(void);

/**
 * @brief This function stores the given background color palette into global array @link ppu_bg_palette @endlink in the BSS
 * section of the PPU RAM.
 * Use function @link ppu_load_bg_palette @endlink in order to load the new palette to the PPU afterwards.
 *
 * @param bg_palette Background color palette which should be stored into the BSS section. Make sure it is
 * 'const unsigned char [16]'.
 */
void set_bg_palette(const unsigned char *bg_palette);

/**
 * @brief This function stores the given sprite color palette into global array @link ppu_sprite_palette @endlink in the BSS
 * section of the PPU RAM.
 * Use function @link ppu_load_sprite_palette @endlink in order to load the new palette to the PPU afterwards.
 *
 * @param sprite_palette Sprite color palette which should be stored into the BSS section. Make sure it is
 * 'const unsigned char [16]'.
 */
void set_sprite_palette(const unsigned char *sprite_palette);

/**
 * @brief This functions stores the griven background array to the given nametable in order to draw it onto the sreen.
 * Remember to call function @link wait_Vblank() @endlink before you turn on the screen again, using function
 * @link ppu_turn_all_on() @endlink.
 *
 * @param bg RLE-compressed background as array of 'const unsigned char' .
 * @param nametable Nametable to which the background should be drawn ('a', 'b', 'c', 'd').
 */
void ppu_draw_background(const unsigned char *bg, unsigned char nametable);
 																 	  
/**
 * @brief This function loads the given collision map into global array @link bg_collision_map @endlink in section 'MAP',
 * which will be used for collision calculation.
 * Remember to call function @link wait_Vblank() @endlink before you turn on the screen again, using function
 * @link ppu_turn_all_on() @endlink.
 *
 * @param col_map Collision map. Make sure this is 'unsigned char [256]'.
 */
void load_background_collision_map(unsigned char *col_map);
														   


/**
 * @brief This function reads controller ports and stores the result into globals @link gamepad_1 @endlink
 * and @link gamepad_2 @endlink .
 * Use button mask macros to determine, whether a certain button has been pressed. Example:
 * @code if(gamepad_1&DIR_UP)@endcode .
 */
void __fastcall__ get_controller_input(void);

/**
 * @brief This function unpacks an RLE-compressed nametable and stores it to the address,
 * register @link PPU_ADDRESS @endlink is set to.
 *
 * @param data RLE-compressed background as array of 'const unsigned char' .
 */
void __fastcall__ unpack_RLE(const unsigned char *data);

/**
 * This function waits until the next VBlank period is reached. Use this function every time, you turn on the screen
 * again, using function @link ppu_turn_all_on() @endlink .
 * The difference to function  @link wait_until_nmi() @endlink is, that this function only checks, if VBlank is reached,
 * not if the NMI interrupt is triggered. Therefore this function should be used to identify VBlank, when the screen is
 * turned of.
 */
void __fastcall__ wait_Vblank(void);

/**
 * @brief This function copies data bulkwise . This is much faster than using a loop for
 * copying data.
 *
 * @param dest Destination of copy routine.
 * @param src Source of copy routine.
 * @param count Bytewise size of data bulk.
 */
void __fastcall__ memcpy (void* dest, const void* src, int count);

/**
 * @brief This function resets the APU and initializes the FamiTone music engine.
 *
 * Everytime you would like to play a new song, first you have the reset the engine, using function
 * @link reset_music() @endlink, then you have to select the music you would like to play, using
 * function @link play_music() @endlink. Afterwards you have to update the music processing every
 * frame, using function @link update_music() @endlink .
 */
void __fastcall__ reset_music(void);

/**
 * @brief This function starts playing the song with the given number in the famitone music file, specified
 * in file 'config/reset.s' with label 'music_data'.
 *
 * Everytime you would like to play a new song, first you have the reset the engine, using function
 * @link reset_music() @endlink, then you have to select the music you would like to play, using
 * function @link play_music() @endlink. Afterwards you have to update the music processing every
 * frame, using function @link update_music() @endlink .
 */
void __fastcall__ play_music(unsigned char song);

/**
 * This function updates music processing. Call this function once every frame.
 *
 * Everytime you would like to play a new song, first you have the reset the engine, using function
 * @link reset_music() @endlink, then you have to select the music you would like to play, using
 * function @link play_music() @endlink. Afterwards you have to update the music processing every
 * frame, using function @link update_music() @endlink .
 *
 */
void __fastcall__ update_music(void);

/**
 * @brief This function plays the sound effect with the given number in the famitone sfx file, specified
 * in file 'config/reset.s' with label 'sounds_data'.
 */
void __fastcall__ play_soundeffect(unsigned char effect);