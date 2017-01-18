cmake_minimum_required(VERSION 3.6)
project(CLI)

set(CMAKE_CXX_STANDARD 11)
add_custom_target(CLI COMMAND make -C . CLION_EXE_DIR=.)