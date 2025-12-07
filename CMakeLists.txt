cmake_minimum_required(VERSION 3.15)
project(pdb2fasta VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Options
option(BUILD_CLI "Build command-line interface" ON)
option(BUILD_PYTHON "Build Python bindings" OFF)

# Library sources
set(LIB_SOURCES
    src/parser.cpp
    src/converter.cpp
)

# Create the core library
add_library(pdb2fasta_core STATIC ${LIB_SOURCES})
target_include_directories(pdb2fasta_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Command-line interface
if(BUILD_CLI)
    add_executable(pdb2fasta src/main.cpp)
    target_link_libraries(pdb2fasta PRIVATE pdb2fasta_core)
    
    install(TARGETS pdb2fasta
        RUNTIME DESTINATION bin
    )
endif()

# Python bindings
if(BUILD_PYTHON)
    find_package(pybind11 CONFIG REQUIRED)
    
    pybind11_add_module(_pdb2fasta src/bindings.cpp)
    target_link_libraries(_pdb2fasta PRIVATE pdb2fasta_core)
    
    install(TARGETS _pdb2fasta
        LIBRARY DESTINATION .
    )
endif()

# Install headers
install(DIRECTORY include/
    DESTINATION include
)

# Install library
install(TARGETS pdb2fasta_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
