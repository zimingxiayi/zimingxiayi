cmake_minimum_required(VERSION 3.9)
project(thread_pool)

set(CMAKE_CXX_STANDARD 11)

add_executable(thread_pool main.c thread_cond.c thread_mutex.c thread_pool.c)

target_link_libraries(thread_pool pthread)
