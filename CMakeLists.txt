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
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
endif()

add_library(${PROJECT_NAME} STATIC src/pdb2fasta.c src/codon3to1.c)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES LINKER_LANGUAGE C
             SOVERSION ${CMAKE_MAJOR_VERSION}
             PREFIX lib
             C_STANDARD 99)

add_subdirectory(example)
