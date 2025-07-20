# Minimum CMake version required
cmake_minimum_required(VERSION 3.12)

project(add VERSION 1.0)

add_executable(add add.cpp)


find_package(SEAL 4.1 REQUIRED)

target_link_libraries(add SEAL::seal)

target_compile_features(add PRIVATE cxx_std_17)
