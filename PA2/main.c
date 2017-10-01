// PA2 Assignment - main.c
// Author: Chris Wilcox
// Date:   9/1/2014
// Class:  CS160
// Email:  wilcox@cs.colostate.edu

#include <stdio.h>
#include "convert.h"

// Test Program
int main(int argc, char *argv[]) {

    // Base 16 (hexadecimal)
    int value = 1234;
    printf("%d (base 10) = ", value); int2ascii(value, 16); printf(" (base 16)\n");
    char *string = "ABCD";
    printf("%s (base 16) = ", string); ascii2int(string, 16);  printf(" (base 10)\n");

    // Base 8 (octal)
    value = 1234;
    printf("%d (base 10) = ", value); int2ascii(value, 8); printf(" (base 8)\n");
    string = "4567";
    printf("%s (base 8) = ", string); ascii2int(string, 8);  printf(" (base 10)\n");

    // Base 2 (binary)
    value = 1234;
    printf("%d (base 10) = ", value); int2ascii(value, 2); printf(" (base 2)\n");
    string = "10011011";
    printf("%s (base 2) = ", string); ascii2int(string, 2);  printf(" (base 10)\n");

    // Base 6 (unusual!)
    value = 1234;
    printf("%d (base 10) = ", value); int2ascii(value, 6); printf(" (base 6)\n");
    string = "4532";
    printf("%s (base 6) = ", string); ascii2int(string, 6);  printf(" (base 10)\n");
}
