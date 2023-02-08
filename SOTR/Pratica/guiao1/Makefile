CC =  gcc # Set the compiler
L_FLAGS = -lrt -lpthread -lm
#C_FLAGS = -g

all: pt pt_A1 pt_A2 pt_A3
.PHONY: all

# Project compilation
pt: periodicTask.c
	$(CC) $< -o $@ $(C_FLAGS) $(L_FLAGS)

pt_A1: periodicTask_A1.c
	$(CC) $< -o $@ $(C_FLAGS) $(L_FLAGS)

pt_A2: periodicTask_A2.c
	$(CC) $< -o $@ $(C_FLAGS) $(L_FLAGS)

pt_A3: periodicTask_A3.c
	$(CC) $< -o $@ $(C_FLAGS) $(L_FLAGS)

.PHONY: clean 

clean:
	rm -f *.c~ 
	rm -f *.o
	rm pt
	rm pt_A1
	rm pt_A2
	rm pt_A3
	

# Some notes
# $@ represents the left side of the ":"
# $^ represents the right side of the ":"
# $< represents the first item in the dependency list   

