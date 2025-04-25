cmake_minimum_required(VERSION 3.26.4)

# Define the Engine library
project(simple-engine)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define asset path
set(SE_ASSET_PATH ${CMAKE_CURRENT_SOURCE_DIR}/assets)

set(SE_BACKEND "SDL")

if(SE_BACKEND STREQUAL "SDL")
    # Add SDL dependencies
    set(BUILD_SHARED_LIBS OFF)
    set(SDL2TTF_VENDORED ON)
    set(SDL2MIXER_VENDORED ON)
    add_subdirectory(vendor/SDL2)
    add_subdirectory(vendor/SDL2_ttf)
    add_subdirectory(vendor/SDL2_image)
    add_subdirectory(vendor/SDL2_mixer)

    set(LINKED_LIBRARIES SDL2::SDL2-static SDL2::SDL2main SDL2_ttf::SDL2_ttf-static SDL2_image::SDL2_image-static SDL2_mixer::SDL2_mixer-static)
    set(INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL)
    file(GLOB_RECURSE SOURCE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/source/SDL/*.cpp)
    set(COMPILER_DEFINITIONS SE_SDL_BACKEND=1)

elseif(SE_BACKEND STREQUAL "SMT")
    # Add SMT dependencies
    add_subdirectory(vendor/SMT)

    set(LINKED_LIBRARIES smt)
    set(INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include/SMT)
    file(GLOB_RECURSE SOURCE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/source/SMT/*.cpp)
    set(COMPILER_DEFINITIONS SE_SMT_BACKEND=1)
    
endif()

# Add other dependencies
add_subdirectory(vendor/cppvec)

# Define the source files
file(GLOB_RECURSE ENGINE_SOURCES source/SE/*.cpp)

# Add the Engine library target
add_library(simple-engine STATIC ${ENGINE_SOURCES} ${SOURCE_DIRECTORIES})

# Specify include directories for the Engine library
target_include_directories(simple-engine PUBLIC ${INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/include/SE)

# Specify private include directories for the Engine library
target_include_directories(simple-engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Link libraries to Engine
target_link_libraries(simple-engine PUBLIC ${LINKED_LIBRARIES} cppvec)

# Compiler definitions
target_compile_definitions(simple-engine PUBLIC ${COMPILER_DEFINITIONS})