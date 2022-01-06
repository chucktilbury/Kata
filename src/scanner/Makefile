
TARGET	=	../../lib/libscanner.a

OBJS	=	scanner.o \
			token.o

SRCS	=	$(OBJS:.o=.c)

DEBUG	=	-g
CARGS	=	-Wall -Wextra -I. -I../util
LIBS	=	-lm
CC		=	gcc

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS) keywords.h
	$(AR) -crs $(TARGET) $(OBJS)

.c.o:
	$(CC) $(HSRC) $(CARGS) $(DEBUG) -c $< -o $@

mk_keywords: mk_keywords.c
	$(CC) $(HSRC) $(CARGS) $(DEBUG) $< -o $@

keywords.h: kw.txt mk_keywords
	./mk_keywords

token.o: keywords.h

clean:
	-rm -f $(OBJS) $(TARGET) keywords.h mk_keywords
