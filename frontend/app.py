from flask import Flask, render_template, redirect, request, jsonify
from flask_cors import CORS  # Importa CORS

import file_organization as fo
import re

app = Flask(__name__)
CORS(app)
structure_identifier = None

records = []
current_table = "playstore"
num_records = len(records)
structure = ""
valid_structures = {"Playstore": None, "Youtube": None}
last_query = "CREATE TABLE Youtube FROM FILE 'data_youtube.csv' USING INDEX AVL('');"
structure_dict = {
    "Playstore": {
        "AVL": "avlFilePlaystore",
        "EXTENDIBLE": "extendibleFilePlaystore",
        "SEQUENTIAL": "sequentialFilePlaystore",
    },
    "Youtube": {
        "AVL": "avlFileYoutube",
        "EXTENDIBLE": "extendibleFileYoutube",
        "SEQUENTIAL": "sequentialFileYoutube",
    },
}
# tables = YouTube, Playstore


@app.route("/", methods=["GET", "POST"])
def index():
    global last_query
    global structure
    global records
    global current_table
    global num_records
    global valid_structures
    print("Index")
    return render_template(
        "index.html",
        applications=records,
        table=current_table,
        rows=num_records,
        last_query=last_query,
    )


@app.route("/query_create", methods=["POST"])
def procesar_consulta():
    global structure_identifier
    global last_query
    global current_table
    print("Procesando consulta...")
    data = request.json
    scanner = data["query"]  # Usa la consulta enviada desde el HTML
    match = re.match(r"^\w+", scanner)
    if match and match.group(0) == "CREATE":
        match = re.search(r"TABLE\s+(\w+).*INDEX\s+(\w+)", scanner)
        if match and match.group(1) == "Youtube" and match.group(2) == "AVL":
            current_table = "youtube"
            print("Youtube")
            structure_identifier = "avlFileYoutube"
        elif match and match.group(1) == "Playstore" and match.group(2) == "AVL":
            current_table = "playstore"
            print("Playstore")
            structure_identifier = "avlFilePlaystore"
        elif match and match.group(1) == "Youtube" and match.group(2) == "EXTENDIBLE":
            current_table = "youtube"
            structure_identifier = "extendibleFileYoutube"
        elif match and match.group(1) == "Playstore" and match.group(2) == "EXTENDIBLE":
            current_table = "playstore"
            structure_identifier = "extendibleFilePlaystore"
        elif match and match.group(1) == "Youtube" and match.group(2) == "SEQUENTIAL":
            current_table = "youtube"
            structure_identifier = "sequentialFileYoutube"
        elif match and match.group(1) == "Playstore" and match.group(2) == "SEQUENTIAL":
            current_table = "playstore"
            structure_identifier = "sequentialFilePlaystore"
        scanner = fo.Scanner(scanner)

        if current_table == "youtube":
            parser = fo.ParserYT(scanner, structure_identifier)
        else:
            parser = fo.ParserPS(scanner, structure_identifier)
        try:
            parser.parse()
            print("Parseo exitoso!")
            return jsonify({"resultado": "Creación exitosa a tabla: " + current_table + " con estructura: " + structure_identifier})
        except Exception as e:
            print(f"Error durante la creacion: {e}")


