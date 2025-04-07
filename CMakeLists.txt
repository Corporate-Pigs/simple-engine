cmake_minimum_required(VERSION 3.26.4)

# Define the Engine library
project(simple-engine)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the source files
file(GLOB_RECURSE ENGINE_SOURCES "source/*.cpp")

# Add the Engine library target
add_library(simple-engine STATIC ${ENGINE_SOURCES})

# Add dependencies
add_subdirectory(vendor/SDL2)
add_subdirectory(vendor/SDL2_image)
# add_subdirectory(vendor/cppvec)

# Specify include directories for the Engine library
target_include_directories(simple-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Specify private include directories for the Engine library
target_include_directories(simple-engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Link libraries to Engine
#target_link_libraries(simple-engine PUBLIC SDL2::SDL2 SDL2_image cppvec)
target_link_libraries(simple-engine PUBLIC SDL2::SDL2 SDL2_image)
