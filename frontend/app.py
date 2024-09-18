from flask import Flask, render_template, redirect, request
from utils import * 

app = Flask(__name__)


people = [Person("Sebastian", "Chu", "CS"),
          Person("Paolo", "Medrano", "DS"),
          Person("Jesus", "Egusquiza", "CS"),
          Person("Fabricio", "Chavez", "CS")]


apps = read_csv("playstore_mini.csv")
current_table = "playstore"
num_rows = len(apps)

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        print(request.form["queries"])
        return redirect('/')
    else:
        return render_template('index.html', 
                               applications = apps, 
                               table = current_table,
                               rows = num_rows)

if __name__ == "__main__":
    app.run(debug = True)