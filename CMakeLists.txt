cmake_minimum_required(VERSION 3.13)
project(farm2)

set(CMAKE_CXX_STANDARD 14)

add_executable(farm2 library2.cpp DataCenterManager.h library2.h AVLtree.h UpTree.h DataCenterManager.cpp HashTable.h main.cpp)