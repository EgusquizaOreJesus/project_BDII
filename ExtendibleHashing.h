//
// Created by Jesus Egusquiza on 15/09/2024.
//

#ifndef PROJECTO1_EXTENDIBLEHASHING_H
#define PROJECTO1_EXTENDIBLEHASHING_H
#include <iostream>
#include <functional>
#include <fstream>
#include "cmath"
#include "ReadCSV.h"
#define ll long long

using namespace std;


const int factor = 100;             // factor de crecimiento
const int globalDepth = 16;         // profundidad global inicial


template<typename TK>
struct Bucket {
    int localDepth = 1;         // 4 bytes
    int size = 0;           // 4 bytes
    Record<TK> records[factor+1];   // 4 * 7 = 28 bytes
    int next = -1;          // 4 bytes
    int free_pos = -1;      // 4 bytes

};
template<typename TK>
class Buffer{
    unordered_map<int, Bucket<TK>> buffer;  // Buffer que almacena buckets temporales
    string fileName;
public:
    const int bufferSize = 5000;             // Tamaño máximo del buffer
    Buffer(const string &fileName = "data.dat");
    int size(){
        return buffer.size();
    }
    // insertar o actualizar un bucket en el buffer
    void insert_or_update(int pos, Bucket<TK> &bucket);

    // obtener el bucket del buffer, si no esta en el buffer, se lee del archivo
    Bucket<TK> get(int pos);

    // volcar los buckets del buffer al archivo
    void flush_to_disk();

};

template<typename TK>
Buffer<TK>::Buffer(const string &fileName) {
    this->fileName = fileName;
}

template<typename TK>
void Buffer<TK>::insert_or_update(int pos, Bucket<TK> &bucket) {
    buffer[pos] = bucket;

    // si el buffer esta lleno, escribimos los buckets en el archivo
    if (buffer.size() >= bufferSize) {
        flush_to_disk();
    }
}

template<typename TK>
Bucket<TK> Buffer<TK>::get(int pos) {
    if (buffer.find(pos) != buffer.end()) {
        return buffer[pos];
    }

    // Leer el bucket del archivo
    fstream file(this->fileName, ios::binary | ios::in | ios::out);
    file.seekg(pos, ios::beg);
    Bucket<TK> bucket;
    file.seekg(sizeof(int), ios::beg); // saltar el head
    file.read((char *) &bucket, sizeof(bucket));
    file.close();
    return bucket;
}

template<typename TK>
void Buffer<TK>::flush_to_disk() {
    fstream file(fileName, ios::binary | ios::in | ios::out);

    // Escribir todos los buckets al archivo
    for (auto &entry : buffer) {
        int head = -1;
        int pos = entry.first;
        Bucket<TK> &bucket = entry.second;
        file.seekp(pos, ios::beg);
        file.write((char *)&head, sizeof(head));
        file.write((char *)&bucket, sizeof(bucket));
    }

    // Limpiar el buffer en RAM
    buffer.clear();
    file.close();
}

template<typename TK>
class ExtendibleHashing {
    string fileName;
    string directoryName;
    vector<int> directory;     // tabla de dispersión
    Buffer<TK> buffer;          // Buffer para almacenar buckets temporales

    hash<string> hasher;
    int sizeDirectory = 0;
    int pos_final = 0;
    int count_split = 1;

public:
    bool flag_update_directory = false;
    int hash_binary(Record<TK> key);   // convertir un entero a una cadena de bits
    int hash_binary(TK key);   // convertir un entero a una cadena de bits
    ExtendibleHashing(const string &fileName = "data.dat", const string &directoryName = "directory.dat");

