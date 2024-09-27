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
};
