# Makefile template for CS 270

# List of files
C_SRCS		= field.c testField.c
C_OBJS		= field.o testField.o
C_HEADERS	= field.h

OBJS		= ${C_OBJS} 
EXE	        = testField

# Compiler and loader commands and flags
GCC		= gcc
GCC_FLAGS	= -g -std=c99 -Wall -c
LD_FLAGS	= -g -std=c99 -Wall

# Compile .c files to .o files
.c.o:
	$(GCC) $(GCC_FLAGS) $<

# Target is the executable
default: $(OBJS)	
	$(GCC) $(LD_FLAGS) $(OBJS) -o $(EXE)

# Recompile C objects if headers change
${C_OBJS}:      ${C_HEADERS}

# Clean up the directory
clean:
	rm -f *.o *~ $(EXE) 

# Package the directory
package:
	@echo "Packaging up project directory ..."
