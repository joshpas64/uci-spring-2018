CC = gcc
INCLUDES = -I./include/
CFLAGS = -Wall
LIBS = -lm
RM = rm -f
SRCS = include/*.c 
OBJS = simulator.o queue.o
TARGET = $(OBJS) main

all: $(TARGET)
queue.o: include/queue.c
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -c -g include/queue.c
simulator.o: include/simulator.c
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -c -g include/simulator.c
main: main.c
	$(CC) -o main -g main.c $(OBJS) $(INCLUDES) $(LIBS)
	mv *.o include/

clean:
	$(RM) $(TARGET) include/*.o
