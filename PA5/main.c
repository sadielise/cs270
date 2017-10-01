/** @mainpage CS270 Fall 2014
 *  \htmlinclude "PA5.html"
 */

/** @file: main.c
 *
 *  @brief Driver to test functions of logic.c (do not modify)
 *  @details This is a driver program that tests the functions of 
 *  logic.c, including gates, sequential circuits, and combinational circuits.
 *
 *  @author Sadie Henry
 *  @date   10/5/2014
 */

// Include files
#include "logic.h"

/** Entry point of the program
 * @param   argc count of arguments, will always be at least 1
 * @param   argv array of parameters to program argv[0] is the name of
 *          the program, so additional parameters will begin at index 1
 * @details If one of the test cases fail then the program will assert.
 * @return  0 the Linux convention for success.
 */
int main(int argc, char *argv[])
{
    // check NOT gate truth table
    // A     NOT(A)
    // 0     1
    // 1     0
    printf("===== Testing not_gate =====\n");
    assert(not_gate(FALSE) == TRUE); 
    assert(not_gate(TRUE) == FALSE); 


    // check OR gate truth table
    // A B  (A OR B)
    // 0 0   0
    // 0 1   1
    // 1 0   1
    // 1 1   1
    printf("===== Testing or_gate =====\n");
    assert(or_gate(FALSE,FALSE) == FALSE); 
    assert(or_gate(FALSE,TRUE) == TRUE); 
    assert(or_gate(TRUE,FALSE) == TRUE); 
    assert(or_gate(TRUE,TRUE) == TRUE); 
    
    // check RS latch truth table
    // the order matters so that the saved_bit will be true when expected
    //  S	R	saved_bit   rs_latch(S,R)
    //  FALSE	FALSE	FALSE	    will assert in rs_latch
    //  FALSE	FALSE	TRUE	    will assert in rs_latch
    //  TRUE	FALSE	FALSE	    FALSE
    //	TRUE	TRUE	FALSE	    FALSE
    //  FALSE	TRUE	FALSE	    TRUE
    //  FALSE	TRUE	TRUE	    TRUE
    //	TRUE	TRUE	TRUE	    TRUE
    //  TRUE	FALSE	TRUE	    FALSE
    printf("===== Testing rs_latch =====\n");
    assert( rs_latch( TRUE, FALSE )==FALSE );
    assert( rs_latch( TRUE, TRUE )==FALSE );
    assert( rs_latch( FALSE, TRUE )==TRUE );
    assert( rs_latch( FALSE, TRUE )==TRUE );
    assert( rs_latch( TRUE, TRUE )==TRUE );
    assert( rs_latch( TRUE, FALSE )==FALSE );

    // check AND gate truth table
    // A	B	and_gate
    // FALSE	FALSE	FALSE
    // FALSE	TRUE	FALSE
    // TRUE	FALSE	FALSE
    // TRUE	TRUE	TRUE
    printf("===== Testing and_gate =====\n");
    assert( and_gate( FALSE, FALSE )==FALSE );
    assert( and_gate( FALSE, TRUE )==FALSE );
    assert( and_gate( TRUE, FALSE )==FALSE );
    assert( and_gate( TRUE, TRUE )==TRUE );

    // check XOR gate truth table
    // A	B	xor_gate
    // FALSE	FALSE	FALSE
    // FALSE	TRUE	TRUE
    // TRUE	FALSE 	TRUE
    // TRUE	TRUE	FALSE
    printf("===== Testing xor_gate =====\n");
    assert( xor_gate( FALSE, FALSE )==FALSE );
    assert( xor_gate( FALSE, TRUE )==TRUE );
    assert( xor_gate( TRUE, FALSE )==TRUE );
    assert( xor_gate( TRUE, TRUE )==FALSE );
    
    //  check D latch truth table
    //  D	WE	saved_bit	d_latch
    //  FALSE	FALSE   FALSE		FALSE
    //  FALSE	TRUE    FALSE		FALSE
    //  TRUE	FALSE   FALSE		FALSE
    //	TRUE	TRUE    TRUE		TRUE
    //  FALSE	FALSE   TRUE		TRUE
    //  TRUE	FALSE   TRUE		TRUE
    //	TRUE	TRUE    TRUE		TRUE
    //  FALSE	TRUE   	FALSE		FALSE
    printf("===== Testing d_latch =====\n");
    d_latch( FALSE, FALSE );
    assert( d_latch( FALSE, FALSE )==FALSE );
    assert( d_latch( FALSE, TRUE )==FALSE );
    assert( d_latch( TRUE, FALSE )== FALSE );
    assert( d_latch( TRUE, TRUE )==TRUE );    
    assert( d_latch( FALSE, FALSE )==TRUE );
    assert( d_latch( TRUE, FALSE )==TRUE );
    assert( d_latch( TRUE, TRUE )==TRUE );
    assert( d_latch( FALSE, TRUE )== FALSE );

    // check Adder function
    // O1	O2	carryIn	*carryOut
    // 0101	0101	FALSE	FALSE
    // 0101	0101	TRUE	FALSE
    // 0001	0111	TRUE	FALSE
    // 0111	0111	FALSE	FALSE
    // 1101	1101	TRUE	TRUE
    printf("===== Testing adder =====\n");
    BIT carryOut;
    int a = adder( 0b0101, 0b0101, FALSE, &carryOut );
    assert( a == 10 && carryOut == 0 );
    int b = adder( 0b0101, 0b0101, TRUE, &carryOut );
    assert( b == 11 && carryOut == 0 );
    int c = adder( 0b0001, 0b0111, TRUE, &carryOut );
    assert( c == 9 && carryOut == 0 );
    int d = adder( 0b0111, 0b0111, FALSE, &carryOut );
    assert( d == 14 && carryOut == 0 );
    int e = adder( 0b1101, 0b1101, TRUE, &carryOut );
    assert( e == 27 && carryOut == 1 );


    // check Multiplexer function
    // A	B	C	D	S1	S0	Output
    // FALSE	FALSE	FALSE	FALSE	FALSE	FALSE	FALSE
    // TRUE	TRUE	TRUE	TRUE	TRUE	TRUE	TRUE
    // TRUE	FALSE	TRUE	FALSE	TRUE	FALSE	TRUE
    // FALSE	TRUE	FALSE	TRUE	FALSE	TRUE	TRUE
    printf("===== Testing multiplexer =====\n");
    assert( multiplexer( FALSE, FALSE, FALSE, FALSE, FALSE, FALSE )==FALSE );
    assert( multiplexer( TRUE, TRUE, TRUE, TRUE, TRUE, TRUE )==TRUE );
    assert( multiplexer( TRUE, FALSE, TRUE, FALSE, TRUE, FALSE )==TRUE );
    assert( multiplexer( FALSE, TRUE, FALSE, TRUE, FALSE, TRUE )==TRUE );

    // check Decoder function
    // A	B	*O0	*O1	*O2	*O3
    // 0	0	1	0	0	0	
    // 0	1	0	1	0	0
    // 1	0	0	0	1	0
    // 1	1	0	0	0	1
    printf("===== Testing decoder =====\n");
    BIT O0, O1, O2, O3;
    decoder(0, 0, &O0, &O1, &O2, &O3);
    assert( O0 == 1 && O1 == 0 && O2 == 0 && O3 == 0 );
    decoder(0, 1, &O0, &O1, &O2, &O3);
    assert( O0 == 0 && O1 == 1 && O2 == 0 && O3 == 0 );
    decoder(1, 0, &O0, &O1, &O2, &O3);
    assert( O0 == 0 && O1 == 0 && O2 == 1 && O3 == 0 );
    decoder(1, 1, &O0, &O1, &O2, &O3);
    assert( O0 == 0 && O1 == 0 && O2 == 0 && O3 == 1 );

    return 0;

}
