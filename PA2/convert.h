// PA2 Assignment - convert.h
// Author: Chris Wilcox
// Date:   9/1/2014
// Class:  CS160
// Email:  wilcox@cs.colostate.edu

// char2int
// Converts from a character to an integer digit
// if character '0'..'9' convert to 0..9
// else if character 'A'..'F' convert to 10..15
// else convert to -1
int char2int(char digit);

// int2char
// Converts from an integer digit to a character
// if integer 0..9 convert to '0'..'9'
// else if integer 10..15 convert to 'A'..'F'
// else convert to 'X'
char int2char(int digit);

// Convert integer to string in specified base and print
// 2 <= base <= 16
void int2ascii(int value, int base);

// Convert string in specified base to integer and print
// 2 <= base <= 16
void ascii2int(char *ascii, int base);

