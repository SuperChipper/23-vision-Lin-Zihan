aux_source_directory(src MODULE_DIRS1)

add_library(A1 SHARED ${MODULE_DIRS1})

target_include_directories(A1 PUBLIC include)
