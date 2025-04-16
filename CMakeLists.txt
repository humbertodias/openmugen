cmake_minimum_required(VERSION 3.27)
set(PROJECT OpenMugen)
project(${PROJECT})

# Add global definitions
add_definitions("-Wall")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(${PROJECT} ${SOURCE_FILES})
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 98)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED 98)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}
CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif ()

# Ensure compiler compatibility
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)
    message(FATAL_ERROR "Require at least clang++-3.4")
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)
    message(FATAL_ERROR "Require at least g++-5.1")
endif ()

# Find SDL12_Compat, SDL2_image, and SDL2_gfx
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL12 REQUIRED sdl12_compat)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)

# Print debug info
message("SDL12_INCLUDE_DIRS: ${SDL12_INCLUDE_DIRS}")
message("SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS}")
message("SDL2_GFX_INCLUDE_DIRS: ${SDL2_GFX_INCLUDE_DIRS}")

message("SDL12_LIBRARIES: ${SDL12_LIBRARIES}")
message("SDL2_IMAGE_LIBRARIES: ${SDL2_IMAGE_LIBRARIES}")
message("SDL2_GFX_LIBRARIES: ${SDL2_GFX_LIBRARIES}")

message("SDL12_LIBRARY_DIRS: ${SDL12_LIBRARY_DIRS}")
message("SDL2_IMAGE_LIBRARY_DIRS: ${SDL2_IMAGE_LIBRARY_DIRS}")
message("SDL2_GFX_LIBRARY_DIRS: ${SDL2_GFX_LIBRARY_DIRS}")

# Include directories
include_directories(${SDL12_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS})
link_directories(${SDL12_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS} ${SDL2_GFX_LIBRARY_DIRS})

# Link libraries
target_link_libraries(${PROJECT} -L${SDL12_LIBRARY_DIRS} -L${SDL2_IMAGE_LIBRARY_DIRS} ${SDL2_GFX_LIBRARY_DIRS} -lSDL -lSDL2_image -lSDL2_gfx)

# macOS-specific settings
if(APPLE)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(APPKIT_FRAMEWORK AppKit)
    target_link_libraries(${PROJECT} ${COREFOUNDATION_FRAMEWORK} ${APPKIT_FRAMEWORK} -lSDLMain)
endif()


