cmake_minimum_required(VERSION 3.10)

# Set the project name
project(SharedMemoryServer VERSION 0.1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set up directory structure:
set(COMMON_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/common)
set(SERVER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/server)
set(CLIENT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/client)

# Define new binary output directory:
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# Add the executable
add_executable(server ${COMMON_SRC_DIR}/shared_memory.cpp
                      ${SERVER_SRC_DIR}/handlers.cpp
                      ${SERVER_SRC_DIR}/server.cpp)
add_executable(client ${CLIENT_SRC_DIR}/client.cpp)

# Add common include directory
target_include_directories(server PUBLIC ${COMMON_SRC_DIR})
