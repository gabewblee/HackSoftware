CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = JackCompiler

SRCS = JackCompiler.c CompilationEngine.c JackTokenizer.c Utils.c Statements.c Expressions.c Declarations.c SymbolTable.c VMWriter.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TEST_OBJS) $(TARGET)

.PHONY: all clean
