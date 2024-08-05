cmake_minimum_required(VERSION 3.20.2)

project(dishanveshi)

include_directories(include)

include_directories(src/common/image/bitmap)

add_compile_options(-m32)

#file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/common/*/*.c)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/common/*.c)

add_library(dsvi ${SRC_FILES})

add_executable(convolution_test src/convolution_test.c)
target_link_libraries(convolution_test dsvi)

add_compile_options(-m32)


