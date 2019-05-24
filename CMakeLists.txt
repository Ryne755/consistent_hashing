# CMakeList.txt: consistent_hashing 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.3)

if(MSVC)
message("BUILD IN MSVC...")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
elseif("${CMAKE_SYSTEM}" MATCHES "Linux")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")
message("BUILD IN linux...")
else(UNIX)
message("BUILD IN unix...")
#TODO
endif()

if(CMAKE_CL_64)
ADD_DEFINITIONS(-DBITS_PER_LONG=64)
else()
ADD_DEFINITIONS(-DBITS_PER_LONG=32)
endif()
# 将源代码添加到此项目的可执行文件。
add_executable (consistent_hashing "consistent_hashing.h" "test.cc")

# TODO: 如有需要，请添加测试并安装目标。
