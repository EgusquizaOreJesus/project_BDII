#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "ReadCSV.h"
using namespace std;

template <typename TK>
class Sequential
{
private:
  string main_file;
  string insert_file;

  static bool compare_key(const Record<TK> &a, const Record<TK> &b)
  {
    return strcmp(a.key, b.key) < 0;
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
    // Obtener CSV y ordenarlo
    vector<Record<TK>> records = readCSV_youtube(csv_file);
    sort(records.begin(), records.end(), compare_key);

    // Se construye el archivo principal
    fstream file(main_file, ios::in | ios::out | ios::binary);

    // Al construir la cantidad de registros es 0
    int size = 0;

    for (int i = 0; i < records.size(); i++)
    {
      file.seekp(0, ios::end);
      file.write((char *)&records[i], sizeof(Record<TK>));
      size++;
    }

    file.seekp(0, ios::beg);
    file.write((char *)&size, sizeof(int));

    file.close();
  }

  void insert(Record<TK> record)
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
    ifile.write((char *)&record, sizeof(Record<TK>));

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

      Record<TK> main_record;

      int msize;
      mfile.read((char *)&msize, sizeof(int));

      for (int i = 0; i < msize; i++)
      {
        mfile.read((char *)&main_record, sizeof(Record<TK>));
        nfile.seekp(0, ios::end);
        nfile.write((char *)&main_record, sizeof(Record<TK>));
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

    vector<Record<TK>> new_records;
    for (int i = 0; i < isize; i++)
    {
      Record<TK> record;
      ifile.read((char *)&record, sizeof(Record<TK>));
      new_records.push_back(record);
    }

    // Ordenar registros en RAM
    sort(new_records.begin(), new_records.end(), compare_key);

    // Escribir size del nuevo archivo
    int nsize = 0;
    nfile.write((char *)&nsize, sizeof(int));

    // Leer registros del main file
    Record<TK> main_record;
    int msize;
    mfile.read((char *)&msize, sizeof(int));
    mfile.read((char *)&main_record, sizeof(Record<TK>));

    // Iterar por el main file y el insert file hasta terminar el merge
    int ptr = 0;
    while (!mfile.eof())
    {
      if (new_records[ptr].is_removed == true)
        ptr++;

      if (main_record.is_removed == true)
        mfile.read((char *)&main_record, sizeof(Record<TK>));

      if (new_records[ptr].is_removed == false && main_record.is_removed == false)
      {
        if (ptr < isize)
        {
          if (new_records[ptr].key < main_record.key)
          {
            nfile.seekp(0, ios::end);
            nfile.write((char *)&new_records[ptr], sizeof(Record<TK>));
            nsize++;

            // avanzamos el ptr del los new_records
            ptr++;
          }

          else if (new_records[ptr].key > main_record.key)
          {
            nfile.seekp(0, ios::end);
            nfile.write((char *)&main_record, sizeof(Record<TK>));
            nsize++;

            // Avanzamos la posicion del main file leyendo el siguiente record
            mfile.read((char *)&main_record, sizeof(Record<TK>));
          }

          // No se permiten keys repetidas, entonces solo guardamos la del main file
          else if (new_records[ptr].key == main_record.key)
          {
            nfile.seekp(0, ios::end);
            nfile.write((char *)&main_record, sizeof(Record<TK>));
            nsize++;

            // Avanzamos ambos ptrs
            mfile.read((char *)&main_record, sizeof(Record<TK>));
            ptr++;
          }
        }

        // En caso se acaben los new_records, se escriben los records restantes del main file
        else
        {
          nfile.write((char *)&main_record, sizeof(Record<TK>));
          nsize++;

          // Avanzamos la posicion del main file leyendo el siguiente record
          mfile.read((char *)&main_record, sizeof(Record<TK>));
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

  int pos_search(const char *key)
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

    Record<TK> record;

    while (low <= high)
    {
      mid = low + (high - low) / 2;
      pos = mid * sizeof(Record<TK>) + sizeof(int);

      file.seekg(pos, ios::beg);
      file.read((char *)&record, sizeof(Record<TK>));

      int compare = strcmp(record.key, key);

      if (compare == 0)
      {
        file.close();
        return pos;
      }

      else if (compare < 0)
        low = mid + 1;

      else
        high = mid - 1;
    }

    file.close();
    return -1;
  }

  Record<TK> search(const char *key)
  {
    // Primero se busca en el archivo principal
    int pos = pos_search(key);

    // Si la pos encontrada es diferente a -1
    // Esta en el archivo principal
    if (pos != -1)
    {
      Record<TK> record = read_record(pos);
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
        return Record<TK>();
      }

      // Buscar elementos en el archivo de inserts
      Record<TK> record;
      int isize;
      file.read((char *)&isize, sizeof(int));
      for (int i = 0; i < isize; i++)
      {
        file.read((char *)&record, sizeof(Record<TK>));
        int compare = strcmp(record.key, key);
        if (compare == 0)
        {
          file.close();
          return record;
        }
      }
      file.close();
    }
    cout << "No se encontro elemento en ningun archivo" << endl;
    return Record<TK>();
  }

  bool remove_record(const char *key)
  {
    // Buscar elemento en el archivo de main
    int pos = pos_search(key);

    if (pos != -1)
    {
      fstream file(main_file, ios::in | ios::out | ios::binary);
      Record<TK> record = read_record(pos);
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
        cout << "No se encontro elemento" << endl;
        return false;
      }

      Record<TK> record;
      int isize;
      file.read((char *)&isize, sizeof(int));
      for (int i = 0; i < isize; i++)
      {
        file.read((char *)&record, sizeof(Record<TK>));
        int compare = strcmp(record.key, key);
        if (compare == 0)
        {
          record.is_removed = true;
          file.seekp(i * sizeof(Record<TK>) + sizeof(int), ios::beg);
          file.write((char *)&record, sizeof(Record<TK>));
          file.close();
          return true;
        }
      }
      file.close();
    }
    return false;
  }

  vector<Record<TK>> range_search(const char *low_key, const char *high_key)
  {
    vector<Record<TK>> result;
    ifstream mfile(main_file, ios::binary);
    ifstream ifile(insert_file, ios::binary);

    if (mfile)
    {
      int msize;
      mfile.read((char *)&msize, sizeof(int));

      int low = 0;
      int high = msize - 1;
      int mid;
      int pos;
      Record<TK> record;
      bool found = false;

      while (low <= high)
      {
        mid = (low + high) / 2;
        pos = mid * sizeof(Record<TK>) + sizeof(int);
        mfile.seekg(pos, ios::beg);
        mfile.read((char *)&record, sizeof(Record<TK>));

        int compare = strcmp(record.key, low_key);

        if (compare >= 0)
        {
          high = mid - 1;
          found = true;
        }
        else
        {
          low = mid + 1;
        }
      }

      if (found)
      {
        pos = low * sizeof(Record<TK>) + sizeof(int);
        mfile.seekg(pos, ios::beg);

        while (mfile.read((char *)&record, sizeof(Record<TK>)))
        {
          if (strcmp(record.key, high_key) > 0)
          {
            break;
          }
          if (!record.is_removed)
            result.push_back(record);
        }
      }

      mfile.close();
    }

    // Buscar en el archivo de inserts
    if (ifile)
    {
      int isize;
      ifile.read((char *)&isize, sizeof(int));
      Record<TK> record;

      for (int i = 0; i < isize; ++i)
      {
        ifile.read((char *)&record, sizeof(Record<TK>));

        int compare_a = strcmp(record.key, low_key);
        int compare_b = strcmp(record.key, high_key);
        if (!record.is_removed && compare_a >= 0 && compare_b <= 0)
          result.push_back(record);
      }
      ifile.close();
    }

    sort(result.begin(), result.end(), compare_key);

    return result;
  }

  Record<TK> read_record(int pos)
  {
    Record<TK> record;
    ifstream file(main_file, ios::binary);
    file.seekg(pos, ios::beg);
    file.read((char *)&record, sizeof(Record<TK>));
    file.close();
    return record;
  }

  void write_record(Record<TK> record, int pos)
  {
    fstream file(main_file, ios::in | ios::out | ios::binary);
    file.seekp(pos, ios::beg);
    file.write((char *)&record, sizeof(Record<TK>));
    file.close();
  }

  void print_mfile()
  {
    ifstream file(main_file, ios::binary);
    Record<TK> record;
    int size;
    file.read((char *)&size, sizeof(int));
    while (file.read((char *)&record, sizeof(Record<TK>)))
    {
      if (record.is_removed == false)
        cout << record.show();
    }

    cout << "size: " << size << endl;
    file.close();
  }

  void print_ifile()
  {
    ifstream file(insert_file, ios::binary);
    Record<TK> record;
    int size;
    file.read((char *)&size, sizeof(int));
    while (file.read((char *)&record, sizeof(Record<TK>)))
    {
      if (record.is_removed == false)
        cout << record.show();
    }

    cout << "size: " << size << endl;
    file.close();
  }
};

template <typename TK>
void build_test()
{
  Sequential<TK> file("main.bin");
  file.buildCSV("YTStats.csv");
}

template <typename TK>
void show_all_test()
{
  Sequential<TK> file("main.bin");
  file.print_mfile();
}

template <typename TK>
void search_test()
{
  Sequential<TK> file("main.bin");
  file.buildCSV("YTStats.csv");
  Record record = file.search("__4c1JCHvaQ");
  cout << record.show() << endl;
}

template <typename TK>
void rangesearch_test()
{
  Sequential<TK> file("main.bin");
  file.buildCSV("YTStats.csv");

  vector<Record<const char *>> records = file.range_search("__4c1JCHvaQ", "_a7bgLyeaxY");
  for (auto record : records)
    cout << record.show() << endl;
}

template <typename TK>
void remove_test()
{
  Sequential<TK> file("main.bin");
  file.buildCSV("YTStats.csv");
  file.remove_record("zzVFyVNgtsc");
  file.print_mfile();
}
