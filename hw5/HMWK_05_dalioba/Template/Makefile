# Dalio, Brian A.
# dalioba
# 2020-11-21

PROJS   = cullTest pipeline

CC        = gcc
CFLAGS    = -g -Wall -Wextra -fsanitize=address -fsanitize=leak -static-libasan
LIBS      = -lm

LIBFILES = matrix.c transforms.c support.c

HFILES   = matrix.h transforms.h pipeline.h
CFILES   = clip.c cull.c display.c eulerRotate.c project.c $(LIBFILES)

all : $(PROJS)

cullTest : $(HFILES) $(CFILES) cullTest.c
	$(CC) $(CFLAGS) -o cullTest $(CFILES) cullTest.c $(LIBS)

pipeline : $(HFILES) $(CFILES) main.c
	$(CC) $(CFLAGS) -o pipeline $(CFILES) main.c $(LIBS)

clean:
	rm -f *.o $(PROJS)
