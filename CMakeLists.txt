cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.cc src/*.h src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(shell ${SOURCE_FILES})

find_library(READLINE_LIB NAMES readline libreadline.so.8)
if(READLINE_LIB)
  target_link_libraries(shell PRIVATE ${READLINE_LIB})
endif()
