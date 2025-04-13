cmake_minimum_required(VERSION 3.26.4)

add_library(stb_image source/stb_image.cpp)

target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)