
TARGET	=	../bin/simple

OBJS	=	token.o \
			errors.o \
			trace.o \
			memory.o \
			strings.o \
			symbols.o \
			scanner.o \
			emit.o \
			parser.o \
			class.o \
			func_def.o \
			import.o \
			main.o

SRCS	=	$(OBJS:.o=.c)

CARGS	=	-Wall -Wextra
INCS	=	-I. -I../common
CC		=	gcc

ifeq ($(RELEASE), 1)
	CARGS += -O3
else
	CARGS += -g -DTRACE
endif

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS) keywords.h
	$(CC) $(INCS) $(CARGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(INCS) $(CARGS) -c $< -o $@

mk_keywords: mk_keywords.c
	$(CC) $(INCS) $(CARGS) $< -o $@

keywords.h: kw.txt mk_keywords
	./mk_keywords

token.o: keywords.h

clean:
	-rm -f $(OBJS) $(TARGET) keywords.h mk_keywords
