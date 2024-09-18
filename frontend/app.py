from flask import Flask, render_template, redirect, request

app = Flask(__name__)

class Person:
    name : str
    last_name : str
    career : str
    def __init__(self, n, l, c):
        self.name = n
        self.last_name = l
        self.career = c

people = [Person("Sebastian", "Chu", "CS"),
          Person("Paolo", "Medrano", "DS"),
          Person("Jesus", "Egusquiza", "CS"),
          Person("Fabricio", "Chavez", "CS")]

def separate_queries(input : str):
    queries = [query.strip() for query in input.split(';')]
    return queries


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        print(request.form["queries"])
        return redirect('/')
    else:
        return render_template('index.html', people = people)

if __name__ == "__main__":
    app.run(debug = True)