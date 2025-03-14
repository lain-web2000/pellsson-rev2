BANK_SELECTED = $07ff

SaveFramesLeft = $013a
UserFramesLeft = $013b

.define PF_SockMode $01 ; Todo move elsewhere...
.define PF_SaveState $02
.define PF_LoadState $04
.define PF_RestartLevel $08
.define PF_LevelEntrySaved $10
.define PF_DisablePracticeInfo $20
.define PF_EnableInputDisplay $40

.macro PF_SetToLevelEnd_A
	lda WRAM_PracticeFlags
	and #(PF_LevelEntrySaved^$FF)
	sta WRAM_PracticeFlags
.endmacro

.macro MACRO_ThrowFrameImpl
ThrowFrame:
		lda Mirror_PPU_CTRL_REG2
		sta PPU_CTRL_REG2
		lda Mirror_PPU_CTRL_REG1
		and #$FC
		sta Mirror_PPU_CTRL_REG1
		sta PPU_CTRL_REG1 ; Force NT-0 (in case no button was pressed)

		;
		; Wait for sprite 0
		;
		lda Sprite0HitDetectFlag  ;check for flag here
		beq @SkipSprite0
		ldx #0
		stx PPU_SPR_ADDR
		lda Sprite0Data
		sta PPU_SPR_DATA
		lda Sprite0Data+1
		sta PPU_SPR_DATA
		lda Sprite0Data+2
		sta PPU_SPR_DATA
		lda Sprite0Data+3
		sta PPU_SPR_DATA
		lda PPU_STATUS
		stx PPU_SCROLL_REG
		stx PPU_SCROLL_REG
@Sprite0Clr:
		lda PPU_STATUS
		and #$40
		bne @Sprite0Clr
@Sprite0Hit:
		lda PPU_STATUS
		and #$40
		beq @Sprite0Hit
		ldy #$14
@HBlankDelay:
		dey
		bne @HBlankDelay
@SkipSprite0:
		lda PPU_STATUS
		lda HorizontalScroll
		sta PPU_SCROLL_REG
		lda VerticalScroll
		sta PPU_SCROLL_REG

		lda ScreenLeft_PageLoc
		lsr Mirror_PPU_CTRL_REG1
		and #$01
		ror
		rol Mirror_PPU_CTRL_REG1
		lda Mirror_PPU_CTRL_REG1
		sta PPU_CTRL_REG1

		lda WRAM_SlowMotion
		cmp #4
		bne @done
		;
		; Latch controllers.
		;
		ldx #1
		stx $4016
		dex
		stx $4016
		;
		; Check A
		;
		lda $4017
		lsr
		bcs @done ; A pressed. Advance next frame
		lda $4017 ; B
		lda $4017 ; Select
		lda $4017 ; Start
		lsr
		bcc @VBlank0
		;
		; Start pressed - Exit frame advance mode.
		;
		stx WRAM_SlowMotion
		jmp @done ; Exit out of this.
@VBlank0:
		lda PPU_STATUS ; Wait for VBlank
		bpl @VBlank0
		jmp ThrowFrame
@done:
		lda Mirror_PPU_CTRL_REG1
		ora #$80 ; NMI
		sta PPU_CTRL_REG1
		jmp Enter_SoundEngine
.endmacro

.macro MACRO_RunSlowMo gm
		dec WRAM_SlowMotionLeft
		bmi @allow_frame
		lda DisableScreenFlag
		bne @allow_frame
		lda GamePauseStatus
		and #3
		bne @allow_frame
		ldx OperMode
		dex
		bne @allow_frame
		lda OperMode_Task
		cmp #gm
		bne @allow_frame
		jsr ThrowFrame
		rti
@allow_frame:
		lda WRAM_SlowMotion
		sta WRAM_SlowMotionLeft
.endmacro

TITLE_INIT		= $01

	BANK_LOADER		= 0
.ifdef ORG
	BANK_CHR		= 2
	BANK_ORG		= 4
	BANK_COMMON		= 6
	BANK_ANN		= 12
.elseif .defined(LOST)
	BANK_COMMON		= 4
	BANK_LLDATA		= 8
	BANK_SMBLL		= 12
.else
	BANK_COMMON		= 4
	BANK_ANNDATA	= 8
	BANK_ANN		= 12
.endif

.ifdef ORG
	CHR_ORG_SPR = 0
	CHR_ORG_BG = 1
	CHR_ORG_BG_ALTFONT = 2
	CHR_LOST_SPR = 3
	CHR_LOST_BG = 4
	CHR_PEACH_SPR = 5
	CHR_INTRO_BG = 6
	CHR_INTRO_SPR0 = 7
.elseif .defined(LOST)
	CHR_LOST_SPR = 0
	CHR_LOST_BG = 4
	CHR_LOST_VICTORY = 8
	CHR_LOST_BG_ALTFONT = 10
.else
	CHR_NIPPON_SPR = 0
	CHR_NIPPON_BG = 4
	CHR_NIPPON_VICTORY = 8
	BANK_NIPPON_CHAR_L = 10
	BANK_NIPPON_CHAR_R = 11
.endif

.ifndef ORG
	CHR_INTRO_BG = 24
	CHR_INTRO_SPR0 = 28
.endif

SavedRule = $07d9
RuleIndex = $0717 ; Previously DemoAction (can be changed to temp?)
PowerUps = $07e3 ; Previously unused
CategorySelect = $07e4
CurrentRule = $7df

.ifndef RESTART_LEVEL_BUTTONS
	.define RESTART_LEVEL_BUTTONS Select_Button|Up_Dir
.endif

.ifndef RESTART_GAME_BUTTONS
	.define RESTART_GAME_BUTTONS Select_Button|Down_Dir
.endif

.ifndef SAVE_STATE_BUTTONS
	.define SAVE_STATE_BUTTONS Select_Button|Right_Dir
.endif

.ifndef LOAD_STATE_BUTTONS
	.define LOAD_STATE_BUTTONS Select_Button|Left_Dir
.endif