    void insert(Record<TK> key);
    Record<TK> search(TK key);
    bool remove(TK key);
    void split(Bucket<TK> &bucket, int index);
    void display_directory();
    void display_buckets();
    void update_disk();
    void print_bucket(Bucket<TK> &bucket);
    void insert_encajado(Bucket<TK> &bucket, Record<TK> key, int pos);
    void save_directory();
    bool remove_bucket(Bucket<TK> &bucket, TK key, int head, int pos, int index);
    ~ExtendibleHashing(){
        if (buffer.size() > 0){
            buffer.flush_to_disk();
        }
    }
};

template<typename TK>
void ExtendibleHashing<TK>::update_disk() {
    if (buffer.size() > 0){
        buffer.flush_to_disk();
    }
    if (flag_update_directory){
        save_directory();
    }
}

template<typename TK>
bool ExtendibleHashing<TK>::remove(TK key){
    cout << "REMOVING" << endl;
    int index = hash_binary(key);
    fstream file(this->fileName, ios::binary | ios::in | ios::out);
    int pos = directory[index];
    file.seekg(pos, ios::beg);
    int head;
    Bucket<TK> bucket;
    file.read((char *) &head, sizeof(head));
    file.read((char *) &bucket, sizeof(bucket));
    file.close();
    if (remove_bucket(bucket, key, head, pos, index)){
        return true;
    }
    cout << "No se encontró el registro" << endl;
    return false;

}
template<typename TK>
bool ExtendibleHashing<TK>::remove_bucket(Bucket<TK> &bucket, TK key, int head, int pos, int index) {
    for (int i = 0; i < bucket.size; ++i) {
        if (bucket.records[i] == key){
            cout << "Key encontrado" << endl;
            // eliminamos el key y movemos los elementos
            for (int j = i; j < bucket.size - 1; ++j) {
                bucket.records[j] = bucket.records[j + 1];
            }
            bucket.size--;
            if (pos != head && head != -1){
                bucket.free_pos = head;
            }
            // escribimos el bucket en el archivo de datos
            fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
            file_data.seekp(this->directory[index] + sizeof(int), ios::beg);
            file_data.write((char *) &bucket, sizeof(bucket));
            file_data.close();
            // actualizamos el head
            if (head != -1){
                fstream file_head(this->fileName, ios::binary | ios::in | ios::out);
                file_head.seekp(this->directory[index], ios::beg);
                int posreal = pos + sizeof(int);
                file_head.write((char *) &posreal, sizeof(posreal));
                file_head.close();
            }
            return true;
        }
    }
    while (bucket.next != -1){
        fstream file(this->fileName, ios::binary | ios::in | ios::out);
        file.seekg(bucket.next, ios::beg);
        int pos = bucket.next;
        file.read((char *) &bucket, sizeof(bucket));
        file.close();
        for (int i = 0; i < bucket.size; ++i) {
            if (bucket.records[i] == key){
                // eliminamos el key y movemos los elementos
                for (int j = i; j < bucket.size - 1; ++j) {
                    bucket.records[j] = bucket.records[j + 1];
                }
                bucket.size--;
                if (pos != head){
                    bucket.free_pos = head;
                }
                // escribimos el bucket en el archivo de datos
                fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
                file_data.seekp(pos, ios::beg);
                file_data.write((char *) &bucket, sizeof(bucket));
                file_data.close();
                // actualizamos el head
                fstream file_head(this->fileName, ios::binary | ios::in | ios::out);
                file_head.seekp(this->directory[index], ios::beg);
                file_head.write((char *) &pos, sizeof(pos));
                file_head.close();
                return true;
            }
        }
    }
    return false;
}
template<typename TK>
void ExtendibleHashing<TK>::split(Bucket<TK> &bucket, int index) {
    this->flag_update_directory = true;     // se actualizara el directorio
    if (bucket.localDepth == globalDepth) {
        this->buffer.flush_to_disk();
        insert_encajado(bucket, bucket.records[bucket.size-1], this->directory[index]);
        return;
    }
    // proceso de mascaras
    int mask = index & ((1 << (globalDepth - bucket.localDepth)) - 1);
    mask = index - mask;
    mask = mask | (1 << (globalDepth - (bucket.localDepth + 1)));
    int recorres = (1 << globalDepth)/(1 << (bucket.localDepth + 1));

    // creamos buckets de split
    Bucket<TK> left_bucket;
    Bucket<TK> right_bucket;
    left_bucket.localDepth = bucket.localDepth + 1;
    right_bucket.localDepth = bucket.localDepth + 1;

    for (int i = 0; i < bucket.size; ++i) {
        int hash = hash_binary(bucket.records[i]);
        if (hash >= mask){
            right_bucket.records[right_bucket.size] = bucket.records[i];
            right_bucket.size++;
        }else{
            left_bucket.records[left_bucket.size] = bucket.records[i];
            left_bucket.size++;
        }

    }
    left_bucket.next = -1;
    right_bucket.next = -1;

    // escribimos en el buffer los nuevos buckets

    buffer.insert_or_update(this->directory[index], left_bucket);
    // calculamos la posicion final
    int new_pos = this->pos_final + (sizeof(int) + sizeof(Bucket<TK>)) * this->count_split;
    this->count_split++;
    buffer.insert_or_update(new_pos, right_bucket);

    // actualizamos el directorio
    for (int i = 0; i < recorres; ++i) {
        this->directory[mask + i] = new_pos;
    }
    this->sizeDirectory++;

    // llamar recursivamente a split, si hay algun bucket que se deba dividir
    if (left_bucket.size > factor){
        cout << "Split left" << endl;
        split(left_bucket, index);
    }
    if (right_bucket.size > factor){
        cout << "Split right" << endl;
        split(right_bucket, index);
    }

    // escribir en disco si el buffer esta lleno
    if (buffer.size() >= buffer.bufferSize){
        cout << "uwu" << endl;
        buffer.flush_to_disk();
    }
}

