#ifndef PROJECTO1_READCSV_H
#define PROJECTO1_READCSV_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

template<typename TK>
struct Record {
    TK key;
};
// Estructura del registro
template<>
struct Record<double> {
    int anime_id;
    char name[100];
    char genre[100];
    char type[50];
    int episodes;
    double key; // Llave para ordenar
    int members;

    // para un arbol binario
    int left = -1; // 4 bytes
    int right = -1;    // 4 bytes
    int height = 0;     // 4 bytes
    int next_del = -2;  // 4 bytes

    friend ostream& operator<<(ostream& os, const Record<double>& record) {
        os << record.key;
        return os;
    }
    bool operator==(const Record<double>& other) const {
        return key == other.key;
    }
    bool operator==(const double key) const {
        return this->key == key;
    }
    void show() {
        cout << "Anime ID: " << anime_id << '\n';
        cout << "Name: " << name << '\n';
        cout << "Genre: " << genre << '\n';
        cout << "Type: " << type << '\n';
        cout << "Episodes: " << episodes << '\n';
        cout << "Rating: " << key << '\n';
        cout << "Members: " << members << '\n';
        cout << '\n';
    }
    void showAVL(){
        cout << key << " | " << left << " | " << right << " | " << height << endl;
    }
    bool operator<(const Record<double>& other) const {
        return key < other.key;
    }
    bool operator>(const Record<double>& other) const {
        return key > other.key;
    }
    bool operator<(const double key) const {
        return this->key < key;
    }
    bool operator>(const double key) const {
        return this->key > key;
    }
};

template<>
struct Record<int> {
    int anime_id;
    char name[100];
    char genre[100];
    char type[50];
    int episodes;
    double key; // Llave para ordenar
    int members;

    // para un arbol binario
    int left = -1; // 4 bytes
    int right = -1;    // 4 bytes
    int height = 0;    // 4 bytes
    int next_del = -2; // 4 bytes

    string getName()
    {
        return string(name);
    }
    string getGenre()
    {
        return string(genre);
    }
    string getType()
    {
        return string(type);
    }
    void setName(string name_)
    {
        strcpy(name, name_.c_str());
        name[name_.length()] = '\0';
    }
    void setGenre(string genre_)
    {
        strcpy(genre, genre_.c_str());
        genre[genre_.length()] = '\0';
    }
    void setType(string type_)
    {
        strcpy(type, type_.c_str());
        type[type_.length()] = '\0';
    }

    void show() {
        cout << "Anime ID: " << anime_id << '\n';
        cout << "Name: " << name << '\n';
        cout << "Genre: " << genre << '\n';
        cout << "Type: " << type << '\n';
        cout << "Episodes: " << episodes << '\n';
        cout << "Rating: " << key << '\n';
        cout << "Members: " << members << '\n';
        cout << '\n';
    }

    friend ostream& operator<<(ostream& os, const Record<int>& record) {
        os << record.anime_id;
        return os;
    }
    bool operator==(const Record<int>& other) const {
        return anime_id == other.anime_id;
    }
    bool operator==(const int key) const {
        return anime_id == key;
    }
    bool operator<(const Record<int>& other) const {
        return anime_id < other.anime_id;
    }
    bool operator>(const Record<int>& other) const {
        return anime_id > other.anime_id;
    }
    bool operator<(const int key) const {
        return anime_id < key;
    }
    bool operator>(const int key) const {
        return anime_id > key;
    }
    void showAVL(){
        cout << anime_id << " | " << left << " | " << right << " | " << height << endl;
    }
};
// Estructura del registro
template<>
struct Record<const char*> {
    char App_name[100];
    char App_id[100];
    char category[50];
    double rating;
    double rating_count;
    char installs[100];
    double minimum_installs;
    double maximum_installs;
    bool free;
    double price;
    char currency[10];
    char size[50];
    int anime_id = 0;
    double key = 0; // Llave para ordenar

    // para un arbol binario
    int left = -1; // 4 bytes
    int right = -1;    // 4 bytes
    int height = 0;    // 4 bytes
    int next_del = -2; // 4 bytes

    string getApp_name()
    {
        return string(App_name);
    }
    string getCategory()
    {
        return string(category);
    }
    std::string getApp_id() const
    {
        return std::string(App_id);
    }

    std::string getInstalls() const
    {
        return std::string(installs);
    }

    std::string getCurrency() const {
        return std::string(currency);
    }

    std::string getSize() const {
        return std::string(size);
    }

    void setApp_name(string app_name) {
        strcpy(App_name, app_name.c_str());
        App_name[app_name.length()] = '\0';
    }

    void setApp_id(const std::string &app_id) {
        strncpy(App_id, app_id.c_str(), sizeof(App_id) - 1);
        App_id[sizeof(App_id) - 1] = '\0';
    }
    void setCategory(string category_) {
        strcpy(category, category_.c_str());
        category[category_.length()] = '\0';
    }
    void setInstalls(const std::string &installs_)
    {
        strncpy(installs, installs_.c_str(), sizeof(installs) - 1);
        installs[sizeof(installs) - 1] = '\0';
    }

