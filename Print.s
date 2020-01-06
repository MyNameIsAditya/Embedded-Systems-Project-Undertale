; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix
		
THOUSANDS	EQU		-3
HUNDREDS	EQU		-2
TENS		EQU		-1
ONES		EQU		0
DECIMAL		EQU		0x2E
LARGEST		EQU		0x0270F

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH {R4-R11}				;Save Registers
	MOV R11, SP					;Frame Pointer
	SUB R11, #1					;Allocation
	MOV R1, R0
	MOV R4, #10					;R4 = Divisor
	MOV R10, #0					;Length of Value In R0
LOOP	
	UDIV R1, R4					;Checks Length of Value In R0
	CMP R1, #0
	BEQ FINISH
	ADD R10, R10, #1			;Counter For Length In R10
	B LOOP
FINISH
	SUB R11, R10				;Allocate Memory
	MOV R9, #0
STORE_STACK
	UDIV R5, R0, R4				;R5 = Quotient
	MLS R6, R5, R4, R0			;MODULUS ---> R6 = R0 - (R4 * R5) ---> INPUT = (QUOTIENT * DIVISOR)
	UDIV R0, R0, R4
	STRB R6, [R11, R9]			;Stores Value Of Specific Place In Memory In Sequential Oreder
	CMP R9, R10
	BEQ COMPARE
	ADD R9, R9, #1
	B STORE_STACK
COMPARE
	MOV R9, R10
PRINT
	LDRB R0, [R11, R9]			
	ADD R0, R0, #0x30			;Hexadecimal To ASCII Conversion
	SUB SP, #64
	PUSH {LR, R11}
	BL ST7735_OutChar			;Prints Values In ASCII From Memory In Order
	POP {LR, R11}
	ADD SP, #64
	CMP R9, #0
	BEQ DONE
	SUB R9, #1	
	B PRINT
DONE
	POP {R4-R11}				;Restore Registers
    BX  LR						;Return
	
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH {R4-R11}				;Save Registers
	MOV R11, SP					;Frame Pointer
	SUB R11, #1					;Allocation
;------------------------------------CHECK IF TOO LARGE - ASTERISKS------------------------------
	LDR R4, =LARGEST			;Checks With 0x0270F
	CMP R0, R4
	BHI ASTERISK
;------------------------------------STORES VALUES-----------------------------------------------
	MOV R4, #10					;R4 = 10 = DIVISOR
	UDIV R5, R0, R4				;R5 = QUOTIENT
	MLS R6, R5, R4, R0			;MODULUS ---> R6 = R0 - (R4 * R5) ---> INPUT = (QUOTIENT * DIVISOR)
	STRB R6, [R11, #ONES]
	UDIV R0, R4					;DIVIDE BY 10
	UDIV R5, R0, R4				;R5 = QUOTIENT
	MLS R6, R5, R4, R0			;MODULUS ---> R6 = R0 - (R4 * R5) ---> INPUT = (QUOTIENT * DIVISOR)
	STRB R6, [R11, #TENS]
	UDIV R0, R4					;DIVIDE BY 10
	UDIV R5, R0, R4				;R5 = QUOTIENT
	MLS R6, R5, R4, R0			;MODULUS ---> R6 = R0 - (R4 * R5) ---> INPUT = (QUOTIENT * DIVISOR)
	STRB R6, [R11, #HUNDREDS]
	UDIV R0, R4					;DIVIDE BY 10
	UDIV R5, R0, R4				;R5 = QUOTIENT
	MLS R6, R5, R4, R0			;MODULUS ---> R6 = R0 - (R4 * R5) ---> INPUT = (QUOTIENT * DIVISOR)
	STRB R6, [R11, #THOUSANDS]
	B PRINT_DECIMAL
ASTERISK
	MOV R0, #0x2A				;R0 = ASTERISK HEX VALUE
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {LR, R11}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	MOV R0, #DECIMAL			;R0 = DECIMAL HEX VALUE
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {LR, R11}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	MOV R0, #0x2A				;R0 = ASTERISK HEX VALUE
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {LR, R11}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	MOV R0, #0x2A				;R0 = ASTERISK HEX VALUE
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {LR, R11}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	MOV R0, #0x2A				;R0 = ASTERISK HEX VALUE
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {LR, R11}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	B RETURN
PRINT_DECIMAL
	LDRB R0, [R11, #THOUSANDS]	;R0 = THOUSANDS VALUE
	ADD R0, #0x30
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {R11, LR}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	MOV R0, #DECIMAL			;R0 = DECIMAL HEX VALUE
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {R11, LR}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	LDRB R0, [R11, #HUNDREDS]	;R0 = HUNDREDS VALUE
	ADD R0, #0x30
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {R11, LR}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	LDRB R0, [R11, #TENS]		;R0 = TENS VALUE
	ADD R0, #0x30
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {R11, LR}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
	LDRB R0, [R11, #ONES]		;R0 = ONES VALUE
	ADD R0, #0x30
	SUB SP, #8					;ALLOCATE MEMORY FOR FUNCTION CALL
	PUSH {R11, LR}
	BL ST7735_OutChar
	POP {LR, R11}
	ADD SP, #8					;DEALLOCATE MEMORY FOR FUNCTION CALL
RETURN
	POP {R4-R11}				;Restore Registers
    BX  LR	
 
    ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
