;asm funtions that can be called by the c code. Written by Doug Fraker 2015.

.export _wait_Vblank

.segment "ZEROPAGE"

.segment "CODE"

_wait_Vblank:
	lda $2002
	bpl _wait_Vblank
	rts

