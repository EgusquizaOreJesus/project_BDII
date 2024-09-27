#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Definición de Record para simplificar el ejemplo
template<typename T>
struct Record {
    T key;
    Record(T key) : key(key) {}
};

// Clase base templada
template <typename TK>
struct AVLFileBase {
    virtual ~AVLFileBase() = default;

    // Método virtual para la inserción
    virtual void insert(Record<TK> record) = 0;
};

// Clase templada que hereda de AVLFileBase
template <typename TK>
class AVLFile : public AVLFileBase<TK> {
public:
    AVLFile() = default;
    AVLFile(string filename) : filename(filename) {}

    // Sobrescribe el método insert
    void insert(Record<TK> record) override {
        cout << "Inserting record into AVLFile: " << record.key << endl;
        // Lógica específica de inserción
        insert2(record);
    }

    void insert2(Record<TK> record) {
        cout << "Inserting record into AVLFile: " << record.key << endl;
        // Lógica específica de inserción
    }

private:
    string filename;
};

template<typename TK>
class ExtendibleHash : public AVLFileBase<TK> {
public:
    ExtendibleHash() = default;

    void insert(Record<TK> record) override {
        cout << "Inserting record into ExtendibleHash: " << record.key << endl;
        // Lógica específica de inserción
    }
};

// Factory para crear instancias de las clases derivadas
class DataStructureFactory {
public:
    template <typename TK>
    static std::unique_ptr<AVLFileBase<TK>> createAVL() {
        return std::make_unique<AVLFile<TK>>();
    }

    template<typename TK>
    static std::unique_ptr<AVLFileBase<TK>> createHash() {
        return std::make_unique<ExtendibleHash<TK>>();
    }
};

// Función para leer registros basada en el tipo de plantilla
template <typename TK>
Record<TK> readRecord();

template <>
Record<int> readRecord(){
    cout << "Reading record" << endl;
    return Record<int>(10);
}
template <>
Record<string> readRecord() {
    cout << "Reading record" << endl;
    return Record<string>("Hello");
}

template <typename TK>
class Template{
    public:
    std::unique_ptr<AVLFileBase<TK>> instance;
    Template(){
        instance = DataStructureFactory::createAVL<TK>();
    }
    Template(string filestruct){
        if(filestruct == "avl"){
            instance = DataStructureFactory::createAVL<TK>();
        }else if(filestruct == "hash"){
            instance = DataStructureFactory::createHash<TK>();
        }
    }
    void insert(Record<TK> record){
        instance->insert(record);
    }
    void ejemplo(bool isavl){
        if (isavl){
            Record<TK> record = readRecord<TK>();
        }
        else{
            Record<TK> record = readRecord<TK>();
        }
    }
};


// Ejemplo de uso
int main() {
    // Crear un AVLFile<int>
    Template<string> avlInt;
    avlInt.insert(Record<string>("hola"));

    avlInt.ejemplo(true);
    return 0;
}
