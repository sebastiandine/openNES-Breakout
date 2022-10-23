@echo off

REM ################################################################
REM # Variables (might differ from project to project)             #
REM ################################################################
SET WORKSPACE=%CD%
SET WORKSPACE=%WORKSPACE:~,-5%
SET SOURCE_DIR=%WORKSPACE%src
SET BUILD_DIR=%WORKSPACE%build
SET GRAPHICS_DIR=%WORKSPACE%gfx
SET SOUND_DIR=%WORKSPACE%sfx
SET CONFIG_DIR=%WORKSPACE%config
SET ASM_DIR=%WORKSPACE%src\asm
SET TEMP_DIR=%BUILD_DIR%\temp

SET MAIN_FILE_NAME=main

REM ################################################################
REM # Setting up temporary build environment                       #
REM ################################################################
CD %BUILD_DIR%
MKDIR %TEMP_DIR%
COPY %CONFIG_DIR%\* %TEMP_DIR%
COPY %GRAPHICS_DIR%\* %TEMP_DIR%
COPY %SOUND_DIR%\* %TEMP_DIR%
COPY %ASM_DIR%\* %TEMP_DIR%
XCOPY /s %SOURCE_DIR%\* %TEMP_DIR%

REM ###############################################################
REM # Compiling and Linking                                       #
REM ###############################################################
CD %TEMP_DIR%

REM compile ASM functions first
ca65 reset.s
ca65 lib_get_input.s
ca65 lib_unpack_RLE.s
ca65 lib_wait_Vblank.s

REM compile actual project files
cc65 -Oi %MAIN_FILE_NAME%.c --add-source
ca65 %MAIN_FILE_NAME%.s

REM link everything to a .nes file
ld65 -C nes.cfg -o %BUILD_DIR%\%MAIN_FILE_NAME%.nes reset.o %MAIN_FILE_NAME%.o lib_get_input.o lib_unpack_RLE.o lib_wait_Vblank.o nes.lib



ECHO Delete temporary build-directory
CD %BUILD_DIR%
RMDIR /q/s %TEMP_DIR% 

REM RENAME MAIN.NES to openNES-Pong.nes
RENAME main.nes breakout.nes

PAUSE

REM run .nes file
%MAIN_FILE_NAME%.nes
