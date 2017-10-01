/** @file       logic.c
 *  @brief      This program performs various logic operations.
 *  @details    It contains four logical gates, two sequential circuits, 
 *		and three combinational circuits. 
 *
 *  @author     Sadie Henry
 *  @date       10/5/14
 */

#include "logic.h"

/**
 * Simulates NOT logic gate.
 *
 *  @param A    input to the NOT gate
 *  @return     NOT gate output, returns NOT A
 *
 * Truth table
 <pre>
        A   return
        0   1
        1   0
 </pre>
 */
BIT not_gate(BIT A)
{
    if (A==FALSE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Simulates OR logic gate.
 *
 *  @param A    first input to the OR gate
 *  @param B    second input to the OR gate
 *  @return     returns A OR B
 *
 * Truth table
 <pre>
        A B  return
        0 0    0    
        0 1    1    
        1 0    1    
        1 1    1    
 </pre>
 */
BIT or_gate(BIT A, BIT B)
{
    if (A==FALSE && B==FALSE) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * Simulates RS latch circuit.
 * Asserts if S = R = 0.
 *
 *  @param S    set input line
 *  @param R    reset input line
 *  @return     returns RS latch output
 *
 * Truth table
 <pre>
        S R   return
        0 0   assert
        0 1   1 
        1 0   0 
        1 1   previous output 
 </pre>
 */
BIT rs_latch(BIT S, BIT R)
{
    static BIT saved_bit = FALSE;
    
    // if both bits are zero then behavior of R-S latch
    // is undefined so we will have the simulation assert
    assert(!(S==FALSE && R==FALSE));
    
    // implements the truth table
    if (S==FALSE) {
        saved_bit = TRUE; // Set
    } else if (R==FALSE) {
        saved_bit = FALSE; // Reset
    } else {
        saved_bit = saved_bit; // Unchanged
    }

    return saved_bit;
}

/**
 * Simulates AND logic gate.
 *
 *  @param A    first input to the AND gate
 *  @param B    second input to the AND gate
 *  @return     returns A AND B
 *
 * Truth table
 <pre>
        A B  return
        0 0    0    
        0 1    0    
        1 0    0    
        1 1    1    
 </pre>
 */
BIT and_gate(BIT A, BIT B)
{
    if(A == FALSE && B == FALSE){
	return FALSE;
    } else if(A == FALSE && B == TRUE){
	return FALSE;
    } else if(A == TRUE && B == FALSE){
	return FALSE;
    } else {
	return TRUE;
    }
}

/**
 * Simulates XOR logic gate.
 *
 *  @param A    first input to the XOR gate
 *  @param B    second input to the XOR gate
 *  @return     returns A XOR B
 *
 * Truth table
 <pre>
        A B  return
        0 0    0    
        0 1    1    
        1 0    1    
        1 1    0    
 </pre>
 */
BIT xor_gate(BIT A, BIT B)
{
    if(A == FALSE && B == FALSE){
	return FALSE;
    } else if(A == FALSE && B == TRUE){
	return TRUE;
    } else if(A == TRUE && B == FALSE){
	return TRUE;
    } else {
	return FALSE;
    }
}

/**
 * Simulates D latch circuit.
 *
 *  @param D    value of latch when WE is enabled
 *  @param WE   toggles write-enable functionality
 *  @return     d latch output
 *
 * Truth table
 <pre>
        D WE  return
        0 0   0
        0 1   0 
        1 0   previous output 
        1 1   1 
	0 0   previous output
	1 0   previous output
	1 1   1
	0 1   0
 </pre>
 */
BIT d_latch(BIT D, BIT WE)
{
    static BIT saved_bit = FALSE;

    if(D == TRUE && WE == TRUE){
	saved_bit = TRUE;
    } else if(D == FALSE && WE == TRUE){
	saved_bit = FALSE;
    } else{
	saved_bit = saved_bit;
    }

    return saved_bit;

}

/**
 * Simulates adder function. Adds two integers together and a carry in. 
 * 
 * @param O1		first int to be added
 * @param O2		second int to be added
 * @param carryIn	adds 1 to sum if true, 0 if false
 * @param *carryOut	returns true if sum is > 15, false if not
 */
int adder(int O1, int O2, BIT carryIn, BIT *carryOut)
{
    int sum = O1 + O2;
    if(carryIn == TRUE){
	sum += 1;
    }
    if(sum > 15){
	*carryOut = TRUE;
    }
    if(sum <= 15){
	*carryOut = FALSE;
    }
    return sum;
}

/**
 * Simulates Multiplexer.
 *
 *  @param A    first input to the multiplexer
 *  @param B    second input to the multiplexer
 *  @param C	third input to the multiplexer
 *  @param D	fourth input to the multiplexer
 *  @param S1	fifth input to the multiplexer
 *  @param S0	sixth input to the multiplexer
 *  @return     returns result of input to the multiplexer
 *
 * Truth table
 <pre>
        A B C D S1 S0  return
        0 0 0 0 0  0   0    
        1 1 1 1 1  1   1 
        1 0 1 0 1  0   1    
        0 1 0 1 0  1   1    
 </pre>
 */
BIT multiplexer(BIT A, BIT B, BIT C, BIT D, BIT S1, BIT S0)
{
    if(S1 == FALSE && S0 == FALSE && A == FALSE){
	return FALSE;
    }

    if(S1 == TRUE && S0 == TRUE && D == TRUE){
	return TRUE;
    }

    if(S1 == TRUE && S0 == FALSE && C == TRUE){
	return TRUE;
    }

    if(S1 == FALSE && S0 == TRUE && B == TRUE){
	return TRUE;
    }

    return FALSE;
}

/**
 * Simulates Decoder.
 *
 *  @param A    first input to the decoder
 *  @param B    second input to the decoder
 *  @param *O0	first output from the decoder
 *  @param *O1	second output from the decoder
 *  @param *O2	third output from the decoder
 *  @param *O3	fourth output from the decoder
 *  @return     returns result of input to the decoder
 *
 * Truth table
 <pre>
        A B O0 O1 O2 O3  
        0 0 1  0  0  0      
        0 1 0  1  0  0  
        1 0 0  0  1  0      
        1 1 0  0  0  1   
 </pre>
 */
void decoder(BIT A, BIT B, BIT *O0, BIT *O1, BIT *O2, BIT *O3)
{
    *O0 = FALSE;
    *O1 = FALSE;
    *O2 = FALSE;
    *O3 = FALSE;

    if(A == FALSE && B == FALSE){
	*O0 = TRUE;
    } else if(A == FALSE && B == TRUE){
	*O1 = TRUE;
    } else if(A == TRUE && B == FALSE){
	*O2 = TRUE;
    } else{
	*O3 = TRUE;
    }
}










