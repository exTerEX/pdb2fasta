add_executable(cli cli.c)
set_target_properties(cli PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(cli ${PROJECT_NAME})
