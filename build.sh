#!/usr/bin/bash

set -e

PRESET="debug"

if [ $# -ge 1 ]; then
	PRESET="release"
fi

echo "Using CMake preset: $PRESET"

cmake --preset $PRESET
cmake --build --preset $PRESET
