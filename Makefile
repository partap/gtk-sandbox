all : myapp

run : myapp
	./myapp

CC = gcc
LD = gcc

TARGETS = myapp
SRCS = main.c myscaledimage.c s3-reticle.c
OBJS = $(SRCS:%.c=%.o)

CFLAGS = -g -Wall `pkg-config --cflags gtk+-3.0`
LDFLAGS = -g -Wall 
LIBS = `pkg-config --libs gtk+-3.0` -lm
clean:
	rm myapp $(OBJS)

myapp : $(OBJS) Makefile
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $@ 

%o : %.c %.h
	$(CC) $(CFLAGS)  -c $^ -o $@
