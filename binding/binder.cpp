#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../ReadCSV.h"

namespace py = pybind11;

PYBIND11_MODULE(file_organization, m)
{
    py::class_<Record<int>>(m, "AnimeRecord")
        .def(py::init<>())
        .def_readwrite("anime_id", &Record<int>::anime_id)
        .def_readwrite("episodes", &Record<int>::episodes)
        .def_readwrite("key", &Record<int>::key)
        .def_readwrite("members", &Record<int>::members)
        .def_readwrite("left", &Record<int>::left)
        .def_readwrite("right", &Record<int>::right)
        .def_readwrite("height", &Record<int>::height)
        .def_readwrite("next_del", &Record<int>::next_del)
        .def("getName", &Record<int>::getName)
        .def("getGenre", &Record<int>::getGenre)
        .def("getType", &Record<int>::getType)
        .def("setName", &Record<int>::setName)
        .def("setGenre", &Record<int>::setGenre)
        .def("setType", &Record<int>::setType)
        .def("show", &Record<int>::show)
        .def("showAVL", &Record<int>::showAVL);
    // .def("__eq__", py::overload_cast<const Record<int> &>(&Record<int>::operator==, py::const_))
    // .def("__eq__", py::overload_cast<const int>(&Record<int>::operator==, py::const_))
    // .def("__lt__", py::overload_cast<const Record<int> &>(&Record<int>::operator<, py::const_))
    // .def("__lt__", py::overload_cast<const int>(&Record<int>::operator<, py::const_))
    // .def("__gt__", py::overload_cast<const Record<int> &>(&Record<int>::operator>, py::const_))
    // .def("__gt__", py::overload_cast<const int>(&Record<int>::operator>, py::const_))
    // .def("__str__", [](const Record<int> &record)
    //      {
    //     std::ostringstream oss;
    //     oss << record;
    //     return oss.str(); });
    py::class_<Record<const char *>>(m, "AppRecord")
        .def(py::init<>())
        .def_readwrite("rating", &Record<const char *>::rating)
        .def_readwrite("rating_count", &Record<const char *>::rating_count)
        .def_readwrite("minimum_installs", &Record<const char *>::minimum_installs)
        .def_readwrite("maximum_installs", &Record<const char *>::maximum_installs)
        .def_readwrite("free", &Record<const char *>::free)
        .def_readwrite("price", &Record<const char *>::price)
        .def("getApp_name", &Record<const char *>::getApp_name)
        .def("getApp_id", &Record<const char *>::getApp_id)
        .def("getCategory", &Record<const char *>::getCategory)
        .def("getInstalls", &Record<const char *>::getInstalls)
        .def("getCurrency", &Record<const char *>::getCurrency)
        .def("getSize", &Record<const char *>::getSize)
        .def("setApp_name", &Record<const char *>::setApp_name)
        .def("setApp_id", &Record<const char *>::setApp_id)
        .def("setCategory", &Record<const char *>::setCategory)
        .def("setInstalls", &Record<const char *>::setInstalls)
        .def("setCurrency", &Record<const char *>::setCurrency)
        .def("setSize", &Record<const char *>::setSize)
        .def("show", &Record<const char *>::show);
}