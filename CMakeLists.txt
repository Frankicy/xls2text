cmake_minimum_required(VERSION 3.10)
project(xls2text)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 头文件 / 库
include_directories(/opt/libxls/include)
link_directories   (/opt/libxls/lib)

include_directories(${CMAKE_SOURCE_DIR})   # 找到 xls_wrapper.h

add_executable(xls2text
    xls_wrapper.c
    xls2text.cpp)

target_link_libraries(xls2text PRIVATE xlsreader)
target_link_options(xls2text PRIVATE "-Wl,-rpath,/opt/libxls/lib")