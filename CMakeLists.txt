cmake_minimum_required(VERSION 3.23)
project(Projecto1)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(Projecto1 mainPrueb.cpp parserSQL.h tokenS.h tokenS.cpp estructuras/avlFile.h estructuras/ExtendibleHashing.h
                estructuras/FileStructureBase.h estructuras/ReadCSV.h)

