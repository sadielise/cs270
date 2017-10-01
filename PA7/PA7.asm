; PA7 Assignment
; Author: Fritz Sieker and Chris Wilcox
; Date:   3/10/2014
; Email:  fsieker,wilcox@cs.colostate.edu
; Class:  CS270
;
; Description: Implements the manipulation of half precision (16-bit) floating point numbers

;------------------------------------------------------------------------------
; Begin reserved section: do not change ANYTHING in reserved section!

                .ORIG x3000
                BR MAIN

; A jump table defined as an array of addresses
Functions       .FILL flt16_add      ; (option 0)
                .FILL flt16_sub      ; (option 1)
                .FILL flt16_get_sign ; (option 2)
                .FILL flt16_get_exp  ; (option 3)
                .FILL flt16_get_val  ; (option 4)
                .FILL flt16_abs      ; (option 5)
                .FILL flt16_neg      ; (option 6)
                .FILL left_most1     ; (option 7)
                .FILL left_shift     ; (option 8)
                .FILL right_shift    ; (option 9)
                .FILL binary_or      ; (option 10)

Main            LEA R0,Functions     ; get base of jump table
                LD  R1,Option        ; get option to use, no error checking
                ADD R0,R0,R1         ; add index of array
                LDR R0,R0,#0         ; get address of function
                JSRR R0              ; call selected function
                HALT

; Here are some useful constants:

SIGN_MASK       .FILL x8000          ; 1 in left most bit 
EXP_MASK        .FILL x001F          ; exactly 5 bits     
VAL_MASK        .FILL x03FF          ; exactly 10 bits    
IMPL1_MASK      .FILL x0400          ; 1 in the 11 bit
IMPL1_MASK2	.FILL xFBFF	     ; 0 in the 11 bit    
ONE             .FILL #1             ; the number 1       
TEN             .FILL #10            ; the number 10 
NEGTEN		.FILL #-10	     ; the number -10     
SIXTEEN         .FILL #16            ; the value 16
ABS_MASK	.FILL x7FFF	     ; mask for absolute value
EXP_MASK2	.FILL x7C00	     ; mask for exponent
NEG_MASK1	.FILL x8000	     ; mask for negation
NEG_MASK2	.FILL x7FFF	     ; mask for negation
FIFTEEN		.FILL x000F	     ; the number 15
ALL_ONES	.FILL xFFFF	     ; all ones

;------------------------------------------------------------------------------

; Local variables, this is how you will be tested for PA6
; Do not change the names!
X               .BLKW 1              ; X parameter
Y               .BLKW 1              ; Y parameter
signX           .BLKW 1              ; sign of X
expX            .BLKW 1              ; exponent of X
valX            .BLKW 1              ; mantissa of X
signY           .BLKW 1              ; sign of Y
expY            .BLKW 1              ; exponent of Y
valY            .BLKW 1              ; mantissa of Y
signSum         .BLKW 1              ; sign of sum
expSum          .BLKW 1              ; exponent of sum
valSum          .BLKW 1              ; mantissa of sum
left1           .BLKW 1              ; location of left most 1
rShift_RA	.BLKW 1		     ; return address storage for right_shift
exp_RA		.BLKW 1		     ; return address storage for get_exp
exp_val		.BLKW 1		     ; address storage for get_exp value
lm1_val		.BLKW 1		     ; address storage for lm1 value
lm1_RA		.BLKW 1		     ; return address storage for lm1
lm1_count	.BLKW 1	  	     ; address storage for lm1 counter 
;-------------------------------------------------------------------------------

flt16_add_ra    .BLKW 1              ; return address

flt16_add       ST R7,flt16_add_ra   ; save return address
                LD R0,Param1         ; read X parameter
                ST R0,X              ; store X parameter
                LD R0,Param2         ; read Y parameter
                ST R0,Y              ; store Y parameter

get_signs	JSR flt16_get_sign   ; STEP ONE) Extract fields from operands
		LD R1,Result	     ; load result
		ST R1,signX	     ; store result
		LD R1,Y		     ; load Y
		ST R1,Param1	     ; store Y to Param1
		JSR flt16_get_sign   ; jump to get exponent
		LD R1,Result	     ; load result
		ST R1,signY	     ; store result

