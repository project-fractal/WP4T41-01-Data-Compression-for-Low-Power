# CMakeList.txt : CMake project for Compression, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("Compression")
set (CMAKE_CXX_STANDARD 11)

# Add source to this project's executable.
add_library (Compression STATIC "data_compression.h" "FileManag.cpp" "FileManag.h" "LZW.h" "LZW.cpp" "LZWCompression.cpp" "Utility.cpp" "Utility.h")

# TODO: Add tests and install targets if needed.
install(TARGETS Compression DESTINATION lib)