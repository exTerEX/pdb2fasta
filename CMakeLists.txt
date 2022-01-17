# Works with 3.9 and tested through 3.16
cmake_minimum_required(VERSION 3.9...3.16 FATAL_ERROR)

if(${CMAKE_VERSION} VERSION_LESS 3.22)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
  cmake_policy(VERSION 3.22)
endif()

# Project name and a few useful settings
project(
  pdb2fasta
  DESCRIPTION "A simple library and tool to convert PDB files to FASTA files."
  HOMEPAGE_URL "https://github.com/exTerEX/pdb2fasta"
  LANGUAGES C
  VERSION 0.1.0
)

# Only if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_C_EXTENSIONS OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  include(CTest)

  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_C_CLANG_TIDY clang-tidy -checks=-*,readability-*)
  endif()

  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

# The compiled library code
add_subdirectory(src)

# The executable CLI tool code
add_subdirectory(extras)

# The executable example code
add_subdirectory(example)
