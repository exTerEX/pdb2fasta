cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(
  pdb2fasta
  LANGUAGES C
  VERSION 0.1.0)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(CMAKE_STATIC_LIBRARY_SUFFIX .so)
  set(CMAKE_EXECUTABLE_SUFFIX)

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
else()
  set(CMAKE_STATIC_LIBRARY_SUFFIX .debug.so)
  set(CMAKE_EXECUTABLE_SUFFIX .debug)
endif()

add_subdirectory(src)

add_subdirectory(example)
