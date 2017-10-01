// File:        main.c
// Description: ... fill this in
// Author:      Sadie Henry
// Date:        10/2/2014

// Include files
#include <stdio.h>
#include "struct.h"


//Global variables
#define MAX_STUDENTS 5

Student students[MAX_STUDENTS];

// Function:     main
// Description:  entry point  
int main(int argc, char *argv[])
{

    for(int i = 0; i < 5; i++){
	inputScores(&(students[i]));	
    }
 
    for(int i = 0; i < 5; i++){
	outputScores(students[i]);	
    }

    return 0;
}
