cmake_minimum_required(VERSION 3.22)

project(YourProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Ensure the BOOST_ROOT environment variable is used
set(BOOST_ROOT $ENV{BOOST_ROOT})
set(CMAKE_PREFIX_PATH ${BOOST_ROOT})

# Set Boost include and library directories
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/include)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib)

# Find Boost
find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem)

set(CLIENT_DIR desktop/client)
set(SERVER_DIR desktop/server)
set(NETWORKING_DIR desktop/networking)

include_directories(${NETWORKING_DIR}/include)

# Define sources
set(CLIENT_SOURCES ${CLIENT_DIR}/main.cpp)
set(SERVER_SOURCES ${SERVER_DIR}/main.cpp)
set(NETWORKING_SOURCES ${NETWORKING_DIR}/src/tcp_server.cpp ${NETWORKING_DIR}/src/tcp_connection.cpp)

# Create a library from networking sources
add_library(networking_lib STATIC ${NETWORKING_SOURCES})

# Link Boost to the networking library
target_link_libraries(networking_lib PRIVATE Boost::system Boost::filesystem)

# Create executables
add_executable(client ${CLIENT_SOURCES})
target_link_libraries(client PRIVATE Boost::system Boost::filesystem networking_lib)
target_include_directories(client PUBLIC shared ${CLIENT_DIR}/include)

add_executable(server ${SERVER_SOURCES})
target_link_libraries(server PRIVATE Boost::system Boost::filesystem networking_lib)
target_include_directories(server PUBLIC shared ${SERVER_DIR}/include)






