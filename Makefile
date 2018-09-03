all : myapp

run : myapp
	./myapp

CC = gcc
LD = gcc

TARGETS = myapp
SRCS = main.c myscaledimage.c
OBJS = $(SRCS:%.c=%.o)

CFLAGS = -Wall `pkg-config --cflags gtk+-3.0`
LDFLAGS = -Wall `pkg-config --libs gtk+-3.0`

clean:
	rm myapp *.o 

myapp : $(OBJS) Makefile
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%o : %.c %.h
	$(CC) $(CFLAGS)  -c $^ -o $@
