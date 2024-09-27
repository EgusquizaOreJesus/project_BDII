cmake_minimum_required(VERSION 3.25)
project(proyecto1_1bd)

set(CMAKE_CXX_STANDARD 20)

add_executable(proyecto1_1bd mainPrueb.cpp parserSQL.h tokenS.h tokenS.cpp estructuras/FileStructureBase.h estructuras/avlFile.h estructuras/ExtendibleHashing.h  estructuras/ReadCSV.h)
add_executable(borrador borrador.cpp estructuras/avlFile.h)