;asm funtions that can be called by the c code. Written by Doug Fraker 2015.

.importzp _gamepad_1, _gamepad_1_old, _gamepad_1_test, _gamepad_2, _gamepad_2_old, _gamepad_2_test
.export _get_controller_input

.segment "CODE"

;due to dmc audio playback occasionally messing with input, it is best to read twice and compare them
_get_controller_input:
	lda _gamepad_1
	sta _gamepad_1_old
	lda _gamepad_2
	sta _gamepad_2_old
	
	ldx #$01	;strobe controller 1
	stx $4016
	dex
	stx $4016
	
	ldy #$08
Get_Input2:		;get first read, store them as a test
	lda $4016
	and #$03
	cmp #$01
	rol _gamepad_1_test
	lda $4017
	and #$03
	cmp #$01
	rol _gamepad_2_test
	dey
	bne Get_Input2
	
GetInputFixBug:
	ldx #$01	;restrobe strobe controller 1
	stx $4016
	dex
	stx $4016
	
	ldy #$08
Get_Input3:		;read again, store them as joypads
	lda $4016
	and #$03
	cmp #$01
	rol _gamepad_1
	lda $4017
	and #$03
	cmp #$01
	rol _gamepad_2
	dey
	bne Get_Input3
	
CompareInput:
	lda _gamepad_1
	cmp _gamepad_1_test
	bne :+
	lda _gamepad_2
	cmp _gamepad_2_test
	bne :+
	rts				;if same, done
	
:	lda _gamepad_1
	sta _gamepad_1_test
	lda _gamepad_2
	sta _gamepad_2_test

	jmp GetInputFixBug ;if different, reread
	