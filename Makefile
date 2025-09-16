# Compiler and flags
CC = gcc
CFLAGS = -Wall -I./thirdparty/raylib/src/
LDFLAGS = -L./thirdparty/raylib/src/ -lraylib -framework IOKit -framework Cocoa -framework OpenGL

# Target
TARGET = cnake
SRC = cnake.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
