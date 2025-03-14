MMC3_BankSelect		  = $8000
MMC3_BankData		  = $8001
MMC3_Mirroring		  = $a000
MMC3_PRGRAMProtect    = $a001

MMC3_IRQLatch		  = $c000
MMC3_IRQReload		  = $c001
MMC3_IRQDisable		  = $e000
MMC3_IRQEnable		  = $e001

.macro lsr_by n
	.repeat n, i
		lsr
	.endrep
.endmacro

.macro DoUpdateSockHash
		.local @dont_render
		lda WRAM_PracticeFlags
		and #PF_SockMode
	.ifndef ORG
		beq @dont_render	;enable RULE as default on SMB2J and Nippon
	.else
		bne @dont_render	;enable SOCK as default on SMB1
	.endif
		lda IntervalTimerControl
		and #3
		cmp #2
		bne @dont_render
		jsr Enter_ForceUpdateSockHash
	@dont_render:
.endmacro

	.macro practice_callgate
		.res $FE00 - *, $EA

		Enter_UpdateGameTimer:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp UpdateGameTimer

		Enter_InitializeWRAM:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp InitializeWRAM

		Enter_SetDefaultWRAM:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp SetDefaultWRAM

		Enter_FactoryResetWRAM:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp FactoryResetWRAM

		Enter_RedrawSockTimer:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp RedrawSockTimer

		Enter_PracticeInit:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp PracticeInit

		Enter_ForceUpdateSockHash:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp ForceUpdateSockHash

		Enter_PracticeOnFrame:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp PracticeOnFrame

		Enter_PracticeTitleMenu:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp PracticeTitleMenu

		Enter_UpdateFrameRule:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp UpdateFrameRule

		Enter_WritePracticeTop:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp WritePracticeTop

		Enter_RedrawUserVars:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp RedrawUserVars

		Enter_RedrawAll:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp RedrawAll

		Enter_HideRemainingFrames:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp HideRemainingFrames
			
		Enter_RedrawFrameNumbers:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp RedrawFrameNumbers

		Enter_ProcessLevelLoad:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp ProcessLevelLoad
			
	.endmacro


	.macro scenarios_callgate
		.res $FE00 - *, $EA
		Enter_ScenLoadArea:
			lda #BANK_SCEN_DATA
			jsr SetBankFromA
			jmp ScenLoadArea

		Enter_ScenReset:
			lda #BANK_SCEN_DATA
			jsr SetBankFromA
			jmp ScenReset

		Enter_ScenUpdateSelected:
			lda #BANK_SCEN_DATA
			jsr SetBankFromA
			jmp ScenUpdateSelected

		Enter_ScenValidate:
			lda #BANK_SCEN_DATA
			jsr SetBankFromA
			jmp ScenValidate

	.endmacro

	;
	; Lower banks
	; 
	.macro control_bank
		.res $FF00 - *, $EA

		Enter_SoundEngine:
			lda #BANK_COMMON
			jsr SetBankFromA
			jmp SoundEngineExternal

		ReturnBank:
			lda BANK_SELECTED
			jmp SetBankFromA

		SetChrBank0FromA:
			sta $A000
			lsr
			sta $A000
			lsr
			sta $A000
			lsr
			sta $A000
			lsr
			sta $A000
			rts

		SetChrBank1FromA:
			sta $C000
			lsr
			sta $C000
			lsr
			sta $C000
			lsr
			sta $C000
			lsr
			sta $C000
			rts

		SetChrBanksFromAX:
			jsr SetChrBank0FromA
			txa
			jmp SetChrBank1FromA
			
		SetBankFromA:
			sta $E000
			lsr
			sta $E000
			lsr
			sta $E000
			lsr
			sta $E000
			lsr
			sta $E000
			rts

		MapperReset:
			;
			; Clear MMC1 state
			;
			sei
			ldx #$FF
			txs
			stx $8000
			;
			; Set 32kb prg bank switch, 4KB chr ram switch, vertical mirroring 
			;
			lda #$12
			sta $8000
			lsr
			sta $8000
			lsr
			sta $8000
			lsr
			sta $8000
			lsr
			sta $8000
			lsr
		StartBank:
			;
			; 0 Bank selected
			;
			sta BANK_SELECTED
			ldx #$00
			stx PPU_CTRL_REG1
			stx PPU_CTRL_REG2
			jsr SetBankFromA
			jmp Start

			.res $FFFA - *, $ea
			;
			; Interrupt table
			;
			.word NonMaskableInterrupt
			.word MapperReset
			.word MapperReset
	.endmacro