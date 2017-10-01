# Makefile template for CS 270 (specialized for R12) 

# List of files
C_SRCS		= pointers.c
C_OBJS		= pointers.o

OBJS		= ${C_OBJS} 
EXE	        = R12

# Compiler and loader commands and flags
GCC		    = gcc
GCC_FLAGS	= -g -std=c99 -Wall -c
LD_FLAGS	= -g -std=c99 -Wall

# Target is the executable
R12: $(OBJS)
	@echo "Linking all object modules ..."
	$(GCC) $(LD_FLAGS) $(OBJS) -o $(EXE)
	@echo ""

# Compile .c files to .o files
.c.o:
	@echo "Compiling each C source file separately ..."
	$(GCC) $(GCC_FLAGS) $<
	@echo ""

# Clean up the directory
clean:
	@echo "Cleaning up project directory ..."
	rm -f *.o *~ $(EXE) 
	@echo ""

