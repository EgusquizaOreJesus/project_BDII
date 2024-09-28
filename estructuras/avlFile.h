//
// Created by Jesus Egusquiza on 16/09/2024.
//

#ifndef PROJECTO1_AVLFILE_H
#define PROJECTO1_AVLFILE_H
#include "iostream"
#include "vector"
#include "fstream"
#include <string>
#include <sstream>
#include "ReadCSV.h"
#include <cstring>
#include "FileStructureBase.h"
using namespace std;


template <typename TK>
class AVLFile: public FileStructure<TK>{
private:
    int pos_root;
    int head = -1;
    void insert(fstream &file, int& parent,int &pos, Record<TK> record, bool isRight = false);
    bool remove(fstream &file, long parent,long pos_node, const char* key);
    int height(long pos_node, fstream &file);
    void update_head(long pos_node, Record<TK> a, fstream &file);
    void updateHeight(long pos_node, fstream &file);
    int balancingFactor(long pos_node, fstream &file);
    int rotacion(long parent,long pos_node, fstream &file);
    bool isHoja(long pos_node, fstream &file);
    void inorder(long pos, fstream &file);
    int rota_left(long parent,long pos_node, fstream &file);
    int rota_right(long parent,long pos_node, fstream &file);
public:
    // constructor por defecto
    AVLFile() = default;
    void update_disk() override {

    }
    vector<Record<TK>> range_search(const char* begin, const char* end) override;
    string filename;
    AVLFile(string filename);
    void insert(Record<TK> record) override;
    void printAll() override;
    void buildCSV(vector<Record<TK>> records) override;
    Record<TK> search(const char* key) override;
    void inorder();
    void bfs();
    bool remove(const char* key) override;
    ~AVLFile() = default;

};

template<typename TK>
void AVLFile<TK>::buildCSV(vector<Record<TK>> records) {

}

