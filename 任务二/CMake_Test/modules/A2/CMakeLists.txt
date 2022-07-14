aux_source_directory(src MODULE_DIRS2)

add_library(A2 SHARED ${MODULE_DIRS2})

target_include_directories(A2 PUBLIC include)
