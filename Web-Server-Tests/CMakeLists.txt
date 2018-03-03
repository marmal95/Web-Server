# C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra -Werror")

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Find Boost
find_package(Boost 1.66.0 REQUIRED COMPONENTS system thread)
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

# Include Directories
include_directories(../Web-Server/include)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

# Link Directories
link_directories(${Boost_LIBRARY_DIRS})

# Add files
file(GLOB_RECURSE HEADERS "*.hpp")
file(GLOB_RECURSE SOURCES "*.cpp")

# Create executable
add_executable(Server_Project_Tests ${HEADERS} ${SOURCES})

# Link libraries
target_link_libraries(Server_Project_Tests Server_Lib)
target_link_libraries(Server_Project_Tests ${Boost_LIBRARIES})
target_link_libraries(Server_Project_Tests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(Server_Project_Tests ${GMOCK_BOTH_LIBRARIES})