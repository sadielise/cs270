#simple Makefile for FLOAT Assignment

# these are defined so that on a different system only these definitions
# need to change if gcc and rm are actually in different locations
# The rest of the Makefile need not change

RM   = /bin/rm
GCC  = /usr/bin/gcc

CFLAGS  = -g -Wall -c -std=c99
LDFLAGS = -g

testFlt32: field.o flt32.o testFlt32.o
	${GCC} ${LDFLAGS} -o testFlt32 field.o flt32.o testFlt32.o

flt32.o: field.h flt32.h flt32.c
	${GCC} ${CFLAGS} flt32.c

testFlt32.o: field.h flt32.h testFlt32.c
	${GCC} ${CFLAGS} testFlt32.c

clean:
	${RM} -f flt32.o testFlt32.o testFlt32


