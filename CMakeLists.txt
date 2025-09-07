cmake_minimum_required(VERSION 4.0)
project(SimpleCLIGame)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

add_executable(SimpleCLIGame main.cpp
        main.h
        Player.cpp
        Player.h
        Enemy.cpp
        Enemy.h
        json.hpp
        Items.cpp
        Items.h
        Inventory.cpp
        Inventory.h)

target_link_libraries(SimpleCLIGame PRIVATE -static-libgcc -static-libstdc++ -static)
