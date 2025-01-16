# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name
project("Calc_C")

# Specify the C standard version (e.g., C99)
set(CMAKE_C_STANDARD 99)

# Add the include directory for header files
include_directories(include)

# Add the source files
# We specify the source directory and all .c files in it
file(GLOB SRC_FILES "src/*.c")

# Create the executable from source files
add_executable(myprogram ${SRC_FILES})

# Optionally, add compile options (e.g., -Wall, -g for debugging)
target_compile_options(myprogram PRIVATE -Wall -g)

# If you want to organize build outputs into a separate build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Optionally, link with libraries (add external libraries here if needed)
# Example: target_link_libraries(myprogram ${LIBRARY_NAME})

# Specify the build type (optional, e.g., Release or Debug)
# set(CMAKE_BUILD_TYPE Release)
