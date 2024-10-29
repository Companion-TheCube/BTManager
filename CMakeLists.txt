cmake_minimum_required(VERSION 3.25)
project(CubeBT_Manager VERSION 1.0 LANGUAGES CXX)
add_definitions(-DMAJOR_VERSION=${PROJECT_VERSION_MAJOR})
add_definitions(-DMINOR_VERSION=${PROJECT_VERSION_MINOR})
add_definitions(-DPATCH_VERSION=${PROJECT_VERSION_PATCH})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(Qt6 REQUIRED COMPONENTS Bluetooth)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/libraries)
set(HTTPLIB_HEADER ${CMAKE_SOURCE_DIR}/libraries/httplib.h)
if(NOT EXISTS ${HTTPLIB_HEADER})
    file(DOWNLOAD
        https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
        ${HTTPLIB_HEADER}
    )
endif()
set(ARGPARSE_HEADER ${CMAKE_SOURCE_DIR}/libraries/argparse.hpp)
if(NOT EXISTS ${ARGPARSE_HEADER})
    file(DOWNLOAD
        https://raw.githubusercontent.com/p-ranav/argparse/master/include/argparse/argparse.hpp
        ${ARGPARSE_HEADER}
    )
endif()
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/logger)
include_directories(${CMAKE_SOURCE_DIR}/libraries)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp" "src/**/**/*.cpp")
add_executable(CubeBT_Manager ${SOURCES})
target_link_libraries(CubeBT_Manager PRIVATE 
    nlohmann_json::nlohmann_json
    Qt6::Bluetooth
)
target_compile_features(CubeBT_Manager PRIVATE cxx_std_23)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(CubeBT_Manager PRIVATE DEBUG)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(CubeBT_Manager PRIVATE RELEASE)
endif()
target_include_directories(CubeBT_Manager PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)
