cmake_minimum_required(VERSION 3.13)
project(kahoot_server)

set(CMAKE_CXX_STANDARD 14)

add_executable(kahoot_server main.cpp src/Server.cpp src/Server.h src/Client.cpp src/Client.h src/Kahoot.cpp src/Kahoot.h)