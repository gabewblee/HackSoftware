CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = Assembler

SRCS = Assembler.c Parser.c Code.c SymbolTable.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
