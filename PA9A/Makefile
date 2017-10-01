# List of files
C_SRCS		= testUtil.c util.c
C_OBJS		= testUtil.o util.o
C_HEADERS	= lc3.h util.h
EXE             = testUtil

# Compiler and loader commands and flags
GCC		= gcc
GCC_FLAGS	= -g -std=c99 -Wall -c
LD_FLAGS	= -g -std=c99 -Wall

# Compile .c files to .o files
.c.o:
	$(GCC) $(GCC_FLAGS) $<

# Target is the executable
$(EXE): $(C_OBJS)
	$(GCC) $(LD_FLAGS) $(C_OBJS) -o $(EXE)

# Recompile C objects if headers change
${C_OBJS}: ${C_HEADERS}

# Clean up the directory
clean:
	rm -f *.o *~ $(EXE) 
