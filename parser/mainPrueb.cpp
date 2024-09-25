//
// Created by paolo on 17/09/2024.
//

#include <iostream>
#include "tokenS.h"
#include "parserSQL.h"
#include "avlFile.h"

using namespace std;

int main() {
    const char* input =
            "CREATE TABLE Animes FROM FILE 'anime_data.csv' USING INDEX AVL('anime_id');"
            "INSERT INTO Animes VALUES (1, 'Naruto', 'Shounen', 'TV', 220, 8.3, 1000000);"
            "INSERT INTO Animes VALUES (2, 'One Piece', 'Shounen', 'TV', 1000, 9.0, 2000000);"
            "SELECT * FROM Animes WHERE anime_id = 1;"
            "SELECT * FROM Animes WHERE anime_id BETWEEN 1 AND 2;"
            "DELETE FROM Animes WHERE anime_id = 1;"
            "CREATE TABLE Apps FROM FILE 'app_data.csv' USING INDEX AVL('App_id');"
            "INSERT INTO Apps VALUES ('com.example.app', 'Example App', 'Productivity', 4.5, 10000, '100K', 100000, 500000, true, 0.0, 'USD', '50MB');"
            "SELECT * FROM Apps WHERE App_id = 'com.example.app';";

    Scanner scanner(input);

    AVLFile<const char*> avlFileApps("app_data.bin");
    AVLFile<int> avlFileAnimes("anime_data.bin");


    Parser parser(&scanner, &avlFileApps, &avlFileAnimes);
    parser.parse();

    return 0;
}
