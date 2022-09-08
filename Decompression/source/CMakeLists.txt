# CMakeList.txt : CMake project for Decompression, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("Decompression")
set (CMAKE_CXX_STANDARD 11)

# Add source to this project's executable.
add_library (Decompression STATIC "data_compression.h" "FileManag.cpp" "FileManag.h" "LZW.h" "LZW.cpp" "LZWDeCompression.cpp" "Utility.cpp" "Utility.h")

# TODO: Add tests and install targets if needed.
install(TARGETS Decompression DESTINATION lib)