get_exponents	LD R0,X		     ; load X
		ST R0,Param1	     ; store X to Param1
		JSR flt16_get_exp    ; jump to get exponent
		LD R1,Result	     ; load result
		ST R1,expX	     ; store result
		LD R0,Y		     ; load Y
		ST R0,Param1	     ; store Y to Param1
		JSR flt16_get_exp    ; jump to get exponent
		LD R1,Result	     ; load result
		ST R1,expY	     ; store result

get_mantissas	LD R0,X		     ; load X
		ST R0,Param1	     ; store X to Param1
		JSR flt16_get_val    ; jump to get value
		LD R1,Result	     ; load result
		ST R1,valX	     ; store result
		LD R0,Y		     ; load Y
		ST R0,Param1	     ; store Y to Param1
		JSR flt16_get_val    ; jump to get value
		LD R1,Result	     ; load result
		ST R1,valY	     ; store result
		
equalize_exp	LD R0,expX	     ; STEP TWO) Equalize operand exponents
		NOT R0,R0	     ; flip all bits
		ADD R0,R0,#1	     ; add 1
		LD R1,expY	     ; load expY into R1
		ADD R0,R0,R1	     ; add negative expX and positive expY
		BRz store_exp	     ; branch if equal
xIsBigger	LD R0,valX	     ; load mantissa of X
		LD R1,expX	     ; load exponent of X
xExpShift	ADD R0,R0,R0	     ; left shift mantissa of X
		ADD R1,R1,#-1	     ; decrement exponent
		ADD R3,R3,#-1	     ; decrement counter
		BRp xExpShift	     ; loop
		ST R0,valX	     ; store adjusted mantissa			
store_exp	ST R1,expSum	     ; store final exponent
			
convert_val     LD R0,signX          ; STEP THREE) Convert operands to 2's complement
		BRz checkY	     ; branch if value is positive
		LD R0,valX	     ; load mantissa X into R0
		NOT R0,R0	     ; flip all the bits
		ADD R0,R0,#1	     ; add 1
		ST R0,valX	     ; store converted value for X
checkY		LD R0,signY	     ; load signY into R0
		BRz add_mantissa     ; branch if value is positive
		LD R0,valY	     ; load mantissa Y into R0
		NOT R0,R0	     ; flip all bits
		ADD R0,R0,#1	     ; add 1
		ST R0,valY	     ; store converted value for Y

add_mantissa    LD R0,valX           ; STEP FOUR) Add mantissas
		LD R1,valY	     ; load mantissa Y into R1
		ADD R0,R0,R1	     ; add mantissas
		ST R0,valSum	     ; store result

convert_sum 	LD R0,valSum         ; STEP FIVE) Convert sum from 2's complement
		BRzp normalize_sum   ; branch if positive or 0; nothing to do
		NOT R0,R0	     ; flip all bits
		ADD R0,R0,#1	     ; add 1
		ST R0,valSum	     ; store converted result

normalize_sum	ST R0,Param1         ; STEP SIX) Normalize sum
		JSR left_most1	     ; jump to left most 1 function
		LD R0,Result	     ; load result into R0
		LD R1,NEGTEN	     ; load -10 into R1 
		ADD R3,R0,R1	     ; add result with -10
		BRz compose_sum      ; branch if in the correct place
		LD R2,expSum	     ; load exponent of sum into R2
		ADD R4,R0,R1	     ; add result with -10
		BRp shiftRight	     ; branch if position > 10
		LD R1,valSum	     ; load mantissa into R1
shiftLeft	ADD R1,R1,R1	     ; shift mantissa left
		ADD R2,R2,#-1	     ; decrement exponent
		ADD R4,R4,#1	     ; increment counter 
		BRn shiftLeft	     ; loop
		ST R1,valSum	     ; store shifted mantissa
		ST R2,expSum	     ; store decremented exponent
		BR compose_sum       ; branch to finish
shiftRight	LD R5,expSum	     ; load exponent of sum into R5
		ADD R5,R4,R5	     ; add position difference to exponent
		ST R5,expSum	     ; store incremented exponent
		LD R1,valSum	     ; load mantissa into R1
		ST R1,Param1	     ; store mantissa into Param1
		ST R4,Param2	     ; store counter into Param2
		JSR right_shift      ; jump to right shift
		LD R0,Result	     ; load result into R0
		ST R0,valSum	     ; store shifted mantissa