@app.route("/query_select", methods=["POST"])
def procesar_consulta_select():
    global structure_identifier
    global records
    global last_query
    global current_table
    print("Procesando consulta...")

    data = request.json
    scanner = data["query"]  # Usa la consulta enviada desde el HTML
    last_query = scanner
    match = re.search(r"FROM\s+(\w+)", scanner)
    scanner = fo.Scanner(scanner)
    if match and match.group(1) == "Youtube":
        current_table = "youtube"
        print("Youtube")
        parser = fo.ParserYT(scanner, structure_identifier)

    elif match and match.group(1) == "Playstore":
        current_table = "playstore"
        parser = fo.ParserPS(scanner, structure_identifier)

    try:
        print("Parseando...")
        parser.parse()
        print("Parseo exitoso!")
        Records = parser.get_records()
        print("Registros:")
        print(Records)
        temp = {}
        records = []
        if current_table == "playstore":
            for record in Records:
                temp = {
                    "key": record.getKey(),
                    "app_name": record.getApp_name(),
                    "category": record.getCategory(),
                    "rating": record.rating,
                    "rating_count": record.rating_count,
                    "installs": record.getInstalls(),
                    "minimum_installs": record.minimum_installs,
                    "maximum_installs": record.maximum_installs,
                    "free": record.free,
                    "price": record.price,
                    "currency": record.getCurrency(),
                    "size": record.getSize(),
                }
                records.append(temp)
        elif current_table == "youtube":
            for record in Records:
                temp = {
                    "key": record.getKey(),
                    "title": record.getTitle(),
                    "channel_title": record.getChannel_title(),
                    "views": record.views,
                    "likes": record.likes,
                    "dislikes": record.dislikes,
                    "comment_count": record.comment_count,
                }
                records.append(temp)

        print("Parseo exitoso!")
        print(records)
        # crear un json con los registros
        return jsonify(
            {
                "resultado": "Se encontró en la BD: "
                + str(len(records))
                + " registros",
                "records": records,
                "table": current_table,
            }
        )
    except Exception as e:
        print(f"Error durante el parseo: {e}")


@app.route("/set_table", methods=["POST"])
def set_table():
    global structure_identifier
    data = request.json
    query = data["query"]
    match = re.search(r"TABLE\s+(\w+).*INDEX\s+(\w+)", query)
    if match and match.group(1) == "Youtube" and match.group(2) == "AVL":
        print("Youtube")
        structure_identifier = "avlFileYoutube"
    elif match and match.group(1) == "Playstore" and match.group(2) == "AVL":
        print("Playstore")
        structure_identifier = "avlFilePlaystore"
    elif match and match.group(1) == "Youtube" and match.group(2) == "EXTENDIBLE":
        structure_identifier = "extendibleFileYoutube"
    elif match and match.group(1) == "Playstore" and match.group(2) == "EXTENDIBLE":
        structure_identifier = "extendibleFilePlaystore"
    elif match and match.group(1) == "Youtube" and match.group(2) == "SEQUENTIAL":
        structure_identifier = "sequentialFileYoutube"
    elif match and match.group(1) == "Playstore" and match.group(2) == "SEQUENTIAL":
        structure_identifier = "sequentialFilePlaystore"
    return jsonify({"resultado": "cambio de tabla exitoso: " + structure_identifier})


@app.route("/query_insert", methods=["POST"])
def procesar_consulta_insert():
    global structure_identifier
    global last_query
    print("Procesando consulta...")

    data = request.json
    scanner = data["query"]
    last_query = scanner
    match = re.search(r"INTO\s+(\w+)", scanner)
    scanner = fo.Scanner(scanner)
    if match and match.group(1) == "Youtube":
        parser = fo.ParserYT(scanner, structure_identifier)
    elif match and match.group(1) == "Playstore":
        parser = fo.ParserPS(scanner, structure_identifier)

    try:
        print("Parseando...")
        parser.parse()
        print("Parseo exitoso!")
        return jsonify({"resultado": "Inserción exitosa"})
    except Exception as e:
        print(f"Error durante el parseo: {e}")
        return jsonify({"resultado": "Error durante la inserción"})


@app.route("/query_delete", methods=["POST"])
def procesar_consulta_delete():
    global structure_identifier
    global last_query
    print("Procesando consulta...")

    data = request.json
    scanner = data["query"]
    last_query = scanner
    match = re.search(r"FROM\s+(\w+)", scanner)
    scanner = fo.Scanner(scanner)
    if match and match.group(1) == "Youtube":
        parser = fo.ParserYT(scanner, structure_identifier)
    elif match and match.group(1) == "Playstore":
        parser = fo.ParserPS(scanner, structure_identifier)

    try:
        print("Parseando...")
        parser.parse()
        print("Parseo exitoso!")
        return jsonify({"resultado": "Eliminación exitosa"})
    except Exception as e:
        print(f"Error durante el parseo: {e}")
        return jsonify({"resultado": "Error durante la eliminación"})


if __name__ == "__main__":
    app.run(debug=True)
