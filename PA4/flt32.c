#include "field.h"
#include "flt32.h"

/** @file flt32.c
 *  @brief You will modify this file and implement nine functions
 *  @details Your implementation of the functions defined in flt32.h.
 *  You may add other function if you find it helpful. Added function
 *  should be declared <b>static</b> to indicate they are only used
 *  within this file.
 *  <p>
 *  @author <b>Sadie Henry</b> goes here
 */

// Extract the sign of the argument
int flt32_get_sign (flt32 x) {
    return getBit(x, 31);
}

// Extract the exponent of the argument
int flt32_get_exp (flt32 x) {
    return (getField(x, 30, 23, 0)); 
}

// Extract the digits of the argument
int flt32_get_val (flt32 x) {
    
    int field = getField(x, 0, 22, 0);
    return setBit(field, 23);
}

// Get the sign, exponent, and value in a single call
void flt32_get_all(flt32 x, int* sign, int* exp, int* val) {

    *sign = flt32_get_sign(x);
    *exp = flt32_get_exp(x);
    *val = flt32_get_val(x);
}

// Extract the position of the left most 1 in the argument's bits
int flt32_left_most_1 (int value) {
 
    //return -1 if value is 0
    if(value==0){
	return -1;
    }

    //use a while to find the left most 1
    else{
    	int counter = 31;
    	bool found = false;
    	while(found == false){
	    if(getBit(value, counter)==1){
	    	found = true;
	    }
	    else{
	    	counter--;
	    }
	}

    	return counter;   
    }
}

// Absolute value of the argument
flt32 flt32_abs (flt32 x) {
    return clearBit(x, 31);
}

// Negate the argument
flt32 flt32_negate (flt32 x) {
    //case for when x is negative
    if(x < 0){
	return flt32_abs(x);
    }

    //case for when x is 0
    else if(x == 0){
	return 0;
    }

    //case for when x is positive
    else{
	return setBit(x, 31);
    }
}

// Add 2 floating point values
flt32 flt32_add (flt32 x, flt32 y) {

  //CHECK IF ONE OF THE VALUES IS 0
    if(x == 0){
	return y;
    }

    if(y == 0){
	return x;
    }
  

  //STEP 1: DECOMPOSE OPERANDS

    //extract fields from x
    int xSign = flt32_get_sign(x);
    int xExp = flt32_get_exp(x);
    int xVal = flt32_get_val(x);

    //extract fields from y
    int ySign = flt32_get_sign(y);
    int yExp = flt32_get_exp(y);
    int yVal = flt32_get_val(y);

  //STEP 2: EQUALIZE OPERAND EXPONENTS

    //if xExp is smaller than yExp
    if(xExp < yExp){

	//find difference between the exponents
	int diff1 = (yExp - xExp);

	//shift x mantissa to the right by diff1
	xVal = (xVal>>diff1);

	//increase x exponent by diff1
	xExp = (xExp + diff1);

    }

    //if yExp is smaller than xExp
    if(yExp < xExp){

	//find difference between the exponents
	int diff2 = (xExp - yExp);

	//shift y mantissa to the right by diff2
	yVal = (yVal>>diff2);

	//increase y exponent by diff2
	yExp = (yExp + diff2);

    }

    //variable for resultExp
    int resultExp = 0;

    //confirm that exponents are equal
    if(yExp == xExp){

	//set resultExp to yExp
	resultExp = yExp;
    }

  //STEP 3: CONVERT OPERANDS FROM SIGNED MAGNITUDE TO 2's COMPLEMENT

    //if x is negative
    if(xSign == 1){

	//invert the bits of the mantissa
	xVal = (~xVal);

	//add 1
	xVal = (xVal + 1);

    }

    //if y is negative
    if(ySign == 1){

	//invert the bits of the mantissa
	yVal = (~yVal);

	//add 1
	yVal = (yVal + 1);

    }

  //STEP 4: ADD MANTISSAS

    //since both mantissas have the same exponent, add them
    int result = (yVal + xVal);

  //STEP 5: CONVERT RESULT FROM 2's COMPLEMENT TO SIGNED MAGNITUDE

    //if the result is negative
    if(result < 0){
	
	//invert the bits of the result
	result = (~result);

	//add 1
	result = (result + 1);

    }

  //STEP 6: NORMALIZE RESULT (FIX IMPLICIT 1 POSITION)

    //if the leftmost 1 of the result is left of bit 23
    if(flt32_left_most_1(result) > 23){

	//find the difference between the position and 23
	int diff3 = (flt32_left_most_1(result)-23);

	//shift result to the right by diff3
	result = (result>>diff3);

	//increase exponent
	resultExp = (resultExp + diff3);

    }

    //if the leftmost 1 of the result is right of bit 23
    if(flt32_left_most_1(result) < 23){

	//find the difference between the position and 23
	int diff4 = (23 - flt32_left_most_1(result));

	//shift result to the left by diff4
	result = (result<<diff4);

	//decrease exponent
	resultExp = (resultExp - diff4);

    }


  //STEP 7: COMPOSE RESULT (AND REMOVE IMPLICIT 1)

    //variable for resulting sign
    int resultSign;

    //figure out if the value should be negative or positive
    if(x < 0 && flt32_abs(x) > y){
	resultSign = 1;
    }

    else if(y < 0 && flt32_abs(y) > x){
	resultSign = 1;
    }

    else{
	resultSign = 0;
    }

    //remove implicit 1 by clearing bit 24 of the result
    result =  clearBit(result, 23);

    //variable for final answer
    int final = 0;

    //set the sign of the final answer
    final = setField(final, 31, 31, resultSign);

    //set the exponent of the final answer
    final = setField(final, 30, 23, resultExp);

    //set the mantissa of the final answer
    final = setField(final, 22, 0, result);

    //return the final value
    return final;

}

//Subtract value x from value y
flt32 flt32_sub (flt32 x, flt32 y) {
  
    //check if the values are equal
    if(x == y){
	return 0.0;
    }

    else{

	//negate y
	y = flt32_negate(y);

	//add x and y
	int result = flt32_add(x, y);

	//return result
	return result;

    }

}

