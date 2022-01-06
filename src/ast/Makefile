
TARGET	=	../../lib/libast.a

OBJS	=	ast.o

SRCS	=	$(OBJS:.o=.c)

CARGS	=	-Wall -Wextra
INCS	=	-I. -I../common -I../scanner -I../ast -I../parser
CC		=	gcc

ifeq ($(RELEASE), 1)
	CARGS += -O3
else
	CARGS += -g
endif

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) -crs $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CARGS) $(INCS) -c $< -o $@

clean:
	-rm -f $(OBJS) $(TARGET)
