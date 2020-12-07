# Dalio, Brian A.
# dalioba
# 2020-12-04

PROJS   = pipeline

CC        = gcc
CFLAGS    = -g -Wall -Wextra -fsanitize=address -fsanitize=leak -static-libasan
LIBS      = -lm

LIBFILES = matrix.c transforms.c support.c

HFILES   = matrix.h transforms.h pipeline.h
CFILES   = clip.c cull.c display.c eulerRotate.c patch.c project.c $(LIBFILES)

all : $(PROJS)

pipeline : $(HFILES) $(CFILES) main.c
	$(CC) $(CFLAGS) -o pipeline $(CFILES) main.c $(LIBS)

clean:
	rm -f *.o $(PROJS)
