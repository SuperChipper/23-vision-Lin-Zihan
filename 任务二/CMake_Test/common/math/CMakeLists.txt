aux_source_directory(src MATH_DIRS)

find_package(OpenCV REQUIRED)

add_library(Math SHARED ${MATH_DIRS})

target_link_libraries(Math ${OpenCV_LIBS})

target_include_directories(Math PUBLIC include ${OpenCV_INCLUDE_DIRS})
