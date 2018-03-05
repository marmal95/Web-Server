# C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -Wall -Wextra -Werror")
set(CMAKE_BUILD_TYPE "Release")

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Find Boost
find_package(Boost 1.66.0 REQUIRED COMPONENTS system thread)

# Include Directories
include_directories(include)
include_directories(${Boost_INCLUDE_DIR})

# Link Directories
link_directories(${Boost_LIBRARY_DIRS})

# Add files
file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE SOURCES "source/*.cpp")

# Create executable
add_library(Server_Lib ${HEADERS} ${SOURCES})
add_executable(Server_Project ${HEADERS} ${SOURCES} main.cpp)

# Link libraries
target_link_libraries(Server_Project ${Boost_LIBRARIES})
