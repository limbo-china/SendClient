CC      = gcc
CFLAGS	= -Wall -ggdb3 -std=gnu99 -D_GNU_SOURCE 

HDRS	= $(wildcard *.h)
OBJS	= $(wildcard *.c )
INC   	= -I ./
LIBS  	= -lpthread
EXEC	= vLogServer

.PHONY : all clean

all : $(EXEC)
	mv $(EXEC) ../
$(EXEC) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)  $(LIBS)

install :
	cp -f $(EXEC) ../bin/
	
clean :
	$(RM) $(EXEC)