    void setCurrency(const std::string &currency_)
    {
        strncpy(currency, currency_.c_str(), sizeof(currency) - 1);
        currency[sizeof(currency) - 1] = '\0';
    }

    void setSize(const std::string &size_)
    {
        strncpy(size, size_.c_str(), sizeof(size) - 1);
        size[sizeof(size) - 1] = '\0';
    }

    friend ostream& operator<<(ostream& os, const Record<const char*>& record) {
        os << record.App_id;
        return os;
    }

    bool operator==(const Record<const char*>& other) const {
        return strcmp(App_id, other.App_id) == 0;
    }

    bool operator==(const char* key) const {
        return strcmp(this->App_id, key) == 0;
    }
    bool operator<(const Record<const char*>& other) const {
        return strcmp(App_id, other.App_id) < 0;
    }
    bool operator>(const Record<const char*>& other) const {
        return strcmp(App_id, other.App_id) > 0;
    }
    bool operator<(const char* key) const {
        return strcmp(App_id, key) < 0;
    }
    bool operator>(const char* key) const {
        return strcmp(App_id, key) > 0;
    }
    void show() {
        cout << "App Name: " << App_name << '\n';
        cout << "App ID: " << App_id << '\n';
        cout << "Category: " << category << '\n';
        cout << "Rating: " << rating << '\n';
        cout << "Rating Count: " << rating_count << '\n';
        cout << "Installs: " << installs << '\n';
        cout << "Minimum Installs: " << minimum_installs << '\n';
        cout << "Maximum Installs: " << maximum_installs << '\n';
        cout << "Free: " << free << '\n';
        cout << "Price: " << price << '\n';
        cout << "Currency: " << currency << '\n';
        cout << "Size: " << size << '\n';
        cout << '\n';
    }
    void showAVL(){
        cout << App_id << " | " << left << " | " << right << " | " << height << endl;
    }
};

// Función para eliminar las comillas alrededor de un campo
inline std::string trimQuotes(const std::string& str) {
    if (str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2); // Eliminar comillas al principio y al final
    }
    return str;
}

// Función para copiar de forma segura un string a un char[]
inline void safeCopy(char* dest, const std::string& src, size_t size) {
    strncpy(dest, src.c_str(), size - 1);
    dest[size - 1] = '\0'; // Asegurarse de que esté terminado en null
}

// Función para leer el archivo CSV
template<typename TK>
std::vector<Record<TK>> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Record<TK>> records;
    std::string line;

    // Ignorar la primera línea de encabezados
    std::getline(file, line);

    // Leer cada línea
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Record<TK> record;

        std::string App_name, App_id, category, rating, rating_count, installs, minimum_installs, maximum_installs, free, price, currency, size;

        try {
            // Leer los campos de la línea
            std::getline(ss, App_name, ',');
            std::getline(ss, App_id, ',');
            std::getline(ss, category, ',');
            std::getline(ss, rating, ',');
            std::getline(ss, rating_count, ',');
            std::getline(ss, installs, ',');
            std::getline(ss, minimum_installs, ',');
            std::getline(ss, maximum_installs, ',');
            std::getline(ss, free, ',');
            std::getline(ss, price, ',');
            std::getline(ss, currency, ',');
            std::getline(ss, size, ',');

            // Procesar y almacenar los valores en el Record
            safeCopy(record.App_name, trimQuotes(App_name), sizeof(record.App_name));
            safeCopy(record.App_id, trimQuotes(App_id), sizeof(record.App_id));
            safeCopy(record.category, trimQuotes(category), sizeof(record.category));

            // Manejo de errores en conversión numérica
            try {
                record.rating = stod(rating);
            } catch (const std::invalid_argument&) {
                cerr << "Error de conversión en rating: " << rating << '\n';
                throw;
            }

            try {
                record.rating_count = stod(rating_count);
            } catch (const std::invalid_argument&) {
                cerr << "Error de conversión en rating_count: " << rating_count << '\n';
                throw;
            }

            safeCopy(record.installs, trimQuotes(installs), sizeof(record.installs));

            try {
                record.minimum_installs = stod(minimum_installs);
            } catch (const std::invalid_argument&) {
                cerr << "Error de conversión en minimum_installs: " << minimum_installs << '\n';
                throw;
            }

            try {
                record.maximum_installs = stod(maximum_installs);
            } catch (const std::invalid_argument&) {
                cerr << "Error de conversión en maximum_installs: " << maximum_installs << '\n';
                throw;
            }

            record.free = (free == "True");

            try {
                record.price = stod(price);
            } catch (const std::invalid_argument&) {
                cerr << "Error de conversión en price: " << price << '\n';
                throw;
            }

            safeCopy(record.currency, trimQuotes(currency), sizeof(record.currency));
            safeCopy(record.size, trimQuotes(size), sizeof(record.size));

            // Añadir el record al vector
            records.push_back(record);

        } catch (const std::exception& e) {
            cerr << "Error al procesar la línea: " << line << '\n';
            cerr << "Excepción: " << e.what() << '\n';
        }
    }

    return records;
}

#endif //PROJECTO1_READCSV_H
