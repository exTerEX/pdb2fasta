add_executable(cli cli.c)
set_target_properties(cli PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(cli PRIVATE ${PROJECT_NAME})
target_compile_features(cli PRIVATE c_std_99)
