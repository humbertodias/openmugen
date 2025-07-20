cmake_minimum_required(VERSION 3.27)
project(OpenMugen)

# Detect if building with Emscripten
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
    set(USING_EMSCRIPTEN TRUE)
endif()

# Add global definitions
add_definitions("-Wall")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 98)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 98)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message("USING_EMSCRIPTEN: ${USING_EMSCRIPTEN}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-g -O0 -Wall -Wextra -Wpedantic")
    if(NOT USING_EMSCRIPTEN)
        add_definitions("-fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()
endif()

# Compiler version requirements
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)
    message(FATAL_ERROR "Require at least clang++-3.4")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.1)
    message(FATAL_ERROR "Require at least g++-5.1")
endif()

# target copy_data
add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "${CMAKE_BINARY_DIR}/data"
        COMMENT "Copying data directory to build folder"
)
# always copy data folder
add_dependencies(${PROJECT_NAME} copy_data)

if(EMSCRIPTEN)

    # SDL2 is built-in with Emscripten: -sUSE_SDL=2
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='[\"png\",\"bmp\"]' -sFULL_ES3=1 -sASYNCIFY -sALLOW_MEMORY_GROWTH=1 --preload-file data")
    # Define explicit output for the HTML file
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")

# Dependencies (only for native builds)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)

    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS})
    link_directories(${SDL2_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS} ${SDL2_GFX_LIBRARY_DIRS})

    target_link_libraries(${PROJECT_NAME}
        -L${SDL2_LIBRARY_DIRS}
        -L${SDL2_IMAGE_LIBRARY_DIRS}
        -L${SDL2_GFX_LIBRARY_DIRS}
        -lSDL2
        -lSDL2_image
        -lSDL2_gfx
    )

    # macOS-specific settings
    if(APPLE)
        find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
        find_library(APPKIT_FRAMEWORK AppKit)
        target_link_libraries(${PROJECT_NAME} ${COREFOUNDATION_FRAMEWORK} ${APPKIT_FRAMEWORK} -lSDL2)
    endif()

endif()
