cmake_minimum_required(VERSION 3.23)
project(Projecto1)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(Projecto1 ExtendibleHashing.h ReadCSV.h avlFile.h parser/mainPrueb.cpp parser/parserSQL.cpp parser/parserSQL.h parser/tokenS.cpp parser/tokenS.h parser/mainPrueb.cpp)


