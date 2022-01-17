set(HEADER_LIST "${pdb2fasta_SOURCE_DIR}/include/modern/lib.hpp")

add_library(${PROJECT_NAME} pdb2fasta.c codon3to1.c)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)

# All users of this library will need at least C99
target_compile_features(${PROJECT_NAME} PUBLIC c_std_99)
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES LINKER_LANGUAGE C
             SOVERSION ${PROJECT_VERSION_MAJOR}
             PREFIX lib
             C_STANDARD 99)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
)
