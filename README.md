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
- [Sequential]
### Implementació de funciones

Para cada técnica, se implementó las siguientes funciones:

- vector<Registro> search(T key)
- vector<Registro> rangeSearch(T begin-key, T end-key)
- bool add(Registro registro)
- bool remove(T key)

### DataSet1
Desarrollamos el proyecto con el dataset [Google Playstore](https://www.kaggle.com/datasets/lava18/google-play-store-apps). Esta elección fue debida a los siguientes factores:

- **Complejidad de los datos**: ofrece una amplia variedad de información relacionada con aplicaciones, como el nombre de la aplicación, el ID, la categoría, el número de instalaciones, el precio, y calificaciones. Estos datos ofrecen un gran desafío para las pruebas y permiten explorar diferentes tipos de consultas basadas en el rendimiento y la estructura.

- **Tamaño adecuado**: ontiene un número significativo de registros, lo que es adecuado para pruebas exhaustivas y evaluaciones de rendimiento de las técnicas de organización de archivos. Su tamaño equilibrado nos permite trabajar con datos suficientes sin ser tan grande como para hacer el proyecto inviable.

- **Relaciones entre datos**: Permite explorar relaciones entre (-----), lo que es útil para entender las estructuras de datos.

- **Versatilidad de consultas**: Con precio, categorías, código de aplicación, calificación o instalaciones, se pueden realizar diversas consultas para probar las funciones implementadas en las técnicas de organización.

```c++
struct AppRecord {
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
    double key = 0;         
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

### DataSet2
Desarrollamos el proyecto con un 2do dataset  [YouTube Stats](https://www.kaggle.com/datasets/datasnaek/youtube-new). Esta elección fue debida a los siguientes factores:

- **Complejidad de los datos**: Este dataset ofrece una amplia variedad de información relacionada con videos de YouTube, como el título del video, el nombre del canal, vistas, likes, dislikes, y cantidad de comentarios. Estos datos permiten realizar pruebas exhaustivas y explorar diferentes tipos de consultas basadas en el rendimiento y la estructura de los registros.

- **Tamaño adecuado**: El dataset contiene un número significativo de registros, lo que es adecuado para realizar pruebas exhaustivas y evaluar el rendimiento de las técnicas de organización de archivos. Su tamaño es equilibrado, permitiendo trabajar con datos suficientes sin ser demasiado grande para dificultar el proyecto.

- **Relaciones entre datos**: Permite explorar relaciones entre popularidad de los videos (vistas, likes, dislikes) y la interacción de los usuarios (cantidad de comentarios). Esto es útil para entender cómo los diferentes factores afectan la relevancia de un video en la plataforma y para optimizar las consultas de rendimiento.

- **Versatilidad de consultas**: Con datos como el título del video, nombre del canal, vistas, likes, dislikes y cantidad de comentarios, se pueden realizar diversas consultas para probar las funciones implementadas en las técnicas de organización de archivos.

```c++
struct VideoRecord {
    char key[100];              // ID único del video
    char title[200];            // Título del video
    char channel_title[100];    // Nombre del canal
    long long views;            // Número de vistas del video
    long long likes;            // Número de likes recibidos
    long long dislikes;         // Número de dislikes recibidos
    long long comment_count;    // Cantidad de comentarios
};
```





|        Campo        |          Descripción           |
|:-------------------:|:-----------------------------:|
|     ```key```       |  ID único del video           |
|    ```title```      |  Título del video             |
| ```channel_title``` |  Nombre del canal             |
|    ```views```      |  Número de vistas del video   |
|    ```likes```      |  Número de likes              |
|  ```dislikes```     |  Número de dislikes           |
| ```comment_count``` |  Cantidad de comentarios      |

## Técnicas Utilizadas
### AVL File
#### Insert
##### Descripción

1. **Verificación de Existencia del Registro**:
    - Se verifica si el registro ya existe en el archivo. Si el registro existe, se muestra un mensaje de error y se cierra el archivo:
      ```cpp
      if (find(record.key) != nullptr) {
          cerr << "El registro ya existe en el archivo." << endl;
          return false;
      }
      ```

2. **Inserción del Registro**:
    - Si la posición del nodo es `-1`, se crea un nuevo nodo al final del archivo:
      ```cpp
      if (position == -1) {
          Node newNode(record);
          writeToEndOfFile(newNode);
      }
      ```

3. **Inserción en un Nodo Existente**:
    - Si hay un nodo en la posición indicada, se lee el nodo actual:
      ```cpp
      Node currentNode = readFromFile(position);
      ```
    - Se decide si insertar a la izquierda o a la derecha del nodo actual:
      ```cpp
       if (record.key < currentNode.key) {
          currentNode.left = insertLeft(currentNode, record);
      } else {
          currentNode.right = insertRight(currentNode, record);
      }
      ```
    - Se actualizan los valores de `left` y `right` del nodo y se reescriben en el archivo:
      ```cpp
       updateNodeInFile(position, currentNode);
      ```

4. **Actualización y Balanceo del Árbol AVL**:
    - Se actualiza la altura del nodo y se balancea el árbol:
      ```cpp
      currentNode.height = 1 + max(getHeight(currentNode.left), getHeight(currentNode.right));
      currentNode = balance(currentNode);
      ```
    - Se escribe el nodo actualizado en el archivo:
      ```cpp
       writeToFile(currentNode);
      ```
#### Consideraciones
1. **Balance del Árbol**:
   - El árbol AVL mantiene el equilibrio tras cada operación de inserción y eliminación. Esto garantiza que la profundidad del árbol se mantenga en \(O(\log n)\), lo que hace que las búsquedas, inserciones y eliminaciones sean eficientes.
   - Sin embargo, mantener el equilibrio requiere actualizaciones adicionales después de cada inserción y eliminación, lo que añade cierta sobrecarga computacional.

2. **Uso en Aplicaciones**:
   - El árbol AVL es ideal para situaciones donde se realizan muchas consultas de búsqueda y es importante que estas consultas sean rápidas. Sin embargo, su uso en escenarios donde predominan las inserciones y eliminaciones puede no ser óptimo debido al reequilibrado.

3. **Complejidad Espacial**:
   - Cada nodo debe almacenar información adicional como el factor de balance y la altura, lo que puede aumentar ligeramente el consumo de memoria en comparación con otros árboles binarios.



### B+ Tree

#### Función Insert

1. **Verificación de Espacio en el Nodo**:
   - Antes de insertar un nuevo registro, se verifica si el nodo tiene espacio disponible. Si no tiene espacio, se divide el nodo:
     ```cpp
     if (node.isFull()) {
         splitNode(node);
     }
     ```

2. **Inserción del Registro**:
   - Si el nodo tiene espacio, se inserta el registro en el nodo:
     ```cpp
     node.addRecord(record);
     ```

3. **Propagación de la División**:
   - Si el nodo se divide, se propaga la división hacia el nodo padre para mantener el equilibrio del árbol:
     ```cpp
     propagateSplit(parentNode, node);
     ```

4. **Actualización de Punteros**:
   - Tras la inserción, se actualizan los punteros a los nodos hijo para mantener la estructura del B+ Tree:
     ```cpp
     updatePointersAfterInsert(node);
     ```

#### Función Search

1. **Búsqueda por Clave**:
   - Se realiza una búsqueda en el árbol B+ utilizando una clave específica. Si la clave se encuentra, se devuelve el registro correspondiente:
     ```cpp
     auto *it = tree.search(key);
     if(it == nullptr)
         cout << key << " not found..." << endl;
     else
         cout << *it << endl;
     ```

#### Función RangeSearch

1. **Búsqueda en un Rango de Claves**:
   - La función `range_search()` realiza una búsqueda de todos los registros que se encuentran entre dos claves especificadas:
     ```cpp
     vector<Record<int>> ans = tree.range_search(20, 30);
     ```

2. **Devolución de Resultados**:
   - Los registros que se encuentran dentro del rango son devueltos como un vector de registros:
     ```cpp
     for(auto val : ans)
         cout << val << endl;
     ```

#### Función PrintAll

1. **Mostrar Todos los Nodos**:
   - La función `print_all()` imprime la información de todos los nodos índice y nodos de datos en el B+ Tree:
     ```cpp
     tree.print_all();
     ```

2. **Visualización de Nodos**:
   - Se visualizan tanto los nodos índice como los nodos de datos, junto con su contenido, para propósitos de depuración y visualización:
     ```cpp
     cout << "-------------INDEX NODES BEGIN ---------------------------" << endl;
     for (int i = 0; i < index_total; ++i) {
         index_node = read_index_node_in_pos(file_index, i);
         cout << "-------index Node number [" << i << "]-----------" << endl;
         index_node.showdata();
         cout << "---------------------------------------------" << endl;
     }
     ```

#### Consideraciones
1. **Optimización para Almacenamiento en Disco**:
   - El B+ Tree está diseñado para minimizar las operaciones de entrada/salida en disco. Almacena todos los registros en las hojas del árbol, lo que permite que las búsquedas en los nodos índice sean más rápidas.
   - Este tipo de árbol es especialmente útil para bases de datos y sistemas de archivos donde se realizan muchas operaciones de lectura/escritura.

2. **Rendimiento de Búsqueda y Actualización**:
   - Las búsquedas en un B+ Tree son rápidas debido a que el árbol está siempre balanceado y la profundidad es baja. Las operaciones de inserción y eliminación también son eficientes, pero pueden implicar la división y fusión de nodos, lo que genera sobrecarga.

3. **Almacenamiento de Múltiples Registros en las Hojas**:
   - En un B+ Tree, las hojas contienen punteros a los registros, lo que hace que la estructura sea eficiente en términos de almacenamiento y permite realizar búsquedas por rango de manera eficiente. Este comportamiento es útil en aplicaciones donde se requieren consultas por rango de claves.


### Extendible Hashing

#### Función Insert

1. **Cálculo del Hash**:
   - Para insertar un nuevo registro, primero se calcula el valor del hash utilizando la clave proporcionada:
     ```cpp
     int hashValue = hash(record.key);
     ```

2. **Verificación del Bucket**:
   - Se verifica si el bucket correspondiente al valor del hash tiene espacio suficiente para el nuevo registro:
     ```cpp
     if (bucket.isFull()) {
         expandBucket(bucket);
     }
     ```

3. **Inserción del Registro**:
   - Si el bucket tiene espacio, se inserta el registro:
     ```cpp
     bucket.addRecord(record);
     ```

4. **Expansión de la Tabla de Hash**:
   - Si el bucket está lleno, la tabla de hash se expande para acomodar el nuevo registro:
     ```cpp
     if (directory.isFull()) {
         expandDirectory();
     }
     ```

#### Función Remove

1. **Buscar el Bucket**:
   - Primero, se busca el bucket correspondiente a la clave utilizando el hash de la clave:
     ```cpp
     fstream file(this->fileName, ios::binary | ios::in | ios::out);
     file.seekg(this->directory[index], ios::beg);
     ```

2. **Eliminar el Registro**:
   - Si el registro se encuentra en el bucket, se elimina y se reordenan los registros restantes:
     ```cpp
     for (int i = 0; i < bucket.size; ++i) {
         if (bucket.records[i] == key) {
             for (int j = i; j < bucket.size - 1; ++j) {
                 bucket.records[j] = bucket.records[j + 1];
             }
             bucket.size--;
             break;
         }
     }
     ```

3. **Actualización del Bucket**:
   - Después de eliminar el registro, se actualiza el bucket en el archivo de datos:
     ```cpp
     fstream file_data(this->fileName, ios::binary | ios::in | ios::out);
     file_data.seekp(this->directory[index] + sizeof(int), ios::beg);
     file_data.write((char *) &bucket, sizeof(bucket));
     file_data.close();
     ```

4. **Actualización del Head**:
   - Si el bucket eliminado no es el primer bucket en la lista, se actualiza el head para apuntar al bucket correcto:
     ```cpp
     if (head != -1) {
         fstream file_head(this->fileName, ios::binary | ios::in | ios::out);
         file_head.seekp(this->directory[index], ios::beg);
         int posreal = pos + sizeof(int);
         file_head.write((char *) &posreal, sizeof(posreal));
         file_head.close();
     }
     ```

#### Función DisplayDirectory

1. **Mostrar el Directorio**:
   - Se imprimen todas las posiciones del directorio que contienen los punteros a los buckets:
     ```cpp
     for (int i = 0; i < this->directory.size(); ++i) {
         cout << "directory[" << i << "] -> " << this->directory[i] << endl;
     }
     ```

2. **Mostrar la Relación entre Buckets**:
   - También se muestran los buckets vinculados a cada entrada del directorio:
     ```cpp
     cout << "Displaying directory" << endl;
     for (int i = 0; i < this->directory.size(); ++i) {
         cout << "directory[" << i << "] -> " << this->directory[i] << endl;
     }
     ```

#### Consideraciones
1. **Expansión Dinámica**:
   - Una de las principales ventajas del Hashing Extensible es su capacidad para expandir el tamaño de la tabla hash a medida que aumenta el número de elementos, sin necesidad de rehash completo de todos los elementos. Esto reduce la sobrecarga en comparación con otros métodos de hash.

2. **Coste de Expansión**:
   - Si bien el hashing extensible es eficiente en la mayoría de los casos, su rendimiento puede verse afectado durante las expansiones, ya que se requiere reconfigurar los punteros y posibles duplicaciones de directorios. Las expansiones frecuentes pueden generar sobrecarga si los datos crecen rápidamente.

3. **Claves No Ordenadas**:
   - Dado que las claves no se almacenan en orden (a diferencia de los árboles), el Hashing Extensible no es adecuado para consultas por rango. Este método es ideal para aplicaciones que requieren búsquedas rápidas por clave exacta, pero no es eficiente para operaciones que requieren un recorrido secuencial de los datos.

4. **Complejidad Espacial**:
   - La estructura puede consumir más memoria que una tabla de hash convencional si la tabla de directorio crece significativamente debido a las expansiones. Sin embargo, esta sobrecarga es compensada por el rendimiento en búsquedas exactas.


### Organización Secuencial

#### Función_Insert

1. **Conversión de Registro desde CSV**:
   - Al insertar un nuevo registro, primero se convierte una línea de un archivo CSV a un objeto `Record`:
     ```cpp
     if (record.fromCSV(line)) {
         file.seekp(0, ios::end);
         file.write((char *)&record, sizeof(Record));
     }
     ```

2. **Escribir el Registro**:
   - El registro convertido se escribe en el archivo principal binario (`main_file`):
     ```cpp
     file.write((char *)&record, sizeof(Record));
     ```

3. **Inserción de un Nuevo Registro en el Archivo**:
   - Se pueden agregar nuevos registros al archivo de inserciones, y estos son luego fusionados en el archivo principal:
     ```cpp
     void insert(int key) {
         Record record(key);
         fstream file(insert_file, ios::in | ios::out | ios::binary);
         file.seekp(0, ios::end);
         file.write((char *)&record, sizeof(Record));
         file.close();
     }
     ```

#### Función Search

1. **Búsqueda de Registro por Clave**:
   - Se realiza una búsqueda secuencial en el archivo para encontrar la posición de un registro con una clave específica:
     ```cpp
     int pos = search(key);
     ```

2. **Lectura del Registro**:
   - Una vez que se encuentra la posición, se lee el registro desde el archivo:
     ```cpp
     Record record = read_record(pos);
     ```

3. **Visualización del Registro**:
   - El registro recuperado es mostrado en la consola:
     ```cpp
     record.show();
     ```

#### Función Remove

1. **Buscar el Registro para Eliminar**:
   - Primero se busca la posición del registro que se desea eliminar:
     ```cpp
     int pos = search(key);
     ```

2. **Marcar como Eliminado**:
   - Si el registro es encontrado, se marca como eliminado modificando el campo `is_removed` a `true`:
     ```cpp
     record.is_removed = true;
     ```

3. **Escribir el Registro Actualizado**:
   - Se escribe el registro modificado en el archivo en su posición original:
     ```cpp
     write_record(record, pos);
     ```

#### Función PrintAll

1. **Imprimir Todos los Registros**:
   - La función `print_all()` lee todos los registros en el archivo y los muestra si no están marcados como eliminados:
     ```cpp
     while (file.read((char *)&record, sizeof(Record))) {
         if (record.is_removed == false)
             record.show();
     }
     ```

2. **Mostrar el Tamaño del Archivo**:
   - Al final, se muestra el tamaño del archivo, que corresponde al número de registros:
     ```cpp
     cout << "size: " << size << endl;
     ```


#### Consideraciones
1. **Eficiencia en Búsquedas Secuenciales**:
   - La organización secuencial es adecuada cuando se accede a registros de manera ordenada o por rango. Las consultas secuenciales son más rápidas que en estructuras no ordenadas, ya que los registros están almacenados en orden.

2. **Inserción Costosa**:
   - La principal desventaja de la organización secuencial es la inserción, ya que al agregar nuevos registros es posible que sea necesario reorganizar y desplazar múltiples registros para mantener el orden. Esto puede resultar en una gran sobrecarga si las inserciones son frecuentes.

3. **Eliminaciones Lógicas**:
   - En lugar de eliminar físicamente un registro, el archivo secuencial marca el registro como "eliminado". Esto puede causar que el archivo crezca con registros innecesarios, lo que impacta el rendimiento a largo plazo si no se realiza una compactación periódica del archivo.

4. **Reordenamiento de Datos**:
   - Debido a la naturaleza secuencial, se debe realizar una reordenación de los datos tras la inserción o eliminación de registros. Esto añade complejidad computacional, especialmente si el archivo es muy grande.




## Integrantes
|                                   **Paolo Medrano Terán**                                   |                             **RELLENA**                             |                               **--**                                |                               **--**                                |                               **--**                                |
|:-------------------------------------------------------------------------------------------:|:-------------------------------------------------------------------:|:-------------------------------------------------------------------:|:-------------------------------------------------------------------:|:-------------------------------------------------------------------:|
| <a href="https://github.com/paolomedrano04" target="_blank">`github.com/paolomedrano04`</a> | <a href="https://github.com/--" target="_blank">`github.com/--`</a> | <a href="https://github.com/--" target="_blank">`github.com/--`</a> | <a href="https://github.com/--" target="_blank">`github.com/--`</a> | <a href="https://github.com/--" target="_blank">`github.com/--`</a> |
