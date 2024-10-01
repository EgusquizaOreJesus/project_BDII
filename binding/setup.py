from setuptools import setup, Extension
import pybind11
from pybind11.setup_helpers import Pybind11Extension, build_ext

# Define el módulo de extensión
ext_modules = [
    Pybind11Extension(
        "file_organization",  # nombre del módulo
        ["binder.cpp"],  # archivos de código fuente
        include_dirs=[pybind11.get_include()],  # incluye las cabeceras de Pybind11
        language="c++",  # lenguaje de compilación
    ),
]

# Configuración del paquete
setup(
    name="file_organization",  # nombre del paquete
    version="0.1",
    description="Un módulo de organización de archivos con Pybind11",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},  # usa el comando de compilación de Pybind11
)
