include $(PWD)/config.mk

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: libcog

%.o: %.c
	$(COMPILE_OBJ)

libcog: $(OBJS)
	$(MAKE_STATIC_LIB)
