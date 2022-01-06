
TARGET	=	../../bin/simple

OBJS	=	main.o

LDEPS	=	../../lib/libutil.a \
			../../lib/libscanner.a

SRCS	=	$(OBJS:.o=.c)

DEBUG	=	-g
CARGS	=	-Wall -Wextra
LIBS	=	-lm
INCS	=	-I../util -I../scanner
CC		=	gcc

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS) $(LDEPS)
	$(CC) $(DEBUG) $(CARGS) $(INCS) $(OBJS) -o $(TARGET) $(LDEPS)

.c.o:
	$(CC) $(HSRC) $(CARGS) $(DEBUG) $(INCS) -c $< -o $@

../lib/libutil.a:
	make -C util

../lib/libscanner.a:
	make -C scanner

clean:
	-rm -f $(OBJS) $(TARGET)
