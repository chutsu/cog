include $(PWD)/config.mk

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

default: $(OBJS) libcog

%.o: %.c
	$(COMPILE_OBJ)

libcog:
	$(MAKE_STATIC_LIB)