compose_sum	LD R0,X	             ; STEP SEVEN) Compose sum from fields
		LD R1,Y		     ; load Y into R1
		AND R2,R2,#0	     ; clear out R2
		ADD R1,R1,R0	     ; add X and Y
		BRp removeImp1	     ; branch if positive
		LD R2,NEG_MASK1	     ; load 1 into R3
removeImp1	LD R0,IMPL1_MASK2    ; load impl1 mask into R0
		LD R1,valSum	     ; load mantissa into R1
		AND R3,R0,R1	     ; & mantissa w/ mask
		LD R4,expSum	     ; load R4 with the exponent
		LD R5,TEN	     ; load counter into R5
shift_exp	ADD R4,R4,R4	     ; shift exponent left
		ADD R5,R5,#-1	     ; decrement counter
		BRp shift_exp	     ; loop
		NOT R3,R3	     ; ~ mantissa
		NOT R4,R4	     ; ~ exponent
		AND R3,R3,R4	     ; & exponent and mantissa
		LD R1,signSum	     ; load sign into R1
		NOT R1,R1	     ; ~ sign
		AND R3,R3,R1	     ; & sign and R3
		NOT R3,R3	     ; ~ result 
		ST R3,Result	     ; store R3 into Result
                LD R7,flt16_add_ra   ; restore return address
                RET
;------------------------------------------------------------------------------

; Parameters and return values for all functions
Option          .BLKW 1              ; which function to call
Param1          .BLKW 1              ; space to specify first parameter
Param2          .BLKW 1              ; space to specify second parameter
Result          .BLKW 1              ; space to store result
;------------------------------------------------------------------------------
flt16_sub_ra    .BLKW 1              ; return address
 
flt16_sub                            ; Result is Param1 minus Param2

                ST R7,flt16_sub_ra   ; save return address
                LD R0,Param1	     ; load Param1 into R0
		ST R0,X		     ; store R0 into X
		LD R1,Param2	     ; load Param2 into R1
		ST R1,Param1	     ; store R1 into Param1
		JSR flt16_neg	     ; jump to negate
		LD R0,Result	     ; load result into R0
		LD R1,X		     ; load X into R1	
		ST R0,Param2	     ; store R0 into Param2
		ST R1,Param1	     ; store R1 into Param1
		JSR flt16_add	     ; jump to add
                LD R7,flt16_sub_ra   ; restore return address
                RET
;------------------------------------------------------------------------------
flt16_get_sign                       ; Result is 0 if Param1 is positive, 1 otherwise

                AND R0,R0,#0         ; initialize result
                LD R1,Param1         ; load parameter
                LD R2,SIGN_MASK      ; load sign mask   
                AND R3,R2,R1         ; sign bit set?
                BRz return_sign      ; not set, return 0
                ADD R0,R0,#1         ; set, return 1
return_sign     ST R0,Result         ; save result
                RET
;------------------------------------------------------------------------------
flt16_exp_ra    .BLKW 1              ; return address

flt16_get_exp                        ; Result is biased exponent from Param1

                ST R7,flt16_exp_ra   ; save return address
		LD R0,Param1	     ; load Param1 into R0 (value)
		LD R1,EXP_MASK2	     ; load exponent mask 
		AND R1,R0,R1	     ; & R0 and R1
		ST R1,Param1	     ; store R1 into Param1
		LD R2,TEN	     ; load shift counter into R2
		ST R2,Param2	     ; store shift counter into Param2
		JSR right_shift	     ; jump to right shift
		LD R3,Result	     ; load result
		LD R7,flt16_exp_ra   ; reload return address
		ST R3,Result

                RET
;------------------------------------------------------------------------------
flt16_get_val                        ; Result is mantissa from Param1 plus implicit 1

		LD R0,Param1	     ; load R0 Param1 into
		BRnp continue1	     ; if not zero, continue
		BR finish	     ; if zero, finish
continue1	LD R1,VAL_MASK	     ; load value mask into R1
		LD R2,IMPL1_MASK     ; load implicit 1 mask into R2
		AND R1,R1,R0	     ; & R0 and R1
		ADD R2,R2,R1	     ; & R1 and R2
