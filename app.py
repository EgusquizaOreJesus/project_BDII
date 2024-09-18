from flask import Flask, request, jsonify
from flask_cors import CORS  # Importa CORS
import kf_cpp  # Importa el módulo C++
app = Flask(__name__)
CORS(app)  # Habilita CORS para todas las rutas


# Ruta para sumar dos números
@app.route("/suma", methods=["POST"])
def suma():
    data = request.json
    p = kf_cpp.Point(1, 2)
    a = int(data["a"])
    b = int(data["b"])
    result = p.add(a,b)  # Llamada a la función C++ 'suma'
    return jsonify({"resultado": result})


# Ruta para imprimir una variable
@app.route("/print_variable", methods=["POST"])
def print_variable():
    data = request.json
    var = data["var"]
    kf_cpp.print_variable(var)  # Llamada a la función C++ 'print_variable'
    return jsonify({"message": f'Variable "{var}" enviada a C++ para impresión'})


if __name__ == "__main__":
    app.run(debug=True)
