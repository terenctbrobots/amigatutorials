CC=gcc
INCLUDES=-ISDK:MUI/C/include
LIBS=-lauto -lm
CFLAGS=-D__USE_INLINE__
OBJ=hellowin.o

%.o: %.c
	$(CC) -c -o $@ $< $(INCLUDES) $(CFLAGS)

hellowin: $(OBJ)
	$(CC) -o hellowin $(OBJ) $(LIBS) $(CFLAGS)