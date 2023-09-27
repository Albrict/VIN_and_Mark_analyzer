cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(vin_database)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")

# Our Project
include(src/src.cmake)

add_executable(${PROJECT_NAME} ${db_source})
