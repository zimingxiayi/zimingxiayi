cmake_minimum_required( VERSION 3.10.2 )

project(jet_queue)

SET( CMAKE_CXX_COMPILER "gcc")

add_executable(jet_queue jet_queue_test.c jet_queue.c)
