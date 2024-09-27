# Proyecto 1: Organización ed Archivos (Data Fusion DB)

## Introducción

El presente proyecto busca aplicar algoritmos de almacenamiento de archivos físicos y acceso concurrente.
Con la creación y manipulación de un dataset específico utilizando técnicas de organización de archivos.

### Objetivos
1. **Exploración de estrategias**:
    - Estudiar particularidades de archivos AVL, B+ y Extendible Hashing.
    - Identificar las ventajas y desventajas de cada estrategia de org. de archivos.
    - Analizar el rendimiento computacional con la implementación.

2. **Implementación de funciones básicas**:
    - Desarrollar algoritmos de inserción, búsqueda y eliminación para cada método de organización.
    - Comprender la interacción entre usuarios y servidores al crear GUI para la manipulación de bases de datos.

### Resultados esperados

1. **Reducción de Acceso a Memoria Secundaria**:
    - Minimizar lecturas extensas en RAM al acceder a través de índices directamente a los datos significativos  .

2. **Mejora en la Velocidad de Acceso**:
    - Las consultas y búsquedas se ejecutarán más rápidamente al utilizar índices para localizar rápidamente los datos pertinentes en lugar de recorrer toda la base de datos.

3. **Optimización del Tiempo de Procesamiento**:
    - Operaciones como inserciones, actualizaciones y eliminaciones se realizarán de manera más eficiente al utilizar índices para localizar registros y realizar modificaciones.

4. **Mejora del Rendimiento General**:
    - La indexación mejora el rendimiento y la eficiencia del sistema de gestión de bases de datos, lo que permite manejar cargas de trabajo más grandes y proporcionar una experiencia de usuario más rápida y fluida.

### Técnicas de indexación

Las técnicas usadas para este proyecto son las siguientes:

- [B+ Tree]
- [Extendible Hashing]
- [AVL]
### Implementació de funciones

Para cada técnica, se implementó las siguientes funciones:

- vector<Registro> search(T key)
- vector<Registro> rangeSearch(T begin-key, T end-key)
- bool add(Registro registro)
- bool remove(T key)

### DataSet
Desarrollamos el proyecto con el dataset [Google Playstore](https://www.kaggle.com/datasets/lava18/google-play-store-apps). Esta elección fue debida a los siguientes factores:

- **Complejidad de los datos**: ofrece una amplia variedad de información relacionada con aplicaciones, como el nombre de la aplicación, el ID, la categoría, el número de instalaciones, el precio, y calificaciones. Estos datos ofrecen un gran desafío para las pruebas y permiten explorar diferentes tipos de consultas basadas en el rendimiento y la estructura.

- **Tamaño adecuado**: ontiene un número significativo de registros, lo que es adecuado para pruebas exhaustivas y evaluaciones de rendimiento de las técnicas de organización de archivos. Su tamaño equilibrado nos permite trabajar con datos suficientes sin ser tan grande como para hacer el proyecto inviable.

- **Relaciones entre datos**: Permite explorar relaciones entre (-----), lo que es útil para entender las estructuras de datos.

- **Versatilidad de consultas**: Con precio, categorías, código de aplicación, calificación o instalaciones, se pueden realizar diversas consultas para probar las funciones implementadas en las técnicas de organización.

```c++
struct AppRecord {
    char app_name[100];
    char app_id[50];       
    char category[30];
    float rating;            
    int rating_count;       
    int min_installs;        
    int max_installs;        
    bool is_free;            
    float price;            
    char currency[5];       
    char size[10];           
};
```

|       Campo        |         Descripción          |
|:------------------:|:----------------------------:|
|   ```app_name```   |   Nombre de la aplicación    |
|    ```app_id```    |  ID único de la aplicación   |
|   ```category```   | Categoría a la que pertenece |
|    ```rating```    |    Calificación promedio     |
| ```rating_count``` |   Número de calificaciones   |
| ```min_installs``` |    Instalaciones mínimas     |
| ```max_installs``` |    Instalaciones máximas     |
|   ```is_free```    |  Indica si es gratuita o no  |
|    ```price```     |   Precio de la aplicación    |
|   ```currency```   |      Moneda del precio       |
|    ```size ```     |   Tamaño de la aplicación    |

## Técnicas Utilizadas
### AVL File
#### Insert
##### Descripción

1. **Verificación de Existencia del Registro**:
    - Se verifica si el registro ya existe en el archivo. Si el registro existe, se muestra un mensaje de error y se cierra el archivo:
      ```cpp
      ```

2. **Inserción del Registro**:
    - Si la posición del nodo es `-1`, se crea un nuevo nodo al final del archivo:
      ```cpp
      ```

3. **Inserción en un Nodo Existente**:
    - Si hay un nodo en la posición indicada, se lee el nodo actual:
      ```cpp
      ```
    - Se decide si insertar a la izquierda o a la derecha del nodo actual:
      ```cpp
      ```
    - Se actualizan los valores de `left` y `right` del nodo y se reescriben en el archivo:
      ```cpp
      ```

4. **Actualización y Balanceo del Árbol AVL**:
    - Se actualiza la altura del nodo y se balancea el árbol:
      ```cpp
      ```
    - Se escribe el nodo actualizado en el archivo:
      ```cpp

      ```



## Integrantes
|                                   **Paolo Medrano Terán**                                   |                             **Sebastian Chu Lama**                             |                               **--**                                |                               **--**                                |                               **--**                                |
|:-------------------------------------------------------------------------------------------:|:-------------------------------------------------------------------:|:-------------------------------------------------------------------:|:-------------------------------------------------------------------:|:-------------------------------------------------------------------:|
| <a href="https://github.com/paolomedrano04" target="_blank">`github.com/paolomedrano04`</a> | <a href="https://github.com/ChuSebastian" target="_blank">`https://github.com/ChuSebastian`</a> | <a href="https://github.com/--" target="_blank">`github.com/--`</a> | <a href="https://github.com/--" target="_blank">`github.com/--`</a> | <a href="https://github.com/--" target="_blank">`github.com/--`</a> |
