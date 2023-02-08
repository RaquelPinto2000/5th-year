CC_FLAGS= -c -lm 
INCLUDE_FLAGS = -I h/

FILE=imageProcessing
SRC_DIR=src
OUTPUT_DIR=bin

test: all
	gcc ${INCLUDE_FLAGS} test/main.c -o bin/test ${OUTPUT_DIR}/${FILE}.o -lm
	chmod +x bin/test

all:
	mkdir -p bin
	gcc ${CC_FLAGS} ${INCLUDE_FLAGS} ${SRC_DIR}/${FILE}.c -o ${OUTPUT_DIR}/${FILE}.o

clean:
	rm -f bin/${FILE}.o
	rm -f bin/test
	rmdir bin/