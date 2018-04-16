CC=gcc
INCLUDES=-ISDK:MUI/C/include
OBJ=hellowinos4.o
LIBS=-lauto -lm

%.o: %.c
	$(CC) -c -o $@ $< $(INCLUDES)

hellowinos4: $(OBJ)
	$(CC) -o helloswinos4 $(OBJ) $(LIBS)