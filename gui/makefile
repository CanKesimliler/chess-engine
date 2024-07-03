# Makefile

# Compiler
CC = gcc

# Directories
INCLUDE_DIR = $(RAYLIB_HOME)/include
LIB_DIR = $(RAYLIB_HOME)/lib

# Source files
SRC = gui.c board.c ../parser.c ../engine.c

# Output executable
TARGET = gui

# Compiler flags
CFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Build target
build: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LDFLAGS)
	@if [ $$? -eq 0 ]; then \
		echo "Compilation successful."; \
	else \
		echo "Compilation failed."; \
	fi

# Run target
run: build
	@if [ -f $(TARGET) ]; then \
		echo "Running the GUI application..."; \
		./$(TARGET); \
	else \
		echo "Application not found. Please compile first by running 'make build'."; \
	fi

# Debug target
debug: CFLAGS += -g
debug: build
	@if [ $$? -eq 0 ]; then \
		echo "Compilation successful with debug information."; \
	else \
		echo "Compilation failed."; \
	fi

# Clean target
clean:
	rm -f $(TARGET)