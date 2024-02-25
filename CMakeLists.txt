# CMakeList.txt : CMake project for trie

# Minimum required CMake version
cmake_minimum_required(VERSION 3.18)

project(trie)

# Define a C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add your source files here
set(SOURCES
    "src/trie.hpp"
    "src/test.cpp"
)

# Include subdirectory containing hash-table.hpp
add_subdirectory("./dependencies")

# Add sources for this project's executable
add_executable(trie ${SOURCES})

# Target "dependencies" directory to include the "hash-table.hpp" header
target_include_directories(trie PRIVATE "./dependencies")

# Link executable with hashtable library
target_link_libraries(trie hashtable)

# Ensure the required C++ standard is used to build this project
set_property(TARGET trie PROPERTY CXX_STANDARD 20)