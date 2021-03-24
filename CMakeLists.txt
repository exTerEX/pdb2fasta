cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  pdb2fasta
  LANGUAGES C
  VERSION 0.1.0)

set(CMAKE_STATIC_LIBRARY_SUFFIX .so)

include_directories(include)

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

add_executable(main example/main.c)
set_target_properties(main PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(main pdb2fasta)
