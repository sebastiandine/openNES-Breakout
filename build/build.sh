#Individual-Project-Settings
MAIN_FILE_NAME=main			#name of main-'.c'-file in $SOURCE_DIR-directory, WITHOUT! extension
NES_ROM_MAP_NAME=nrom_128_horz.cfg	#name of rom-mapping-file in $CONFIG_DIR-directory
ASM_RESET_FILE=crt0			#name of 6502-assembler file in $SOURCE_DIR-directory, which contains
					        #startup-memory-organization, WITHOUT! extension

rm -rf temp
mkdir temp
cp ../src/* ../src/asm/* ../gfx/* ../sfx/* ../config/* ./temp

cd ./temp 
echo "Start compiling and linking"
echo "Compile ASM helper functions"
ca65 reset.s 
ca65 lib_get_input.s 
ca65 lib_unpack_RLE.s 
ca65 lib_wait_Vblank.s
echo "Compile ${MAIN_FILE_NAME}.c and referenced files into 6502-Assembler file ${MAIN_FILE_NAME}.s"
cc65 -Oi ${MAIN_FILE_NAME}.c --add-source
echo "Compile ${MAIN_FILE_NAME}.s game 6502-Assembler file ${MAIN_FILE_NAME}.s into NES-bytecode"
ca65 ${MAIN_FILE_NAME}.s
echo "Link everything to NES-Game-File"
ld65 -C nes.cfg -o ../${MAIN_FILE_NAME}.nes reset.o ${MAIN_FILE_NAME}.o lib_get_input.o lib_unpack_RLE.o lib_wait_Vblank.o nes.lib

cd .. && rm -rf temp
echo "Compiling project has been done. The final .nes-gamefile can be found at ${MAIN_FILE_NAME}.nes"