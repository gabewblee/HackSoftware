CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = VMTranslator
SRCS = VMTranslator.c CodeWriter.c Parser.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	find . -name "*.asm" -delete