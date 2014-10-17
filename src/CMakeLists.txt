cmake_minimum_required (VERSION 2.6)

project (AlleleCounter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ALLELE_COUNTER_SOURCE
  main.cpp
  AlleleCountManager.cpp
  AlleleCountFile.cpp
)

add_executable(AlleleCounter ${ALLELE_COUNTER_SOURCE})
