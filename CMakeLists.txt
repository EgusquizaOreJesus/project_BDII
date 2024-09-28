cmake_minimum_required(VERSION 3.23)
project(Projecto1)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(proyecto1_1bd mainPrueb.cpp parserSQL.h tokenS.h tokenS.cpp estructuras/FileStructureBase.h
        estructuras/avlFile.h estructuras/ExtendibleHashing.h estructuras/Sequential.h
        estructuras/ReadCSV.h)

add_executable(BenchMark benchmark/time_test.cpp
        estructuras/FileStructureBase.h
        estructuras/avlFile.h
        estructuras/ExtendibleHashing.h
        estructuras/Sequential.h
        estructuras/ReadCSV.h)

