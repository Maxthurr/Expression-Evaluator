# Project variables
CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla
PROJECT_DIR=evaluator
TARGET=eval

# Sources
SRC=$(PROJECT_DIR)/src/main.c $(PROJECT_DIR)/src/evalrpn.c $(PROJECT_DIR)/src/lexer.c $(PROJECT_DIR)/src/parser.c \
    $(PROJECT_DIR)/src/operation.c $(PROJECT_DIR)/src/data_structures/queue.c $(PROJECT_DIR)/src/data_structures/stack.c \
	$(PROJECT_DIR)/src/data_structures/token_stack.c
OBJS=$(SRC:.c=.o)
TESTSUITE= ./$(PROJECT_DIR)/tests/testsuite.sh

# Rules

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS) $(LDFLAGS)

debug: CFLAGS+=-fsanitize=address -g
debug: LDFLAGS+=-fsanitize=address
debug: $(TARGET)

check: $(TARGET)
	$(TESTSUITE)

clean:
	$(RM) $(OBJS) $(TARGET)
