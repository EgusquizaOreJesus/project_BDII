//
// Created by Fabricio on 9/27/2024.
//
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include "estructuras/FileStructureBase.h"
#include "estructuras/avlFile.h"
#include "estructuras/ExtendibleHashing.h"
#include "estructuras/Sequential.h"
using namespace std;
using namespace std::chrono;
string test_file_times = "times.bin";
void delete_file(const std::string& filename) {
    if (std::remove(filename.c_str()) != 0) {
        std::perror("Error deleting file");
    } else {
        std::cout << "File " << filename << " deleted successfully." << std::endl;
    }
}


// Función para medir tiempo y guardar en CSV
void save_times_to_csv(const std::string& filename, const std::vector<double>& times) {
    std::ofstream file(filename);
    for (const auto& time : times) {
        file << time << "ms" << std::endl;
    }
    file.close();
}

// Función para medir tiempos de inserción
template < typename Tk >
void benchmark_insert(FileStructure<Tk> *structure, const vector<Record<Tk>> records,const vector<int>& data_points  , const std::string& filename  ){
    std::vector<double> times;
    for(auto data_size : data_points){


        auto start = high_resolution_clock::now();
        for (int i = 0; i < data_size; ++i) {

            structure->insert(records[i]);
        }

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        times.push_back(elapsed.count());  // Guardar el tiempo en milisegundos
        delete_file(test_file_times);
    }
    save_times_to_csv(filename, times);  // Guardar en CSV
    delete_file(test_file_times);
}


template < typename Tk >
void benchmark_insert_hash(FileStructure<Tk> *structure, const vector<Record<Tk>> records,const vector<int>& data_points  , const std::string& filename  , string index){
    std::vector<double> times;
    for(auto data_size : data_points){


        auto start = high_resolution_clock::now();
        for (int i = 0; i < data_size; ++i) {
            structure->insert(records[i]);
        }

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        times.push_back(elapsed.count());  // Guardar el tiempo en milisegundos
        delete_file(test_file_times);
        delete_file(index);
    }
    save_times_to_csv(filename, times);  // Guardar en CSV
    delete_file(test_file_times);
}

// Función para medir tiempos de búsqueda
template < typename Tk>
void benchmark_search(FileStructure<Tk>* structure, const vector<Record<Tk>> records,const vector<int>& data_points  , const std::string& filename   )  {
    std::vector<double> times;
    for(auto data_size : data_points){


        auto start = high_resolution_clock::now();
        for (int i = 0; i < data_size; ++i) {
            auto c= structure->search(records[i]);
        }

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        times.push_back(elapsed.count());  // Guardar el tiempo en milisegundos
        delete_file(test_file_times);

    }
    save_times_to_csv(filename, times);  // Guardar en CSV

}
template < typename Tk>
void benchmark_search_hash(FileStructure<Tk>* structure, const vector<Record<Tk>> records,const vector<int>& data_points  , const std::string& filename   , string index)  {
    std::vector<double> times;
    for(auto data_size : data_points){


        auto start = high_resolution_clock::now();
        for (int i = 0; i < data_size; ++i) {
            auto c= structure->search(records[i]);
        }

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        times.push_back(elapsed.count());  // Guardar el tiempo en milisegundos
        delete_file(test_file_times);

    }
    save_times_to_csv(filename, times);  // Guardar en CSV

}

template < typename Tk>
void benchmark_range_search(FileStructure<Tk>* structure, const vector<Record<Tk>> records,const vector<int>& data_points  , const std::string& filename  ){
    std::vector<double> times;
    auto start = high_resolution_clock::now();
    for(auto data_size : data_points){


        auto start = high_resolution_clock::now();
        for (int i = 0; i < data_size; ++i) {
            structure->insert(records[i]);
        }

        auto end = high_resolution_clock::now();
        duration<double, std::milli> elapsed = end - start;
        times.push_back(elapsed.count());  // Guardar el tiempo en milisegundos
        delete_file(test_file_times);
    }
    save_times_to_csv(filename, times);  // Guardar en CSV

}

int main(){

    vector<Record<const char*>> r = readCSV<const char *>("data_youtube.csv");
    sort(r.begin() , r.end());
    vector<int> data_points{10 , 50 , 100 , 1000 , 2000 , 5000 , 10000  , 15000,  20000};
   cout<<"--------------------------PRUEBAS YOUTUBE----------------------------------"<<endl;

    FileStructure<const char *>* test_youtube;

    test_youtube = new AVLFile<const char *>(test_file_times);
    benchmark_insert<const char *>(test_youtube , r , data_points , "times_insert_avl.csv");
    test_youtube = nullptr;
    cout<<"TEST AVL END"<<endl;
    test_youtube = new ExtendibleHashing<const char*>(test_file_times , "hash_index.bin");
    benchmark_insert_hash<const char *>(test_youtube , r , data_points , "times_insert_ehash.csv" , "hash_index.bin" );
    test_youtube = nullptr;
    cout<<"TEST EHASH END"<<endl;
    test_youtube = new Sequential<const char *>(test_file_times);
    benchmark_insert(test_youtube , r , data_points , "times_insert_sequential.csv");
    cout<<"TEST Sequential END"<<endl;

    cout<<"--------------------------PRUEBAS PLAYSTORE ----------------------------------"<<endl;

    FileStructure<const char *>* test_playstore;
    test_playstore = new AVLFile<const char *>(test_file_times);
    benchmark_insert<const char *>(test_playstore , r , data_points , "times_insert_avl_app.csv");
    test_playstore = nullptr;
    cout<<"TEST AVL END"<<endl;
    test_playstore = new ExtendibleHashing<const char*>(test_file_times , "hash_index.bin");
    benchmark_insert_hash<const char *>(test_playstore , r , data_points , "times_insert_ehash__app.csv" , "hash_index.bin" );
    test_playstore = nullptr;
    cout<<"TEST EHASH END"<<endl;
    test_playstore = new Sequential<const char *>(test_file_times);
    benchmark_insert(test_playstore , r , data_points , "times_insert_sequential__app.csv");
    cout<<"TEST Sequential END"<<endl;




}