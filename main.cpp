#include <iostream>
#include "ExtendibleHashing.h"
#include "avlFile.h"

template<typename TK>
void testing_AVL(AVLFile<TK>& file);

template<typename TK>
void testing_Extendible(ExtendibleHashing<TK>& dh);

int main() {
    AVLFile<const char*> file("data3.dat");
    ExtendibleHashing<const char*> dh("prueba1.dat",  "prueba2.dat");
    // testing_AVL(file);
    testing_Extendible(dh);
    
    // Testing sequential file:
    // YT DATASET:
    buildYT_test<const char *>();
    showYT_test<const char*>();
    // searchYT_test<const char *>();
    // rangeYT_test<const char *>();
    // removeYT_test<const char *>();
    
    // PLAYSTORE DATASET
    // buildPlay_test<int>();
    // showPlay_test<int>();
    // searchPlay_test<int>();
    // rangePlay_test<int>();
    // removePlay_test<int>();
    return 0;
}

template<typename TK>
void testing_Extendible(ExtendibleHashing<TK>& dh){
    vector<Record<const char*>> records = readCSV<const char*>("../playstore_clean3.csv");
    cout << "Records size: " << records.size() << endl;
    for (int i = 0; i < 100000; ++i) {
        dh.insert(records[i]);
    }
    dh.update_disk();
    cout << "---------------------- FIND ELEMENTS ----------------------\n";
    const char *key = "com.eqra.android.BSTS.parent";
    Record<const char*> record = dh.search(key);
    record.show();
    cout << "---------------------- REMOVE ELEMENTS ----------------------\n";
    cout << "Remove " << key << ": " << dh.remove(key) << endl;

//    dh.display_directory();
//    dh.display_buckets();
}

template<typename TK>
void testing_AVL(AVLFile<TK>& file){
    cout << "TESTING CON DATA DE CSV Y LLAVE CODIGO\n";
    vector<Record<TK>> records = readCSV<TK>("../playstore_clean3.csv");
    cout << "Records size: " << records.size() << endl;
    for (int i = 0; i < 10000; ++i) {
        file.insert(records[i]);
    }
    cout << "---------------------- PRINT ALL ----------------------\n";
    file.printAll();
    cout << "---------------------- REMOVE ELEMENTS ----------------------\n";
    const char *key = "com.jieapp.taoyuanmetro";
    cout << "Remove " << key << ": " << file.remove(key) << endl;
    cout << "---------------------- PRINT ALL ----------------------\n";
    file.printAll();
    cout << "---------------------- FIND ELEMENTS ----------------------\n";
    Record<const char*> record = file.find(key);
    cout << "Record: "<< endl;
    record.show();
    cout << "---------------------- INORDER ----------------------\n";
    file.inorder();

}
