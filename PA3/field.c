#include "field.h"

/** @file field.c
 *  @brief You will modify this file and implement six functions
 *  @details Your implementation of the functions defined in field.h.
 *  You may add other function if you find it helpful. Added function
 *  should be declared <b>static</b> to indicate they are only used
 *  within this file. For example, methods you write may need a mask value.
 *  You might write a method to compute mask for you and use it wherever
 *  it is needed as oposed to just puting the code in line.
 * <p>
 * @author <b>Sadie Henry</b> goes here
 */

//method to create a mask
static int mask(int position){

    int number = 0x00000001;
    int shifted = number<<position;
    return shifted;

}

//Get the specified bit from a value
int getBit (int value, int position) {
	
    //get the mask 
    int getMask = mask(position);

    //& the mask with the value
    int cleared = value&getMask;

    //shift that value all the way to the left
    int shifted = cleared>>position;

    if(shifted == 0)
	return 0;
    else if(shifted == 1)
	return 1;
    else
	return -1;
}

//Set the specified bit in a value to 1
int setBit (int value, int position) {
    
    //get the mask
    int getMask = mask(position);

    // | the mask with the value
    int set = value|getMask;

    return set;
}


//Set the specified bit in a value to 0
int clearBit (int value, int position) {

    //get the mask
    int getMask = mask(position);

    //flip the bits of the mask
    int flipped = ~(getMask);

    // & the mask with the value
    int cleared = value&flipped;

    return cleared;
}

/** @todo Implement in field.c based on documentation contained in field.h */
int getField (int value, int hi, int lo, int isSigned) {
    
    //make sure hi and lo are correct
    if(lo > hi){
	int temp = lo;
	lo = hi;
	hi = temp;
    }

    int set = 0;

    if(isSigned == 0){

        //variable for mask
        int getMask = mask(lo);

	//loop to create mask
    	for(int i = (lo+1); i <= hi; i++){
		int tempMask = mask(i);
		getMask = (getMask|tempMask);
   	}	
	
	// & the mask with the value
	set = (getMask&value);

	// shift set down
	set = (set>>lo);

	return set;
    }

    else{

	//get mask
	int getMask1 = mask(31);

	// & mask with value
	int testBit = (value&getMask1);

	//shift to the left to find value
	int shifted = (testBit>>31);

	//get another mask
	int getMask2 = mask(lo);

	for(int j = (lo+1); j <= hi; j++){
		int tempMask1 = mask(j);
		getMask2 = (getMask2|tempMask1);
	}	

	// & the mask with the value
	int set1 = (getMask2&value);

	//shift set down
	set1 = (set>>lo);

	if(shifted == 1){
		set1 = (set*-1);
	}

	return set1;

    }
    
}

//Change the bits of OldValue between hi and lo to the new Value,
//leaving the other bits unchanged
int setField (int oldValue, int hi, int lo, int newValue) {
    
    //make sure hi and lo are correct
    if(lo > hi){
	int temp = lo;
	lo = hi;
	hi = temp;
    }

    //variable for mask
    int mask1 = mask(lo);

    //loop to create mask
    for(int i = (lo+1); i <= hi; i++){
    	int tempMask = mask(i);
    	mask1 = (mask1|tempMask);
    }	

    //negate the mask
    int newMask1 = (~mask1);

    // & the oldValue with the mask
    int clearedOld = (oldValue&newMask1);

    //get range of field
    int range = hi-lo;

    //get another mask
    int mask2 = mask(0);

    //loop to create mask
    for(int k = 1; k <=range; k++){
	int tempMask1 = mask(k);
	mask2 = (mask2|tempMask1);
    }

    //get the correct range of bits from newValue
    int fixedNewValue = (mask2&newValue);

    //shift the fixedNewValue
    int shiftNewValue = (fixedNewValue<<lo);

    // | the shiftNewValue with the clearedOld value
    int finalValue = (shiftNewValue|clearedOld);

    return finalValue;

}

//Determine if a value will fit in a specified field
int fieldFits (int value, int width, int isSigned) {

    if(isSigned == 0){
	
    	//get a mask to find width value
    	int getMask = mask(0);

   	//loop to create mask
    	for(int k = 1; k < width; k++){
		int tempMask1 = mask(k);
		getMask = (getMask|tempMask1);
    	}

	if(value > getMask){
		return 0;
	}
	else{
		return 1;
	}

    }

    else{

	//get a mask to find width value
	int getMask1 = mask(0);

	//loop to create mask
	for(int j = 1; j < width; j++){
		int tempMask2 = mask(j);
		getMask1 = (getMask1|tempMask2);
	}

	//find upper limit
	int upper = (getMask1/2);

	//find lower limit
	int lower = (getMask1-getMask1-getMask1-1);

	if(value > upper || value < lower){
		return 0;
	}

	else{
		return 1;
	}

    }

}
