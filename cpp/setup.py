from setuptools import setup, Extension
import pybind11

# Define the extension module
ext_modules = [
    Extension(
        "kf_cpp",
        ["wrappers.cpp"],  # Agrega todos los archivos .cpp necesarios
        include_dirs=[pybind11.get_include(), "./include"],
        language="c++",
    ),
]

# Configuración del paquete
setup(
    name="kf_cpp",
    version="0.1",
    author="Your Name",
    description="Ejemplo de pybind11 con C++",
    ext_modules=ext_modules,
    zip_safe=False,
)
