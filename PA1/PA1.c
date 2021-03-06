// PA1 Assignment
// Author: Sadie Henry
// Date:   8/28/2014
// Class:  CS270
// Email:  sadiet@rams.colostate.edu

// Include files
#include <stdio.h>
#include <stdlib.h>

double computeCircle(double radius)
{
    return (3.141593 * radius * radius);
}

double computeTriangle(double side)
{
    return (0.433013 * side * side);
}

double computeSquare(double otherSide)
{
    return (otherSide * otherSide);
}

double computePentagon(double anotherSide)
{
    return (1.720477 * anotherSide * anotherSide);
}

double computeHexagon(double lastSide)
{
    return (2.598076 * lastSide * lastSide);
}

double square(double value)
{
    return (value * value);
}

int main(int argc, char *argv[])
{
    // Check number of arguments
    if (argc < 2){
        printf("No arguments supplied! ");
	return 1;
	}

    double one = atof(argv[1]);
    printf("CIRCLE, radius = %0.5f, area = %0.5f.\n", one, computeCircle(one));

    double two = atof(argv[2]);
    printf("TRIANGLE, length = %0.5f, area = %0.5f.\n", two, computeTriangle(two));

    double three = atof(argv[3]);
    printf("SQUARE, length = %0.5f, area = %0.5f.\n", three, computeSquare(three));

    double four = atof(argv[4]);
    printf("PENTAGON, length = %0.5f, area = %0.5f.\n", four, computePentagon(four));

    double five = atof(argv[5]);
    printf("HEXAGON, length = %0.5f, area = %0.5f.\n", five, computeHexagon(five));

    return 0;
}

