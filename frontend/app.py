from flask import Flask, render_template, redirect, request
from utils import * 
import file_organization as fo
import re

app = Flask(__name__)


records = []
current_table = "playstore"
num_records = len(records)
structure = ""
valid_structures = {
    "Playstore":None,
    "Youtube":None
}
last_query = "CREATE TABLE Youtube FROM FILE 'data_youtube.csv' USING INDEX EXTENDIBLE('anime_id');"
structure_dict = {
    "Playstore": {
        "AVL":"avlFilePlaystore",
        "EXTENDIBLE":"extendibleFilePlaystore",
        "SEQUENTIAL":"sequentialFilePlaystore"
    },
    "Youtube": {
        "AVL":"avlFileYoutube",
        "EXTENDIBLE":"extendibleFileYoutube",
        "SEQUENTIAL":"sequentialFileYoutube"
    }
}
# tables = YouTube, Playstore

@app.route('/', methods=['GET', 'POST'])
def index():
    global last_query
    global structure
    global records
    global current_table
    global num_records
    global valid_structures

    def getIndex(query):
        match = re.search(r'INDEX\s+([^\(]+)\(', query)
        if match:
            return match.group(1).strip()
        return None
    
    if request.method == 'POST':
        queries = request.form["queries"]
        last_query = queries
        queries = queries.replace("\r", "").replace("\n", "")
        queries = queries.split(";")
        queries = [q.strip() + ";" for q in queries]
        if queries and len(queries[-1]) < 5:
            queries.pop()
        print(queries)

        # ITERAR CADA QUERY Y POR CADA UNA INSTANCIAR UN SCANNER & PARSER
        for query in queries:
            scanner = fo.Scanner(query)
            if "Youtube" in query:
                if ("CREATE" in query):
                    index = getIndex(query)
                    valid_structures["Youtube"] = structure_dict["Youtube"][index]
                print("structure: ", structure)

                structure = valid_structures["Youtube"]
                parser = fo.ParserYT(scanner, structure)
                parser.parse() # se ejecuta parse para cualquier query, SELECT, INSERT, ETC.

                if ("SELECT" in query):
                    current_table = "youtube"
                    records = parser.getRecords() # acceder al atributo inter
                    num_records = len(records)
                    for record in records:
                        record.show()
                
            elif "Playstore" in query:
                if ("CREATE" in query):
                    index = getIndex(query)
                    valid_structures["Playstore"] = structure_dict["Playstore"][index]
                print("structure: ", structure)

                structure = valid_structures["Playstore"]
                parser = fo.ParserAPP(scanner, structure)
                parser.parse()
                
                if ("SELECT" in query):
                    current_table = "playstore"
                    #acceder al vector dentro del parser
                    records = parser.getRecords()
                    num_records = len(records)
                    for record in records:
                        record.show()

        return redirect('/')
    else:
        return render_template('index.html', 
                               applications = records, 
                               table = current_table,
                               rows = num_records,
                               last_query=last_query)

if __name__ == "__main__":
    app.run(debug = True)