template<typename TK>
Record<TK> ExtendibleHashing<TK>::search(TK key){
    cout << "Buscando" << endl;
    int index = hash_binary(key);
    fstream file(this->fileName, ios::binary | ios::in | ios::out);
    int pos = directory[index];
    file.seekg(pos, ios::beg);
    int head;
    Bucket<TK> bucket;
    file.read((char *) &head, sizeof(head));
    file.read((char *) &bucket, sizeof(bucket));
    file.close();
    for (int i = 0; i < bucket.size; ++i) {
        if (bucket.records[i] == key){
            return bucket.records[i];
        }
    }
    while (bucket.next != -1){
        fstream file2(this->fileName, ios::binary | ios::in | ios::out);
        file2.seekg(bucket.next, ios::beg);
        file2.read((char *) &bucket, sizeof(bucket));
        file2.close();
        for (int i = 0; i < bucket.size; ++i) {
            if (bucket.records[i] == key){
                return bucket.records[i];
            }
        }
    }
    cout << "No se encontró el registro" << endl;
    return Record<TK>{};
}
template<typename TK>
void ExtendibleHashing<TK>::insert_encajado(Bucket<TK> &bucket, Record<TK> key, int pos) {
    fstream file(this->fileName, ios::binary | ios::in | ios::out);
    int head;
    file.seekg(pos, ios::beg);
    file.read((char *) &head, sizeof(head));
    file.close();

    // si el main bucket tiene espacio
    if (head == -1 && bucket.size < factor){
        bucket.records[bucket.size] = key;
        bucket.size++;

        // escribimos el bucket en el archivo de datos
        fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
        file_data.seekp(pos + sizeof(int), ios::beg);
        file_data.write((char *) &bucket, sizeof(bucket));
        file_data.close();

    }
    else if (head == -1){
        // caso donde el main bucket no tiene enlace
        // creamos un nuevo bucket
        this->count_split++;        // aumentamos el contador de split
        Bucket<TK> new_bucket;

        // si el main bucket esta lleno
        if (bucket.size > factor) {
            bucket.size--;
            // escribimos el bucket en el archivo de datos
            fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
            file_data.seekp(pos + sizeof(int), ios::beg);
            file_data.write((char *) &bucket, sizeof(bucket));
            file_data.close();
        }
        new_bucket.records[0] = key;
        new_bucket.size++;
        new_bucket.localDepth = bucket.localDepth;
        // escribimos el bucket en el archivo de datos
        fstream file(this->fileName, ios::binary | ios::in | ios::out);
        file.seekp(0, ios::end);
        int pos2 = file.tellp();
        file.write((char *) &new_bucket, sizeof(new_bucket));

        // actualizamos el puntero del bucket
        bucket.next = pos2;
        // actualizamos el head
        file.seekp(pos, ios::beg);
        file.write((char *) &pos2, sizeof(pos2));
        // escribimos el bucket en el archivo de datos
        file.write((char *) &bucket, sizeof(bucket));
        file.close();
    }
    else{
        // si hay encadenados, nos ubicamos en la posicion del HEAD
        fstream file(this->fileName, ios::binary | ios::in | ios::out);
        file.seekg(head, ios::beg);
        Bucket<TK> head_bucket;
        file.read((char *) &head_bucket, sizeof(head_bucket));
        file.close();
        // verificamos si tiene espacio
        if (head_bucket.size < factor) {
            head_bucket.records[head_bucket.size] = key;
            head_bucket.size++;

            // verificamos si el bucket head esta lleno
            if (head_bucket.size == factor){
                // actualizamos el head
                if (head_bucket.free_pos != -1){
                    fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
                    file_data.seekp(pos, ios::beg);
                    file_data.write((char *) &head_bucket.free_pos, sizeof(head_bucket.next));
                    file_data.close();
                    head_bucket.free_pos = -1;
                }
            }
            // escribimos el bucket en el archivo de datos
            fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
            file_data.seekp(head, ios::beg);
            file_data.write((char *) &head_bucket, sizeof(head_bucket));
            file_data.close();
        }else{
            // si esta lleno, verificamos si hay posiciones libres
            if (head_bucket.free_pos != -1){
                fstream file(this->fileName, ios::binary | ios::in | ios::out);
                file.seekg(head_bucket.free_pos, ios::beg);
                Bucket<TK> free_bucket;
                file.read((char *) &free_bucket, sizeof(free_bucket));
                file.close();
                free_bucket.records[free_bucket.size] = key;
                free_bucket.size++;
                // escribimos el bucket en el archivo de datos
                fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
                file_data.seekp(head_bucket.free_pos, ios::beg);
                file_data.write((char *) &free_bucket, sizeof(free_bucket));
                // actualizamos el puntero del bucket
                head_bucket.free_pos = -1;
                // escribimos el bucket en el archivo de datos
                file_data.seekp(head, ios::beg);
                file_data.write((char *) &head_bucket, sizeof(head_bucket));

            }else{
                // si no hay posiciones libres, creamos un nuevo bucket
                this->count_split++;        // aumentamos el contador de split
                Bucket<TK> new_bucket;
                new_bucket.records[0] = key;
                new_bucket.size++;
                new_bucket.localDepth = head_bucket.localDepth;
                // escribimos el bucket en el archivo de datos
                fstream file(this->fileName, ios::binary | ios::in | ios::out);
                file.seekp(0, ios::end);
                int pos2 = file.tellp();
                file.write((char *) &new_bucket, sizeof(new_bucket));
                file.close();

                // actualizamos el puntero del bucket
                head_bucket.next = pos2;
                // escribimos el bucket en el archivo de datos
                fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
                file_data.seekp(head, ios::beg);
                file_data.write((char *) &head_bucket, sizeof(head_bucket));
                file_data.close();

                // actualizamos el head
                fstream file_head(this->fileName, ios::binary | ios::in | ios::out);
                file_head.seekp(pos, ios::beg);
                file_head.write((char *) &pos2, sizeof(pos2));
                file_head.close();
            }
        }
    }
}

