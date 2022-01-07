
TARGET	=	../../lib/libcommon.a

OBJS	=	errors.o \
			memory.o \
			strings.o \
			trace.o

SRCS	=	$(OBJS:.o=.c)

CARGS	=	-Wall -Wextra
INCS	=	-I. -I../scanner
CC		=	gcc

ifeq ($(RELEASE), 1)
	CARGS += -O3
else
	CARGS += -g -DTRACE
endif

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) -crs $(TARGET) $(OBJS)

.c.o:
	$(CC) $(INCS) $(CARGS) -c $< -o $@

clean:
	-rm -f $(OBJS) $(TARGET)
