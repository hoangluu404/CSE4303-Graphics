# Dalio, Brian A.
# dalioba
# 2020-10-04

PROJS   = matrixTest2 transformsTest2 IDTest2

CC      = gcc
CFLAGS  = -g -Wall -Wextra -fsanitize=address -static-libsan
LIBS    = -lm

all : $(PROJS)

matrixTest2 : matrix.h matrix.c matrixTest2.c
	$(CC) $(CFLAGS) -o matrixTest2 matrix.c matrixTest2.c

transformsTest2 : matrix.h matrix.c transforms.h transforms.c transformsTest2.c
	$(CC) $(CFLAGS) -o transformsTest2 matrix.c transforms.c transformsTest2.c $(LIBS)

IDTest2 : matrix.h matrix.c transforms.h transforms.c IDTest2.c
	$(CC) $(CFLAGS) -o IDTest2 matrix.c transforms.c IDTest2.c $(LIBS)

clean:
	rm -f *.o $(PROJS)
