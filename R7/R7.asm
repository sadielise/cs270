; Recitation 7
; Author: <name>
; Date:   <date>
; Email:  <email>
; Class:  CS270
;
; Description: Implements integer (16-bit) addition and subtraction
;
;------------------------------------------------------------------------------
; Begin reserved section: do not change ANYTHING in reserved section!

                .ORIG x3000
                BR Main

; A jump table defined as an array of addresses
Functions       .FILL IntAdd         ; address of add routine      (option 0)
                .FILL IntSub         ; address of subtract routine (option 1)
                .FILL IntMul         ; address of multiply routine (option 2)

Main            LEA R0,Functions     ; get base of jump table
                LD  R1,Option        ; get option to use, no error checking
                ADD R0,R0,R1         ; add index of array
                LDR R0,R0,0          ; get address of function
                JSRR R0              ; call selected function
                HALT

; Parameters and return values for all functions
Option          .BLKW 1              ; which function to call
Param1          .BLKW 1              ; space to specify first parameter
Param2          .BLKW 1              ; space to specify second parameter
Result          .BLKW 1              ; space to store result

; End reserved section: do not change ANYTHING in reserved section!
;------------------------------------------------------------------------------
IntAdd          LD R0,Param1         ; load first parameter
                LD R1,Param2         ; load second parameter
		ADD R2,R0,R1         ; Add the two parameters
		ST R2,Result	     ; Store the result
                RET
;------------------------------------------------------------------------------
IntSub          LD R0,Param1         ; load first parameter
                LD R1,Param2         ; load second parameter
		NOT R1,R1 	     ; negate the second parameter
		ADD R1,R1,#1	     ; add 1 to the second parameter
		ADD R2,R0,R1	     ; add the two parameters
		ST R2,Result	     ; store the result
                RET

;------------------------------------------------------------------------------
IntMul          LD R0,Param1         ; load first parameter (counter)
                LD R1,Param2         ; load second parameter
		AND R2,R2,0
	LOOP		
		ADD R2,R2,R1
		ADD R0,R0,#-1
		BRp LOOP

		ST R2,Result
                RET

;------------------------------------------------------------------------------
               .END

