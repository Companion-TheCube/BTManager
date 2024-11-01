cmake_minimum_required(VERSION 3.25)
project(BT_Manager VERSION 1.0.0 LANGUAGES CXX)
add_definitions(-DMAJOR_VERSION=${PROJECT_VERSION_MAJOR})
add_definitions(-DMINOR_VERSION=${PROJECT_VERSION_MINOR})
add_definitions(-DPATCH_VERSION=${PROJECT_VERSION_PATCH})
add_definitions(-DPATCH_VERSION=${PROJECT_VERSION_PATCH})
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
if(${PROJECT_VERSION_TWEAK})
    add_definitions(-DPROJECT_VERSION_STRING="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")
else()
    add_definitions(-DPROJECT_VERSION_STRING="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(WIN32)
    set(CMAKE_PREFIX_PATH C:/Qt6/6.8.0/mingw_64)
else()
    set(CMAKE_PREFIX_PATH /home/andrew/Qt/6.8.0/gcc_64)
endif()
find_package(Qt6 REQUIRED COMPONENTS Bluetooth Network Core)

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
add_executable(BT_Manager ${SOURCES})
target_link_libraries(BT_Manager PRIVATE 
    nlohmann_json::nlohmann_json
    Qt6::Bluetooth
    Qt6::Network
    Qt6::Core
)
target_compile_features(BT_Manager PRIVATE cxx_std_23)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(BT_Manager PRIVATE DEBUG)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(BT_Manager PRIVATE RELEASE)
endif()
target_include_directories(BT_Manager PRIVATE ${CMAKE_SOURCE_DIR}/src/libs)
if(WIN32)
    target_compile_options(BT_Manager PRIVATE "/Zc:__cplusplus") 
endif()
# Define the path to the Qt bin directory
set(QT_BIN_DIR "C:/Qt6/6.8.0/mingw_64/bin")

# Define the list of DLLs to copy
set(DLL_FILES
    "libgcc_s_seh-1.dll"
    "libstdc++-6.dll"
    "libwinpthread-1.dll"
    "Qt6Core.dll"
)

if(WIN32)
    foreach(DLL ${DLL_FILES})
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_BIN_DIR}/${DLL}" "${CMAKE_BINARY_DIR}/bin/Debug"
            COMMENT "Copying ${DLL} to ${CMAKE_BINARY_DIR}/bin/Debug"
        )
    endforeach()
endif()