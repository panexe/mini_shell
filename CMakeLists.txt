cmake_minimum_required(VERSION 3.16)
project(mini_shell)

set(CMAKE_CXX_STANDARD 14)

add_executable(mini_shell main.cpp MiniShell.cpp MiniShell.h ShellKeyboard.cpp ShellKeyboard.h)