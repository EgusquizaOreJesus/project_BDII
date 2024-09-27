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
struct BaseRecord {
public:
    TK key;

    // campos comunes


};

template<typename TK>
struct Record : BaseRecord<TK> {};

template<>
struct Record<const char*> : public BaseRecord<const char*> {
    char key[100];
    char title[200];
    char channel_title[100];
    long long views;
    long long likes;
    long long dislikes;
    long long comment_count;
    // Campos comunes al AVL
    int left = -1;
    int right = -1;
    int height = 0;
    int next_del = -2;
    string show() {
        stringstream ss;
        ss << "key: " << key << '\n';
        ss << "title: " << title << '\n';
        ss << "channel_title: " << channel_title << '\n';
        ss << "views: " << views << '\n';
        ss << "likes: " << likes << '\n';
        ss << "dislikes: " << dislikes << '\n';
        ss << "comment_count: " << comment_count << '\n';
        ss << '\n';
        return ss.str();
    }


    void showAVL(){
        cout << key << " | " << left << " | " << right << " | " << height << endl;
    }
    friend ostream& operator<<(ostream& os, const Record<const char*>& record) {
        os << record.key;
        return os;
    }
    bool operator==(const Record<const char*>& other) const {
        return strcmp(key, other.key) == 0;
    }

    bool operator==(const char* key) const {
        return strcmp(this->key, key) == 0;
    }

    bool operator<(const Record<const char*>& other) const {
        return strcmp(key, other.key) < 0;
    }

    bool operator>(const Record<const char*>& other) const {
        return strcmp(key, other.key) > 0;
    }

    bool operator<(const char* key) const {
        return strcmp(this->key, key) < 0;
    }

    bool operator>(const char* key) const {
        return strcmp(this->key, key) > 0;
    }
};
template<>
struct Record<int>: public BaseRecord<const char*> {
    char key[100];
    char App_name[100];
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
    // Campos comunes al AVL
    int left = -1;
    int right = -1;
    int height = 0;
    int next_del = -2;
    string show() {
        stringstream ss;
        ss << "App Name: " << App_name << '\n';
        ss << "App ID: " << key << '\n';
        ss << "Category: " << category << '\n';
        ss << "Rating: " << rating << '\n';
        ss << "Rating Count: " << rating_count << '\n';
        ss << "Installs: " << installs << '\n';
        ss << "Minimum Installs: " << minimum_installs << '\n';
        ss << "Maximum Installs: " << maximum_installs << '\n';
        ss << "Free: " << (free ? "Yes" : "No") << '\n';
        ss << "Price: " << price << '\n';
        ss << "Currency: " << currency << '\n';
        ss << "Size: " << size << '\n';
        ss << '\n';
        return ss.str();
    }
    void showAVL(){
        cout << key << " | " << left << " | " << right << " | " << height << endl;
    }
    bool operator==(const Record<const char*>& other) const {
        return strcmp(key, other.key) == 0;
    }

    bool operator==(const char* key) const {
        return strcmp(this->key, key) == 0;
    }

    bool operator<(const Record<int>& other) const {
        return strcmp(key, other.key) < 0;
    }

    bool operator>(const Record<int>& other) const {
        return strcmp(key, other.key) > 0;
    }

    bool operator<(const char* key) const {
        return strcmp(this->key, key) < 0;
    }

    bool operator>(const char* key) const {
        return strcmp(this->key, key) > 0;
    }

};
// Función para eliminar las comillas alrededor de un campo
inline std::string trimQuotes(const std::string& str) {
    if (str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

// Función para copiar de forma segura un string a un char[]
inline void safeCopy(char* dest, const std::string& src, size_t size) {
    strncpy(dest, src.c_str(), size - 1);
    dest[size - 1] = '\0'; // Asegurar que termine en null
}

// FUNCION GENERICA
template<typename TK>
vector<Record<TK>> readCSV(const string& filename);

// Función para leer el archivo CSV de YouTube
template<>
std::vector<Record<const char*>> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Record<const char*>> records;
    std::string line;

    // Ignorar la primera línea de encabezados
    std::getline(file, line);

    // Leer cada línea
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Record<const char*> record;

        std::string key, title, channel_title, views, likes, dislikes, comment_count;
        try {
            // Leer los campos de la línea
            std::getline(ss, key, ',');
            std::getline(ss, title, ',');
            std::getline(ss, channel_title, ',');
            std::getline(ss, views, ',');
            std::getline(ss, likes, ',');
            std::getline(ss, dislikes, ',');
            std::getline(ss, comment_count, ',');

            // Procesar y almacenar los valores en el Record
            safeCopy(record.key, trimQuotes(key), sizeof(record.key));
            safeCopy(record.title, trimQuotes(title), sizeof(record.title));
            safeCopy(record.channel_title, trimQuotes(channel_title), sizeof(record.channel_title));

            record.views = std::stoll(views);
            record.likes = std::stoll(likes);
            record.dislikes = std::stoll(dislikes);
            record.comment_count = std::stoll(comment_count);

            // Añadir el record al vector
            records.push_back(record);
        } catch (const std::exception& e) {
//            cerr << "Error al procesar la línea: " << line << '\n';
//            cerr << "Excepción: " << e.what() << '\n';
        }
    }

    return records;
}
template<>
std::vector<Record<int>> readCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Record<int>> records;
    std::string line;

    // Ignorar la primera línea de encabezados
    std::getline(file, line);

    // Leer cada línea
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Record<int> record;

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
            safeCopy(record.key, trimQuotes(App_id), sizeof(record.key));
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
