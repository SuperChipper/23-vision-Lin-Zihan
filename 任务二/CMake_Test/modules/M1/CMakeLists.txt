aux_source_directory(src MODULE_DIRS3)

add_library(M1 SHARED ${MODULE_DIRS3})

target_include_directories(M1 PUBLIC include)

target_link_libraries(M1 A1)
