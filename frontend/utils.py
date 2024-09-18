# modelos de ayuda
import csv

class Person:
    name : str
    last_name : str
    career : str
    def __init__(self, n, l, c):
        self.name = n
        self.last_name = l
        self.career = c

class Application:
    def __init__(self, app_name, app_id, category, rating, rating_count, installs, 
                 minimum_installs, maximum_installs, free, price, currency, size):
        self.app_name = app_name
        self.app_id = app_id
        self.category = category
        self.rating = rating
        self.rating_count = rating_count
        self.installs = installs
        self.minimum_installs = minimum_installs
        self.maximum_installs = maximum_installs
        self.free = free
        self.price = price
        self.currency = currency
        self.size = size


# función separadora de sentencias por ;
def separate_queries(input : str):
    queries = [query.strip() for query in input.split(';')]
    return queries

def read_csv(filename: str):
    applications = []
    
    with open(filename, mode='r', encoding='utf-8') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            print(row)
            app = Application(
                app_name=row[0],
                app_id=row[1],
                category=row[2],
                rating=row[3],
                rating_count=float(row[4]),
                installs=row[5],
                minimum_installs=row[6],
                maximum_installs=row[7],
                free=row[8].lower() == 'true',
                price=row[9],
                currency=row[10],
                size=row[11],
            )
            applications.append(app)
    
    return applications