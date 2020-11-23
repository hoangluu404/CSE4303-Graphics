# Dalio, Brian A.
# dalioba
# 2020-11-09

CC        = gcc
CFLAGS    = -g -Wall -Wextra -fsanitize=address -fsanitize=leak -static-libasan
LIBS      = -lm

LIBFILES = matrix.c transforms.c support.c
HFILES   = matrix.h transforms.h pipeline.h

eulerTest : $(HFILES) $(LIBFILES) eulerRotate.c  eulerTest.c
	$(CC) $(CFLAGS) -o eulerTest $(LIBFILES) eulerRotate.c eulerTest.c $(LIBS)

projectTest : $(HFILES) $(LIBFILES) eulerRotate.c project.c projectTest.c
	$(CC) $(CFLAGS) -o projectTest $(LIBFILES) eulerRotate.c project.c projectTest.c $(LIBS)

pipeline : $(HFILES) $(LIBFILES) eulerRotate.c project.c clip.c display.c main.c
	$(CC) $(CFLAGS) -o pipeline $(LIBFILES) eulerRotate.c project.c clip.c display.c main.c $(LIBS)

clean:
	rm -f *.o eulerTest projectTest pipeline
