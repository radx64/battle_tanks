#!/bin/bash

# Cross-compile script for Windows using MinGW-w64

set -e

echo "Setting up cross-compilation for Windows..."

# Check if MinGW-w64 is installed
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "MinGW-w64 not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y mingw-w64 mingw-w64-x86-64-freetype
else
    echo "MinGW-w64 is already installed."
fi

# Create toolchain file
TOOLCHAIN_FILE="toolchain-mingw.cmake"
cat > "$TOOLCHAIN_FILE" << 'EOF'
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

# Set SFML to static for cross-compilation
set(SFML_STATIC_LIBRARIES TRUE)

# Build static libraries
set(BUILD_SHARED_LIBS FALSE)

# Static linking for MinGW
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")
EOF

echo "Toolchain file created: $TOOLCHAIN_FILE"

# Create build directory
BUILD_DIR="build_windows"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring CMake with MinGW toolchain..."
cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=../"$TOOLCHAIN_FILE" -DBUILD_TESTS=OFF -DSFML_BUILD_AUDIO=FALSE ..

echo "Building project..."
ninja

echo "Cross-compilation completed successfully!"
echo "Executables are in $BUILD_DIR/"