finish		ST R2,Result	     ; store result

                RET
;------------------------------------------------------------------------------
flt16_abs                            ; Result is absolute value of Param1

		LD R0,Param1	     ; load Param1 into R0 (value)
		LD R1,ABS_MASK	     ; load abs mask into R1
		AND R0,R0,R1	     ; & the value with the abs mask
                ST R0,Result         ; store result

                RET
;------------------------------------------------------------------------------
flt16_neg                            ; Result is negative value of Param1

		LD R0,Param1	     ; load Param1 into R0
		BRp positive	     ; branch if positive
		LD R1,NEG_MASK2	     ; load mask into R1
		AND R0,R0,R1	     ; & the parameter and the 2nd mask
		BR finish1	     ; branch to finish
positive	LD R1,NEG_MASK1	     ; positive condition
		ADD R0,R0,R1	     ; add parameter to the 1st mask
finish1         ST R0,Result         ; store result

                RET
;------------------------------------------------------------------------------
left_most1                           ; Result is bit position of leftmost 1 in Param1
		ST R7,lm1_RA	     ; store return address
		LD R0,Param1	     ; load Param1 into R0
		BRz isZero	     ; branch if value is zeros
		ST R0,lm1_val	     ; store Param1 into lm1_val
		LD R1,SIGN_MASK	     ; load sign mask into R1
		LD R2,FIFTEEN	     ; load counter into R2 

compare		ST R2,lm1_count	     ; store counter into lm1_count
		LD R0,lm1_val	     ; load lm1_val into R0
		AND R3,R0,R1	     ; & R1 and R0
		BRnp foundIt	     ; branch if found
		LD R4,ONE	     ; load counter for shift
		ST R4,Param2	     ; store counter for shift
		ST R1,Param1	     ; store mask to be shifted
		JSR right_shift	     ; jump to right shift
		LD R1,Result	     ; load shifted mask into R1
		LD R2,lm1_count	     ; reload the counter
		ADD R2,R2,#-1	     ; decrement counter	
		BRp compare	
		BR foundIt		

isZero		LD R2,ALL_ONES	     ; set result to xFFFF

foundIt		ST R2,Result
		LD R7,lm1_RA	     ; reload the return address

                RET
;------------------------------------------------------------------------------
left_shift                           ; Result is Param1 shifted left by Param2 bits

		LD R1,Param1	     ; load Param1 into R1 (value)
		LD R2,Param2	     ; load Param2 into R2 (counter)
lShift		ADD R1,R1,R1	     ; shift value to the left
		ADD R2,R2,#-1	     ; decrement counter
		BRp lShift                                   
finish2		ST R1,Result	     ; store the result
                RET
;------------------------------------------------------------------------------
right_shift                          ; Param1 shifted right by Param2 bits
		LD R0,Param1	     ; load Param1 into R0 (value)
		LD R2,Param1	     ; load Param1 into R2
		LD R1,Param2	     ; load Param2 into R1 (counter)
		BRnz return_rs	     ; count must be positive
		AND R2,R2,#0	     ; clear result
		LD R3,ONE	     ; source mask = 1
		LD R4,ONE	     ; destination mask = 1
rshift_loop1	ADD R3,R3,R3	     ; left shift source mask
		ADD R1,R1,#-1	     ; decrement count
		BRp rshift_loop1     ; continue looping
rshift_loop2	AND R5,R0,R3	     ; source bit set?
		BRz rshift_next	     ; not set, do nothing
		ADD R2,R2,R4	     ; set, update result
rshift_next	ADD R4,R4,R4	     ; shift destination mask
		ADD R3,R3,R3	     ; shift source mask
		BRnp rshift_loop2    ; continue looping
return_rs	ST R2,Result	     ; store result
                RET
;------------------------------------------------------------------------------
binary_or       LD R1,Param1         ; bitwise OR of Param1 and Param2
		LD R2,Param2	     ; load Param2 into R2
		NOT R1,R1	     ; negate R1
		NOT R2,R2	     ; negate R2
		AND R3,R1,R2	     ; and R1 and R2
		NOT R3,R3	     ; negate R3
		ST R3,Result	     ; store result
                RET
;------------------------------------------------------------------------------
                .END


