#!/bin/zsh

# Compile the GUI application using gcc with specified libraries and frameworks
gcc -o gui gui.c pieces.c -I$RAYLIB_HOME/include -L$RAYLIB_HOME/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the GUI application..."
    ./gui
else
    echo "Compilation failed."
fi