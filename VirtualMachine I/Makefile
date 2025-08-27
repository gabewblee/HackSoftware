CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = VMTranslator
SOURCES = VMTranslator.c CodeWriter.c Parser.c
HEADERS = CodeWriter.h Parser.h Config.h
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f *.asm

test: $(TARGET)
	./$(TARGET) StackArithmetic/SimpleAdd/SimpleAdd.vm
	./$(TARGET) StackArithmetic/StackTest/StackTest.vm
	./$(TARGET) MemoryAccess/BasicTest/BasicTest.vm
	./$(TARGET) MemoryAccess/PointerTest/PointerTest.vm
	./$(TARGET) MemoryAccess/StaticTest/StaticTest.vm

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)
