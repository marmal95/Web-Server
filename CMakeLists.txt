cmake_minimum_required(VERSION 3.9)

# Solution Name
project(Server_Project)

# C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Find Boost
find_package(Boost 1.66.0 REQUIRED COMPONENTS system thread)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Include Directories
include_directories(include)
include_directories(${Boost_INCLUDE_DIR})

# Link Directories
link_directories(${Boost_LIBRARY_DIRS})

# Add files
file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE SOURCES "source/*.cpp")

# Create executable
add_executable(Server_Project ${HEADERS} ${SOURCES})

# Link libraries
target_link_libraries(Server_Project ${Boost_LIBRARIES})