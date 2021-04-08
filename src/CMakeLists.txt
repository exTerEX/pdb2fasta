add_library(${PROJECT_NAME} STATIC pdb2fasta.c codon3to1.c)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES LINKER_LANGUAGE C
             SOVERSION ${PROJECT_VERSION_MAJOR}
             PREFIX lib
             C_STANDARD 99)
