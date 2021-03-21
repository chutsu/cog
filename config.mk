# DIRS
OBJ_DIR = ${PWD}/build/obj
BIN_DIR = ${PWD}/build/bin
LIB_DIR = ${PWD}/lib

# INCLUDE AND LIBRARY PATHS
INCLUDES = -I${PWD}/lib -I/usr/include -I/usr/local/include
LIBS = -L$(LIB_DIR) -lcog -lrt -L/usr/lib -L/usr/local/lib

# C COMPILER
CC = gcc
STANDARD = -std=gnu99
DEBUG_FLAGS = -g
WARN_FLAGS = -Wall

# WARN_FLAGS = -Weverything \
# 			 -Wno-padded \
# 			 -Wno-gnu-zero-variadic-macro-arguments \
# 			 -Wno-empty-translation-unit
CFLAGS = $(DEBUG_FLAGS) $(WARN_FLAGS) $(STANDARD) $(INCLUDES)

# ARCHIVER
AR = ar
ARFLAGS = rvs

# COMMANDS
COMPILE_OBJ = \
	echo "CC [$<]"; \
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(OBJ_DIR)/, $@);

MAKE_TEST = \
	echo "CC [$@.c]"; \
	$(CC) $(CFLAGS) $@.c -o $(BIN_DIR)/$@ $(LIBS);

MAKE_STATIC_LIB = \
	echo "AR [$@]"; \
	$(AR) $(ARFLAGS) $(LIB_DIR)/$@.a $(wildcard $(OBJ_DIR)/*.o);
