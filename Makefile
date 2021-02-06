## This is a simple Makefile 

# Define what compiler to use and the flags.
CC=cc
CXX=CC
CCFLAGS= -g -std=c99 -Wall -Werror

#all: test_list
all: hw2_solution 

# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


test_queue: hw2_solution.o test_queue.o
	$(CC) -o test_queue hw2_solution.o test_queue.o
	

###################################
# BEGIN SOLUTION
hw2_solution: hw2_solution.o test_queue.o
	$(CC) -o hw2 hw2_solution.o test_queue.o

test_list: test_list
	@echo ""
	@echo "***** NOW TESTING YOUR CODE (not compiling any more) *****"
	@echo ""
	./hw2
# END SOLUTION
###################################


clean:
	rm -f core *.o hw2 test_queue

