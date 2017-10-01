// PA2 Assignment - convert.h
// Author: Sadie Henry
// Date:   9/3/2014
// Class:  CS270
// Email:  sadiet@rams.colostate.edu


#include <string.h>
#include <stdio.h>

// char2int
// Converts from a character to an integer digit
// if character '0'..'9' convert to 0..9
// else if character 'A'..'F' convert to 10..15
// else convert to -1
int char2int(char digit){

    switch(digit)
    {
	case '0': return 0; break;
	case '1': return 1; break;
	case '2': return 2; break;
	case '3': return 3; break;
	case '4': return 4; break;
	case '5': return 5; break;
	case '6': return 6; break;
	case '7': return 7; break;
	case '8': return 8; break;
	case '9': return 9; break;
	case 'A': return 10; break;
	case 'B': return 11; break;
	case 'C': return 12; break;
	case 'D': return 13; break;
	case 'E': return 14; break;
	case 'F': return 15; break;
	default: return -1; break;
    }

}

// int2char
// Converts from an integer digit to a character
// if integer 0..9 convert to '0'..'9'
// else if integer 10..15 convert to 'A'..'F'
// else convert to 'X'
char int2char(int digit){

    switch(digit)
    {
	case 0: return '0'; break;
	case 1: return '1'; break;
	case 2: return '2'; break;
	case 3: return '3'; break;
	case 4: return '4'; break;
	case 5: return '5'; break;
	case 6: return '6'; break;
	case 7: return '7'; break;
	case 8: return '8'; break;
	case 9: return '9'; break;
	case 10: return 'A'; break;
	case 11: return 'B'; break;
	case 12: return 'C'; break;
	case 13: return 'D'; break;
	case 14: return 'E'; break;
	case 15: return 'F'; break;
	default: return 'X'; break;
    }

}

// Convert integer to string in specified base and print
// 2 <= base <= 16
// from base 10 to a specified base
void int2ascii(int value, int base){

    if(value/base == 0){
	printf("%c", int2char(value%base));
    }

    else{
	char a = int2char(value%base);
	int2ascii(value/base, base);
	printf("%c", a);
    }
}

// Convert string in specified base to integer and print
// 2 <= base <= 16
// from a specified base to base 10
void ascii2int(char *ascii, int base){

	int result = 0;

	for(int i = 0; i < strlen(ascii); i++){
		char c = ascii[i];
		int i = char2int(c);	
		result = result * base + i;
	
	}

	printf("%i", result);
}


