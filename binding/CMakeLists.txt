cmake_minimum_required(VERSION 3.22)
project(file_organization)

add_subdirectory(pybind11)
pybind11_add_module(file_organization binder.cpp)