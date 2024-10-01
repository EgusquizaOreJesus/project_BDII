#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../parserSQL.h"
#include "../tokenS.cpp"
#define _CRT_SECURE_NO_WARNINGS

namespace py = pybind11;

PYBIND11_MODULE(file_organization, m)
{
    py::class_<Scanner>(m, "Scanner")
        .def(py::init<const char *>()); // constructor

    // const char* -> DATA YOUTUBE
    py::class_<Parser<const char *>>(m, "ParserYT")
        .def(py::init<Scanner *, const char *>())               // bind the constructor with Scanner* and const char*
        .def("parse", &Parser<const char *>::parse)             // bind the parse method
        .def("get_records", &Parser<const char *>::getRecords); // Método getRecords, usa el vector de Record<const char*>

    py::class_<Record<const char *>>(m, "RecordYT")
        .def(py::init<>())
        .def_readwrite("views", &Record<const char *>::views)
        .def_readwrite("likes", &Record<const char *>::likes)
        .def_readwrite("dislikes", &Record<const char *>::dislikes)
        .def_readwrite("comment_count", &Record<const char *>::comment_count)
        .def("getKey", &Record<const char *>::getKey)
        .def("getKey", &Record<const char *>::getKey)
        .def("getTitle", &Record<const char *>::getTitle)
        .def("getChannel_title", &Record<const char *>::getChannel_title)
        .def("show", &Record<const char *>::show);

    // int -> DATA PLAYSTORE
    py::class_<Parser<int>>(m, "ParserPS")
        .def(py::init<Scanner *, const char *>())
        .def("parse", &Parser<int>::parse)
        .def("get_records", &Parser<int>::getRecords);

    py::class_<Record<int>>(m, "RecordPS")
        .def(py::init<>())
        .def("getKey", &Record<int>::getKey)
        .def("getApp_name", &Record<int>::getApp_name)
        .def("getCategory", &Record<int>::getCategory)
        .def("getInstalls", &Record<int>::getInstalls)
        .def("getCurrency", &Record<int>::getCurrency)
        .def("getSize", &Record<int>::getSize)
        .def("show", &Record<int>::show)
        .def_readwrite("rating", &Record<int>::rating)
        .def_readwrite("rating_count", &Record<int>::rating_count)
        .def_readwrite("minimum_installs", &Record<int>::minimum_installs)
        .def_readwrite("maximum_installs", &Record<int>::maximum_installs)
        .def_readwrite("free", &Record<int>::free)
        .def_readwrite("price", &Record<int>::price);
}