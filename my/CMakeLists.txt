# Minimum CMake version required
cmake_minimum_required(VERSION 3.12)

project(my_project VERSION 1.0)

# Add 'add' program
# add_executable(add add.cpp)

# # Add 'subtract' program
# add_executable(subtract subtract.cpp)

# Add 'operations' file
add_executable(operations operations.cpp)

# Find SEAL library
find_package(SEAL 4.1 REQUIRED)

# Link SEAL to both executables
# target_link_libraries(add SEAL::seal)
# target_link_libraries(subtract SEAL::seal)
target_link_libraries(operations SEAL::seal)

# Use C++17 for both programs
# target_compile_features(add PRIVATE cxx_std_17)
# target_compile_features(subtract PRIVATE cxx_std_17)
target_compile_features(operations PRIVATE cxx_std_17)
