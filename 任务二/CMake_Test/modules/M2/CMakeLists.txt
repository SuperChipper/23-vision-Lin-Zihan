project(M2)

aux_source_directory(src MODULE_DIRS3)

add_library(M2 SHARED ${MODULE_DIRS3} )

target_include_directories(M2 PUBLIC include ${Common_INCLUDE_DIRS} ${kalman_INCLUDE_DIRS})

target_link_libraries(M2 ${OpenCV_LIBS} A1 A2)
