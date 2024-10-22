cmake_minimum_required(VERSION 3.19)
project(main)

# C++23
set(CMAKE_CXX_STANDARD 23)

# Collect all source files in src/
file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES})

# PostgreSQL
find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(main PRIVATE libpqxx::pqxx)

# Enable parallel compilation on Windows
if(WIN32)
  target_compile_options(main PRIVATE "/MP")
endif()
