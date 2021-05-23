# DIRS
LIB_DIR = ${PWD}/lib
BLD_DIR = ${PWD}/build
OBJ_DIR = $(BLD_DIR)/obj
BIN_DIR = $(BLD_DIR)/bin

# INCLUDE AND LIBRARY PATHS
INCLUDES = -I${PWD}/lib -I/usr/include -I/usr/local/include
LIBS = -L$(BLD_DIR) -lm -lcog -lrt -L/usr/lib -L/usr/local/lib

# C COMPILER
# CC = gcc
CC = tcc
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
	$(AR) $(ARFLAGS) $(BLD_DIR)/$@.a $(wildcard $(OBJ_DIR)/*.o);
