#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../parserSQL.h"
#include "../tokenS.cpp"

namespace py = pybind11;

PYBIND11_MODULE(file_organization, m)
{
    py::class_<Scanner>(m, "Scanner")
        .def(py::init<const char*>());
        // .def_readonly("input", &Scanner::input); verificar que se guarda el input

    // const char * ->  YT
    py::class_<Parser<const char*>>(m, "ParserYT")
        .def(py::init<Scanner*, const char*>())
        .def("parse", &Parser<const char*>::parse);

    // int -> APP
    py::class_<Parser<int>>(m, "ParserAPP")
        .def(py::init<Scanner*, const char*>())
        .def("parse", &Parser<int>::parse);

    py::class_<Record<const char*>>(m, "VideoRecord")
        .def(py::init<>())
        .def_readwrite("views", &Record<const char *>::views)
        .def_readwrite("likes", &Record<const char *>::likes)
        .def_readwrite("dislikes", &Record<const char *>::dislikes)
        .def_readwrite("comment_count", &Record<const char *>::comment_count)
        .def("show", &Record<const char*>::show)
        .def("getKey", &Record<const char*>::getKey)
        .def("getTitle", &Record<const char*>::getTitle)
        .def("getChannel_title", &Record<const char*>::getChannel_title)
        .def("setKey", &Record<const char*>::setKey)
        .def("setTitle", &Record<const char*>::setTitle)
        .def("setChannel_title", &Record<const char*>::setChannel_title);

    py::class_<Record<int>>(m, "AppRecord")
        .def(py::init<>())
        .def_readwrite("rating", &Record<int>::rating)
        .def_readwrite("rating_count", &Record<int>::rating_count)
        .def_readwrite("minimum_installs", &Record<int>::minimum_installs)
        .def_readwrite("maximum_installs", &Record<int>::maximum_installs)
        .def_readwrite("free", &Record<int>::free)
        .def_readwrite("price", &Record<int>::price)
        .def("show", &Record<int>::show)
        .def("getApp_name", &Record<int>::getApp_name)
        .def("getKey", &Record<int>::getKey)
        .def("getCategory", &Record<int>::getCategory)
        .def("getInstalls", &Record<int>::getInstalls)
        .def("getCurrency", &Record<int>::getCurrency)
        .def("getSize", &Record<int>::getSize)
        .def("setApp_name", &Record<int>::setApp_name)
        .def("setKey", &Record<int>::setKey)
        .def("setCategory", &Record<int>::setCategory)
        .def("setInstalls", &Record<int>::setInstalls)
        .def("setCurrency", &Record<int>::setCurrency)
        .def("setSize", &Record<int>::setSize);
}