#include <pybind11/pybind11.h>
#include <iostream>
#include "./include/suma.h" // Asegúrate de que esta línea apunte correctamente a tu archivo de encabezado


namespace py = pybind11;
// Función que recibe una variable y la imprime
void print_variable(std::string var)
{
    std::cout << "La variable recibida es: " << var << std::endl;
}

struct Point
{
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
    Point() : x(0), y(0) {}

    int add(int a, int b)
    {
        return a + b;
    }
};



// Enlace con Python
PYBIND11_MODULE(kf_cpp, m)
{
    m.doc() = "Módulo de Python escrito en C++";                                  // Documentación del módulo
    m.def("suma", &add, "Función que suma dos números");                          // Función suma
    m.def("print_variable", &print_variable, "Función que imprime una variable"); // Función print_variable
}
