# Makefile template for CS 270 (specialized for R6) 

# List of files
C_SRCS		= main.c struct.c
C_OBJS		= main.o struct.o
C_HEADERS	= struct.h

OBJS		= ${C_OBJS} 
EXE	        = R6

# Compiler and loader commands and flags
GCC		    = gcc
GCC_FLAGS	= -g -std=c99 -Wall -c
LD_FLAGS	= -g -std=c99 -Wall

# Target is the executable
R6: $(OBJS)
	@echo "Linking all object modules ..."
	$(GCC) $(LD_FLAGS) $(OBJS) -o $(EXE)
	@echo ""

# Recompile C objects if headers change
${C_OBJS}:      ${C_HEADERS}

# Compile .c files to .o files
%.o: %.c
	@echo "Compiling each C source file separately ..."
	$(GCC) $(GCC_FLAGS) $<
	@echo ""

# Clean up the directory
clean:
	@echo "Cleaning up project directory ..."
	rm -f *.o *~ $(EXE) 
	@echo ""

# Package the directory
package:
	@echo "Packaging up project directory ..."
	tar cvf $(EXE).tar ../$(EXE)
	@echo ""

# Define my own target
date:
	@echo "Running my commands ..."
	date
	uname -a
	pwd
	@echo ""