template<typename TK>
void ExtendibleHashing<TK>::print_bucket(Bucket<TK> &bucket) {
    cout << "Size: " << bucket.size << endl;
    cout << "Local depth: " << bucket.localDepth << endl;
    cout << "free pos: " << bucket.free_pos << endl;
    for (int i = 0; i < bucket.size; ++i) {
        cout << bucket.records[i] << " ";
    }
    while (bucket.next != -1){
        cout << " -> ";
        fstream file(this->fileName, ios::binary | ios::in | ios::out);
        file.seekg(bucket.next, ios::beg);
        file.read((char *) &bucket, sizeof(bucket));
        for (int i = 0; i < bucket.size; ++i) {
            cout << bucket.records[i] << " ";
        }
        file.close();
    }
    cout << endl;
    cout << " ----------------- " << endl;
}

template<typename TK>
void ExtendibleHashing<TK>::display_buckets() {
    cout << "Displaying buckets" << endl;
    fstream file(this->fileName, ios::binary | ios::in | ios::out);
    for (int i = 0; i < directory.size(); ++i) {
        int pos = directory[i];
        file.seekg(pos, ios::beg);
        int head;
        Bucket<TK> bucket;
        file.read((char *) &head, sizeof(head));
        file.read((char *) &bucket, sizeof(bucket));
        cout << "Bucket[" << i << "]: " << endl;
        cout << "Head: " << head << endl;
        print_bucket(bucket);
        cout << endl;
    }
    file.close();

}

