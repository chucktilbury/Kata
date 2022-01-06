
TARGET	=	../../lib/libcommon.a

OBJS	=	errors.o \
			memory.o \
			strings.o \
			trace.o

SRCS	=	$(OBJS:.o=.c)

DEBUG	=	-g
CARGS	=	-Wall -Wextra -I. -I../scanner
LIBS	=	-lm
CC		=	gcc

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) -crs $(TARGET) $(OBJS)

.c.o:
	$(CC) $(HSRC) $(CARGS) $(DEBUG) -c $< -o $@

clean:
	-rm -f $(OBJS) $(TARGET) keywords.h mk_keywords
