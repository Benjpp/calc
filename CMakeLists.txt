#Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name
project("Calc_C")

# Specify the C standard version (e.g., C99)
set(CMAKE_C_STANDARD 99)

#Setting variable for graphic library, SDL in this case
set(SDL_LIB /usr/local/include/SDL2)

# Add the include directory for header files
include_directories(include)
include_directories(SDL_LIB)

#variable to represent the relative path of source files
set(SRC_PATH "./src/")

# Create the executable from source files
add_executable(myprogram
                "${SRC_PATH}bintree.c" 
                "${SRC_PATH}interpreter.c"
                #The following line is to compile for testing tokenizing utilities from interpreter.c
                #"${SRC_PATH}prueba_interprete1-extraccion_de_tokens.c"
                #The following line is to compile for testing the construction of the binary opeartiontree
                #"${SRC_PATH}prueba_interprete2-construccion_arbol_operacion.c"
                #The following line is to test for the binary tree utilities
                #"${SRC_PATH}prueba-bintree.c"
                #The following line is to test for the computing type and control flow of inputted expresions
                #"${SRC_PATH}prueba_compute_type-reconocer_expresiones.c"    
                #The following line is to test and use the function plotter, which was the main goal of this project
                "{SRC_PATH}function_plotter.c"
        )


# Optionally, add compile options (e.g., -Wall, -g for debugging)
target_compile_options(myprogram PRIVATE -Wall -g -lm)

target_link_libraries(myprogram PRIVATE m)


# If you want to organize build outputs into a separate build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Optionally, link with libraries (add external libraries here if needed)
# Example: target_link_libraries(myprogram ${LIBRARY_NAME})

# Specify the build type (optional, e.g., Release or Debug)
# set(CMAKE_BUILD_TYPE Release)
