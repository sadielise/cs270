# Makefile template for CS 270 (specialized for PA4) 

# List of files
C_SRCS		= main.c logic.c
C_OBJS		= main.o logic.o
C_HEADERS	= logic.h

OBJS		= ${C_OBJS} 
EXE	        = PA5

# Compiler and loader commands and flags
GCC		    = gcc
GCC_FLAGS	= -g -std=c99 -Wall -O0 -c
LD_FLAGS	= -g -std=c99 -Wall -O0

# Target is the executable
PA4 : $(OBJS)
	@echo "Linking all object modules ..."
	$(GCC) $(LD_FLAGS) $(OBJS) -o $(EXE)
	@echo ""

# Recompile C objects if headers change
${C_OBJS}:      ${C_HEADERS}

# Compile .c files to .o files
.c.o:
	@echo "Compiling each C source file separately ..."
	$(GCC) $(GCC_FLAGS) $<
	@echo ""

# Clean up the directory
clean:
	@echo "Cleaning up project directory ..."
	rm -f *.o *~ $(EXE).tar $(EXE) core
	@echo ""

# Package the directory
package:
	@echo "Packaging up project directory ..."
	tar cvf $(EXE).tar ../$(EXE)
	@echo ""


