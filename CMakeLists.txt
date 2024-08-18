cmake_minimum_required(VERSION 3.20.2)

project(dishanveshi)

include_directories(include)

include_directories(src/common/image/bitmap)

add_compile_options(-m32)

#file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.c)
#file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/common/*/*.c)
#file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/common/*.c)

#add_library(dsvi ${SRC_FILES})

#add_executable(convolution_test src/convolution_test.c)
#target_link_libraries(convolution_test dsvi)

# CMAKE add flags -lm -lpthread -ldl -Os
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm -lpthread -ldl -Os")

include_directories(${PROJECT_SOURCE_DIR}/extrlibs/webui/include)
link_directories(${PROJECT_SOURCE_DIR}/extrlibs/webui/dist)

add_executable(ui_practice src/ui/ui_practice.c)
target_link_libraries(ui_practice webui-2-static pthread m dl)

# add framework Cocoa and WebKit
find_library(COCOA_FRAMEWORK Cocoa)
find_library(WEBKIT_FRAMEWORK WebKit)

#target_link_libraries(ui_practice ${COCOA_FRAMEWORK} ${WEBKIT_FRAMEWORK})
if(APPLE)
target_link_libraries(ui_practice "-framework Cocoa")
target_link_libraries(ui_practice "-framework WebKit")
endif()

target_compile_features(ui_practice PRIVATE cxx_std_17)

add_compile_options(-m32)


