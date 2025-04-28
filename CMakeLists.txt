cmake_minimum_required(VERSION 3.15)
project(LingosBypass)

set(CMAKE_BUILD Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -Wall -Werror -std=c++17")
set(source_dir "${PROJECT_SOURCE_DIR}/src/")
set(project_dir "${CMAKE_CURRENT_SOURCE_DIR}/")
file(GLOB source_files "${source_dir}/*.cpp")

set(FullOutputDir "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullOutputDir}/static libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullOutputDir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")

add_executable(LingosBypass ${source_files})

find_package(GnuTLS REQUIRED)
find_package(CURL REQUIRED)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin") # macOS
    message(STATUS "Configuring for macOS")
    target_link_libraries(LingosBypass
        GnuTLS::GnuTLS
        CURL::libcurl)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux") # Linux
    message(STATUS "Configuring for Linux")
    target_link_libraries(LingosBypass 
        CURL::libcurl)
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()



target_include_directories(LingosBypass PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)