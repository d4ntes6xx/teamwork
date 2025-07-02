cmake_minimum_required(VERSION 3.14)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_ROOT "D:/SFM")
set(BOOST_ROOT "D:/boost")

# SFML
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")
find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics)

# Boost
set(Boost_NO_SYSTEM_PATHS ON)
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
find_package(Boost REQUIRED COMPONENTS system)

add_executable(server
    server.cpp
    game.hpp
    network.hpp
)

add_executable(client
    client.cpp
    game.hpp
    network.hpp
)

target_link_libraries(server PRIVATE sfml-graphics sfml-window sfml-system Boost::system pthread)
target_link_libraries(client PRIVATE sfml-graphics sfml-window sfml-system Boost::system pthread)

if(NOT TARGET Boost::system)
    target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(server PRIVATE ${Boost_LIBRARIES})
    target_include_directories(client PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(client PRIVATE ${Boost_LIBRARIES})
endif()
