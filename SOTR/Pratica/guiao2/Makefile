# Standard Makefile to buiild Xenomai 3 applications, 
#   with alchemy skin (former native skin)
# Use xeno-config to get the correct compile and link flags
# Don't forget to specify the skin
XENO_CONFIG := /usr/xenomai/bin/xeno-config
CFLAGS := $(shell $(XENO_CONFIG) --skin=alchemy --cflags)
LDFLAGS := $(shell $(XENO_CONFIG) --skin=alchemy --ldflags)
# Add -lm if math functions are necessary 
LDFLAGS += -lm  
CC := $(shell $(XENO_CONFIG) --cc)

#EXECUTABLE1 := periodicTask_a1
EXECUTABLE2 := periodicTask_a2
EXECUTABLE3 := periodicTask_a3

all: $(EXECUTABLE2) $(EXECUTABLE3)

%: %.c
	$(CC) -g -o $@ $< $(CFLAGS) $(LDFLAGS) 
	
clean:	
	rm $(EXECUTABLE2) $(EXECUTABLE3)
	
