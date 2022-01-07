
TARGET	=	../../bin/simple

OBJS	=	main.o

LDEPS	=	../../lib/libemit.a \
			../../lib/libparser.a \
			../../lib/libscanner.a \
			../../lib/libcommon.a

SRCS	=	$(OBJS:.o=.c)

CARGS	=	-Wall -Wextra
LIBS	=	-lm
INCS	=	-I../common -I../scanner -I../emit -I../parser -I../symbols
CC		=	gcc

ifeq ($(RELEASE), 1)
	CARGS += -O3
else
	CARGS += -g -DTRACE
endif

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS) $(LDEPS)
	$(CC) $(CARGS) $(INCS) $(OBJS) -o $(TARGET) $(LDEPS)

.c.o:
	$(CC) $(CARGS) $(INCS) -c $< -o $@

../lib/libutil.a:
	make -C util

../lib/libscanner.a:
	make -C scanner

../lib/libast.a:
	make -C scanner

../lib/libparser.a:
	make -C scanner

clean:
	-rm -f $(OBJS) $(TARGET)
