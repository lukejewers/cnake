# Compiler and flags
CC = gcc
CFLAGS = -Wall -I./thirdparty/raylib/src/
LDFLAGS = -L./thirdparty/raylib/src/ -lraylib
FRAMEWORKS = -framework IOKit -framework Cocoa -framework OpenGL

# Target
TARGET = cnake
SRC = cnake.c

# Check if raylib is built
RAYLIB = ./thirdparty/raylib/src/libraylib.a

$(TARGET): $(SRC) $(RAYLIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS) $(FRAMEWORKS)

$(RAYLIB):
	$(MAKE) -C ./thirdparty/raylib/src/

clean:
	rm -f $(TARGET)

.PHONY: clean
