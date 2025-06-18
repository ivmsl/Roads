cmake_minimum_required(VERSION 3.10)
project(CityBuilder VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RAYLIB_PATH "${CMAKE_SOURCE_DIR}/src/Imports/raylib-5.5_macos")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(RAYLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/Imports/raylib-5.5_macos/include")
set(RAYLIB_LIB_DIR "${CMAKE_SOURCE_DIR}/src/Imports/raylib-5.5_macos/lib")

# Find raylib library
find_library(RAYLIB_LIBRARY
    NAMES raylib libraylib
    PATHS ${RAYLIB_LIB_DIR}
    REQUIRED
)

include_directories(${RAYLIB_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/Game)

file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/Game/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Game/**/*.cpp"
)

file(GLOB_RECURSE HEADERS     
    "${CMAKE_SOURCE_DIR}/src/Game/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/Game/**/*.hpp"
)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARY})


if(APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "-framework CoreVideo"
        "-framework IOKit"
        "-framework Cocoa"
        "-framework GLUT"
        "-framework OpenGL"
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

file(COPY ${CMAKE_SOURCE_DIR}/resources 
     DESTINATION ${CMAKE_BINARY_DIR}/bin)

message(STATUS "CMAKE SOURCE DIR ${CMAKE_SOURCE_DIR}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Raylib library: ${RAYLIB_LIBRARY}")
message(STATUS "Source files found: ${SOURCES}")
message(STATUS "Header files found: ${HEADERS}")
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}/bin")