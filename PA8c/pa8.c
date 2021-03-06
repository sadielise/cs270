#include <stdlib.h>
#include <stdio.h>

/**
* @author Sadie Henry
* @date 11/4/14
* @details This program prints a 16-bit decimal number
* that is input by the user. 
*/

void printDigit(int digit){

    switch(digit){
	case 0: printf("%c", '0'); break;
	case 1: printf("%c", '1'); break;
	case 2: printf("%c", '2'); break;
	case 3: printf("%c", '3'); break;
	case 4: printf("%c", '4'); break;
	case 5: printf("%c", '5'); break;
	case 6: printf("%c", '6'); break;
	case 7: printf("%c", '7'); break;
	case 8: printf("%c", '8'); break;
	case 9: printf("%c", '9'); break;
    }
}

void printValue(int input){

    // base case
    if(input < 10){
	printDigit(input%10);
    }

    // recursive case
    else{
	printValue(input/10);
	printDigit(input%10);

    }
}

int main(int argc, char *argv[]){

    // variable for user input
    int userInput;

    // ask user for decimal number
    printf("Please enter a 16-bit decimal number: ");
    scanf("%d", &userInput);

    if(userInput > 32767){
	printf("Error: number is outside of acceptable range.\n");
        exit(0);
    }
    else{
	printValue(userInput);
    }

    printf("\n");

}


	



