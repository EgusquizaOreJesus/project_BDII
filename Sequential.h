#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "RecordSeq.h"
using namespace std;

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
    ifstream mfile(main_file, ios::binary);
    ifstream ifile(insert_file, ios::binary);

    ifstream check(new_file, ios::binary);
    if (!check)
    {
      ofstream create(new_file, ios::binary);
      create.close();
    }

    fstream nfile(new_file, ios::in | ios::out | ios::binary);

    // Solo existe el archivo main y no hay archivo de inserts.
    if (!ifile)
    {

      int nsize = 0;
      nfile.write((char *)&nsize, sizeof(int));

      Record main_record;

      int msize;
      mfile.read((char *)&msize, sizeof(int));

      for (int i = 0; i < msize; i++)
      {
        mfile.read((char *)&main_record, sizeof(Record));
        nfile.seekp(0, ios::end);
        nfile.write((char *)&main_record, sizeof(Record));
        nsize++;
      }

      nfile.seekp(0, ios::beg);
      nfile.write((char *)&nsize, sizeof(int));

      mfile.close();
      nfile.close();

      remove((char *)&main_file);
      rename((char *)&new_file, (char *)&main_file);
      return;
    }

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
      if (new_records[ptr].is_removed == true)
        ptr++;

      if (main_record.is_removed == true)
        mfile.read((char *)&main_record, sizeof(Record));

      if (new_records[ptr].is_removed == false && main_record.is_removed == false)
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
    }

    // Actualizar la cantidad de registros en el nuevo archivo
    nfile.seekp(0, ios::beg);
    nfile.write((char *)&nsize, sizeof(int));

    // Cerrar los archivos
    ifile.close();
    mfile.close();
    nfile.close();

    // Eliminar archivos extra y solo nos quedamos con el nuevo archivo.
    remove((char *)&main_file);
    remove((char *)&insert_file);
    rename((char *)&new_file, (char *)&main_file);
  }

  int pos_search(int key)
  {
    ifstream file(main_file, ios::binary);
    if (!file)
      throw runtime_error("Error en search");

    int low = 0;
    int high;
    int mid;
    int pos;

    file.read((char *)&high, sizeof(int));
    high = high - 1;

    Record record;

    while (low <= high)
    {
      mid = (high + low) / 2;
      pos = mid * sizeof(Record) + sizeof(int);

      file.seekg(pos, ios::beg);
      file.read((char *)&record, sizeof(Record));

      if (record.key == key)
      {
        file.close();
        return pos;
      }

      else if (record.key < key)
        low = mid + 1;

      else
        high = mid - 1;
    }

    file.close();
    return -1;
  }

  Record search(int key)
  {
    // Primero se busca en el archivo principal
    int pos = pos_search(key);

    // Si la pos encontrada es diferente a -1
    // Esta en el archivo principal
    if (pos != -1)
    {
      Record record = read_record(pos);
      return record;
    }

    // Si no esta en el archivo principal,
    // Buscar en el archivo de los nuevos inserts
    else
    {
      ifstream file(insert_file, ios::binary);

      // Si el archivo no existe, no se encontro elemento buscado
      if (!file)
      {
        cout << "No se encontro insert file" << endl;
        return Record();
      }

      // Buscar elementos en el archivo de inserts
      Record record;
      int isize;
      file.read((char *)&isize, sizeof(int));
      for (int i = 0; i < isize; i++)
      {
        file.read((char *)&record, sizeof(Record));
        if (record.key == key)
        {
          file.close();
          return record;
        }
      }
      file.close();
    }
    cout << "No se encontro elemento en ningun archivo" << endl;
    return Record();
  }

  bool remove_record(int key)
  {
    // Buscar elemento en el archivo de main
    int pos = pos_search(key);

    if (pos != -1)
    {
      fstream file(main_file, ios::in | ios::out | ios::binary);
      Record record = read_record(pos);
      record.is_removed = true;
      write_record(record, pos);
      file.close();
      rebuild();
      return true;
    }

    // Buscar elemento en el archivo de inserts
    else
    {
      fstream file(insert_file, ios::in | ios::out | ios::binary);
      if (!file)
      {
        cout << "No se encontro insert file" << endl;
        return false;
      }

      Record record;
      int isize;
      file.read((char *)&isize, sizeof(int));
      for (int i = 0; i < isize; i++)
      {
        file.read((char *)&record, sizeof(Record));
        if (record.key == key)
        {
          record.is_removed = true;
          file.seekp(i * sizeof(Record) + sizeof(int), ios::beg);
          file.write((char *)&record, sizeof(Record));
          file.close();
          return true;
        }
      }
      file.close();
    }
    return false;
  }

  vector<Record> range_search(int low_key, int high_key)
  {
    vector<Record> result;
    ifstream mfile(main_file, ios::binary);
    ifstream ifile(insert_file, ios::binary);

    // Realizar búsqueda binaria en el archivo principal
    if (mfile)
    {
      int msize;
      mfile.read((char *)&msize, sizeof(int));

      int low = 0;
      int high = msize - 1;
      int mid;
      int pos;
      Record record;
      bool found = false;

      while (low <= high)
      {
        mid = (low + high) / 2;
        pos = mid * sizeof(Record) + sizeof(int);
        mfile.seekg(pos, ios::beg);
        mfile.read((char *)&record, sizeof(Record));

        if (record.key >= low_key)
        {
          high = mid - 1;
          found = true;
        }
        else
        {
          low = mid + 1;
        }
      }

      // Si encontramos al menos un elemento >= low_key, low quedará en la posición deseada
      if (found)
      {
        pos = low * sizeof(Record) + sizeof(int);
        mfile.seekg(pos, ios::beg);

        // Leer registros secuencialmente desde `low` hasta `high_key`
        while (mfile.read((char *)&record, sizeof(Record)) && record.key <= high_key)
        {
          if (!record.is_removed) // Si no está eliminado, lo añadimos al resultado
          {
            result.push_back(record);
          }
        }
      }

      mfile.close();
    }

    // Buscar en el archivo de inserts
    if (ifile)
    {
      int isize;
      ifile.read((char *)&isize, sizeof(int));
      Record record;

      for (int i = 0; i < isize; ++i)
      {
        ifile.read((char *)&record, sizeof(Record));
        if (!record.is_removed && record.key >= low_key && record.key <= high_key)
        {
          result.push_back(record);
        }
      }
      ifile.close();
    }

    sort(result.begin(), result.end(), compare_key);

    return result;
  }

  Record read_record(int pos)
  {
    Record record;
    ifstream file(main_file, ios::binary);
    file.seekg(pos, ios::beg);
    file.read((char *)&record, sizeof(Record));
    file.close();
    return record;
  }

  void write_record(Record record, int pos)
  {
    fstream file(main_file, ios::in | ios::out | ios::binary);
    file.seekp(pos, ios::beg);
    file.write((char *)&record, sizeof(Record));
    file.close();
  }

  void print_mfile()
  {
    ifstream file(main_file, ios::binary);
    Record record;
    int size;
    file.read((char *)&size, sizeof(int));
    while (file.read((char *)&record, sizeof(Record)))
    {
      if (record.is_removed == false)
        record.show();
    }

    cout << "size: " << size << endl;
    file.close();
  }

  void print_ifile()
  {
    ifstream file(insert_file, ios::binary);
    Record record;
    int size;
    file.read((char *)&size, sizeof(int));
    while (file.read((char *)&record, sizeof(Record)))
    {
      if (record.is_removed == false)
        record.show();
    }

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
  file.print_mfile();
}

void show_insertfile_test()
{
  Sequential file("insert.bin");
  file.print_ifile();
}

void search_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  Record record = file.search(700);
  record.show();
}

void search_insertfile_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  file.insert(950);
  Record record = file.search(950);
  record.show();
}

void rangesearch_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  vector<Record> records = file.range_search(400, 1000);
  for (auto record : records)
    record.show();
}

void rebuild_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  file.insert(150);
  file.insert(350);
  file.insert(950);
  file.print_mfile();
}

void remove_test()
{
  Sequential file("main.bin");
  file.buildCSV("test.csv");
  file.remove_record(900);
  file.print_mfile();
}

int main()
{
  build_test();
  show_all_test();
  show_insertfile_test();
  search_test();
  search_insertfile_test();
  rangesearch_test();
  rebuild_test();
  remove_test();
  return 0;
}