template<typename TK>
void AVLFile<TK>::insert(fstream &file, int &parent, int &pos, Record<TK> record, bool isRight) {
    if (this->pos_root == -1){
        cout << "insertando en la raiz" << endl;
        this->pos_root = 0;
        file.seekp(0, ios::beg);   // posicionar el puntero al inicio del archivo
        file.write((char*)(&pos_root), sizeof(long));   // escribir la posicion de la raiz
        file.write((char*)(&head), sizeof(int));   // escribir la cantidad de registros
        file.write((char*)(&record), sizeof(Record<TK>));   // escribir el registro
        return;
    }
    if (pos == -1){
        file.seekg(sizeof(long), ios::beg);
        file.read((char*)(&head), sizeof(int));
        Record<TK> padre;
        if (head == -1){
            file.seekp(0, ios::end);

            int index = file.tellp() / sizeof(Record<TK>);
            file.write((char*)(&record), sizeof(Record<TK>));


            // actualizo el puntero del registro anterior
            file.seekg(sizeof(long) + sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
            file.read((char*)(&padre), sizeof(Record<TK>));
            if (padre > record){
                padre.left = index;
            }else{
                padre.right = index;
            }
            file.seekp(sizeof(long) + sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
            file.write((char*)(&padre), sizeof(Record<TK>));
        }else{
            // insertar en el registro eliminado
            // nos movemos al siguiente registro eliminado
            Record<TK> record_aux;
            file.seekg(sizeof(long) + sizeof(long) + head * sizeof(Record<TK>), ios::beg);
            file.read((char*)(&record_aux), sizeof(Record<TK>));
            int head_temporal = record_aux.next_del;
            file.seekp(sizeof(long), ios::beg);
            file.write((char*)(&head_temporal), sizeof(int));
            // posicionamos en donde vamos a insertar:
            file.seekp(sizeof(long) + sizeof(long) + head * sizeof(Record<TK>), ios::beg);
            int index = file.tellp() / sizeof(Record<TK>);
            file.write((char*)(&record), sizeof(Record<TK>));

            // actualizo el puntero del registro anterior osea su padre
            file.seekg(sizeof(long) + sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
            file.read((char*)(&padre), sizeof(Record<TK>));
            if (padre > record){
                padre.left = index;
            }else{
                padre.right = index;
            }
            file.seekp(sizeof(long) + sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
            file.write((char*)(&padre), sizeof(Record<TK>));
            head = head_temporal;
        }

    }else{
        Record<TK> aux;
        // mover el puntero a la posicion pos + pos_root
        file.seekg(pos * sizeof(Record<TK>) + sizeof(long) + sizeof(long), ios::beg);
        file.read((char*)(&aux), sizeof(Record<TK>));
        if (record < aux){
            insert(file, pos,aux.left, record, false);
        }else{
            insert(file, pos,aux.right, record , true);
        }
    }
    updateHeight(pos, file);
    rotacion( parent,pos, file);
}

template<typename TK>
bool AVLFile<TK>::remove(fstream &file, long parent, long pos_node, const char* key) {
    if (pos_node == -1){
        cerr << "no se encontro el registro -> ";
        return false;
    }else{
        Record<TK> a;
        file.seekg(sizeof(long) +sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
        file.read((char *)(&a), sizeof(Record<TK>));
        if (a == key){
            if (isHoja(pos_node,file)){
                if (parent == pos_node){
                    // si el nodo a eliminar es la raiz
                    this->pos_root = -1;
                    update_head(pos_node, a, file);
                }
                else {
                    // si el nodo a eliminar es una hoja
                    Record<TK> parent_record;
                    file.seekg(sizeof(long) +sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
                    file.read((char *)(&parent_record), sizeof(Record<TK>));
                    // actualizar el puntero del padre
                    if (parent_record > a){
                        parent_record.left = -1;
                    }else{
                        parent_record.right = -1;
                    }
                    file.seekp(sizeof(long) +sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
                    file.write((char *)(&parent_record), sizeof(Record<TK>));

                    update_head(pos_node, a, file);
                }
            }else if ( a.left == -1){
                // no tiene hijo izquierdo
                if (parent == pos_node){
                    this->pos_root = a.right;
                    file.seekp(0, ios::beg);
                    file.write((char*)(&this->pos_root), sizeof(long));
                    update_head(pos_node, a, file);
                }else {
                    Record<TK> parent_record;
                    file.seekg(sizeof(long) +sizeof(long)+ parent * sizeof(Record<TK>), ios::beg);
                    file.read((char *)(&parent_record), sizeof(Record<TK>));
                    parent_record.right = a.right;
                    file.seekp(sizeof(long) +sizeof(long)+ parent * sizeof(Record<TK>), ios::beg);
                    file.write((char *)(&parent_record), sizeof(Record<TK>));

                    update_head(pos_node, a, file);

                }
            }else if (a.right == -1){
                // no tiene hijo derecho
                if (parent == pos_node){
                    this->pos_root = a.left;
                    file.seekp(0, ios::beg);
                    file.write((char*)(&this->pos_root), sizeof(long));
                    update_head(pos_node, a, file);
                }else {
                    Record<TK> parent_record;
                    file.seekg(sizeof(long) +sizeof(long)+ parent * sizeof(Record<TK>), ios::beg);
                    file.read((char *)(&parent_record), sizeof(Record<TK>));
                    parent_record.left = a.left;
                    file.seekp(sizeof(long) +sizeof(long)+ parent * sizeof(Record<TK>), ios::beg);
                    file.write((char *)(&parent_record), sizeof(Record<TK>));

                    update_head(pos_node, a, file);
                }
            }else{
                Record<TK> predecesor;
                file.seekg(sizeof(long) +sizeof(long)+ a.left * sizeof(Record<TK>), ios::beg);
                file.read((char *)(&predecesor), sizeof(Record<TK>));
                int pos_predecesor = a.left;
                int parent_predecesor = pos_node;
                while (predecesor.right != -1){
                    parent_predecesor = pos_predecesor;
                    pos_predecesor = predecesor.right;
                    file.seekg(sizeof(long) +sizeof(long)+ pos_predecesor * sizeof(Record<TK>), ios::beg);
                    file.read((char *)(&predecesor), sizeof(Record<TK>));
                }
                if (parent == pos_node){
                    this->pos_root = pos_predecesor;
                    file.seekp(0, ios::beg);
                    file.write((char*)(&this->pos_root), sizeof(long));
                }
                Record<TK> parent_predecesor_record;
                file.seekg(sizeof(long) +sizeof(long)+ parent_predecesor * sizeof(Record<TK>), ios::beg);
                file.read((char *)(&parent_predecesor_record), sizeof(Record<TK>));
                parent_predecesor_record.right = -1;
                file.seekp(sizeof(long) +sizeof(long)+ parent_predecesor * sizeof(Record<TK>), ios::beg);
                file.write((char *)(&parent_predecesor_record), sizeof(Record<TK>));
                updateHeight(parent_predecesor, file);
                predecesor.right = a.right;
                predecesor.left = a.left;
                file.seekp(sizeof(long) +sizeof(long)+ pos_predecesor * sizeof(Record<TK>), ios::beg);
                file.write((char *)(&predecesor), sizeof(Record<TK>));
                updateHeight(pos_predecesor, file);
                // actualizar alturas del padre del predecesor
                Record<TK> parent_record;
                file.seekg(sizeof(long) +sizeof(long)+ parent * sizeof(Record<TK>), ios::beg);
                file.read((char *)(&parent_record), sizeof(Record<TK>));
                if (parent_record < a){
                    parent_record.right = pos_predecesor;
                }else{
                    parent_record.left = pos_predecesor;
                }
                file.seekp(sizeof(long) +sizeof(long)+ parent * sizeof(Record<TK>), ios::beg);
                file.write((char *)(&parent_record), sizeof(Record<TK>));
                update_head(pos_node, a, file);
            }
            return true;
        }
        else if (a > key){
            bool result = remove(file, pos_node, a.left, key);
            if (result){
                updateHeight(pos_node, file);
                rotacion(parent,pos_node, file);
            }
            return result;
        }
        else if (a < key){
            bool result = remove(file, pos_node, a.right, key);
            if (result){
                updateHeight(pos_node, file);
                rotacion(parent,pos_node, file);
            }
            return result;
        }
    }
    return false;
}

template<typename TK>
int AVLFile<TK>::height(long pos_node, fstream &file) {
    if (pos_node == -1) return -1;

    Record<TK> a;
    file.seekg(sizeof(long) + sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));
    return a.height;
}

template<typename TK>
void AVLFile<TK>::update_head(long pos_node, Record<TK> a, fstream &file) {
    // capturamos el head
    file.seekg(sizeof(long), ios::beg);
    file.read((char*)(&head), sizeof(int));

    // actualizamos el head con el registro eliminado
    file.seekp(sizeof(long), ios::beg);
    file.write((char*)(&pos_node), sizeof(int));
    // actualizamos el next_del del registro eliminado
    a.next_del = head;
    // escribimos el registro eliminado
    file.seekp(sizeof(long) + sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.write((char*)(&a), sizeof(Record<TK>));
}

template<typename TK>
void AVLFile<TK>::updateHeight(long pos_node, fstream &file) {
    if (pos_node == -1) return;

    Record<TK> a;
    file.seekg(sizeof(long) + sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));
    if (a.right == -1 && a.left == -1){
        a.height = 0;
        file.seekp(sizeof(long) + sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
        file.write((char *)(&a), sizeof(Record<TK>));
        return;
    }

    a.height = max(height(a.left, file), height(a.right, file)) + 1;
    file.seekp(sizeof(long) + sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.write((char *)(&a), sizeof(Record<TK>));
}

template<typename TK>
int AVLFile<TK>::balancingFactor(long pos_node, fstream &file) {
    if (pos_node == -1) return 0;

    Record<TK> a;
    file.seekg(sizeof(long) + sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));

    int h = height(a.left, file) - height(a.right, file);

    return h;
}

template<typename TK>
int AVLFile<TK>::rotacion(long parent, long pos_node, fstream &file) {
    int bf = balancingFactor(pos_node, file);
    long temp_cursor = file.tellg();
    if (pos_node == -1) return 0;

    Record<TK> a;
    file.seekg(sizeof(long) + sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));

    // regresar el cursor a la de temporal
    file.seekg(temp_cursor , ios::beg);
    if (bf >= 2){
        if (balancingFactor(a.left, file) < 0) {
            rota_left(pos_node,a.left, file);
        }

        rota_right(parent,pos_node, file);
    }
    if (bf <= -2){
        if (balancingFactor(a.right, file) > 0){
            rota_right(pos_node,a.right, file);
        }
        rota_left(parent,pos_node, file);
    }
}

template<typename TK>
bool AVLFile<TK>::isHoja(long pos_node, fstream &file) {
    if (pos_node == -1) return false;

    Record<TK> a;
    file.seekg(sizeof(long) +sizeof(long)+ pos_node * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));

    return a.left == -1 && a.right == -1;
}

template<typename TK>
void AVLFile<TK>::inorder(long pos, fstream &file) {
    if (pos == -1) return;

    Record<TK> a;
    file.seekg(sizeof(long) + sizeof(long) + pos * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));

    inorder(a.left, file);
    cout << a << endl;
    inorder(a.right, file);
}

template<typename TK>
int AVLFile<TK>::rota_left(long parent, long pos_node, fstream &file) {
    Record<TK> a;
    file.seekg(sizeof(long)+ sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));

    Record<TK> b;
    file.seekg(sizeof(long) + sizeof(long) + a.right * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&b), sizeof(Record<TK>));
    // si la rotacion involucra al nodo raiz
    if (parent == pos_node){
        this->pos_root = a.right;
        file.seekp(0, ios::beg);
        file.write((char*)(&this->pos_root), sizeof(long));
    }else{
        Record<TK> parent_record;
        file.seekg(sizeof(long)+ sizeof(long)+ parent * sizeof(Record<TK>), ios::beg);
        file.read((char *)(&parent_record), sizeof(Record<TK>));
        if (parent_record > a){
            parent_record.left = a.right;
        }else{
            parent_record.right = a.right;
        }
        file.seekp(sizeof(long)+ sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
        file.write((char *)(&parent_record), sizeof(Record<TK>));
    }
    // actualizar el hijo derecho de a
    int pos = a.right;
    a.right = b.left;
    file.seekp(sizeof(long)+ sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.write((char *)(&a), sizeof(Record<TK>));
    // actualizar el hijo izquierdo de b
    b.left = pos_node;
    file.seekp(sizeof(long)+ sizeof(long) + pos* sizeof(Record<TK>), ios::beg);
    file.write((char *)(&b), sizeof(Record<TK>));
    // actualizar la altura
    updateHeight(pos_node, file);
    updateHeight(pos, file);
    return pos;
}

template<typename TK>
int AVLFile<TK>::rota_right(long parent, long pos_node, fstream &file) {
    Record<TK> a;
    file.seekg(sizeof(long)+ sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&a), sizeof(Record<TK>));
    if (parent != pos_node){
        Record<TK> record_parent;
        file.seekg(sizeof(long)+ sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
        file.read((char *)(&record_parent), sizeof(Record<TK>));
        if (record_parent > a){
            record_parent.left = a.left;
        }else{
            record_parent.right = a.left;
        }
        file.seekp(sizeof(long)+ sizeof(long) + parent * sizeof(Record<TK>), ios::beg);
        file.write((char *)(&record_parent), sizeof(Record<TK>));
    }
    else{
        this->pos_root = a.left;
        file.seekp(0, ios::beg);
        file.write((char*)(&this->pos_root), sizeof(long));
    }
    // a.right almacenado
    Record<TK> b;
    file.seekg(sizeof(long)+ sizeof(long) + a.left * sizeof(Record<TK>), ios::beg);
    file.read((char *)(&b), sizeof(Record<TK>));

    // actualizar el hijo derecho de a
    int pos = a.left;
    a.left = b.right;
    file.seekp(sizeof(long)+ sizeof(long) + pos_node * sizeof(Record<TK>), ios::beg);
    file.write((char *)(&a), sizeof(Record<TK>));

    // actualizar el hijo izquierdo de b
    b.right = pos_node;
    file.seekp(sizeof(long)+ sizeof(long) + pos* sizeof(Record<TK>), ios::beg);
    file.write((char *)(&b), sizeof(Record<TK>));
    // actualizar la altura
    updateHeight(pos_node, file);
    updateHeight(pos, file);
    return pos;
}

template<typename TK>
AVLFile<TK>::AVLFile(string filename) {
    cout << "Creando archivo: " << filename << endl;
    this->filename = filename;
    pos_root = -1;
    fstream file(filename, ios::in | ios::out | ios::binary);

    if (!file){
        // si el archivo no existe lo creamos
        ofstream file2(this->filename, ios::binary);
        file2.write((char*)&pos_root, sizeof(long));
        file2.close();
    }else{
        // si el archivo existe leemos la raiz
        file.read((char*)&pos_root, sizeof(long));
        file.close();

    }
}

template<typename TK>
void AVLFile<TK>::insert(Record<TK> record) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    insert(file, pos_root,pos_root, record);
    file.close();
}
template<typename TK>
void AVLFile<TK>::printAll() {
    fstream file(this->filename, ios::binary | ios::in | ios::out);
    long pos = 0;
    int head = 0;
    Record<TK> record;
    // get the root
    file.seekg(0, ios::beg);
    file.read((char *)(&pos), sizeof(long));
    file.read((char *)(&head), sizeof(int));
    cout << "Root: " << pos << endl;
    cout << "Head: " << head << endl;

    int count = -1;
    while (file.read((char *)(&record), sizeof(Record<TK>)))
    {
        count++;
        if (record.next_del != -2) continue;
        cout << "Record " << count << ": ";
        record.showAVL();
    }
    file.close();
}

template<typename TK>
Record<TK> AVLFile<TK>::search(const char* key){
    fstream file(filename, ios::in | ios::out | ios::binary);
    Record<TK> a;
    long pos = pos_root;
    cout << "pos_root: " << pos_root << endl;
    cout << "Buscando: " << key << endl;
    while (pos != -1){
        file.seekg(sizeof(long)+ sizeof(long) + pos * sizeof(Record<TK>), ios::beg);
        file.read((char *)(&a), sizeof(Record<TK>));
        cout << "pos: " << pos << " key: " << a.key << endl;
        if (a == key){
            file.close();
            return a;
        }
        if (a < key){
            pos = a.right;
        }else{
            pos = a.left;
        }
    }
    cerr << "No se encontro el registro" << endl;
    return Record<TK>();
}

template<typename TK>
void AVLFile<TK>::inorder() {
    fstream file(filename, ios::in | ios::out | ios::binary);
    inorder(pos_root, file);
    file.close();
}

template<typename TK>
void AVLFile<TK>::bfs() {
    fstream file(filename, ios::in | ios::out | ios::binary);
    vector<int> queue;
    queue.push_back(pos_root);
    while (!queue.empty()){
        int pos = queue.front();
        queue.erase(queue.begin());
        Record<TK> a;
        file.seekg(sizeof(long)+ sizeof(long) + pos * sizeof(Record<TK>), ios::beg);
        file.read((char *)(&a), sizeof(Record<TK>));
        if (a.next_del != -2) continue;
        cout << "Record " << pos << ": ";
        cout << a << endl;
        if (a.left != -1) queue.push_back(a.left);
        if (a.right != -1) queue.push_back(a.right);
    }
    file.close();
}

template<typename TK>
vector<Record<TK>> AVLFile<TK>::range_search(const char *begin, const char *end) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    vector<Record<TK>> result;
    vector<int> queue;
    queue.push_back(pos_root);
    while (!queue.empty()){
        int pos = queue.front();
        queue.erase(queue.begin());
        Record<TK> a;
        file.seekg(sizeof(long)+ sizeof(long) + pos * sizeof(Record<TK>), ios::beg);
        file.read((char *)(&a), sizeof(Record<TK>));
        if (a.next_del != -2) continue;
        if (a >= begin && a <= end){
            result.push_back(a);
        }
        if (a.left != -1) queue.push_back(a.left);
        if (a.right != -1) queue.push_back(a.right);
    }
    file.close();
    return result;
}

template<typename TK>
bool AVLFile<TK>::remove(const char* key) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    bool result = remove(file,pos_root, pos_root, key);
    file.close();
    return result;
}



#endif //PROJECTO1_AVLFILE_H