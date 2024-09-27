//
// Created by Jesus Egusquiza on 27/09/2024.
//

#pragma once
template<typename TK>
struct FileStructure {
    virtual ~FileStructure() = default;

    // Método virtual para la inserción, genérico con plantilla
    virtual void insert(Record<TK> record) = 0;
    virtual void printAll() = 0;
    virtual bool remove(const char* key) = 0;
    virtual Record<TK> search(const char* key) = 0;

    // extendible
    virtual void update_disk() = 0;
};
