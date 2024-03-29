cmake_minimum_required(VERSION 3.27)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 20)

add_executable(ThreadPool main.cpp
        SimpleThreadPool.cpp
        SimpleThreadPool.h)
