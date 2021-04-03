add_executable(main main.c)
set_target_properties(main PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(main ${PROJECT_NAME})
