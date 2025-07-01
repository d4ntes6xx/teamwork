cmake_minimum_required(VERSION 3.14)
project(tictactoe_network)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найдём SFML
find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics)

# Найдём Boost (для Boost.Beast и Asio)
find_package(Boost REQUIRED COMPONENTS system)

# Соберём исходники
add_executable(server
    server_main.cpp
    game.hpp
    network.hpp
)

add_executable(client
    client_main.cpp
    game.hpp
    network.hpp
)

# Подключим SFML и Boost к каждому
target_link_libraries(server PRIVATE sfml-graphics sfml-window sfml-system Boost::system pthread)
target_link_libraries(client PRIVATE sfml-graphics sfml-window sfml-system Boost::system pthread)

# Если Boost не даёт Boost::system — попробуем через include path
if(NOT TARGET Boost::system)
    target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(server PRIVATE ${Boost_LIBRARIES})
    target_include_directories(client PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(client PRIVATE ${Boost_LIBRARIES})
endif()