template<typename TK>
int ExtendibleHashing<TK>::hash_binary(TK key) {
    size_t hashcode;
    if (typeid(TK) == typeid(int)){
        hashcode = hasher((to_string(key)));
    }
    hashcode = hasher((to_string(key)));
    int index = hashcode % int(pow(2, globalDepth));
    return index;
}
template<>
int ExtendibleHashing<const char*>::hash_binary(const char* key) {
    size_t hashcode;
    hashcode = hasher(((key)));
    int index = hashcode % int(pow(2, globalDepth));
    return index;
}

template<typename TK>
int ExtendibleHashing<TK>::hash_binary(Record<TK> key) {
    size_t hashcode;
    if (typeid(TK) == typeid(int)){
        hashcode = hasher((to_string(key.anime_id)));
    }
    else if (typeid(TK) == typeid(const char*)){
        hashcode = hasher(((key.App_id)));
    }
    int index = hashcode % int(pow(2, globalDepth));
    return index;
}

template<typename TK>
void ExtendibleHashing<TK>::insert(Record<TK> key) {
    int index = hash_binary(key);
    int pos = this->directory[index];
    // leer el bucket
    Bucket<TK> bucket = buffer.get(pos);
    if (bucket.size < factor+1 && bucket.localDepth == globalDepth){
        buffer.flush_to_disk();
        cout << "Insertando en bucket" << endl;
        insert_encajado(bucket, key, pos);
    }
    else{
        // insertar el bucket
        bucket.records[bucket.size] = key;
        bucket.size++;

        buffer.insert_or_update(pos, bucket);

        if (bucket.size > factor){
            split(bucket, index);
        }
    }
}

template<typename TK>
void ExtendibleHashing<TK>::save_directory() {
    cout << "Saving directory" << endl;
    fstream file(this->directoryName, ios::binary | ios::in | ios::out);
    file.seekp(sizeof(int), ios::beg);
    file.write((char *)& sizeDirectory, sizeof(sizeDirectory));
    int num = this->directory[0];
    int repeat = 0;
    for (int i = 0; i < directory.size(); ++i) {
        if (num == directory[i]) {
            repeat++;
        }else{
            file.write((char *)& repeat, sizeof(repeat));
            file.write((char *)& num, sizeof(num));
            num = directory[i];
            repeat = 1;
        }
    }

    file.write((char *)& repeat, sizeof(repeat));
    file.write((char *)& num, sizeof(num));
    file.close();
}

