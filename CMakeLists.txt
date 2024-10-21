cmake_minimum_required(VERSION 3.19)
project(main)

# C++23
set(CMAKE_CXX_STANDARD 23)

# Collect all source files in src/
file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES})

# Enable parallel compilation on Windows
if(WIN32)
  target_compile_options(main PRIVATE "/MP")
endif()
