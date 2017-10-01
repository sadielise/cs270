; Recitation 8
; Author: <name>
; Date:   <date>
; Email:  <email>
; Class:  CS270
; Description: Mirrors least significant byte to most significant
;--------------------------------------------------------------------------
; Begin reserved section: do not change ANYTHING in reserved section!

                .ORIG x3000

                JSR mirror           ; call function
                HALT

; Parameter and return value
Param           .BLKW 1              ; space to specify parameter
Result          .BLKW 1              ; space to store result

; Constants
One             .FILL #1             ; the number 1       
Eight           .FILL #8             ; the number 8
Mask            .FILL x00ff          ; mask top bits

; End reserved section: do not change ANYTHING in reserved section!
;--------------------------------------------------------------------------
mirror                               ; Mirrors bits 7:0 to 15:8
                                     ; ~20 lines of assembly code
 
                LD R0,Param          ; load parameter into R0
		LD R1,Param	     ; load parameter into R1
                LD R2,Mask           ; load mask into register 2
		AND R1,R1,R2	     ; use mask to clear bits 15:8 in R1
		LD R2,One	     ; initialize register 2 to One (source mask)
		LD R3,One	     ; initialize register 3 to One (destination mask)
		LD R4,Eight	     ; initialize register 4 to Eight (counter)

Loop1		ADD R3,R3,R3	     ; create destination mask
		ADD R4,R4,#-1	     
		BRp Loop1	     ; completed creating destination mask

		LD R4,Eight	     ; initialize register 4 to Eight, again

Loop2		AND R5,R2,R0	     ; AND the source mask with the parameter
		BRz Loop3	     ; if the result is zero, jump to Loop3
		ADD R1,R3,R1	     ; add destination mask to R1
Loop3		ADD R2,R2,R2	     ; shift source mask to the left
		ADD R3,R3,R3	     ; shift destination mask left
		ADD R4,R4,#-1	     ; decrement counter
		BRp Loop2

                ST R1,Result         ; store result
                RET
;--------------------------------------------------------------------------
               .END

