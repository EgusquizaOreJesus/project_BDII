#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

struct Record
{
  int key;

  Record() : key(0) {}
  Record(int _key) : key(_key) {}

  bool fromCSV(const string &line)
  {
    try
    {
      key = stol(line); // Convertir la línea a long
      return true;
    }
    catch (const invalid_argument &e)
    {
      cerr << "Error: No se pudo convertir la línea a número: " << line << endl;
      return false;
    }
    catch (const out_of_range &e)
    {
      cerr << "Error: El número está fuera del rango permitido en la línea: " << line << endl;
      return false;
    }
  }

  void show()
  {
    cout << key << endl;
  }
};

class Sequential
{
private:
  string main_file;
  string insert_file;

  static bool compare_key(const Record &a, const Record &b)
  {
    return a.key < b.key;
  }

public:
  Sequential() {}
  Sequential(string _main) : main_file(_main), insert_file("insert.bin")
  {
    ifstream file(main_file, ios::binary);
    if (!file)
    {
      // Crear file
      ofstream mfile(main_file, ios::binary);

      // Escribir cantidad de registros por archivo
      int size = 0;
      mfile.write((char *)&size, sizeof(int));
      mfile.close();
    }
    // En caso existiera
    file.close();
  }

  int get_size(ifstream &file)
  {
    int size;
    file.seekg(0, ios::beg);
    file.read((char *)&size, sizeof(int));
    return size;
  }

  void buildCSV(string csv_file)
  {
    fstream file(main_file, ios::in | ios::out | ios::binary); // Se construye el archivo principal
    ifstream csv(csv_file);                                    // Archivo CSV
    string line;

    // Al construir la cantidad de registros es 0
    int size = 0;

    while (getline(csv, line))
    {
      Record record;

      if (record.fromCSV(line)) // Convertir la línea CSV en un objeto Record
      {
        file.seekp(0, ios::end);
        file.write((char *)&record, sizeof(Record));
        size++;
      }
    }

    file.seekp(0, ios::beg);
    file.write((char *)&size, sizeof(int));

    csv.close();
    file.close();
  }

  void insert(Record record)
  {
    // Crear el archivo ifile si aun no lo esta
    ifstream file(insert_file, ios::binary);
    if (!file)
    {
      ofstream ifile(insert_file, ios::binary);
      int size = 0;
      ifile.write((char *)&size, sizeof(int));
      ifile.close();
    }
    file.close();

    // Escribir nuevo registro
    fstream ifile(insert_file, ios::in | ios::out | ios::binary);
    ifile.seekp(0, ios::end);
    ifile.write((char *)&record, sizeof(Record));

    // Leer size de nuevo registro
    int isize;
    ifile.seekg(0, ios::beg);
    ifile.read((char *)&isize, sizeof(int));

    // Actualizar size
    isize++;
    ifile.seekp(0, ios::beg);
    ifile.write((char *)&isize, sizeof(int));
    ifile.close();

    // Obtener el size del archivo main
    ifstream mfile(main_file, ios::binary);
    int msize;
    mfile.read((char *)&msize, sizeof(int));

    // Se reconstruye apenas llega al limite de elementos
    if (isize == floor(log2(msize)))
      rebuild();
  }

  void rebuild()
  {
    string new_file = "new.bin";
    ifstream ifile(insert_file, ios::binary);
    ifstream mfile(main_file, ios::binary);

    ifstream check(new_file, ios::binary);
    if (!check)
    {
      ofstream create(new_file, ios::binary);
      create.close();
    }

    fstream nfile(new_file, ios::in | ios::out | ios::binary);

    // Traer registros del archivo de los nuevos inserts a RAM
    int isize;
    ifile.read((char *)&isize, sizeof(int));
  
    vector<Record> new_records;
    for (int i = 0; i < isize; i++)
    {
      Record record;
      ifile.read((char *)&record, sizeof(Record));
      new_records.push_back(record);
    }

    // Ordenar registros en RAM
    sort(new_records.begin(), new_records.end(), compare_key);

    // Escribir size del nuevo archivo
    int nsize = 0;
    nfile.write((char *)&nsize, sizeof(int));

    // Leer registros del main file
    Record main_record;
    int msize;
    mfile.read((char *)&msize, sizeof(int));
    mfile.read((char *)&main_record, sizeof(Record));

    // Iterar por el main file y el insert file hasta terminar el merge
    int ptr = 0;
    while (!mfile.eof())
    {
      if (ptr < isize)
      {
        if (new_records[ptr].key < main_record.key)
        {
          nfile.seekp(0, ios::end);
          nfile.write((char *)&new_records[ptr], sizeof(Record));
          nsize++;

          // avanzamos el ptr del los new_records
          ptr++;
        }

        else if (new_records[ptr].key > main_record.key)
        {
          nfile.seekp(0, ios::end);
          nfile.write((char *)&main_record, sizeof(Record));
          nsize++;

          // Avanzamos la posicion del main file leyendo el siguiente record
          mfile.read((char *)&main_record, sizeof(Record));
        }

        // No se permiten keys repetidas, entonces solo guardamos la del main file
        else if (new_records[ptr].key == main_record.key)
        {
          nfile.seekp(0, ios::end);
          nfile.write((char *)&main_record, sizeof(Record));
          nsize++;

          // Avanzamos ambos ptrs
          mfile.read((char *)&main_record, sizeof(Record));
          ptr++;
        }
      }

      // En caso se acaben los new_records, se escriben los records restantes del main file
      else
      {
        nfile.write((char *)&main_record, sizeof(Record));
        nsize++;

        // Avanzamos la posicion del main file leyendo el siguiente record
        mfile.read((char *)&main_record, sizeof(Record));
      }
    }

    // Actualizar la cantidad de registros en el nuevo archivo
    nfile.seekp(0, ios::beg);
    nfile.write((char*)&nsize, sizeof(int));

    // Cerrar los archivos
    ifile.close();
    mfile.close();
    nfile.close();

    // Eliminar archivos extra y solo nos quedamos con el nuevo archivo. 
    remove((char *)&main_file);
    remove((char *)&insert_file);
    rename((char *)&new_file, (char *)&main_file);
  }

  Record search(int key)
  {
    ifstream file(main_file, ios::binary);
    if (!file)
      throw runtime_error("Error en search");

    int low = 0;
    int high;
    int mid;

    file.read((char *)&high, sizeof(int));
    high = high - 1;

    Record record;

    while (low <= high)
    {
      mid = (high + low) / 2;

      file.seekg(mid * sizeof(Record), ios::beg);
      file.read((char *)&record, sizeof(Record));

      if (record.key == key)
      {
        file.close();
        return record;
      }

      else if (record.key < key)
        low = mid + 1;

      else
        high = mid - 1;
    }

    cout << "Record not found" << endl;
    file.close();
    return Record();
  }

  void print_all()
  {
    ifstream file(main_file, ios::binary);
    Record record;
    int size;
    file.read((char *)&size, sizeof(int));
    while (file.read((char *)&record, sizeof(Record)))
      record.show();

    cout << "size: " << size << endl;
    file.close();
  }
};

void build_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
}

void show_all_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  file.print_all();
}

void search_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  Record record = file.search(700);
  record.show();
}

void rebuild_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  file.insert(150);
  file.insert(950);
  file.insert(550);
  file.print_all();
}

int main()
{
  build_test();
  show_all_test();
  search_test();
  rebuild_test();

  return 0;
}