template<typename TK>
ExtendibleHashing<TK>::ExtendibleHashing( const string &fileName, const string &directoryName):buffer(fileName) {
    cout << "Constructor" << endl;
    this->directoryName = directoryName;
    this->fileName = fileName;

    fstream file;
    file.open(directoryName, ios::binary | ios::in | ios::out);
    cout << "Constructor" << endl;

    if (!file.is_open()) {

        file.open(directoryName, ios::binary | ios::out);
        file.close();
    }

    fstream file_default_data(directoryName, ios::binary | ios::in | ios::out);

    file_default_data.seekg(0, ios::end);//point to the end

    if (file_default_data.tellg() == 0) {
        cout << "Empty file" << endl;
        int size = pow(2, globalDepth);
        file_default_data.close();
        this->directory.resize(size);
        // guardo el tamaño de la tabla de dispersión
        // creamos dos buckets
        Bucket<TK> bucket1;
        Bucket<TK> bucket2;
        fstream file_data(this->fileName, ios::binary| ios::out);
        int head1 = -1;
        int head2 = -1;
        int pos1 = file_data.tellp();
        cout << "Pos1: " << pos1 << endl;
        file_data.write((char *) &head1, sizeof(head1));
        file_data.write((char *) &bucket1, sizeof(bucket1));
        int pos2 = file_data.tellp();
        cout << "Pos2: " << pos2 << endl;
        file_data.write((char *) &head2, sizeof(head2));
        file_data.write((char *) &bucket2, sizeof(bucket2));
        file_data.close();
        this->pos_final = pos2;     // posicion final
        for (int i = 0; i < size/2; ++i) {
            this->directory[i] = pos1;
        }
        for (int i = size/2; i < size; ++i) {
            this->directory[i] = pos2;
        }
        this->sizeDirectory = 2;

        int size_records_repeat = size/2;
        int sizeDirectoryfinal = this->sizeDirectory;
        // guardar en el archivo de directorio
        fstream file_default_data2(directoryName, ios::binary | ios::in | ios::out);
        file_default_data2.write((char *)& size, sizeof(size));
        file_default_data2.write((char *)& sizeDirectoryfinal, sizeof(sizeDirectoryfinal));
        file_default_data2.write((char *)& size_records_repeat, sizeof(size_records_repeat));
        file_default_data2.write((char *)& pos1, sizeof(pos1));
        file_default_data2.write((char *)& size_records_repeat, sizeof(size_records_repeat));
        file_default_data2.write((char *)& pos2, sizeof(pos2));
        file_default_data2.close();
    } else {
        // leer el directorio
        cout << "Reading directory" << endl;
        file_default_data.seekg(0, ios::beg);
        int size;
        file_default_data.read((char *) &size, sizeof(size));
        int size_directory;
        file_default_data.read((char *) &size_directory, sizeof(size_directory));
        this->sizeDirectory = size_directory;
        int max_pos = 0;
        for (int i = 0; i < size_directory; ++i) {
            int size_repeat;
            int pos;
            file_default_data.read((char *) &size_repeat, sizeof(size_repeat));
            file_default_data.read((char *) &pos, sizeof(pos));
            while (size_repeat--){
                this->directory.push_back(pos);
            }
            if (pos > max_pos){
                max_pos = pos;
            }
        }
        this->pos_final = max_pos;
    }
    file_default_data.close();
    cout << "File name: " << fileName << endl;
    file.open(fileName, ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        cout << "Creating file" << endl;
        file.open(fileName, ios::binary | ios::out);
        file.close();
    }
}

template<typename TK>
void ExtendibleHashing<TK>::display_directory() {
    cout << "Displaying directory" << endl;
    for (int i = 0; i < this->directory.size(); ++i) {
        cout << "directory[" << i << "] -> " << this->directory[i] << endl;
    }
}
#endif //PROJECTO1_EXTENDIBLEHASHING_H
