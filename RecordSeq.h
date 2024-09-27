#pragma once
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
  bool is_removed;

  Record() : key(0), is_removed(false) {}
  Record(int _key) : key(_key), is_removed(false) {}

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
    cout << key << ", " << boolalpha << is_removed << endl;
  }
};